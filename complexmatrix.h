#ifndef COMPLEXMATRIX_H
#define COMPLEXMATRIX_H
#include <complex>

class ComplexMatrix
{
public:
    static void getCofactor(std::complex<double> **mat,std::complex<double> **cof,int i,int j,int n);
    static std::complex<double> getDeterminant(std::complex<double> **mat,int n);
    static void getAdjoint(std::complex<double> **mat,std::complex<double> **adj,int n);
    static bool getInverse(std::complex<double> **mat,std::complex<double> **inv,int n);
private:
    ComplexMatrix();
};

#endif // COMPLEXMATRIX_H
