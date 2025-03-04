#pragma once

#include <atomic>
#include <chrono>
#include <thread>

void run_http_server(std::atomic<bool>* stop_flag_ptr){
    while (!stop_flag_ptr->load()) {
        std::cout << "Placeholder server is running" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }

    std::cout << "Placeholder server is turned off" << std::endl;
}
