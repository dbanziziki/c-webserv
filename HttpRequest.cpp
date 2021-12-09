#include "HttpRequest.hpp"

#include <sstream>

HttpRequest::HttpRequest(const char *request) : _request(request) {
    parse_request();
}

HttpRequest::~HttpRequest() {}

void HttpRequest::parse_request() {
    std::istringstream iss(_request);
    std::string line;
    std::string key;
    std::string value;

    std::getline(iss, line);
    std::istringstream l_iss(line);
    l_iss >> _method >> _file_name >> _http_version;
    if (_file_name == "/") _file_name = "/index.html";
    while (getline(iss, line)) {
        std::istringstream temp_iss(line);
        temp_iss >> key >> value;
        _headers[key] = value;
    }
}

std::ostream &operator<<(std::ostream &os, HttpRequest const &rhs) {
    os << rhs._method << " " << rhs._file_name << " " << rhs._http_version
       << std::endl;
    return os;
}