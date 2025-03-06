#pragma once

#include <iostream>

// external
#include "../external/crow/crow_all.h"

void run_http_server(){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load_text("index.html");
        return page;
    });

    app.port(8080).multithreaded().run();

    std::cout << "Server stopped." << std::endl;
}
