#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

// additional module
// #include "lib/user/syscall.h"

// should be deleted after debugging 
#include "threads/vaddr.h"
#include "filesys/filesys.h"

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

  void* esp = f->esp; 
  void** eax = &(f->eax); 
  
  switch(number)
  {
    case(SYS_EXIT):
      sys_exit(esp); 
      NOT_REACHED(); 
      break;
    case(SYS_WRITE):
      *(int**)eax = sys_write(esp); 
      break;
    case(SYS_READ):
      *(int**)eax = sys_read(esp); 
      break; 
    case(SYS_EXEC):
      *(int**)eax = sys_exec(esp); 

  }
  

  // thread_exit ();
}


void
exit (int status)
{
  syscall1 (SYS_EXIT, status); 
  NOT_REACHED (); 
}


void 
check_address(void *addr)
{
  // null인 경우, unmapped된 경우, kernel address인 경우 
  struct thread *t = thread_current();   
  if(!is_user_vaddr(addr) || addr == NULL) // addr is kernel address or null 
  {
    printf("%s: exit(-1)\n", t->name); 
    thread_exit(-1); 
    // exit(-1); 
  }
  if(pagedir_get_page(t->pagedir , addr) == NULL) // addr is unmapped
  {  
    // exit(-1); 
    printf("%s: exit(-1)\n", t->name); 
    thread_exit(-1); 
  }

}

bool
check_file(char * file_name)
{
  struct file * f =filesys_open(file_name);
  return (f == NULL)? false: true;  
}


void
sys_exit(void* esp)
{
  int status = *((int*)esp + 1);
  
  struct thread * t = thread_current(); 
  char* thread_name = t->name; 

  printf("%s: exit(%d)\n", thread_name, status); 

  thread_exit(status); 

  // status 를 shared resource에 저장하는 코드 구현 필요

}


unsigned
sys_write(void* esp)
{
  unsigned size = *((int*)esp + 3);
  const void* buffer = *((int*)esp + 2);
  int fd = *((int*)esp + 1); 

  ASSERT(fd == 1); // in project1, write system call only writes to stdout.

  check_address(buffer);

  putbuf(buffer, size); 

  return size; 
  // write systemcall은 실제로 출력한 글자수를 반환해야 한다. 

}

unsigned
sys_read(void *esp)
{
  unsigned size = *((int*)esp + 3); 
  void* buffer = *((int*)esp + 2); 
  int fd = *((int*)esp + 1);
  char c, CR = 13; 
  int idx = 0; 

  int MAX_SIZE = 128; 
  char tempBuffer[MAX_SIZE]; 

  ASSERT(fd == 0); 
  check_address(buffer); 

  while((c = (char)input_getc()) != CR)
  {
      tempBuffer[idx++] = c; 
  }
  tempBuffer[idx] = '\0'; 

  strlcpy(buffer, tempBuffer, size+1);   

}

tid_t
sys_exec(void* esp)
{
  tid_t tid; 
  int status; 
  char* file_name = *((int *)esp + 1); 

  check_address(file_name); 
  
  if(check_file(file_name))
  {
    tid = process_execute(file_name); 
    status = process_wait(tid); 
  }
  else{
    tid = -1;
    printf("load: %s: open failed\n", file_name); 
  }
  return tid; 
}

