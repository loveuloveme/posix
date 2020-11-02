#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    int logFile = open("server.log", O_CREAT | O_APPEND | O_WRONLY, 0777);
	char* dataPtr;
    int dataFd = shm_open("/LOG",  O_CREAT | O_RDWR, 0666);
    ftruncate(dataFd, 1024);
    dataPtr = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, dataFd, 0);
    close(dataFd);

    //memcpy(dataPtr, "kek", strlen("lol"));
    
    string lastData;
    while(1){
        if(lastData != string(dataPtr)){
            int i = 0;
            string str;
            while(dataPtr[i] != NULL){
                str.push_back(dataPtr[i]);
                i++;
            }

            str.push_back('\n');

            write(logFile, str.c_str(), strlen(str.c_str()));
            lastData = string(dataPtr);
        }
    }

    return 0;
}