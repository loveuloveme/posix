#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "../server/route.h";
#include "../utils/Query.cpp"

#include "../utils/ParseRequestResult.cpp";
#include "../utils/rapidjson/document.h"
#include "../utils/rapidjson/writer.h"
#include "../utils/rapidjson/stringbuffer.h"

#include "../server/server.h";

#include <algorithm>

using namespace rapidjson;
using namespace std;

void parseQuotes(string& params){
    for(int i = 0; i < params.length(); i++){
        if(params[i] == '%' && params[i + 1] == '2' && params[i + 2] == '2'){
            params.replace(i, 3, "\"");
        }
    }
}

ssize_t actionLaunch(ParseRequestResult query, int socket, map<int, string> &server_output){
    ssize_t bytes_sent;

    string executable;
    string uid;
    string real;
    string params;

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
    char** arguments = new char*[paramCount + 4];

    for(int i = 3; i < paramCount + 3; i++){
        arguments[i] = (char*)argDocument[i - 3].GetString();
    }

    arguments[0] = "0";
    arguments[1] = "--background";
    arguments[2] = (char*)executable.c_str();
    arguments[paramCount + 3] = NULL;

    //./output 0 --background ping google.com -t 5
    int output[2];
    pid_t pid = fork();

    

    if(pipe(output) == -1){ 
		const char* header = Server::createResponse("500", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \"pipe_error\", \"data\":\"\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
	}

    if(pid == 0){
		close(output[0]); 
        dup2(output[1], 1);
        //dup2(err[1], 2);
        
        write(output[1], "hehe", strlen("hehe"));
        //execv("./processes/run/output", arguments);
        perror("execv");
        exit(-1);
    }else if(pid == -1){
        const char* header = Server::createResponse("500", "BAD", "{\"status\": \"1\", \"response\": {\"error\": \"fork_error\", \"data\":\"\"}}");
        int length = strlen(header);
        int flags = 0x00;
        bytes_sent = send(socket, header, length, flags);
        return bytes_sent;
    }else{
		close(output[1]);

        int id = server_output.size();
        server_output.insert(pair<int, string>(id, ""));
            
        if(real == "false"){
            int status;
            int app_status;


            while(waitpid(pid, &status, WNOHANG) != -1){
                cout << "haha" << endl;
            }
            
            char buf;
            string out;

            while(read(output[0], &buf, 1)){
                cout << "kek" << endl;
                out.push_back(buf);
                cout << buf << endl;
            }

            cout << "lol" << endl;
            //cout << out << endl;

            close(output[0]); 

            const char* header = Server::createResponse("200", "OK", "{\"status\": \"0\", \"response\": {\"error\": \" \", \"data\":\"{\"id\": \"\"}\"}}");
            int length = strlen(header);
            int flags = 0x00;
            bytes_sent = send(socket, header, length, flags);
            return bytes_sent;
        }else{
            const char* header = Server::createResponse("200", "OK", "{\"status\": \"0\", \"response\": {\"error\": \" \", \"data\":\"{\"id\": \"id\"}\"}}");
            int length = strlen(header);
            int flags = 0x00;
            bytes_sent = send(socket, header, length, flags);
            return bytes_sent;
        }
    }
}

Route launchRoute("launch", actionLaunch);