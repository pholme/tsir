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

#define NAVG 1000000 // number of runs for averages

#define NONE (UINT_MAX - 1)
#define END UINT_MAX // NONE and END are used in various ways, the only purpose of NONE < END is for the S(x) macro

#define S(x) (n[(x)].heap < END) // is x susceptible?

// auxiliary macro
#define SQ(x) ((x) * (x))

typedef struct GLOBALS {
	// INPUT PARAMETERS
	double a; // -1/nu where nu = recovery rate (input nu in units of duration, but internally in units of time steps)
	unsigned short rnd2inx[0x10000]; // mapping 16-bit random number to index
	// NETWORK SPECS
	unsigned int n, dur;
	// OTHER GLOBALS
	unsigned int nheap, *heap;
	// FOR RND
	uint64_t state;
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
extern void read_data ();
extern unsigned int exptime ();

// pcg_rnd.c
extern uint16_t pcg_16 ();
extern uint32_t pcg_32 ();
extern uint32_t pcg_32_bounded ();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
