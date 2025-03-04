#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <csignal>

// external
#include "../external/crow/crow_all.h"

void run_http_server(std::atomic<bool>* stop_flag_ptr){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, World!";
    });

    // Run the server asynchronously
    std::thread server_thread([&]() {
        app.port(8080).multithreaded().run();
    });

    // Wait for the stop flag
    while (!stop_flag_ptr->load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    if (server_thread.joinable()) {
        server_thread.join();
    }

    std::cout << "Server stopped." << std::endl;
}
