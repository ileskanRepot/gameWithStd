from random import randint

def mapFloat(inp, input_start, input_end, output_start, output_end):
	return output_start + ((output_end - output_start) / (input_end - input_start)) * (inp - input_start)

for _ in range(int(1E3)):
	inp = randint(0, 1E4)
	input_start = randint(0, inp)
	input_end = randint(inp, 1E4)
	output_start = randint(0, inp)
	output_end = randint(inp, 1E4)
	print(mapFloat(inp, input_start, input_end, output_start, output_end))
