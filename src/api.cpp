//
// Created by schrodinger on 3/6/20.
//
#include <api.h>

void reset_all() {
    using namespace parser_shared;
    global_address = BASE_ADDR;
    finished.clear();
    labels.clear();
    label_queue.clear();
    global_line_count = 0;
    data_index = 0;
    data_queue.clear();
    job_queue.clear();
    success = true;
}

Result generate_result(std::string path) {
    using namespace parser_shared;
    reset_all();
    source = new std::ifstream{path};
    ErrorStream error_stream{};
    run_parsing();
    auto data_part = generate_data();
    return Result{
            std::move(finished),
            std::move(data_part),
            error_stream.str(),
            success
    };
}

ErrorStream::~ErrorStream() {
    std::cout.rdbuf(old); // reverse redirect
}

ErrorStream::ErrorStream() {
    old = std::cerr.rdbuf(buffer.rdbuf()); // redirect cout to buffer stream
}

std::string ErrorStream::str() {
    return buffer.str(); // get string
}

Result::Result(std::vector<Instruction> instr, std::vector<char> data_part, std::string error_info, bool success)
        : instructions(std::move(instr)), data_part(std::move(data_part)), error_info(std::move(error_info)),
          success(success) {

}
