#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double mapDouble(double inp, double input_start, double input_end, double output_start, double output_end){
	return output_start + ((output_end - output_start) / (input_end - input_start)) * (inp - input_start);
}

int mapInt(int inp, int input_start, int input_end, int output_start, int output_end){
	return output_start + (output_end - output_start) * (inp - input_start) / (input_end - input_start);
}

int randRange(int start, int end){
	return start +rand()%(end-start);
}

int main(){
	srand(time(NULL));
	int range = 10*1000;
	for (int ii = 0; ii < 1000*1000; ii++){
		int inp = randRange(2, range -2);
		// printf("%5d ",inp);
		// fflush(stdout);

		int input_start = randRange(0, inp-1);
		// printf("%5d ",input_start);
		// fflush(stdout);

		int input_end = randRange(inp+1, range);
		// printf("%5d ",input_end);
		// fflush(stdout);

		int output_start = randRange(0, inp-1);
		// printf("%5d ",output_start);
		// fflush(stdout);

		int output_end = randRange(inp+1, range);
		// printf("%5d ",output_end);
		// fflush(stdout);

		int mappedDouble = mapDouble(inp, input_start, input_end, output_start, output_end);
		int mappedInt = mapInt(inp, input_start, input_end, output_start, output_end);
		// printf("%5d %5d\n",
				// (int)mapDouble(inp, input_start, input_end, output_start, output_end),
				// (int)mapInt(inp, input_start, input_end, output_start, output_end)
				// );
		if (mappedDouble != mappedInt){
			printf("ERROR %5d %5d %5d %5d %5d %5d %5d\n",
					inp,
					input_start,
					input_end,
					output_start,
					output_end,
					mappedDouble,
					mappedInt
					);
			return 1;
		}
	}
}
