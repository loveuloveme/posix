#include "server.h";

#include "../utils/httpparser/request.h"
#include "../utils/httpparser/httprequestparser.h"
#include "../utils/httpparser/urlparser.h"
#include "../utils/Query.cpp"

using namespace httpparser;

ParseRequestResult Server::parseRequest(char* buffer, ssize_t bytes_recv){
    Request request;
    HttpRequestParser parser;
    HttpRequestParser::ParseResult res = parser.parse(request, buffer, buffer + strlen(buffer));

    //fprintf(stdout, "%s \n[%ld chars]\n\n", buffer, bytes_recv);

    if(res == HttpRequestParser::ParsingCompleted){
        return ParseRequestResult(Query(request.uri), request);
    }else{
        cout << "Parse Error" << endl; //Error
    }

    return ParseRequestResult();
}