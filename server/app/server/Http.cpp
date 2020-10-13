#include <string>
#include <map>

using namespace std;

class Http{
    public:
    class Property{
        string name;
        string data;

        public:
        Property(string name, string data){

        }
    };

    map<string, Property> property;

    Http(char* buffer){

        string request(buffer);
        
        string name = "";
        string data = "";
        string body = "";

        bool isData = false;
        bool isBody = false;
        for(int i = 0; i < request.length(); i++){
            try{
                if(request[i] == '\r' && request[i + 1] == '\n' && request[i + 2] == '\r' && request[i + 3] == '\n'){
                    isBody = true;
                }
            }catch(exception e){}

            if(!isBody){
                if(request[i] == ' '){
                    isData = true;
                }

                if(request[i] == '\r' && request[i + 1] == '\n'){
                    property.insert(pair<string, Property>(name, Property(name, data)));
                    i += 2;
                }

                if(!isData){
                    name.push_back(request[i]);
                }else{
                    data.push_back(request[i]);
                }
            }else{
                body.push_back(request[i]);
            }
        }
    }
};