#ifndef MATRIXTRANSFORMATION_H
#define MATRIXTRANSFORMATION_H
#include <iostream>
#include <Qvector>
#include <cmath>
#include <stdio.h>

class MatrixOperations
{
public:

    MatrixOperations();

    int h;
    float A,B,C,L,L_sqv,A1,A2,A3,B1;
    bool Meth;
    int n;
    long double eps;

    void convertToSparse(float **arr,int n, int m);
    float getVal(int i,int j);
    void setVal(int i, int j, float val);

    float getCurrSum(int i, float *vec);
    float getPrevSum(int i, float *vec);

    void fillingValues();
    void fillingArray(float **arr, int n);
    void printMatrix();

    void zeidel(float **arr, float *previousVariableValues,float *currentVariableValues);


protected:
    int m_N, m_M, m_elements_count;

    int *m_IA, *m_JA;

    float *m_AN;


};

#endif // MATRIXTRANSFORMATION_H
