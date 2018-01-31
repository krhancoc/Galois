#ifndef GALOIS_H
#define GALOIS_H

struct GaloisField;
static int gfmul(int a, int b, struct GaloisField f);
static int gfinv(int a, struct GaloisField f);
static int gfdiv(int a, int b, struct GaloisField f);
static struct GaloisField* create_field(int size);


#endif // GALOIS_H