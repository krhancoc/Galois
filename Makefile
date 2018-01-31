default: program

program: galois.c galois.h
	gcc -mavx -mavx2  -o galois galois.c
	
assembly: sse.c
	gcc -mavx -mavx2  -S -o sse.asm sse.c
	