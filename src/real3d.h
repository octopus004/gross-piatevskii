#ifndef REALTHREED_H
#define REALTHREED_H
#include <cstdio>
#include <complex>
#include "../3d/3d.h"
#include"cfg.h"

#define MAX(a, b, c) (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c)
#define MAX_FILENAME_SIZE 256

class RealThreeD :public ThreeD {
public:
	
	void readpar() override;
	void alloc_mem() override;
	void write(FILE* out) override;
	double*** tensor();
	void gencoef();
	~RealThreeD();
	RealThreeD();

	std::complex<double>*** psitensor();
	std::complex<double>  Ax0, Ay0, Az0, Ax0r, Ay0r, Az0r, Ax, Ay, Az;
	std::complex<double>* calphax, * calphay, * calphaz;
	std::complex<double>* cgammax, * cgammay, * cgammaz;


	double* x, * y, * z;
	double* x2, * y2, * z2;
	std::complex<double>*** psi;	
	double*** pot;
	double*** dpsix, *** dpsiy, *** dpsiz;
	char* output, * initout, * rmsout, * Nstpout, * Npasout, * Nrunout;
	long outstpx, outstpy, outstpz, outstpt;

	int opt;
	long Nx, Ny, Nz;
	long Nx2, Ny2, Nz2;
	long Nstp, Npas, Nrun;
	double dx, dy, dz;
	double dx2, dy2, dz2;
	double dt;
	double G0, Gpar, G;
	double kappa, lambda;
	double par;


	


};

#endif
