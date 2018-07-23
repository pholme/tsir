// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for temporal network SIR by Petter Holme (2018)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <limits.h>
#include "../SFMT/SFMT.h"

#define NAVG 100000 // number of runs for averages
#define NRND 0x100000 // number of random numbers per batch
#define NREV 1000 // number of iterations to thermalize the RNG

#define NONE (UINT_MAX - 1)
#define END UINT_MAX // NONE and END are used in various ways, the only purpose of NONE < END is for the S(x) macro

#define S(x) (n[(x)].heap < END) // is x susceptible?

// if rng needs a refill within x new random numbers
#define RND_CHK(x) if ((x) + (g.r - g.rnd) >= NRND) {sfmt_fill_array32(&g.sfmt, g.rnd, NRND); g.r = g.rnd;}

// auxiliary macro
#define SQ(x) ((x) * (x))

typedef struct GLOBALS {
	// INPUT PARAMETERS
	double recovery_scale, logq; // recovery time scale, auxiliary value for infection probs
	// NETWORK SPECS
	unsigned int n, dur;
	// OTHER GLOBALS
	unsigned int nheap, *heap;
	// OUTBREAK STATS
	unsigned int s, t, t0;
	// FOR RNG
	uint32_t *rnd, *r; // arrays for random numbers
	sfmt_t sfmt; // state of random number generator
	unsigned int cutoff_source, cutoff_dur; // to get the probabilities right . .
} GLOBALS;

typedef struct NODE {
	unsigned int deg, *nb; // degree, neighbors
	unsigned int *nc, **t; // ordered number of / list of contact times for bisection serach
	unsigned int heap, time; // time is 1st the time of infection (for sorting the heap), then the time of recovery (to check if the node is I or R)
} NODE;

// heap.c
extern void up_heap (unsigned int);
extern void del_root ();

// misc.c
extern void init_rng ();
extern void read_data (FILE *);
extern unsigned int exptime ();
extern unsigned int get_source ();
extern unsigned int get_start_time ();

// quick.c
extern void quick (unsigned int);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
