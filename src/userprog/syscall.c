#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

// should be deleted afeter debuggin 
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int number = *((int*)(f->esp)); 

  switch(number)
  {
    case(SYS_EXIT):
      sys_exit(f); 


  }

  printf ("system call!\n");
  
  hex_dump(f->esp, f->esp, PHYS_BASE - f->esp, true); 

  thread_exit ();
}


void
sys_exit(struct intr_frame* f)
{
  int status = *((int*)f->esp + 1);
  
  thread_exit(); 

  // status 를 shared resource에 저장하는 코드 구현 필요


}