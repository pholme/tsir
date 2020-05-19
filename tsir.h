// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for temporal network SIR by Petter Holme (2018)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define NAVG 100000 // number of runs for averages

#define NONE (UINT_MAX - 1)
#define END UINT_MAX // NONE and END are used in various ways, the only purpose of NONE < END is for the S(x) macro

#define S(x) (n[(x)].heap < END) // is x susceptible?

// auxiliary macro
#define SQ(x) ((x) * (x))

typedef struct GLOBALS {
	// INPUT PARAMETERS
	double recovery_scale; // recovery time scale, auxiliary value for infection probs
	unsigned short rnd2inx[0x10001]; // mapping 16-bit random number to index
	// NETWORK SPECS
	unsigned int n, dur;
	// OTHER GLOBALS
	unsigned int nheap, *heap;
	// FOR RND
	uint64_t state;
	unsigned int cutoff_source, cutoff_dur; // to get the probabilities right . .
	// OUTBREAK STATS
	unsigned int ns, *s;
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

// quick.c
extern void quick (unsigned int);

// pcg_rnd.c
extern uint16_t pcg_16 ();
extern uint32_t pcg_32 ();
extern uint32_t pcg_32_bounded ();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
