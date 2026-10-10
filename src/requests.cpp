#include <iostream>
#include <string>
#include "../httplib/httplib.h"

std::string get(const std::string& url, const std::string& endpoint) {
    httplib::Client cli(url.c_str()); 

    auto res = cli.Get(endpoint.c_str());
    
    if (res) {
        if (res->status == 200) {
            return res->body;
        } else {
            return "ERROR: Status " + std::to_string(res->status);
        }
    } else {
        auto err = res.error();
        return "Connection failed. Error code: " + std::to_string((int)err) + "\n";
    }
}

std::string post(const std::string& url, const std::string& endpoint, const std::string& message) {
    httplib::Client cli(url.c_str());

    auto res = cli.Post(endpoint.c_str(), message, "text/plain");
    
    if (res) {
        if (res->status == 200) {
            return res->body;
        } else {
            return "ERROR: Status " + std::to_string(res->status);
        }
    } else {
        auto err = res.error();
        return "Connection failed. Error code: " + std::to_string((int)err) + "\n";
    }
}
