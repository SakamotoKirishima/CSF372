import numpy as np
import fileinput
# exp_dist <-> f(x, beta) = (1/beta) * exp(-(1/beta) * x)
beta = 10
process_count = 20
a=np.round(np.random.exponential(scale=beta, size=(process_count,2)))
np.savetxt("process", a, delimiter=",",fmt="%i")

with fileinput.FileInput("process", inplace=True, backup='.csv') as file:
	for line in file:
		line = line.replace(",", " ")
		line = line.replace("\n", " ")
		print(line, end='')