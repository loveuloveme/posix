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

ssize_t actionLaunch(ParseRequestResult query, int socket, map<int, string> &server_output){
    ssize_t bytes_sent;

    string executable;
    string uid;
    string real;
    string params;
    string isReal;

    executable = query.params.params["exec"];
    uid = query.params.params["uid"];
    real = query.params.params["real"];
    params = query.params.params["params"];

    if(uid == "" || real == "" || params == "" || executable == ""){
        const char* header = Server::createResponse("400", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \" \", \"data\":\"not enough parameters\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
    }

    Document argDocument;//"[\"google.com\",\"-t\",\"5\"]"

    parseQuotes(params);

    argDocument.Parse(params.c_str());  

    int paramCount = argDocument.Size();
    char** arguments = new char*[paramCount + 2];

    for(int i = 1; i < paramCount + 1; i++){
        arguments[i] = (char*)argDocument[i - 1].GetString();
    }

    arguments[0] = (char*)executable.c_str();
    arguments[paramCount + 1] = NULL;

    for(int i = 0; i < paramCount + 1; i++){
        //cout << i << arguments[i] << endl;
    }


	int fd2[2];

    bool realTime = false;

    if(real == "1"){
        realTime = true;
    }else if(real == "0"){
        realTime = false;
    }else{
        cout << "invalid parameter: " << isReal << endl;
        exit(-1);
    }

    cout << realTime << endl;

	pid_t p; 

	if(pipe(fd2) == -1){ 
		fprintf(stderr, "Pipe Failed"); 
		return 1; 
	}

	p = fork(); 

	if (p < 0){ 
		fprintf(stderr, "fork Failed"); 
		return 1; 
	} 

	else if (p > 0){ 
		char concat_str[100]; 
		close(fd2[1]);
        if(!realTime){
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
            const char* header = Server::createResponse("200", "OK", "{\"status\": \"0\", \"response\": {\"error\": \" \", \"data\":{\"filename\": \""+executable+"\"}}}");
            int length = strlen(header);
            int flags = 0x00;
            bytes_sent = send(socket, header, length, flags);
            close(socket);

            //return bytes_sent;

            wait(NULL);
            
            char buf;
            string output;

            while(read(fd2[0], &buf, 1)){
                output.push_back(buf);
            }

            string date = to_string(chrono::system_clock::to_time_t(chrono::system_clock::now()));
            string filename("logs/"+string("[")+date+string("]")+string(executable.c_str()));
            int file = creat(filename.c_str(), O_RDWR);
            
            write(file, output.c_str(), strlen(output.c_str()));
            close(fd2[0]); 
            return bytes_sent;
        }
	}else{
        setuid(atoi(uid.c_str()));

		close(fd2[0]); 
        //cout << executable << endl;
        dup2(fd2[1], 1);
        //write(fd2[1], "hehe", strlen("hehe"));
        execvp(executable.c_str(), arguments);
        //execl("/sbin/ping", "ping", "-t", "1", "google.com", NULL);
        perror("execvp");
        
        exit(-1);
		
	} 
}

Route launchRoute("launch", actionLaunch);