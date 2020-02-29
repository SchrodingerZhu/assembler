//
// Created by schrodinger on 1/10/20.
//

#include <instructions_types.h>
#include <thread_parser.h>
#include <bitset>

void parser_life(std::atomic_size_t &finished) {
    while (true) {
        try {
            auto t = finished++;
            if (t >= parser_shared::job_queue.size()) {
                break;
            }
            auto &task = parser_shared::job_queue[t];
            line = std::move(task.content);
            counter = 0;
            address = task.address;
            local_line = task.line_count;
            local_prefix = task.prefix;
            parse_label();
            auto inst = next_word();
            if (RMap.contains(inst.data())) {
                parser_shared::push_result(generate_R(inst.data()), address);
            } else if (RIMap.contains(inst.data())) {
                parser_shared::push_result(generate_RI(inst.data()), address);
            } else if (is_cop(inst.data())) {
                parser_shared::push_result(generate_COP(inst.data()), address);
            } else if (IMap.contains(inst.data())) {
                parser_shared::push_result(generate_I(inst.data()), address);
            } else if (is_j(inst.data())) {
                parser_shared::push_result(generate_J(inst.data()), address);
            } else {
                throw parse_error(absl::StrCat("failed to resolve the instruction: ", inst.data()));
            }
        } catch (const parse_error &error) {
            if (test_flag)
                throw parse_error(error);
            else
                parser_shared::output_error(error, local_line, local_prefix + counter);
        }
    }
}

void run_parsing() {
    while (parser_shared::fill_queue()) {
        std::atomic_size_t t = 0;
        std::vector<std::thread> all;
        for (auto i = 0; i < WORKERS; ++i) {
            all.emplace_back([&] { parser_life(t); });
        }
        for (auto &i : all) {
            while (!i.joinable()) std::this_thread::yield();
            i.join();
        }
    }
    parser_recover();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

void parser_recover() {
    using namespace parser_shared;
#pragma omp parallel for schedule(guided) default(shared)
    for (size_t i = 0; i < label_queue.size(); ++i) {
        auto &job = label_queue[i];
        try {
            auto label = get_label(job.pos, job.line);
            if (unlikely(!labels.contains(label))) {
                throw parse_error(absl::StrCat("unable to resolve label: ", label));
            }
            auto addr = labels.at(label);
            switch (job.type) {
                case I:
                    finished[(job.addr - BASE_ADDR) / 4].INST_I.C = ((addr - job.addr) >> 2u) - 1u;
                    break;
                case J:
                    finished[(job.addr - BASE_ADDR) / 4].INST_J.A = addr >> 2u;
                    break;
                case RI:
                    finished[(job.addr - BASE_ADDR) / 4].INST_RI.C = ((addr - job.addr) >> 2u) - 1u;
                    break;
                default:
                    unreachable();
            }
        } catch (const parse_error &e) {
            if (test_flag)
                throw parse_error(e);
            else
                parser_shared::output_error(e, job.line_count, job.prefix + job.pos);
        }
    }
}

#pragma clang diagnostic pop

bool test_flag = false;

void test_mode(bool flag) {
    test_flag = flag;
}
