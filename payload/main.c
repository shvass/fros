
// #include <stdio.h>
// #include <stdlib.h>
#include <payload-array.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    
    int fd = open("dump.bin", O_WRONLY | O_CREAT);
    printf("opened file fd : %d\n", fd);

    int ret = write(fd, payload_elf, payload_elf_len);
    printf("writte bytes : %d\n", ret);

    close(fd);    

    return 0;
}