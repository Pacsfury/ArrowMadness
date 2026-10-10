#include <iostream>
#include <string>
#include "../httplib/httplib.h"

inline constexpr const char* SERVER_URL = "localhost:8080";

std::string get(const std::string& url, const std::string& endpoint);

std::string post(const std::string& url, const std::string& endpoint, const std::string& message);