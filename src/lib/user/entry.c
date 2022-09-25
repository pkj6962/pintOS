#include <syscall.h>

int main (int, char *[]);
void _start (int argc, char *argv[]);

void
_start (int argc, char *argv[]) 
{
  //printf("왜 이 라인은 호출이 안될까?\n")
  exit (main (argc, argv));
}
