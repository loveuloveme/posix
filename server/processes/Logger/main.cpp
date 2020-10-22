#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>


volatile bool newEvent = false;
bool finish = false;
int *last_shm_memory;
void sighandler(int number) 
{
	newEvent = true;
}

void terminator(int number) 
{
	finish = true;
}


struct LogObject {
	std::string shm_name;
	std::string file_name;
	int fd;
	void* memory;
};
std::vector<LogObject> logs;

using namespace std;
int main(int argc, char** argv) {
	char* dataPtr;
    int dataFd = shm_open("/LOG",  O_CREAT | O_RDWR, 0666);
    ftruncate(dataFd, 1024);
    dataPtr = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, dataFd, 0);
    close(dataFd);
    memcpy(dataPtr, "lol", strlen("lol"));
    
    char* lastData;
    
    while(1){
        cout << dataPtr << endl;
    }
    return 0;
}