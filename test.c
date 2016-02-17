#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
	int fd = 0;
	char buf[128] = {};
	if((fd = open("/dev/hello",0444)) < 0)
	{
		printf("open error \n");
	}

	read(fd,buf,sizeof(buf));
	printf("%s \n", buf);
	
	return 0;
}
