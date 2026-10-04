#include"real3d.h"
#include<iostream>
#include<vector>
#include <cstdio>
#include<complex>
#include "psi.h"
#include"cfg.h"
using namespace std;

const std::complex<double> I(0.0, 1.0);

RealThreeD::~RealThreeD() {
 
    delete[] dpsix[0][0];
    delete[] dpsix[0];
    delete[] dpsix;
}
RealThreeD::RealThreeD() {
    readpar();
    alloc_mem();
    psi = init(psi, kappa,lambda, Nx, Ny, Nz, x,x2,y,y2,z,z2,opt,par,dz,dx,dy,pot,Nx2, Ny2,Nz2,dx2,dy2,dz2);
}
void RealThreeD::readpar() {
    char* cfg_tmp;
    
    if ((cfg_tmp = cfg_read("OPTION")) == NULL) {
        fprintf(stderr, "OPTION is not defined in the configuration file\n");
        exit(EXIT_FAILURE);
    }
    opt = atol(cfg_tmp);

    if ((cfg_tmp = cfg_read("G0")) == NULL) {
        fprintf(stderr, "G0 is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    G0 = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("NX")) == NULL) {
        fprintf(stderr, "NX is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    Nx = atol(cfg_tmp);

    if ((cfg_tmp = cfg_read("NY")) == NULL) {
        fprintf(stderr, "NY is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    Ny = atol(cfg_tmp);

    if ((cfg_tmp = cfg_read("NZ")) == NULL) {
        fprintf(stderr, "Nz is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    Nz = atol(cfg_tmp);

    if ((cfg_tmp = cfg_read("DX")) == NULL) {
        fprintf(stderr, "DX is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    dx = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("DY")) == NULL) {
        fprintf(stderr, "DY is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    dy = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("DZ")) == NULL) {
        fprintf(stderr, "DZ is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    dz = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("DT")) == NULL) {
        fprintf(stderr, "DT is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    dt = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("AL")) == NULL) {
        fprintf(stderr, "AL is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    kappa = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("BL")) == NULL) {
        fprintf(stderr, "BL is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    lambda = atof(cfg_tmp);

    if ((cfg_tmp = cfg_read("NPAS")) == NULL) {
        fprintf(stderr, "NPAS is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    Npas = atol(cfg_tmp);

    if ((cfg_tmp = cfg_read("NRUN")) == NULL) {
        fprintf(stderr, "NRUN is not defined in the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    Nrun = atol(cfg_tmp);

    output = cfg_read("OUTPUT");
    initout = cfg_read("INITOUT");
    Npasout = cfg_read("NPASOUT");
    Nrunout = cfg_read("NRUNOUT");

    if ((initout != NULL) || (Npasout != NULL) || (Nrunout != NULL)) {
        if ((cfg_tmp = cfg_read("OUTSTPX")) == NULL) {
            fprintf(stderr, "OUTSTPX is not defined in the configuration file.\n");
            exit(EXIT_FAILURE);
        }
        outstpx = atol(cfg_tmp);

        if ((cfg_tmp = cfg_read("OUTSTPY")) == NULL) {
            fprintf(stderr, "OUTSTPY is not defined in the configuration file.\n");
            exit(EXIT_FAILURE);
        }
        outstpy = atol(cfg_tmp);

        if ((cfg_tmp = cfg_read("OUTSTPZ")) == NULL) {
            fprintf(stderr, "OUTSTPZ is not defined in the configuration file.\n");
            exit(EXIT_FAILURE);
        }
        outstpz = atol(cfg_tmp);
    }

    return;
}

void RealThreeD::alloc_mem() {

    x = new double[Nx];  
    y = new double[Ny];
    z = new double[Nz];

    x2 = new double[Nx];
    y2 = new double[Ny];
    z2 = new double[Nz];

    dpsix = tensor();
    dpsiy = tensor();
    dpsiz = tensor();
    pot = tensor();
    psi = psitensor();
    
    complex<double>* calphax = new complex<double>[Nx - 1];
    complex<double>* calphay = new complex<double>[Ny - 1];
    complex<double>* calphaz = new complex<double>[Nz - 1];
    complex<double>* cbeta = new complex<double>[MAX(Nx, Ny, Nz) - 1];
    complex<double>* cgammax = new complex<double>[Nx - 1];
    complex<double>* cgammay = new complex<double>[Ny - 1];
    complex<double>* cgammaz = new complex<double>[Nz - 1];

    vector<double>* tmpxi= new vector<double>(Nx);
    vector<double>* tmpyi = new vector<double>(Ny);
    vector<double>* tmpzi = new vector<double>(Nz);
    vector<double>* tmpxj= new vector<double>(Nx);
    vector<double>* tmpyj= new vector<double>(Ny);
    vector<double>* tmpzj = new vector<double>(Nz);
}

void RealThreeD::write(FILE* out) {
    fprintf(out, "dkof");
    fprintf(out, "OPTION = %d\n", opt);
    fprintf(out, "NX = %12li   NY = %12li   NZ = %12li\n", Nx, Ny, Nz);
    fprintf(out, "NSTP = %10li   NPAS = %10li   NRUN = %10li\n", Nstp, Npas, Nrun);
    fprintf(out, "DX = %8le   DY = %8le   DZ = %8le\n", dx, dy, dz);
    fprintf(out, "DT = %8le\n", dt);
    fprintf(out, "AL = %8le   BL = %8le\n", kappa, lambda);
    fprintf(out, "G0 = %8le\n\n", G0);
    fprintf(out, "        %12s   %12s   %12s   %12s   %12s\n", "norm", "mu", "en", "rms", "psi(0,0,0)");
    fflush(out);
}

double*** RealThreeD::tensor() {
    long cnti, cntj;
    double*** tensor;
    tensor = new double** [Nx];
    tensor[0] = new double* [Nx * Ny];
    tensor[0][0] = new double[Nx * Ny * Nz];

    for (cntj = 1; cntj < Ny; cntj++)
        tensor[0][cntj] = tensor[0][cntj - 1] + Nz;
    for (cnti = 1; cnti < Nx; cnti++) {
        tensor[cnti] = tensor[cnti - 1] + Ny;
        tensor[cnti][0] = tensor[cnti - 1][0] + Ny * Nz;
        for (cntj = 1; cntj < Ny; cntj++)
            tensor[cnti][cntj] = tensor[cnti][cntj - 1] + Nz;
    }
    return tensor;
}
complex<double>*** RealThreeD::psitensor() {
    long cnti, cntj;
    complex<double>*** tensor;
    tensor = new complex<double>**[Nx];
    tensor[0] = new complex<double>*[Nx * Ny];
    tensor[0][0] = new complex<double>[Nx * Ny * Nz];

    for (cntj = 1; cntj < Ny; cntj++)
        tensor[0][cntj] = tensor[0][cntj - 1] + Nz;
    for (cnti = 1; cnti < Nx; cnti++) {
        tensor[cnti] = tensor[cnti - 1] + Ny;
        tensor[cnti][0] = tensor[cnti - 1][0] + Ny * Nz;
        for (cntj = 1; cntj < Ny; cntj++)
            tensor[cnti][cntj] = tensor[cnti][cntj - 1] + Nz;
    }
    return tensor;
}
void RealThreeD::gencoef() {
    long cnti;

    Ax0 = 1. + I * dt / dx2;
    Ay0 = 1. + I * dt / dy2;
    Az0 = 1. + I * dt / dz2;

    Ax0r = 1. - I * dt / dx2;
    Ay0r = 1. - I * dt / dy2;
    Az0r = 1. - I * dt / dz2;

    Ax = -0.5 * I * dt / dx2;
    Ay = -0.5 * I * dt / dy2;
    Az = -0.5 * I * dt / dz2;

    calphax[Nx - 2] = 0.;
    cgammax[Nx - 2] = -1. / Ax0;
    for (cnti = Nx - 2; cnti > 0; cnti--) {
        calphax[cnti - 1] = Ax * cgammax[cnti];
        cgammax[cnti - 1] = -1. / (Ax0 + Ax * calphax[cnti - 1]);
    }

    calphay[Ny - 2] = 0.;
    cgammay[Ny - 2] = -1. / Ay0;
    for (cnti = Ny - 2; cnti > 0; cnti--) {
        calphay[cnti - 1] = Ay * cgammay[cnti];
        cgammay[cnti - 1] = -1. / (Ay0 + Ay * calphay[cnti - 1]);
    }

    calphaz[Nz - 2] = 0.;
    cgammaz[Nz - 2] = -1. / Az0;
    for (cnti = Nz - 2; cnti > 0; cnti--) {
        calphaz[cnti - 1] = Az * cgammaz[cnti];
        cgammaz[cnti - 1] = -1. / (Az0 + Az * calphaz[cnti - 1]);
    }

    return;
}