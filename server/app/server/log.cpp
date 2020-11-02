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

#include <iostream>

#include <chrono>
#include <ctime>

#include "server.h";

void Server::Log(string str){
	char* dataPtr;
    int dataFd = shm_open("/LOG",  O_CREAT | O_RDWR, 0666);
    ftruncate(dataFd, 1024);

    dataPtr = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, dataFd, 0);
    close(dataFd);

    time_t t = time(0);
    tm* now = localtime(&t);
    string str2 = "[" + to_string((now->tm_year + 1900)) + '-' + to_string((now->tm_mon + 1)) + '-' + to_string(now->tm_mday) + ' ' + to_string(now->tm_hour) + ':' + to_string(now->tm_min) + ':' + to_string(now->tm_sec )+ ']' + ' ' + str;
    
    memcpy(dataPtr, str2.c_str(), 1024);
}

#endif