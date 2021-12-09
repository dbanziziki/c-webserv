#include <iostream>
#include <sstream>
#include <vector>

#include "HttpRequest.hpp"

int main(int argc, char const *argv[]) {
    std::string req =
        "GET /www HTTP/1.1\nHost: localhost:8080\nConnection: "
        "keep-alive\nCache-Control: max-age=0\nUpgrade-Insecure-Requests: "
        "1\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) "
        "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 "
        "Safari/537.36\nAccept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\nSec-Fetch-Site: none\nSec-Fetch-Mode: "
        "navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: "
        "document\nAccept-Encoding: gzip, deflate, br\nAccept-Language: "
        "en-US,en;q=0.9";

    HttpRequest request(req.c_str());
    std::cout << "." + request._file_name << std::endl;
    return 0;
}
