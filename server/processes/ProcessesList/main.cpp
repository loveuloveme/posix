#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <pwd.h>
#include <iostream>

using namespace std;

int main(void) {
    struct kinfo_proc kp[1000] = {};
	size_t len = sizeof(kp);
    int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
	sysctl(mib, 3, &kp, &len, NULL, 0);
    cout << kp << endl;
    for(int i = 0; i < 1000; i++){
        cout << kp[i].kp_proc.p_pid << endl;
    }
    return -1;
}