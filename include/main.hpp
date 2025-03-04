#pragma once

#include <iostream>
#include <atomic>
#include <csignal>
#include <memory>
#include <thread>


// project
#include "http_server.hpp"

// Global atomic flag to signal threads to stop
std::atomic<bool> stop_flag(false);

void handle_sigint(int signal) {
    std::cout << "\nCaught signal " << signal << " (SIGINT). Exiting safely..." << std::endl;
    stop_flag.store(true);
}

int main(){
    std::cout << "Running" << std::endl;

    // Register the signal handler
    std::signal(SIGINT, handle_sigint);

    std::thread http_server_thread(run_http_server, &stop_flag);

    if (http_server_thread.joinable()){
        http_server_thread.join();
    }  
    std::cout << "Execution is finished" << std::endl;
    return 0;
}
