// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for temporal network SIR by Petter Holme (2018)

// miscellaneous routines for tsir

#include "tsir.h"

extern GLOBALS g;
extern NODE *n;
unsigned int *alloc;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// giving exponential random numbers with mean 'scale'

unsigned int exptime () {
	double d = (4294967296 - pcg_32_bounded(4294967295)) / 4294967296.0;

	return (unsigned int) floor(-(log(d) * g.recovery_scale));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// gets the index of you in me's adjacency list

unsigned int get_index (unsigned int me, unsigned int you) {
	unsigned int i;

	for (i = 0; i < n[me].deg; i++) if (n[me].nb[i] == you) return i;

	// alloc check
	if (alloc[me] <= n[me].deg) {
		i = alloc[me];
		alloc[me] = (n[me].deg > 0) ? 2 * n[me].deg : 1; // double the allocated space
		n[me].nb = realloc(n[me].nb, alloc[me] * sizeof(unsigned int));
		n[me].nc = realloc(n[me].nc, alloc[me] * sizeof(unsigned int));
		n[me].t = realloc(n[me].t, alloc[me] * sizeof(unsigned int *));
		for ( ; i < alloc[me]; i++) {
			n[me].nc[i] = n[me].nb[i] = 0;
			n[me].t[i] = NULL;
		}
	}

	n[me].nb[n[me].deg++] = you;

	return n[me].deg - 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// reads the network, assumes a contact list with vertex label 0,N-1,
// ordered in time. If your network has nodes with zero contacts, make sure
// that none of them is the node with largest index

void read_data (FILE *fp) {
	unsigned int i, me, you;

	g.n = 0;

	// scan the system size
	while (2 == fscanf(fp, "%u %u %*u\n", &me, &you)) {
		if (g.n < me) g.n = me;
		if (g.n < you) g.n = you;
	}

	g.n++;

	n = calloc(g.n, sizeof(NODE));
	alloc = calloc(g.n, sizeof(unsigned int));

	rewind(fp);

	// scan the degrees
	while (2 == fscanf(fp, "%u %u %*u\n", &me, &you)) {
		i = get_index(me, you);
		n[me].nc[i]++;
		i = get_index(you, me);
		n[you].nc[i]++;
	}

	rewind(fp);

	for (me = 0; me < g.n; me++) {
		for (i = 0; i < n[me].deg; i++) {
			n[me].t[i] = malloc(n[me].nc[i] * sizeof(unsigned int));
			n[me].nc[i] = 0;
		}
	}

	// scan the times
	while (3 == fscanf(fp, "%u %u %u\n", &me, &you, &g.dur)) {
		i = get_index(me, you);
		n[me].t[i][n[me].nc[i]++] = g.dur;
		i = get_index(you, me);
		n[you].t[i][n[you].nc[i]++] = g.dur;
	}

	// allocate adjacency lists
	for (i = 0; i < g.n; i++) {
		n[i].nb = realloc(n[i].nb, n[i].deg * sizeof(unsigned int));
		n[i].nc = realloc(n[i].nc, n[i].deg * sizeof(unsigned int));
		n[i].t = realloc(n[i].t, n[i].deg * sizeof(unsigned int *));
		quick(i); // sort t in decreasing order of its last element
	}

	free(alloc);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
