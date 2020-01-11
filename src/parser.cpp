//
// Created by schrodinger on 1/9/20.
//

#include <parser.h>

thread_local mod::string line = {};
thread_local size_t counter = 0;
thread_local size_t address = 0x400000;

parse_error::parse_error(std::string str) : msg(std::move(str)) {}

const char *parse_error::what() const noexcept {
    return msg.c_str();
}

uint8_t parse_register() {
    auto reg_name = next_word();
    if (unlikely(reg_name[0] != '$')) {
        throw parse_error(absl::StrCat("syntax error at: ", reg_name.data()));
    }
    if (unlikely(reg_name[5])) {
        throw parse_error(absl::StrCat("unable to resolve register name: ", reg_name.data()));
    }
    switch (reg_name[1]) {
        case 'v': {
            if (reg_name[2] == '0') CHECK_AND_RETURN(3, 2);
            if (reg_name[2] == '1') CHECK_AND_RETURN(3, 3);
            break;
        }
        case 'a': {
            switch (reg_name[2]) {
                case 't': CHECK_AND_RETURN(3, 1)
                case '0': CHECK_AND_RETURN(3, 4)
                case '1': CHECK_AND_RETURN(3, 5)
                case '2': CHECK_AND_RETURN(3, 6)
                case '3': CHECK_AND_RETURN(4, 7)
            }
            break;
        }
        case 't': {
            if (likely(reg_name[2] >= '0' && reg_name[2] <= '7')) {
                CHECK_AND_RETURN(3, reg_name[2] - '0' + 8)
            }
            if (reg_name[2] == '8') CHECK_AND_RETURN(3, 24)
            if (reg_name[2] == '9') CHECK_AND_RETURN(3, 25)
            break;
        }
        case 's': {
            if (likely(reg_name[2] >= '0' && reg_name[2] <= '7')) {
                CHECK_AND_RETURN(3, reg_name[2] - '0' + 16)
            }
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 29)
            break;
        }
        case 'k': {
            if (reg_name[2] == '0') CHECK_AND_RETURN(3, 26);
            if (reg_name[2] == '1') CHECK_AND_RETURN(3, 27);
            break;
        }
        case 'z': {
            if (std::strcmp(reg_name.data() + 2, "ero") == 0) return 0;
            break;
        }
        case 'g': {
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 28)
            break;
        }
        case 'f': {
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 30)
            break;
        }
        case 'r': {
            if (reg_name[2] == 'a') CHECK_AND_RETURN(3, 31)
            break;
        }
    }

    throw parse_error(absl::StrCat("unable to resolve register name: ", reg_name.data()));
}

mod::string cleanup(mod::string a) {
    if (auto r = std::memchr(a.data(), '#', a.size())) {
        a.resize((uintptr_t) r - (uintptr_t) a.data());
    }
    auto m = absl::StripSuffix(absl::StripAsciiWhitespace(a), "\n");
    mod::string res;
    res.resize(m.length());
    std::memcpy(res.data(), m.data(), m.length());
    return res;
}

void eat_whitespace() {
    while (counter < line.size() && line[counter] == ' ') counter++;
}

bool is_sep(char t) {
    return t <= 32 || t == ',';
}

void eat_sep() {
    while (counter < line.size() && is_sep(line[counter])) counter++;
}

bool at_line_end() {
    return counter == line.size();
}

uint8_t peek() {
    eat_whitespace();
    if (unlikely(at_line_end())) {
        throw parse_error("peek from end of line");
    }
    return line[counter];
}

uint8_t parse_u8() { return parse_num<uint8_t>(); }

uint8_t parse_next_u8_or_zero() {
    return (!at_line_end() && (peek() == ',')) ? parse_u8() : 0;
}

std::string get_label(size_t pos, std::string_view content) {
    std::string v;
    while (is_sep(content[pos])) pos++;
    if (pos == content.size()) {
        throw parse_error(absl::StrCat("label or address required but not found"));
    }
    while (pos < content.size() && (std::isalpha(content[pos]) || std::isalnum(content[pos]) || content[pos] == '_')) {
        v.push_back(content[pos++]);
    }
    if (pos != content.size()) {
        throw parse_error(absl::StrCat("label format error with: ", content.data()));
    }
    return v;
}

void parse_label() {
    auto m = std::memchr(line.data(), ':', line.length());
    if (m != nullptr) {
        std::string key;
        key.resize((uintptr_t) m - (uintptr_t) line.data());
        std::memcpy(key.data(), line.data(), key.size());
        for (auto i : key) {
            if (unlikely(!isalnum(i) && !isalpha(i) && i != '_')) {
                throw parse_error(absl::StrCat("wrong label format (contains invalid character): ", key));
            }
        }
        counter += key.size() + 1;
        parser_shared::label_mutex.lock();
        parser_shared::labels.insert({std::move(key), address});
        parser_shared::label_mutex.unlock();
    }
}

std::array<char, 10> next_word() {
    std::array<char, 10> buf = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto a = 0;
    eat_sep();
    if (unlikely(at_line_end())) {
        throw parse_error(absl::StrCat("next word from end of line: ", line, ", position: ", std::to_string(counter)));
    }
    while (!is_sep(line[counter]) && line[counter] != ')') {
        if (unlikely(a == 10)) {
            buf[9] = 0;
            throw parse_error(absl::StrCat("name is too long: ", line, ", with current buf: ", buf.data()));
        }
        buf[a++] = line[counter++];
    }
    return buf;
}


namespace parser_shared {
    std::size_t global_address{BASE_ADDR};
    mod::vector<task> job_queue{};
    std::mutex error_mutex{};
    mod::vector<recover> label_queue;
    std::mutex label_queue_mutex{};
    absl::flat_hash_map<mod::string, size_t> labels;
    std::atomic_bool success = true;

    size_t fill_queue() {
        job_queue.clear();
        bool flag = false;
        std::string buffer{};
        while (source->good() && job_queue.size() < QUEUE_SIZE) {
            mod::string buf;
            std::getline(*source, buf);
            mod::string real = cleanup(std::move(buf));
            if (real.empty()) continue;
            if (real[real.size() - 1] == ':') {
                buffer = std::move(real);
                flag = true;
                continue;
            }
            if (flag) {
                flag = false;
                real = absl::StrCat(buffer, real);
            }
            job_queue.emplace_back(std::move(real), global_address);
            global_address += 4;
        }
        if (job_queue.size())
            finished.resize(job_queue.size() + finished.size());
        return job_queue.size();
    }

    mod::vector<Instruction> finished;
    std::mutex label_mutex;

    void push_result(const Instruction &intr, size_t addr) {
        auto index = (addr - BASE_ADDR) / 4;
        finished[index] = intr;
    }

    void output_error(const parse_error &error) {
        success = false;
        error_mutex.lock();
        std::cerr << "[SYNTAX ERROR] " << error.msg << " (line: " << (address - BASE_ADDR) / 4 << ", pos: "
                  << counter << ')' << std::endl;
        error_mutex.unlock();
    }

    void push_label_queue(size_t pos, RecoverType type) {
        label_queue_mutex.lock();
        label_queue.emplace_back(line, address, pos, type);
        label_queue_mutex.unlock();
    }

    recover::recover(std::string line, size_t addr, size_t pos, RecoverType type)
            : line(std::move(line)), addr(addr), pos(pos), type(type) {}
}
