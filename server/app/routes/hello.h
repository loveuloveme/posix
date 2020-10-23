#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "../server/route.h";
#include "../utils/Query.cpp"

#include "../utils/ParseRequestResult.cpp";

#include "../server/server.h";

using namespace std;

ssize_t actionHello(ParseRequestResult query, int socket, map<int, string> &server_output){
    const char* header = Server::createResponse("200", "OK", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"Hello\"}}");
    int length = strlen(header);
    int flags = 0x00;
    ssize_t bytes_sent = send(socket, header, length, flags);

    return bytes_sent;
}

Route helloRoute("lol", actionHello);