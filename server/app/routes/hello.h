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

ssize_t action(ParseRequestResult query, int socket){
    const char* header = Server::create_response("200", "OK", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"Hello\"}}");
    int length = strlen(header);
    int flags = 0x00;
    ssize_t bytes_sent = send(socket, header, length, flags);

    return bytes_sent;
}

Route helloRoute("lol", action);