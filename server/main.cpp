#include <iostream>

#include "app/server/server.h"
#include "app/server/parse_request.cpp"
#include "app/server/listen_port.cpp"
#include "app/server/create_response.cpp"

#include "app/routes/hello.h"

int main(){
    Server* runtime = new Server();

    runtime->GET("/hello", helloRoute);

    runtime->listen_port("8080");

    return 0;
}