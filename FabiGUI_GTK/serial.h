#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#ifdef ARCH_WIN
    #include <windows.h> /* Windows functions */
    #include <unistd.h>  /* UNIX standard function definitions */
    #include <stdio.h>
#endif // ARCH_WIN

#ifdef ARCH_LINUX
    #include <stdio.h>   /* Standard input/output definitions */
    #include <string.h>  /* String function definitions */
    #include <unistd.h>  /* UNIX standard function definitions */
    #include <fcntl.h>   /* File control definitions */
    #include <errno.h>   /* Error number definitions */
    #include <termios.h> /* POSIX terminal control definitions */
    #include <dirent.h>  /* directory tools */
#endif // ARCH_LINUX

#ifndef ARCH_LINUX
    #ifndef ARCH_WIN
        #error "No ARCH set in project options, use either ARCH_LINUX or ARCH_WIN or implemented others"
    #endif // ARCH_LINUX || ARCH_WIN
#endif

#include <stdlib.h>

void listComPorts(char* list);
int processSerialCommand(char * cmd);

int writeCOM(char* str, int len);
int readCOM(char * buffer, int size);
int openCOM(char* name);
int closeCOM(void);
#endif // SERIAL_H_INCLUDED
