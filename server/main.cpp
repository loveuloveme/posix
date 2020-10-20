#include <iostream>

#include "app/server/server.h"
#include "app/server/parseRequest.cpp"
#include "app/server/listenPort.cpp"
#include "app/server/createResponse.cpp"

#include "app/routes/hello.h"
#include "app/routes/launch.h"
#include "app/routes/output.h"

int main(){
    Server* runtime = new Server();

    runtime->GET("/hello", helloRoute);
    runtime->GET("/launch", launchRoute);
    runtime->GET("/output", outputRoute);

    runtime->listenPort("8080");

    return 0;
}