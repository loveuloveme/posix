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

#include "httpparser/request.h"
#include "httpparser/httprequestparser.h"

using namespace std;
using namespace httpparser;

class Server{
    addrinfo* server_info;
    addrinfo hints;

    const char* hostname = "localhost";
    char* port = "8080";
    struct sockaddr_storage their_addr;

    int status = -1;

    HttpRequestParser parser;
    
    public:
    Server(){
        hints.ai_family = AF_UNSPEC;     //IPv4(AF_INET) или IPv6  
        hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
        hints.ai_flags = AI_PASSIVE;
    }

    void listen_port(char* port_){
        port = port_;

        status = getaddrinfo(hostname, port, &hints, &server_info);

        if(status){
            fprintf(stderr, "getaddrinfo error: %sn", gai_strerror(status));
            return;
        }

        addrinfo* server;

        char ipstr[INET6_ADDRSTRLEN];

        for(server = server_info; server != NULL; server = server->ai_next){
            void *addr;
            char *ipver;

            if (server->ai_family == AF_INET){
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)server->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            }else{
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)server->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }

            inet_ntop(server->ai_family, addr, ipstr, sizeof ipstr);
            printf("  %s: %sn", ipver, ipstr);
        }


        int sockfd;

        for(server = server_info; server != NULL; server = server->ai_next){
            sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

            if(sockfd == -1){
                fprintf(stderr, "socket failed, reason: %s\n", strerror(errno));
                continue;
            }

            bind(sockfd, server->ai_addr, server->ai_addrlen);

            int yes = 1;
            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&yes, sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
            }

            int csockfd;


            int bytes;
             
            while(true){
                if(listen(sockfd, 10) == -1){
                    perror("listen()");
                    break;
                }
                        
                socklen_t addr_size = sizeof(their_addr);

                csockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

                char buffer[1024];
                int flags = 0; // no flags
                size_t length = sizeof(buffer);
                ssize_t bytes_recv = recv(csockfd, (void *)buffer, length, flags);

                if(bytes_recv > 0){
                    Request response;
                    HttpRequestParser::ParseResult res = parser.parse(response, buffer, buffer + strlen(buffer));

                    if(res == HttpRequestParser::ParsingCompleted){
                        cout << response.inspect() << endl;
                    }else{
                        cerr << "Parsing failed" << endl;
                    }

                    //fprintf(stdout, "", buffer, bytes_recv);
                }else if(bytes_recv == 0){
                    //printf("the remote side has closed the connection on you!");
                }else{
                    perror("recv()");
                }
            
                const char header[] =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8"
                "\r\n\r\n"
                "<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>"
                "\r\n";
            
                length = strlen(header); // We want to transmit the null character
                flags = 0x00;
                ssize_t bytes_sent = send(csockfd, header, length, flags);
            
                if(bytes_sent == -1) {
                    perror("send()");
                }else{
                    printf("Sent %ld characters\n",bytes_sent);
                }

                printf("--\n");
                
                close(csockfd);
            }
        }
    
        freeaddrinfo(server_info);
    }
};