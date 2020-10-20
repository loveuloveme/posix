#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "../server/route.h";
#include "../utils/Query.cpp"

#include "../utils/ParseRequestResult.cpp";

#include "../server/server.h";

using namespace std;

ssize_t actionOutput(ParseRequestResult query, int socket, map<int, string> &server_output){
    ssize_t bytes_sent;
    string id_ = query.params.params["id"];
    
    if(id_ == ""){
        const char* header = Server::createResponse("400", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"not enough parameters\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
    }

    int id = atoi(id_.c_str());

    if(server_output.count(id) == 1){
        const char* header = Server::createResponse("200", "OK", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\""+server_output[id]+"\"}}");
        int length = strlen(header);
        int flags = 0x00;
        ssize_t bytes_sent = send(socket, header, length, flags);
    }else{
        const char* header = Server::createResponse("400", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"id does not exist\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
    }

    return bytes_sent;
}

Route outputRoute("lol", actionOutput);