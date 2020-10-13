#include "server.h";

const char* Server::create_response(string code, string status, string json_string){
    string header = "HTTP/1.1 "+code+" "+status+"\r\nContent-Type: application/json; charset=UTF-8\r\n\r\n"+json_string+"\r\n";
    return header.c_str();
}