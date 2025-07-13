#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "render.h"
#include "inputs.h"

int main(){
  	char* shmem = (char*)mmap(
			NULL, 
			8, 
			PROT_READ | PROT_WRITE, 
			MAP_SHARED | MAP_ANON, 
			-1, 
			0
	);

	pid_t pp = fork();
	if (pp < 0){
		perror("Fork failed");
		exit(1);
	}


	if (pp == 0){
		// usleep(0.1 * 1000 * 1000);
		// printf("%p %d\n", shmem, *(char*)shmem);
		mainRender(shmem);
	}else{
		// printf("%p %d\n", shmem, *(char*)shmem);
		takeInput(shmem);
		// usleep(0.1 * 1000 * 1000);
	}
}