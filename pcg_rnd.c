// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for temporal network SIR by Petter Holme (2018)

// this file contains the random number generator, derived from the PCG
// RNG v0.94 http://www.pcg-random.org under the Apache License 2.0
// http://www.apache.org/licenses/LICENSE-2.0

#include "tsir.h"

extern GLOBALS g;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pcg_init () {
	int fd;
	unsigned long sz;
	uint32_t initstate;

	fd = open("/dev/random", O_RDONLY);
	
	if (fd < 1) {
		fprintf(stderr, " can't open /dev/random\n");
		exit(1);
	}

	sz = read(fd, &initstate, sizeof(uint32_t));
	
	close(fd);

	if (sz < sizeof(uint32_t)) {
		fprintf(stderr, " can't read /dev/random\n");
		exit(1);
	}

	g.state = (initstate + 2891336453U) * 747796405U + 2891336453U;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t pcg_32 () {
	uint32_t state = g.state, word;

	g.state = g.state * 747796405U + 2891336453U;
	word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t pcg_32_bounded (uint32_t bound) {
	uint32_t threshold = -bound % bound, r;

	for ( ; ; ) {
		r = pcg_32();
		if (r >= threshold) return r % bound;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint16_t pcg_16 () {
	uint32_t state = g.state;

	g.state = g.state * 747796405U + 2891336453U;
	return (uint16_t)(((state >> 11u) ^ state) >> ((state >> 30u) + 11u));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
