#include "render.h"

void blackBox(unsigned char *frameBuffer){
	for (int yy = 0; yy < HEIGHT; yy++){
		for (int xx = 0; xx < WIDTH; xx++){
			int pos = (yy*WIDTH+xx)*PIXEL_SIZE;
			frameBuffer[pos +0] = 0;
			frameBuffer[pos +1] = 0;
			frameBuffer[pos +2] = 0;
			frameBuffer[pos +3] = 0;
		}
	}
}

void drawSquare(unsigned char *frameBuffer, struct Square* square, unsigned char col0, unsigned char col1, unsigned char col2){
	for (int yy = 0; yy < square->hh;yy++){
		for (int xx = 0; xx < square->ww; xx++){
			// int xxCur = cos(square->rot)*xx - sin(square->rot)*yy;
			// int yyCur = sin(square->rot)*xx + cos(square->rot)*yy;
			int pos = ((((int)square->yy)+yy)*WIDTH + (((int)square->xx)+xx))*PIXEL_SIZE;
            // int pos = (((int)square->yy)*WIDTH + (int)square->xx)*PIXEL_SIZE;
			frameBuffer[pos +0] = col0;
			frameBuffer[pos +1] = col1;
			frameBuffer[pos +2] = col2;
			// frameBuffer[pos +3] = 0;

			// frameBuffer[pos +0] = 255;
			// frameBuffer[pos +1] = 255;
			// frameBuffer[pos +2] = 255;
			// frameBuffer[pos +3] = 0;
		}
	}
}

void drawLine(unsigned char *frameBuffer, struct Point point1, struct Point point2){
    struct Point startPoint;
    struct Point endPoint;

    if (abs(point1.xx - point2.xx) < abs(point1.yy - point2.yy)) {
        startPoint.xx = point1.yy < point2.yy ? point1.xx : point2.xx;
        startPoint.yy = point1.yy < point2.yy ? point1.yy : point2.yy;

        endPoint.xx = point1.yy < point2.yy ? point2.xx : point1.xx;
        endPoint.yy = point1.yy < point2.yy ? point2.yy : point1.yy;

        double oneOverkk = (endPoint.xx - startPoint.xx) / (endPoint.yy - startPoint.yy);
        double bb = (endPoint.yy)-(1.0/oneOverkk*endPoint.xx);
        double zeroOffset = 0;
        if (endPoint.yy - startPoint.yy == 0.0 || endPoint.xx - startPoint.xx == 0.0){
            bb = endPoint.xx;
            oneOverkk = 0;
            zeroOffset = endPoint.xx;
        }
        // printf("kk: %lf bb: %lf\n", oneOverkk, bb);
        for (unsigned int yy = startPoint.yy; yy < endPoint.yy; yy++){
            unsigned int xxCur = (yy - bb) *oneOverkk + zeroOffset;
            unsigned int yyCur = yy;
            // printf("xx: %d yy: %d\n", xxCur, yyCur);
            // break;

            unsigned int pos = ((yyCur)*WIDTH + (xxCur))*PIXEL_SIZE;

            frameBuffer[pos +0] = 255;
            frameBuffer[pos +1] = 0;
            frameBuffer[pos +2] = 255;
            frameBuffer[pos +3] = 0;
        }
    } else {
        startPoint.xx = point1.xx < point2.xx ? point1.xx : point2.xx;
        startPoint.yy = point1.xx < point2.xx ? point1.yy : point2.yy;

        endPoint.xx = point1.xx < point2.xx ? point2.xx : point1.xx;
        endPoint.yy = point1.xx < point2.xx ? point2.yy : point1.yy;

        double kk = (endPoint.yy - startPoint.yy)/(endPoint.xx - startPoint.xx);
        double bb = startPoint.yy;
        if (endPoint.yy - startPoint.yy == 0.0 || endPoint.xx - startPoint.xx == 0.0){
            bb = (endPoint.yy)-(kk*endPoint.xx);
        }


        for (int xx = startPoint.xx; xx < endPoint.xx; xx++){
            unsigned int xxCur = xx;
            unsigned int yyCur = xx*kk+bb;
            unsigned int pos = ((yyCur)*WIDTH + (xxCur))*PIXEL_SIZE;

            frameBuffer[pos +0] = 255;
            frameBuffer[pos +1] = 0;
            frameBuffer[pos +2] = 255;
            frameBuffer[pos +3] = 0;
        }

    }
}

void writeToScreen(int fd, char* frameBuffer){
	lseek(fd, 0, SEEK_SET);
	int ret = write(fd, frameBuffer, BUFF_SIZE);
	if (ret < 0){
		perror("Could not write to frame buffer");
		exit(1);
	}
}

// void writeToFile(int fd, char* frameBuffer){
// 	// lseek(fd, 0, SEEK_SET);
// 	int ret = write(fd, frameBuffer, BUFF_SIZE);
// 	if (ret < 0){
// 		perror("Could not write to file");
// 		exit(1);
// 	}
// }