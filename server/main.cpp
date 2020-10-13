#include <iostream>

#include "app/server/server.h"
#include "app/server/parse_request.cpp"
#include "app/server/listen_port.cpp"

int main(){
    Server* runtime = new Server();
    runtime->listen_port("8080");

    return 0;
}