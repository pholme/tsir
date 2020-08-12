##   Common routines for Python wrapper for temporal network SIR C code

import networkx as nx
from random import getrandbits
from subprocess import Popen, PIPE, STDOUT

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##
# This function reads the network. The input file contains
# three columns [id 1] [id 2] [time], showing that id 1 has
# been in contact with id 2 at time t
# nodes with no contacts are not considered

def read_network (fname):

	G = nx.Graph()

	tmax = 0

	try:
		with open(fname) as f:
			for l in f:
				a = l.strip().split()
				if len(a) == 3:
					if a[0] != a[1]:
						b = int(a[2])
						if tmax < b:
							tmax = b
						if G.has_edge(a[0],a[1]):
							if b not in G.edges[a[0],a[1]]['times']: # excluding identical events
								G.edges[a[0],a[1]]['times'].append(b)
							else:
								print('ignoring multiple events', a[0], a[1], a[2])
						else:
							G.add_edge(a[0],a[1])
							G.edges[a[0],a[1]]['times'] = [b]
					else:
						print('ignoring self-contact of', a[0])
	except:
		print('reading error in:', fname)
		exit(1)

	H = nx.convert_node_labels_to_integers(G) # this can change the id numbers from the original file

	# assembling the temporal network in the format of the C code
	nwk = str(H.number_of_nodes()) + ' ' + str(tmax) + '\n'
	for i in range(H.number_of_nodes()):
		nwk += str(H.degree(i)) + '\n'
		to_sort = []
		for v in H.neighbors(i):
			to_sort.append((max(H.edges[i,v]['times']),v))
		for x,v in sorted(to_sort,reverse=True):
			nwk += str(v) + ' ' + str(len(H.edges[i,v]['times'])) + '\n'
			for t in sorted(H.edges[i,v]['times']):
				nwk += str(t) + '\n'

	return nwk

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##

def run (nwk, beta, nu):

	assert(beta > 0.0)
	if (beta > 1):
		print('beta should be <= 1, so set to 1')
		beta = 1.0
	assert(nu > 0)

	p = Popen(['./tsir', str(beta), str(nu), str(getrandbits(64))], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
	o = p.communicate(input=bytes(nwk,encoding='utf-8'))[0]

	# interpret and report the output
	a = o.decode().split('\n')
	b = a[0].split()

	return float(b[0]), float(b[1])

##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##   ##
