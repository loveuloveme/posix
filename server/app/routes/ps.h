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

void parseQuotes(string& params){
    for(int i = 0; i < params.length(); i++){
        if(params[i] == '%' && params[i + 1] == '2' && params[i + 2] == '2'){
            params.replace(i, 3, "\"");
        }
    }
}

void *sendThreadFun(void *ptr){
    cout << "haha" << endl;
}

ssize_t actionPs(ParseRequestResult query, int socket, map<int, string> &server_output){
    ssize_t bytes_sent;


    const char* header = Server::createResponse("400", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"not enough parameters\"}}");
    int length = strlen(header);
    int flags = 0x00;
    bytes_sent = send(socket, header, length, flags);
    return bytes_sent;


    Document argDocument;//"[\"google.com\",\"-t\",\"5\"]"

	int fd2[2];

	if(pipe(fd2) == -1){ 
		fprintf(stderr, "Pipe Failed"); 
		return 1; 
	}

	p = fork(); 

	if(p < 0){ 
		fprintf(stderr, "fork Failed"); 
		return 1; 
	} 

	else if(p > 0){ 
		close(fd2[1]);
        wait(NULL);
        char buf;
        string output;

        while(read(fd2[0], &buf, 1)){
            output.push_back(buf);
        }

        const char* header = Server::createResponse("200", "OK", "{\"status\": \"0\", \"response\": {\"error\": \" \", \"data\":{\"output\":"+output+"\"}}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;

        fprintf(stdout, output.c_str());// << output << endl;
        close(fd2[0]); 
	}else{
		close(fd2[0]); 
        dup2(fd2[1], 1);
        execl("../processes/ProcessesList/output", "ps");
        perror("execl");
        exit(-1);
		
	} 
}

Route psRoute("ps", actionPs);