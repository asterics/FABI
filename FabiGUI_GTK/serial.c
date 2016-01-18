#include "serial.h"
#include "parser.h"

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
        strcpy("COM1;COM2");
    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
        DIR* dirp;
        struct dirent* dp;

        dirp = opendir("/dev/serial/by-id");
        if(dirp != NULL)
        {
            while ((dp = readdir(dirp)) != NULL)
            {
                if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0)
                {
                    strcat(list,dp->d_name);
                    strcat(list,(char *)";");
                }
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

int readCOM(char * buffer, int size)
{
    #ifdef ARCH_WIN

    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
     return(read(fd,buffer,size));
    #endif // ARCH_LINUX
}

int processSerialCommand(char * cmd)
{
    int totalBytes=0;
    char buffer[8192];
    char * actpos;
    int numBytes,i;

    printf("Processing Command:%s",cmd);
    writeCOM(cmd,strlen(cmd));
    usleep(1000000);  // wait 100 millisecond to let the uC respond

    do
    {
        numBytes=readCOM(buffer,8191);
        if (numBytes>0) {
            totalBytes+=numBytes;
            buffer[numBytes]=0;
            actpos=buffer;
            printf("Read %d Bytes:%s\n",numBytes,actpos);
            for (i=0;i<numBytes;i++)
               parseByte(buffer[i]);
         }
    } while (numBytes >0);
    printf("Serial Read ended with return value %d\n",numBytes);
    return(totalBytes);
}

int openCOM(char* name)
{
    #ifdef ARCH_WIN
        //TODO: Win Implementation
    #endif // ARCH_WIN

    #ifdef ARCH_LINUX
        fd = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
        if(fd > 0)
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
        fcntl(fd,F_SETFL, FNDELAY);  // set non-blocking read
        return fd;
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
    return(0);
}




//#include "parser.h"
#include "commands.h"










