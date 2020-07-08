##   Python wrapper for the temporal network SIR C code

from sys import argv
from read_run import run, read_network
import numpy as np
import matplotlib.pyplot as plt
import progressbar # install progressbar2

NU_MIN = 0.1
NU_MAX = 10.0
NNU = 21

BETA_MIN = 0.01
BETA_MAX = 1.0
NBETA = 21

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##

if __name__ == '__main__':

	if len(argv) != 3:
		print('usage: python3 tsir.py [temporal network] [output file]')
		exit(1)

	nwk = read_network(argv[1])

	# running the C code
	nus = np.geomspace(NU_MIN,NU_MAX,NNU)
	betas = np.geomspace(BETA_MIN,BETA_MAX,NBETA)
	omega = np.empty((NBETA-1,NNU-1))
	X = np.empty((NBETA,NNU))
	Y = np.empty((NBETA,NNU))

	ibar = 0
	with progressbar.ProgressBar(max_value=NBETA*NNU) as bar:
		for i in range(NBETA):
			for j in range(NNU):
				X[i,j] = betas[i]
				Y[i,j] = nus[j]
				if i < NBETA - 1 and j < NNU - 1:
					x = (betas[i] + betas[i + 1]) / 2
					y = (nus[j] + nus[j + 1]) / 2
					omega[i,j], err = run(nwk, x, y)
				ibar += 1
				bar.update(ibar)

	# plotting a heatmap
	fig,ax = plt.subplots()
	ax.set_xscale('log')
	ax.set_yscale('log')
	ax.set_xlabel('transmission probability')
	ax.set_ylabel('recovery rate')
	plt.pcolormesh(X, Y, omega, cmap='cubehelix')
	plt.colorbar(label = 'outbreak size')
	fig.savefig(argv[2])
	plt.close()

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##
