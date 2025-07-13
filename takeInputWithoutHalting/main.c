#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>

#define SHMSIZE 1

char inputEngine(){
	int shmid = shmget(2009, SHMSIZE, 0);
	char* shm = shmat(shmid, 0, 0);
	*shm = 'a';

	char ch = -1;
	while ((*shm != 'q') && (ch != 'q')){
		if (ch == -1){
			ch = getchar();
		}
		if (*shm == -1){
			*shm = ch;
			ch = -1;
		}
	}

	return 0;
}

char mainPros(){
	const int width = 2;
	const int height = 2;

	int shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
	char* shm = shmat(shmid, 0, 0);
	char curAct = 0;

	while ((*shm != 'q') && (curAct != 'q')){
		usleep(.01* 1000 * 1000);
		if (*shm != -1){
			printf("New input. %c %d\n", *shm, *shm);
			curAct = *shm;
			*shm = -1;
		}else{
			printf("Old input. %c %d\n", curAct, curAct);
		}
	}

	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}

int main(){
	printf("WEE\n");

	int test = 10;
	pid_t pp = fork();

	if (pp < 0){
		perror("Fork failed");
		exit(1);
	}

	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);           // Save old settings
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);         // Disable buffering and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply new settings

	if (pp == 0){
		inputEngine();
		return 0;
	}else{
		mainPros();
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	// system("stty cooked"); 

	// printf("Hello world!, process_id(pid) = %d \n",getpid());
	// printf("Hello world!, p = %d \n",p);
	return 0;
}
