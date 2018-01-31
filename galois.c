#include "galois.h"
#include "immintrin.h"
#include "stdio.h"

static int generator = 0x03;

static int gmul(int a, int b) {

	int m = a;
	int shift = 0;
	int sum = 0;
	while(m != 0) {
		if (m & 1) {
			sum ^= (b << shift);
		}
		m = (m >> 1);
		shift++;
	}
	return sum;
}

static int reduce(int a, int size) {
	
	int m = a;
	int poly;
	switch (size) {
		case 2:
			poly = 7;	// x^2 + x + 1
			break;
		case 3:
			poly = 11; // x^3 + x + 1
			break;
		case 4:
			poly = 19; // x^4 + x + 1
			break;
		case 5:
			poly = 37; // x^5 + x^2 + 1
			break;
	}

	int mask = 1 << size;
	mask <<= size - 2;
	for ( int i = size - 2; i >=0; i-- ) {
		if ( m & mask ) {
			m &= ~(mask << 1);
			m ^= (poly << i);
		}
		mask >>= 1;
	}
	return m;

}

static int reduce_gmul(int a, int b, int size) {

	return reduce(gmul(reduce(a, size), reduce(b, size)), size);
}

static int* generate_field(int size) {

	int* field = malloc((sizeof(int) * (1 << size)) - 1);
	int t = 1;
	for(int i = 0; i < (1 << size); i++) {
		field[i] = t;
		t = reduce_gmul(t, generator, size);
	}
	return field;
}

static int gadd(int a, int b) {
	return a^b;
}

static int* create_log(int* ilog_table, int size) {

	int* log = malloc(size * sizeof(int) - 1);
	for(int i = 0; i < size -1; i++) {
		// printf("log(%i) = %i\n", ilog_table[i], i);
		log[ilog_table[i]] = i;

	}
	return log;
}

struct GaloisField {
	int* ilog;
	int* log;
	int size;
};

static int gfmul(int a, int b, struct GaloisField f) {

	return f.ilog[(f.log[a] + f.log[b]) % (f.size - 1)];
}

static int gfinv(int a, struct GaloisField f) {

	return f.ilog[(f.size - 1) - f.log[a]];
}

static int gfdiv(int a, int b, struct GaloisField f) {

	return gfmul(a, gfinv(b, f), f);
}

static struct GaloisField* create_field(int size) {

	struct GaloisField* f = malloc(sizeof(struct GaloisField));
	f->ilog = generate_field(size);
	f->size = (1 << size);
	f->log = create_log(f->ilog, 1 << size);
	return f;
}


int main() {

	// __m256i a = _mm256_set1_epi32(3);
	// __m256i b = _mm256_set1_epi32(1);
	// __m256i result = gadd(&a, &b);
	// /* Display the elements of the result vector */
	// int* f = (int*)&result;
	// printf("%i %i %i %i %i %i %i %i\n",
	// 	f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

	int size = 5;
	struct GaloisField* f = create_field(size);

	int v = gfdiv(13, 10, *f);
	printf("%i\n", v);

	return 0;
}