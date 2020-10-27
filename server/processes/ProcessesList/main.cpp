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
#include <unordered_set>
#include <kvm.h>
#include <dirent.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/proc.h>
#include <sys/param.h>
#include <libprocstat.h>
#include <sys/user.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;

class PsProcess{
    public:
    pid_t pid;
    pid_t parentPid;
    char* name;

    PsProcess(pid_t pid_, pid_t parentPid_, char* name_){
        pid = pid_;
        parentPid = parentPid_;
        name = name_;
        //print();
    }

    void print(){
        printf("%d %d %s\n", pid, parentPid, name);
    }
};

class Ps{
    private:
    map<pid_t, PsProcess> processes;

    public:
    int maxProcessCount = 1044;

    Ps(){
        struct kinfo_proc processInfo[maxProcessCount] = {};
        size_t len = sizeof(processInfo);
        int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC};
        sysctl(mib, 3, &processInfo, &len, NULL, 0);
        cout << processInfo << endl;
        for(int i = 0; i < maxProcessCount; i++){
            if(processInfo[i].ki_pid != 0){
                processes.insert(pair<pid_t, PsProcess>(processInfo[i].ki_pid, PsProcess(processInfo[i].ki_pid, processInfo[i].ki_ppid, processInfo[i].ki_comm)));
            }
        }
    }

    void print(){
        printf("Processes: \n");
        for (auto it = processes.begin(); it != processes.end(); it++){
            std::cout << it->first  // string (key)
                    << ':'
                    << it->second   // string's value 
                    << std::endl ;
        }
    }
};

int main(void) {
    Ps psRun;
    psRun.print();
    return -1;
}