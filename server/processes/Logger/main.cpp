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
    int logFile = open("../../server.log",O_CREAT | O_TRUNC | O_APPEND | O_WRONLY, 0777);
	char* dataPtr;
    int dataFd = shm_open("/LOG",  O_CREAT | O_RDWR, 0666);
    ftruncate(dataFd, 1024);
    dataPtr = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, dataFd, 0);
    close(dataFd);

    // memcpy(dataPtr, "kek", strlen("lol"));
    
    char* lastData;
    
    // while(1){
    //     cout << dataPtr << endl;
    // }
    return 0;
}