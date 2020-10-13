#include "server.h";

#include "../utils/httpparser/request.h"
#include "../utils/httpparser/httprequestparser.h"
#include "../utils/httpparser/urlparser.h"
#include "../utils/queryparser/Query.cpp"

using namespace httpparser;

void Server::parse_request(char* buffer, ssize_t bytes_recv){
    Request request;
    HttpRequestParser parser;
    HttpRequestParser::ParseResult res = parser.parse(request, buffer, buffer + strlen(buffer));

    //fprintf(stdout, "%s \n[%ld chars]\n\n", buffer, bytes_recv);
    if(res == HttpRequestParser::ParsingCompleted){
        if(request.method == "GET"){
            Query params(request.uri);

            cout << "GET request " << params.params["hehe"] << endl;
        }else if(request.method == "POST"){
            cout << "POST request" << endl;
        }
    }else{
        cout << "Parse Error" << endl; //Error
    }

    return;
}