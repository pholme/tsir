# Python wrapper for the temporal network SIR C code

from sys import argv
from read_run import run, read_network

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##

if __name__ == '__main__':

	if len(argv) != 4:
		print('usage: python3 tsir_single.py [temporal network] [beta] [nu]')
		exit(1)
	
	omega, err = run(read_network(argv[1]), float(argv[2]), float(argv[3]))

	print('avg. outbreak size:', omega, '(' + str(err) + ')')
	
##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##
