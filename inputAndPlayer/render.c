#include <stdio.h>

#include "render.h"

void draw(int playerPosX, int playerPosY){
	const int max = 50;

	for (int yy = 0; yy < max;yy++){
		for (int xx = 0; xx < max;xx++){
			if (xx == playerPosX && yy == playerPosY){
				printf("\033[41mP\033[0m");
			}else{
				printf("N");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void renderMain(){}