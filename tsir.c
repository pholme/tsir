// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on temporal networks by Petter Holme (2018)

#include "tsir.h"

GLOBALS g;
NODE *n;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine first localizes the first contact later than 'now' in t
// then picks the contact that can infect (a chain of bernouilli trials)
// among the rest of the contacts. It returns the time of the infecting contact

unsigned int next_contact (unsigned int *t, unsigned int nt, unsigned int now) {
	unsigned int i, lo = 0, mid, hi = nt - 1;

	if (t[hi] <= now) return END; // no need to search further bcoz .t is sorted

	// the actual bisection search
	do {
		mid = (lo + hi) >> 1;
		if (t[mid] > now) hi = mid;
		else lo = mid;
	} while (hi > lo + 1);

	if (now < t[lo]) hi = lo; // the only case lo is correct

	// get a random contact
	i = hi + g.prob[*(g.r++) & 0xffff];

	if (i >= nt) return NONE; // if the contact is too late, skip it

	// return the time of the contact
	return t[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine does the book keeping for an infection event

void infect () {
	unsigned int i, you, t, me = g.heap[1];
	unsigned int now = n[me].time, duration = exptime();

	del_root(); // take the newly infected off the heap

	if (duration > 0) { // if the duration is zero, no one else can be infected
		RND_CHK(n[me].deg);
		n[me].time += duration;

		// go through the neighbors of the infected node . .
		for (i = 0; i < n[me].deg; i++) {
			you = n[me].nb[i];
			if (S(you)) { // if you is S, you can be infected
				// find the infection time of you
				t = next_contact(n[me].t[i], n[me].nc[i], now);
				if (t == END) break; // bcoz the sorting of nbs, we can break

				// if the infection time is before when me gets infected,
				// and (if it was already listed for infection) before the
				// previously listed infection event, then list it
				if ((t <= n[me].time) && (t < n[you].time)) {
					n[you].time = t; // set you's infection time
					if (n[you].heap == NONE) { // if not listed before, then extend the heap
						g.heap[++g.nheap] = you;
						n[you].heap = g.nheap;
					}
					up_heap(n[you].heap); // this works bcoz there the only heap relationship that can be violated is the one between you and its parent
				}
			}
		}
	}

	g.s++; // to get the outbreak size
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine runs one SIR outbreak from a random starting node

void sir () {
	unsigned int i, source;
	
	g.s = 0;
	
	// initialize
	for (i = 0; i < g.n; i++) n[i].heap = n[i].time = NONE;

	// get & infect the source

	source = get_source();
	n[source].time = get_start_time();
	n[source].heap = 1;
	g.heap[g.nheap = 1] = source;

	// run the outbreak
	while (g.nheap) infect();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// main function handling i/o

int main (int argc, char *argv[]) {
	unsigned int i, j;
	double d, s1 = 0.0, s2 = 0.0; // for averages
	FILE *fp;
#ifdef TIME
	struct timespec t0, t1;
#endif
	
	// just a help message
	if (argc != 5) {
		fprintf(stderr, "usage: ./tsir [nwk file] [r.n.g. state file] [beta] [nu (units of the duration of the data)]\n");
		return 1;
	}

	// read network data file
	fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		return 1;
	}
	read_data(fp);
	fclose(fp);

	// initialize parameters
	d = 1.0 / log(1.0 - atof(argv[3]));
	for (i = 0; i < 0x10000; i++)
		g.prob[i] = (unsigned short) floor(d * log((i + 1) / 65536.0));

	// read state or initialize RNG, start - - - - - - - - - - - - - - - - - - - - - - - -
	if (sfmt_get_min_array_size32(&g.sfmt) > NRND) {
		fprintf(stderr, "can't initialize the r.n.g.\n");
		return 1;
	}
	
	// an array for random numbers (slightly faster than generating them one by one)
	g.rnd = malloc(NRND * sizeof(uint32_t));
	
	// check if I can read the RNG state from a file
	fp = fopen(argv[2], "rb");
	if (fp) {
		if (1 != fread(&g.sfmt, sizeof(sfmt_t), 1, fp)) {
			fclose(fp);
			init_rng(); // if reading fails, then re-initialize it
		}
		fclose(fp);
	} else init_rng(); // initialize from the clock
	sfmt_fill_array32(&g.sfmt, g.rnd, NRND);
	g.r = g.rnd;

	g.cutoff_source = (4294967295 / g.n) * g.n; // to get the epidemic seeds with equal probability
	// read state or initialize RNG, stop  - - - - - - - - - - - - - - - - - - - - - - - -

	g.cutoff_dur = (4294967295 / g.dur) * g.dur;

	// allocating the heap (N + 1) because it's indices are 1,...,N
	g.heap = malloc((g.n + 1) * sizeof(unsigned int));

#ifdef TIME
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t0);
#endif
	
	// run the simulations and summing for averages
	for (i = 0; i < NAVG; i++) {
		sir();

		// saving stats for averages
		s1 += (double) g.s;
		s2 += SQ((double) g.s);
	}

#ifdef TIME
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif

	// average
	s1 /= NAVG;
	s2 /= NAVG;

	// print result
	printf("avg. outbreak size: %g (%g)\n", s1, sqrt((s2 - SQ(s1)) / (NAVG - 1)));
#ifdef TIME
	printf("time per outbreak (s): %g\n", ((t1.tv_sec - t0.tv_sec) + 1.0e-9 * (t1.tv_nsec - t0.tv_nsec)) / NAVG);
#endif
	// save the state of the random number generator
	fp = fopen(argv[2], "wb");
	fwrite(&g.sfmt, sizeof(sfmt_t), 1, fp);
	fclose(fp);
	
	// cleaning up
	for (i = 0; i < g.n; i++) {
		for (j = 0; j < n[i].deg; j++) free(n[i].t[j]);
		free(n[i].nb);
		free(n[i].nc);
		free(n[i].t);
	}
	free(g.rnd); free(n); free(g.heap);
	 
	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
