using namespace std;

class Route{
    public:
    string name;
    void (*action)(void) = NULL; 

    Route(string name_, void (*action_)(void)){
        name = name_;
        action = action_;
    }

    Route(){}
};