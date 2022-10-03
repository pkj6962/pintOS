#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#define syscall1(NUMBER, ARG0)                                           \
        ({                                                               \
          int retval;                                                    \
          asm volatile                                                   \
            ("pushl %[arg0]; pushl %[number]; int $0x30; addl $8, %%esp" \
               : "=a" (retval)                                           \
               : [number] "i" (NUMBER),                                  \
                 [arg0] "g" (ARG0)                                       \
               : "memory");                                              \
          retval;                                                        \
        })


void exit (int status);


void syscall_init (void);


unsigned sys_write(void* esp);

unsigned sys_read(void* esp);

int sys_fibonacci(void *esp);

int sys_max_of_four_int(void *esp);

#endif /* userprog/syscall.h */

