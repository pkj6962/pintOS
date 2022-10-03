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
void check_address(void *addr);


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
  check_address((int*)esp+1);
  
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
      break; 
    case(SYS_FIBONACCI):
      *(int**)eax = sys_fibonacci(esp);
      break;
    case(SYS_MAX_OF_FOUR_INT):
      *(int**)eax = sys_max_of_four_int(esp); 
      break;
    default:
      *(int**)eax = -1; 
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
  char* actual_file_name = parse_file_name_from(file_name); 
  struct file * f =filesys_open(actual_file_name);
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


int 
sys_fibonacci(void *esp)
{
  int n = *((int*)esp + 1); 
  
  if (n < 3)
    return 1; 
  
  int a1 = 1, a2 =1 , temp; 
  for(int i = 0; i < n-2; i++)
  {
    temp = a2;
    a2 += a1; 
    a1 = temp;
  }
  return a2;
}


int 
sys_max_of_four_int(void *esp)
{
  int num, max;
  max = *((int *)esp + 1); 
  
  for(int i = 1; i < 4; i++)
  {
    num = *((int *)esp + i + 1); 
    if (max < num)
      max = num; 
  }
  return max; 
}

