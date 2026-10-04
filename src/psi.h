
#include "real3d.h"
#include"math.h"
template<typename T>

T init(T psi, double kappa, double lambda, long Nx, long Ny, long Nz, double* x, double* x2, double* y, double* y2, double* z,
    double* z2, int opt, double par, double dz, double dx, double dy, double*** pot, long& Nx2, long& Ny2, long& Nz2,
    double& dx2, double& dy2, double& dz2) {
    long cnti, cntj, cntk;
    double kappa2, lambda2;
    double pi, cpsi1, cpsi2;
    double tmp;

    if (opt == 2) par = 2.;
    else par = 1.;

    kappa2 = kappa * kappa;
    lambda2 = lambda * lambda;

    Nx2 = Nx / 2; Ny2 = Ny / 2; Nz2 = Nz / 2;
    dx2 = dx * dx; dy2 = dy * dy; dz2 = dz * dz;

    pi = 4. * atan(1.);
    cpsi1 = sqrt(pi * sqrt(pi / (kappa * lambda)));
    cpsi2 = cpsi1 * sqrt(2. * sqrt(2.));

    for (cnti = 0; cnti < Nx; cnti++) {
        x[cnti] = (cnti - Nx2) * dx;
        x2[cnti] = x[cnti] * x[cnti];
        for (cntj = 0; cntj < Ny; cntj++) {
            y[cntj] = (cntj - Ny2) * dy;
            y2[cntj] = y[cntj] * y[cntj];
            for (cntk = 0; cntk < Nz; cntk++) {
                z[cntk] = (cntk - Nz2) * dz;
                z2[cntk] = z[cntk] * z[cntk];
                if (opt == 3) {
                    pot[cnti][cntj][cntk] = 0.25 * (x2[cnti] + kappa2 * y2[cntj] + lambda2 * z2[cntk]);
                    tmp = exp(-0.25 * (x2[cnti] + kappa * y2[cntj] + lambda * z2[cntk]));
                    psi[cnti][cntj][cntk] = tmp / cpsi2;
                }
                else {
                    pot[cnti][cntj][cntk] = x2[cnti] + kappa2 * y2[cntj] + lambda2 * z2[cntk];
                    tmp = exp(-0.5 * (x2[cnti] + kappa * y2[cntj] + lambda * z2[cntk]));
                    psi[cnti][cntj][cntk] = tmp / cpsi1;
                }
            }
        }
    }
    return psi;

}


