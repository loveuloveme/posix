#ifndef LOG
#define LOG

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
#include <chrono>
#include <ctime>  

#include "server.h";

void Server::Log(string str){
    string date = to_string(chrono::system_clock::to_time_t(chrono::system_clock::now()));

	char* dataPtr;
    int dataFd = shm_open("/LOG",  O_CREAT | O_RDWR, 0666);
    ftruncate(dataFd, 1024);

    dataPtr = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, dataFd, 0);
    close(dataFd);
    
    memcpy(dataPtr, str.c_str(), strlen(str.c_str()));
    shm_unlink("/LOG");
}

#endif