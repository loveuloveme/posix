#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <iostream> 
#include <string> 
#include <signal.h> 
#include <vector> 

using namespace std;

int main(int argc, char** argv){
    string executable;
    string uid;
    string isReal;
    
    char** arguments;

    for(int i = 0; i < argc; i++){
        //cout << argv[i] << endl;
    }

    try{
        if(argc < 3){
            return -1;
        }

        uid = argv[0];
        executable = argv[2];
        isReal = argv[1];
    
        if(argc - 3 > 0){
            arguments = new char*[argc - 1];

            for(int i = 3; i < argc; i++){
                arguments[i - 2] = argv[i];
            }
        }
    }catch(exception e){
        cout << "Error" << endl;
        throw e;
        return -1;
    }

    arguments[0] = argv[2];
    arguments[argc - 2] = NULL;

    // for(int i = 0; i < argc - 3; i++){
    //     cout << arguments[i] << endl;
    // }

    // cout << "executable: " << executable << endl;
    // cout << "uid: " << uid << endl;
    // cout << "isReal: " << isReal << endl;

	int fd2[2]; // Used to store two ends of second pipe 

    bool realTime = false;

    if(isReal == "--real"){
        realTime = true;
    }else if(isReal == "--background"){
        realTime = false;
    }else{
        cout << "invalid parameter: " << isReal << endl;
        exit(-1);
    }

	pid_t p; 

	if(pipe(fd2) == -1){ 
		fprintf(stderr, "Pipe Failed"); 
		return 1; 
	}

	p = fork(); 

	if (p < 0){ 
		fprintf(stderr, "fork Failed"); 
		return 1; 
	} 

	else if (p > 0){ 
		char concat_str[100]; 
		close(fd2[1]);
        if(!realTime){
            wait(NULL);
            
            char buf;
            string output;

            while(read(fd2[0], &buf, 1)){
                output.push_back(buf);
            }

            fprintf(stdout, output.c_str());// << output << endl;
            close(fd2[0]); 
        }else{
            char buf;
            bool exit = false;
            int status;
            int app_status;
            string output;
            while(1){
                while(read(fd2[0], &buf, 1)){
                    app_status =  waitpid(p, &status, WNOHANG);
                    printf("");

                    output.push_back(buf);

                    cout << output << endl;

                    if(app_status == -1){
                        exit = true;
                        break;
                    }
                }

                if(exit){
                    kill(p, SIGKILL);
                    break;
                }
            }
        }
	}else{
        setuid(atoi(uid.c_str()));

		close(fd2[0]); 
        //cout << executable << endl;
        dup2(fd2[1], 1);
        //write(fd2[1], "hehe", strlen("hehe"));
        execvp(executable.c_str(), arguments);
        //execl("/sbin/ping", "ping", "-t", "1", "google.com", NULL);
        perror("execvp");
        
        exit(-1);
		
	} 
} 
