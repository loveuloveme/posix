#ifndef PARSEREQUESTRESULT
#define PARSEREQUESTRESULT

#include "Query.cpp"
#include "httpparser/request.h"

using namespace httpparser;

class ParseRequestResult{
    public:
    Query params;
    Request request;
    int status;

    ParseRequestResult(Query params_, Request request_){
        params = params_;
        request = request_;
        status = 1;
    }

    ParseRequestResult(){
        status = 0;
    }
};

#endif