#include "server.h";
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../utils/ParseRequestResult.cpp"

void Server::listenPort(char* port_){
    port = port_;

    status = getaddrinfo("127.0.0.1", "2020", &hints, &server_info);

    if(status < 0){
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
        printf("  %s: %sn \n", ipver, ipstr);
    }

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

        
            
        if(listen(sockfd, 10) == -1){
            perror("listen()");
            break;
        }

        int csockfd = -1;

        while(true){        
            socklen_t addr_size = sizeof(their_addr);

            csockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

            char buffer[1024];
            int flags = 0; // no flags
            size_t length = sizeof(buffer);
            ssize_t bytes_recv = recv(csockfd, (void *)buffer, length, flags);

            ParseRequestResult request;

            if(bytes_recv > 0){
                cout << "Request" << endl;
                request = this->parseRequest(buffer, bytes_recv);
            }else if(bytes_recv == 0){
                //printf("end");
            }else{
                perror("recv()");
            }

            ssize_t sended_length = 0;
            if(request.request.method == "GET"){
                if(routes.count(request.params.url) == 1){
                    sended_length = routes[request.params.url].action(request, csockfd, output);
                }else{
                    const char* header = createResponse("404", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \"not found\"}}");
                    length = strlen(header);
                    flags = 0x00;
                    sended_length= send(csockfd, header, length, flags);
                }
            }else{
                const char* header = createResponse("405", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \"not maintained\"}}");
                length = strlen(header);
                flags = 0x00;
                sended_length= send(csockfd, header, length, flags);
            }
        
            if(sended_length == -1) {
                perror("send()");
            }else{
                //printf("Sent %ld characters\n",bytes_sent);
            }

            close(csockfd);
        }
    }

    freeaddrinfo(server_info);
}