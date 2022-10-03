#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#define MAXARGC 128

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

char* parse_file_name_from(const char* file_name); 
char** argument_parse(char* commandLine, int* argc); 
void argument_stack(char** parse, int count, void **esp); 
void push_argument_value(char **parse, int count ,void **esp, void *** argAddress);
void push_argument_address(void **argAddress, int count, void **esp);
void push_meta_argument(int count, void **esp);
void argument_stack(char** parse, int count, void **esp);

struct thread * find_child(tid_t child_tid);


#endif /* userprog/process.h */
