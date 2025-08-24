#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAX_LEN 100

void concat(char *dest, char *src) {
	while (*dest != '\0') {
    	dest++;
	}
	while (*src != '\0') {
    	*dest = *src;
    	dest++;
    	src++;
	}
	*dest = '\0';  
}

int main() {
	int pipe1[2];
	int pipe2[2];

	if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    	perror("pipe");
    	exit(1);
	}

	pid_t pid = fork();
	if (pid < 0) {
    	perror("fork");
    	exit(1);
	}

	if (pid > 0) {
    	close(pipe1[0]);
    	close(pipe2[1]);

    	char input_str[MAX_LEN];
    	printf("P1: Enter a string: ");
    	fgets(input_str, MAX_LEN, stdin);

    	int i = 0;
    	while (input_str[i] != '\0') {
        	if (input_str[i] == '\n') {
            	input_str[i] = '\0';
            	break;
        	}
        	i++;
    	}

    	write(pipe1[1], input_str, MAX_LEN);
    	close(pipe1[1]);
    	char result_str[2*MAX_LEN] = {0};
    	read(pipe2[0], result_str, 2*MAX_LEN);
    	close(pipe2[0]);

    	printf("P1: Received concatenated string: %s\n", result_str);
    	wait(NULL);

	} else {
    	close(pipe1[1]);
    	close(pipe2[0]);

    	char recv_str[MAX_LEN] = {0};
    	read(pipe1[0], recv_str, MAX_LEN);
    	close(pipe1[0]);

    	char concat_str[] = " - Processed by P2";

    	concat(recv_str, concat_str);
    	write(pipe2[1], recv_str, 2*MAX_LEN);
    	close(pipe2[1]);

    	exit(0);
	}

	return 0;
}
