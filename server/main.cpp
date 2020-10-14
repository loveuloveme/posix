#include <iostream>

#include "app/server/server.h"
#include "app/server/parseRequest.cpp"
#include "app/server/listenPort.cpp"
#include "app/server/createResponse.cpp"

#include "app/routes/hello.h"
#include "app/routes/launch.h"

int main(){
    Server* runtime = new Server();

    runtime->GET("/hello", helloRoute);
    runtime->GET("/launch", launchRoute);

    runtime->listenPort("8080");

    return 0;
}