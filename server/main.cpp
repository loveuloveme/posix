#include <iostream>

#include "app/server/server.h"
#include "app/server/parseRequest.cpp"
#include "app/server/listenPort.cpp"
#include "app/server/createResponse.cpp"
#include "app/server/log.cpp"

#include "app/routes/hello.h"
#include "app/routes/launch.h"
#include "app/routes/signal.h"

int main(){

    pid_t LoggerPid = fork();
    if(LoggerPid == 0){
        Server::Log("Hehe");
        execl("./processes/Logger/output", "Logger", NULL);
        perror("execl");
    }else{
        Server* runtime = new Server();

        runtime->GET("/hello", helloRoute);
        runtime->GET("/launch", launchRoute);
        runtime->GET("/signal", signalRoute);

        runtime->listenPort("8080");
    }
    

    return 0;
}