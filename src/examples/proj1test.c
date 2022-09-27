#include <stdio.h>
#include <syscall.h>

int main(void)
{
    char buffer[20]; 
    read(0, buffer, 5); 
    printf("buffer!!: %s\n", buffer);

}