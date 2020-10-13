#include "server.h";

void Server::parse_request(char* buffer, ssize_t bytes_recv){
    fprintf(stdout, "%s \n[%ld chars]\n\n", buffer, bytes_recv);
}