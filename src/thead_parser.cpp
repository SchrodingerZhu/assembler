//
// Created by schrodinger on 1/10/20.
//

#include "thead_parser.h"

void parser_life(std::atomic_size_t &finished) {
    while (true) {
        auto t = finished++;
        if (t >= parser_shared::job_queue.size()) {
            break;
        }
        auto &task = parser_shared::job_queue[t];
        line = task.first;
        counter = 0;
        address = task.second;
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
    }
}

void run_parsing() {
    while (parser_shared::fill_queue()) {
        std::atomic_size_t t = 0;
        std::vector<std::thread> all;
        for(auto i = 0; i < WORKERS; ++i) {
            all.emplace_back([&] { parser_life(t); });
        }
        for(auto& i : all) {
            while (!i.joinable()) std::this_thread::yield();
            i.join();
        }
    }
}
