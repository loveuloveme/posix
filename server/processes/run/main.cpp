#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <iostream> 
#include <string> 
#include <signal.h> 

using namespace std;

int main(){     
    int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

    bool realTime = true;

	pid_t p; 

	if(pipe(fd1) == -1){ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	}

	if(pipe(fd2) == -1){ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	}

	p = fork(); 

	if (p < 0){ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	else if (p > 0){ 
		char concat_str[100]; 

		close(fd1[0]);
		close(fd1[1]); 

		close(fd2[1]);

        if(!realTime){
            wait(NULL);
            
            char buf;
            string output;

            while(read(fd2[0], &buf, 1)){
                output.push_back(buf);
            }

            cout << output << endl;
            // read(fd2[0], concat_str, 100); 
            // printf("Concatenated string %s\n", concat_str); 
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
		close(fd1[1]); // Close writing end of first pipe 
		close(fd1[0]); 
		close(fd2[0]); 
        dup2(fd2[1], 1);
        write(fd2[1], "hehe", strlen("hehe"));
        execl("/sbin/ping", "ping", "-t", "1", "google.com", NULL);
        //close(fd2[1]); 
        
        exit(0);
		
	} 
} 
