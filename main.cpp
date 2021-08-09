#include <iostream>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "windows.h"


std::string Client(const std::string& host, const std::string& path) {
    std::string rvalue;
    httplib::SSLClient client(host.data(), 443);

    client.set_ca_cert_path("certs/ca_list.pem");
    client.enable_server_certificate_verification(true);
    int verify = client.get_openssl_verify_result();
    if(verify){
        std::cout << X509_verify_cert_error_string(verify) << "\n";
    }
    
    auto response = client.Get(path.data());
    if(response){
        if(response->status == 200){
            rvalue = response->body;
        } else {
            std::cout << "Status: " << response->status << "\n";
        }
    } else {
        std::cout << "Invalid Request\n";
    }

    return rvalue;
}

void Server() {
    httplib::SSLServer server("certs/server.pem", "certs/server.key");

    server.set_keep_alive_max_count(2);
    server.set_keep_alive_timeout(15);

    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) -> httplib::SSLServer::HandlerResponse {
        std::cout << "from -> " << req.remote_addr << ":" << req.remote_port << "\n";
        return httplib::SSLServer::HandlerResponse::Unhandled;
    });

    server.Get("/", [](const httplib::Request &req, httplib::Response &res){
        res.set_content("unknown", "text/plain");
    });

    server.listen("0.0.0.0", 443);

    if(!server.is_running()){
        std::cout << "Server failed to start\n";
    }
}


int main() {
    Server();
    
    
    return 0;
}