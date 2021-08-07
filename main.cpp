#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"


int main() {

    httplib::SSLClient client("github.com", 443);

    client.set_ca_cert_path("./ca.pem");
    client.enable_server_certificate_verification(true);
    int verify = client.get_openssl_verify_result();
    if(verify){
        std::cout << X509_verify_cert_error_string(verify) << "\n";
    }
    
    auto response = client.Get("/jmscreation");
    if(response){
        if(response->status == 200){
            std::cout << response->body << "\n";
        } else {
            std::cout << "Status: " << response->status << "\n";
        }
    } else {
        std::cout << "Invalid Request\n";
    }



    return 0;
}