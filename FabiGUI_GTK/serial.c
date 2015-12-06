#include "serial.h"

#ifdef ARCH_WIN

#endif // ARCH_WIN

#ifdef ARCH_LINUX
    int fd; /* File descriptor for the port */
    struct termios options; /* options for the serial port */
#endif // ARCH_LINUX

void listComPorts(char* list)
{
    #ifdef ARCH_WIN
        //Test...
        list = strdup("COM1;COM2");
    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
        int count = 0;
        DIR* dirp;
        struct dirent* dp;

        dirp = opendir("/dev/serial/by-id");
        while ((dp = readdir(dirp)) != NULL)
        {
            if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0)
            {
                strcat(list,dp->d_name);
                strcat(list,(char *)";");
            }
        }
    #endif // ARCH_LINUX
}

int writeCOM(char* str, int len)
{
    #ifdef ARCH_WIN

    #endif // ARCH_WIN
    #ifdef ARCH_LINUX
        return write(fd, str, len);
    #endif // ARCH_LINUX
}

char* readCOM()
{
    #ifdef ARCH_WIN

    #endif // ARCH_WIN

    #ifdef ARCH_LINUX

    #endif // ARCH_LINUX
}

int openCOM(char* name)
{
    #ifdef ARCH_WIN
        //TODO: Win Implementation
    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
        int ret = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
        if(ret > 0)
        {
            tcgetattr(fd, &options);
            /* set baudrate to 9600 */
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            /* 8N1 configuration */
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            /* disable HW flow control */
            //options.c_cflag &= ~CNEW_RTSCTS;

        }
        return ret;
    #endif // ARCH_LINUX
}

int closeCOM()
{
    #ifdef ARCH_WIN
        //TODO: Win Implementation
    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
        close(fd);
    #endif // ARCH_LINUX
}
