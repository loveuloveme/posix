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

using namespace std;

int main(void) {
    struct kinfo_proc kp[1000] = {};
	size_t len = sizeof(kp);
    int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC};
	sysctl(mib, 3, &kp, &len, NULL, 0);
    cout << kp << endl;
    for(int i = 0; i < 1000; i++){
        cout << kp[i].ki_ppid << endl;
    }
    return -1;
}