// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for temporal network SIR by Petter Holme (2018/2020)

// miscellaneous routines for tsir

#include "tsir.h"

extern GLOBALS g;
extern NODE *n;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// giving exponential random numbers with mean 'g.recovery_scale'

unsigned int exptime () {
	uint32_t r = pcg_32();

	if (r == 4294967295u) return 0;

	return g.a * log((r + 1) / 4294967296.0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine reads the temporal network from the standard input
// the format is strict and unflexible (and meant to be assembled by a wrapper)
// function

void read_data () {
	unsigned int i, j, me;

	if (2 != scanf("%u %u", &g.n, &g.dur)) { // the number of nodes N and duration of the data set
		fprintf(stderr, "reading error 1\n");
		exit(1);
	}

	n = calloc(g.n, sizeof(NODE));

	for (me = 0; me < g.n; me++) { // then for every node
		if (1 != scanf("%u", &n[me].deg)) { // .. scan the degree
			fprintf(stderr, "reading error 2\n");
			exit(1);
		}
		n[me].nb = malloc(n[me].deg * sizeof(unsigned int));
		n[me].nc = malloc(n[me].deg * sizeof(unsigned int));
		n[me].t = malloc(n[me].deg * sizeof(unsigned int *));
		for (i = 0; i < n[me].deg; i++) { // for all neighbors of me
			if (2 != scanf("%u %u", n[me].nb + i, n[me].nc + i)) { // scan the id & number of contacts with that neighbor
				fprintf(stderr, "reading error 3\n");
				exit(1);
			}
			n[me].t[i] = malloc(n[me].nc[i] * sizeof(unsigned int));
			for (j = 0; j < n[me].nc[i]; j++)
				if (1 != scanf("%u", n[me].t[i] + j)) { // the time of contacts to that neighbor, these need to be sorted in decreasing value of its final element
					fprintf(stderr, "reading error 4\n");
					exit(1);
				}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
