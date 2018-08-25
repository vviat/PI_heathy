#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_SIZE 1024
#define MASTER_PORT 8731
#define _DEBUG

int socket_create(int port) {
	int sockfd;
	struct sockaddr_in sock_addr = {0};
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket_create ");
		return -1;
	}

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
		perror("bind");
		return -1;
	}
	if (listen(sockfd, 10) < 0) {
		perror("listen");
		return -1;
	}

	return sockfd;
}

int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr = {0};
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket_create ");
		return -1;
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("connect");
		return -1;
	}


	return sockfd;
}

int check_size(char *filename, int size, char *dir) {
	struct stat st;
	int flag;
	char cmd_1[50] = {0};
	char cmd[100] = {0};
	char basename[10] = {0};
	time_t _time;
	struct tm *lt;
 	stat(filename, &st);
	int size_real = st.st_size / 1048576;
	if (size_real >= size) {
		flag = 0;
	} else {
		return 0;
	}

	sprintf(cmd_1, "basename %s", filename);
	FILE *stream = popen(cmd_1, "r");

	fgets(basename, sizeof(basename), stream);

	time(&_time);
	lt = localtime(&_time);

	sprintf(cmd, "cp -a %s %s/%s_%d%d%d%d", filename, dir, basename, lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour);
	system(cmd);
	return 0;
}



int write_pi_log (char *PiHealthLog, const char *format, ...) {  
    va_list arg;  
    int done;  
    FILE* pFile = fopen(PiHealthLog, "a+");
  
    va_start (arg, format);  
  
    time_t time_log = time(NULL);  
    struct tm* tm_log = localtime(&time_log);  
    fprintf(pFile, "%04d-%02d-%02d %02d:%02d:%02d ", tm_log->tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);  
  
    done = vfprintf (pFile, format, arg);  
    va_end (arg);  
  
    fflush(pFile);
    fclose(pFile);  
    return done;  
} 
