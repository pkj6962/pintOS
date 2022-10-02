#include <stdio.h>
#include <syscall.h>

int main(void)
{
    char buffer[20]; 
    char file_name[] = "echo";
    printf("hello world! from proj1test!\n"); 
    // unsigned size = read(0, buffer, 5);
    // pipid_t c = exec(file_name); 
    // printf("syscall return: %d", size); 

}