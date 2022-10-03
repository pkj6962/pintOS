#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int num[4];
    for(int i = 0; i < argc-1; i++)
    {
        num[i] = atoi(argv[i+1]); 
        printf("%d\n", num[i]); 
    }

    int fibo = fibonacci(num[0]);
    int max = max_of_four_int(num[0], num[1], num[2], num[3]); 

    printf("%d %d\n", fibo, max); 

}