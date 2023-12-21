#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <iostream>
#include <sstream>
#include "ai.h"

class WebSocketServerHandler {
public:
    //WebSocketServerHandler() : ai("C:/Users/feder/Desktop/Digits-recognition/Trainer/save.txt") {}

    void on_message(websocketpp::connection_hdl hdl, websocketpp::config::asio::message_type::ptr msg) {
        std::istringstream ss(msg->get_payload());
        std::vector<int> chars;
        std::string token;
        while (std::getline(ss, token, ','))
           chars.push_back(std::stoi(token));

        //auto predictions = ai.forwardProp(chars);
        auto predictions = std::vector<double>{ 23.2, 213.4, 0.23, 54.0, 00.9, 21.5, 679.7, 0.031, 012, 123.4 };
        std::string packet;
        for (auto value : predictions)
            packet.append(std::to_string(value) + ",");
        packet.erase(packet.end() - 1);

        server.send(hdl, packet, msg->get_opcode());
    }

    //Ai ai;
    websocketpp::server<websocketpp::config::asio> server;
};

int main() {
    WebSocketServerHandler handler;

    handler.server.set_open_handler([](websocketpp::connection_hdl hdl) {
        std::cout << "new connection\n";
    });
    handler.server.set_message_handler(std::bind(&WebSocketServerHandler::on_message, &handler, std::placeholders::_1, std::placeholders::_2));
    handler.server.set_close_handler([](websocketpp::connection_hdl hdl) {
        std::cout << "connection closed\n";
    });

    handler.server.clear_access_channels(websocketpp::log::alevel::all);
    handler.server.set_access_channels(websocketpp::log::alevel::fail);

    handler.server.init_asio();
    handler.server.listen(9002);
    handler.server.start_accept();

    std::cout << "server is running\n";

    handler.server.run();
    
    return 0;
}