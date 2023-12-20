#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    server::connection_ptr con = server::get_instance_from_hdl(hdl);

    try {
        con->send(msg->get_payload(), msg->get_opcode());
    }
    catch (const websocketpp::lib::error_code& e) {
        std::cout << "Echo failed because: " << e
            << "(" << e.message() << ")" << std::endl;
    }
}

int main() {
    server echo_server;

    echo_server.set_message_handler(&on_message);

    echo_server.init_asio();
    echo_server.listen(9002);
    echo_server.start_accept();

    echo_server.run();
    return 0;
}