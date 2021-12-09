#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>
#include <map>

class HttpRequest {
   public:
    const std::string _request;
    std::string _file_name;
    std::string _method;
    std::string _http_version;
    std::map<std::string, std::string> _headers;

   public:
    HttpRequest(const char *request);
    ~HttpRequest();

   private:
    void parse_request();
};

std::ostream &operator<<(std::ostream &os, HttpRequest const &rhs);

#endif