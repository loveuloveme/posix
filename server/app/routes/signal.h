#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include <algorithm>
#include <pthread.h>
#include <chrono>
#include <ctime> 
#include <sys/wait.h>

#include "../server/route.h";
#include "../utils/Query.cpp"

#include "../utils/ParseRequestResult.cpp";
#include "../utils/rapidjson/document.h"
#include "../utils/rapidjson/writer.h"
#include "../utils/rapidjson/stringbuffer.h"

#include "../server/server.h";

using namespace rapidjson;
using namespace std;

ssize_t actionSignal(ParseRequestResult query, int socket, map<int, string> &server_output){
    Server::Log("Got signal request");

    ssize_t bytes_sent;
    int pid;
    int signalId;

    pid = atoi(query.params.params["pid"].c_str());
    signalId = atoi(query.params.params["signalId"].c_str());

    if(query.params.params["pid"] == "" || query.params.params["signalId"] == ""){
        const char* header = Server::createResponse("400", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"not enough parameters\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
    }

    int returnCode = kill(pid, signalId);

    const char* header = Server::createResponse("200", "OK", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\" "+to_string(returnCode)+" \"}}");
    int length = strlen(header);
    int flags = 0x00;
    bytes_sent = send(socket, header, length, flags);
    return bytes_sent;
}

Route signalRoute("signal", actionSignal);