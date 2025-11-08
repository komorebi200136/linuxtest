#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define __NR_helloworld 441

int main(int argc, char **argv){
    printf("Calling helloworld syscall...\n");
    
    long result = syscall(__NR_helloworld);
    
    if (result == 0) {
        printf("Syscall returned successfully: %ld\n", result);
    } else {
        printf("Syscall failed with error: %ld, errno: %d (%s)\n", 
               result, errno, strerror(errno));
    }
    
    return 0;
}
