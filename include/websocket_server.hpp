#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

// external
#include "../external/crow/crow_all.h"

std::optional<crow::json::rvalue> parse_message(const std::string& message){
    auto json_data = crow::json::load(message);
    if (!json_data){
        std::cout << "Invalid JSON" << std::endl;
        return std::nullopt; 
    }

    if (!json_data.has("type") || !json_data.has("timestamp") ){
        std::cout << "Inbound message has incorrect format" << std::endl;
        return std::nullopt;
    }

    return json_data;
}

// Send response based of inbound message
void send_response(const std::string& message, crow::websocket::connection& conn){
    auto json_data = parse_message(message);
    if (json_data){
        auto& data = json_data.value();
        std::string message_type = data["type"].s();

        crow::json::wvalue message_to_sent;
        message_to_sent["timestamp"] = data["timestamp"].s();
        
        if (message_type == "increment"){
            message_to_sent["type"] = "increment_acknowledgement";
        } else if (message_type == "handshake"){
            message_to_sent["type"] = "handshake_acknowledgement";
        } else {
            message_to_sent["type"] = "unknown";
        }
        conn.send_text(message_to_sent.dump());
    }
}

void run_websocket_server() {
    crow::SimpleApp app;
    std::unordered_set<crow::websocket::connection*> active_connections;

    CROW_ROUTE(app, "/ws")
        .websocket()
        .onopen([&active_connections](crow::websocket::connection& conn) {
            std::cout << "WebSocket client connected!\n";
            active_connections.insert(&conn);
        })
        .onclose([&active_connections](crow::websocket::connection& conn, const std::string& reason) {
            std::cout << "WebSocket client disconnected: " << reason << "\n";
            active_connections.erase(&conn);
        })
        .onmessage([&active_connections](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
            std::cout << "Received message: " << data << "\n";
            send_response(data, conn);
        });

    app.port(8765).multithreaded().run();
}
