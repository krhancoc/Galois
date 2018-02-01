#ifndef GALOIS_H
#define GALOIS_H

struct GaloisField;
static int gfadd(int a, int b);
static int gfmul(int a, int b, struct GaloisField f);
static int gfinv(int a, struct GaloisField f);
static int gfdiv(int a, int b, struct GaloisField f);
static struct GaloisField* create_field(int size);
static void destroy_gf(struct GaloisField* f);

#endif // GALOIS_H