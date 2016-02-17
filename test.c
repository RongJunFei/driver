#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "command.h"

#define SIZE 127

void myread(int fd){
	char data[SIZE + 1];

	read(fd, data, SIZE);
	printf("read = %s \n", data);
}

void mywrite(int fd){
	char data[SIZE + 1] = "abcdefg";
	write(fd,data,strlen(data));
}

int main(int argc, const char *argv[])
{
	int fd = 0;

	if((fd = open("/dev/coll", O_RDWR)) < 0)
	{
		printf("open error \n");
		
		return fd;
	}

	myread(fd);
	mywrite(fd);
	myread(fd);
	ioctl(fd, ONE);
	ioctl(fd, TWO);
	ioctl(fd, TRE);

	close(fd);
	
	return 0;
}
