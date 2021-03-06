#include "galois.h"
#include "immintrin.h"
#include "stdio.h"

static int GENERATOR = 9;

struct GaloisField {
	int* ilog;
	int* log;
	int size;
};

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


/**
	TODO: Possibly change irriducible poly's to Lucky poly's as dictated in 
	https://eprint.iacr.org/2007/192.pdf possibly not needed/or possible need
	to read more than just abstract :). Seems they say the choice relies on some
	the polynomial of choice which is either a trinomial or pentanomial 
	where 
		x^m + x^a + x^b + x^c + 1
				OR
		x^m + x^a + 1
			where m > a > b > c
	It requires that m - a >= w where w is our word size. 
**/
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
		case 6:
			poly = 67; // x^6 + x + 1
			break;
		case 7:
			poly = 131;
			break;
		case 8:
			poly = 285; // x^8 + x^4+ x^3 + x^2 + 1; 
			break;
		case 16:
			poly = 65581; //1 + x^2 + x^3 + x^5 + x^16 <--- This for example would be efficient in W=8?  I think?
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
		t = reduce_gmul(t, GENERATOR, size);
	}
	return field;
}

static int* create_log(int* ilog_table, int size) {

	int* log = malloc(size * sizeof(int) - 1);
	for(int i = 0; i < size - 1; i++) {
		//printf("log(%i) = %i\n", ilog_table[i], i);
		log[ilog_table[i]] = i;

	}
	return log;
}

static int gfadd(int a, int b) {
	return a^b;
}

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

static void destroy_gf(struct GaloisField* f) {

	free(f->ilog);
	free(f->log);
	free(f);
}

int main() {

	// __m256i a = _mm256_set1_epi32(3);
	// __m256i b = _mm256_set1_epi32(1);
	// __m256i result = gadd(&a, &b);
	// /* Display the elements of the result vector */
	// int* f = (int*)&result;
	// printf("%i %i %i %i %i %i %i %i\n",
	// 	f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
	int size = 16;
	struct GaloisField* f = create_field(size);

	int v = gfdiv(10, 13, *f);
	printf("%i\n", v);

	destroy_gf(f);
	
	return 0;
}