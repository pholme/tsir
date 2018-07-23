// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// an augmented quicksort to sort the contact time arrays in decreasing order
// of their last elements . . this is needed to cut the search of inactive
// neighbors

#include "tsir.h"

#define CUTOFF 10
#define V(x) (nme->t[(x)][nme->nc[(x)] - 1])
#define SWAP(x,y) {utmp = nme->nc[(x)]; nme->nc[(x)] = nme->nc[(y)]; nme->nc[(y)] = utmp; utmp = nme->nb[(x)]; nme->nb[(x)] = nme->nb[(y)]; nme->nb[(y)] = utmp; uptmp = nme->t[(x)]; nme->t[(x)] = nme->t[(y)]; nme->t[(y)] = uptmp;}

extern struct NODE *n;

struct PIVOTS {unsigned int left, right;} PIVOTS;
unsigned int utmp, *uptmp;
struct NODE *nme;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// to speed up the search for almost sorted arrays

void insertion_sort (int left, int right) {
	int i, j;
	unsigned int snb, snc, *st, save;

	for (i = left + 1; i < right; i++) {
		snb = nme->nb[i];
		snc = nme->nc[i];
		st = nme->t[i];
		save = V(i);
		for (j = i; (j > 0) && (V(j - 1) < save); j--) {
			nme->nb[j] = nme->nb[j - 1];
			nme->nc[j] = nme->nc[j - 1];
			nme->t[j] = nme->t[j - 1];
		}
		nme->nb[j] = snb;
		nme->nc[j] = snc;
		nme->t[j] = st;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

unsigned int median (int left, int right) {
	int mid = (left + right) >> 1;

	if (V(left) < V(mid)) SWAP(left, mid);
	if (V(left) < V(right)) SWAP(left, right);
	if (V(mid) < V(right)) SWAP(mid, right);
	SWAP(mid, right - 1);

	return V(right - 1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct PIVOTS q_partition (int left, int right) {
	int i = left, j = right - 1, k, m = left, n = right - 1;
	unsigned int pivot = median(left, right);
	struct PIVOTS ret;

	for ( ; ; ) {
		do i++; while (V(i) > pivot);
		do j--; while ((V(j) < pivot) && (j > left));
		if (i >= j) break;
		SWAP(i, j);
		if (V(i) == pivot) {
			m++;
			SWAP(m, i);
		}
		if (V(j) == pivot) {
			n--;
			SWAP(n, j);
		}
	}
	SWAP(i, right - 1);

	j = i - 1;
	i++;

	for (k = left; k < m; k++, j--) SWAP(k, j);
	for (k = right - 1; k > n; k--, i++) SWAP(k, i);

	ret.left = i;
	ret.right = j;

	return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void quicksort_r (int left, int right) {

	if (left + CUTOFF <= right) {
		struct PIVOTS pivot = q_partition(left, right);

		quicksort_r(left, pivot.right);
		quicksort_r(pivot.left, right);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void quick (unsigned int i) {
	nme = n + i;

	quicksort_r(0, nme->deg - 1);
	insertion_sort(0, nme->deg);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
