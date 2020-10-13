#ifndef ROUTER
#define ROUTER

#include "../utils/ParseRequestResult.cpp";

using namespace std;

class Route{
    public:
    string name;
    
    ssize_t (*action)(ParseRequestResult, int socket) = NULL; 

    Route(string name_, ssize_t (*action_)(ParseRequestResult, int socket)){
        name = name_;
        action = action_;
    }

    Route(){}
};

#endif