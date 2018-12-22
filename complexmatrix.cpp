#include "complexmatrix.h"

void ComplexMatrix::getCofactor(std::complex<double> **mat, std::complex<double> **cof, int p, int q, int n)
{
    using namespace std;
    int row=0,col=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i!=p&&j!=q){
                cof[row][col++]=mat[i][j];
                if(col==n-1){   //row finished
                    row++;
                    col=0;
                }
            }
        }
    }
}

std::complex<double> ComplexMatrix::getDeterminant(std::complex<double> **mat, int n)
{
    using namespace std;
    if(n==1)return mat[0][0];    //base case
    complex<double> det(0,0),**tmp;
    tmp=new complex<double>*[n];
    for(int i=0;i<n;i++)
        tmp[i]=new complex<double>[n];
    double sign=1;
    for(int j=0;j<n;j++){   //for each column in first row
        getCofactor(mat,tmp,0,j,n);     //get cofactor
        det+=sign*mat[0][j]*getDeterminant(tmp,n-1);
        sign=-sign;
    }
    for(int i=0;i<n;i++)
        delete [] tmp[i];
    delete [] tmp;
    return det;
}

void ComplexMatrix::getAdjoint(std::complex<double> **mat, std::complex<double> **adj,int n)
{
    using namespace std;
    if(n==1){
        adj[0][0]=1;
        return;
    }
    double sign=1;
    complex<double> **tmp;
    tmp=new complex<double>*[n];
    for(int i=0;i<n;i++)
        tmp[i]=new complex<double>[n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            getCofactor(mat,tmp,i,j,n);
            sign = ((i+j)%2==0)? 1: -1;
            adj[j][i]=sign*getDeterminant(tmp,n-1); //[j][i] for transpose
        }
    }
    for(int i=0;i<n;i++)
        delete [] tmp[i];
    delete [] tmp;
}

bool ComplexMatrix::getInverse(std::complex<double> **mat, std::complex<double> **inv,int n)
{
    using namespace std;
    complex<double> det(0,0);
    det=getDeterminant(mat,n);
    if(det==complex<double>(0,0)){
        return false;
    }
    complex<double> **adj;
    adj=new complex<double>*[n];
    for(int i=0;i<n;i++)
        adj[i]=new complex<double>[n];
    getAdjoint(mat,adj,n);  //find adjoint
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            inv[i][j]=adj[i][j]/det;
        }
    for(int i=0;i<n;i++)
        delete [] adj[i];
    delete [] adj;
}

ComplexMatrix::ComplexMatrix()
{

}
