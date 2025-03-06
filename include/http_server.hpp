#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

// external
#include "../external/crow/crow_all.h"

std::string load_file_content(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) return "";  // Return empty if file doesn't exist
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void run_http_server(){
    crow::SimpleApp app;

    // Serve CSS files correctly
    CROW_ROUTE(app, "/css/<string>")
        .methods("GET"_method)
        ([](std::string filename) {
            std::string content = load_file_content("css/" + filename);
            if (content.empty()) return crow::response(404);  // Return 404 if file not found
            crow::response res(content);
            res.set_header("Content-Type", "text/css");
            return res;
        });

    // Serve JavaScript files correctly
    CROW_ROUTE(app, "/js/<string>")
        .methods("GET"_method)
        ([](std::string filename) {
            std::string content = load_file_content("js/" + filename);
            if (content.empty()) return crow::response(404);
            crow::response res(content);
            res.set_header("Content-Type", "application/javascript");
            return res;
        });

    // Serve the HTML page
    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load_text("index.html");
        return page;
    });

    app.port(8080).multithreaded().run();

    std::cout << "Server stopped." << std::endl;
}
