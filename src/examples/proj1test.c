#include <stdio.h>
#include <syscall.h>

int main(void)
{
    char buffer[20] ; 
    // char* file_name = 'hellowerd';
    printf("hello world! from proj1test!\n"); 
    // unsigned size = read(0, buffer, 5);
    pid_t c = exec("no-such-file"); 
    // printf("syscall return: %d", size); 

}

