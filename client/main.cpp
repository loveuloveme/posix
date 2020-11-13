#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sstream>

#include "rang.hpp"

using namespace std;
using namespace rang;

string GET(string url){
    char buf[1024];
    string header = "";

    header += "GET ";
    header += url;
    header += (string)" HTTP/1.1" + "\r\n";
    header += "\r\n";

    int sock;
    struct sockaddr_in addr;
    struct hostent* raw_host;

    raw_host = gethostbyname("localhost");

    if(raw_host == NULL){
        cout<<"ERROR, no such host";
        exit(0);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(2020);
    
    bcopy( (char*)raw_host->h_addr, (char*)&addr.sin_addr, raw_host->h_length );

    int err; 
    err = connect( sock, (struct sockaddr *)&addr, sizeof(addr) );
    //perror("connect");

    char * message = new char[header.size()];

    for(int i = 0; i < header.size(); i++){
        message[i] = header[i];
    }

    send(sock, message, header.size(), 0);
    memset(buf, 0, 255);
    recv(sock, buf, sizeof(buf), 0);
    
    string answer = "";

    for(int j = 0; j < 1024; j++){
        answer += buf[j];
    }


    return answer;
}

void menu(){
    cout << style::bold << fgB::gray << "Commands: " << style::reset;
    cout << fgB::gray << "1. ps" << style::reset;
    cout << fgB::gray << "2. launch" << style::reset;
    cout << fgB::gray << "3. signal" << style::reset;

    string input;

    cout << style::bold << fgB::blue << "Command: " << style::reset;
    cin >> input;
    
    if(input == "ps"){
        string data = GET("/launch/?exec=processes/ProcessesList/output&params=[]&uid=10&real=0");
        cout << data << endl;
    }else if(input == "launch"){
        string name;
        string params;
        string uid;
        string save;

        cout << style::bold << fgB::red << "Name: " << style::reset;
        cin >> name;

        cout << style::bold << fgB::red << "Params: " << style::reset;
        cin.ignore();
        getline(cin, params);

        cout << style::bold << fgB::red << "Uid: " << style::reset;
        cin >> uid;

        cout << style::bold << fgB::red << "Return output" << style::reset <<  style::bold << "[y/n]" << fgB::red << ": " << style::reset;
        cin >> save;

        string paramStr = "[";

        vector<string> paramVec;
        paramVec.push_back(string());

        for(int i = 0; i < params.length(); i++){
            if(params[i] == ' '){
                paramVec.push_back(string());
            }else{
                paramVec[paramVec.size() - 1].push_back(params[i]);
            }
        }

        for(int i = 0; i < paramVec.size(); i++){
            paramStr += "\""+paramVec[i] + "\"";
            if(paramVec.size() - 1 != i) paramStr += ',';
        }
        paramStr += "]";
        
        string isReal;
        if(save == "y") isReal = "0";
        if(save == "n") isReal = "1";
        if(paramStr == "[\"\"]") paramStr = "[]";
        string data = GET("/launch/?exec="+name+"&params="+paramStr+"&uid="+uid+"&real="+isReal);
        cout << data << endl;
    }else if(input == "signal"){
        string pid;
        string signalId;

        cout << style::bold << fgB::red << "Pid: " << style::reset;
        cin >> pid;

        cout << style::bold << fgB::red << "Signal Id: " << style::reset;
        cin >> signalId;

        string data = GET("/signal/?pid="+pid+"&signalId="+signalId);
        cout << data << endl;
    }else{
        cout << "Unknown command" << endl;
    }

    menu();
}

int main(){
    menu();
    //GET("http://localhost:8080/");
    return 0;
}