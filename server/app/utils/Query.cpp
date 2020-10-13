#ifndef QUERY
#define QUERY

#include <map>
#include <string>

using namespace std;

class Query{
    public:
    map<string, string> params;
    string url = "/";

    Query(){}

    Query(string query){
        bool isName = false;
        bool isData = false;

        string name;
        string data;

        for(char& c : query) {
            if(c == '?' || c == '&'){
                if(isData){
                    params.insert(pair<string, string>(name, data));
                    name = "";
                    data = "";
                }
                isName = true;
                isData = false;
                continue;
            }

            if(c == '='){
                isName = false;
                isData = true;
                continue;
            }

            if(!isName && !isData && c != '/'){
                url.push_back(c);
            }

            if(isName){
                name.push_back(c);
            }

            if(isData){
                data.push_back(c);
            }
        }

        if(name != ""){
            params.insert(pair<string, string>(name, data));
        }
    }
};

#endif