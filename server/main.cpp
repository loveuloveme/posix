#include <iostream>

#include "app/server.h"

int main(){
    Server* runtime = new Server();
    runtime->listen_port("8080");

    return 0;
}