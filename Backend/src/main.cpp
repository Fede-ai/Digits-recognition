#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <iostream>

int main() {
    websocketpp::server<websocketpp::config::asio> server;

    server.set_open_handler([](websocketpp::connection_hdl hdl) {
        std::cout << "new connection\n";
    });
    server.set_message_handler([](websocketpp::connection_hdl hdl, websocketpp::config::asio::message_type::ptr msg) {
        std::cout << "new message\n";
    });
    server.set_close_handler([](websocketpp::connection_hdl hdl) {
        std::cout << "connection closed\n";
    });

    server.init_asio();
    server.listen(9002);
    server.start_accept();

    std::cout << "starting server\n";

    server.run();
    
    return 0;
}