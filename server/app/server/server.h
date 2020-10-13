#ifndef SERVER
#define SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <unistd.h>  
#include <map>

#include "route.h"

#include "../utils/ParseRequestResult.cpp"

using namespace std;

class Server{
    addrinfo* server_info;
    addrinfo hints;
    const char* hostname = "localhost";
    char* port = "8080";
    struct sockaddr_storage their_addr;
    int status = -1;
    map<string, Route> routes;
    
    public:
    Server(){
        hints.ai_family = AF_UNSPEC;     //IPv4(AF_INET) или IPv6  
        hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
        hints.ai_flags = AI_PASSIVE;
    }


    void GET(string name, void (*action_)(void)){
        if(routes.count(name) == 1){
            routes[name] = Route(name, action_);
        }else{
            routes.insert(pair<string, Route>(name, Route(name, action_)));
        }
    }

    void get(string name){
        cout << "GET: " << name << endl;
    }

    ParseRequestResult parse_request(char* buffer, ssize_t bytes_recv);
    void listen_port(char* port_);
};


#endif