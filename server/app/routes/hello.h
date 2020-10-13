#include <iostream>

#include "../server/route.h";
#include "../utils/queryparser/Query.cpp"


void action(Query query){
    cout << "lol" << endl;
}

Route helloRoute = Route("lol", action);