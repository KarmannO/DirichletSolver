#include <QApplication>
#include <iostream>
#include <Qvector>
#include <cmath>
#include <stdio.h>
#include <matrixtoperation.h>
#include <widget.h>
using namespace std;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MatrixOperations values;
    cout << "Input h: ";
    while(!(cin >> values.h) || values.h <= 0)
    {
        cout<<"Input h: ";
        cin.clear();
        std::string ignoreLine;
        std::getline(cin, ignoreLine);
    }
    values.fillingValues();
    int n = values.n;

    float **arr = new float*[n];
     for(int i = 0;i<n;i++) arr[i] = new float [n];

    for(int i = 0; i<n; i++)
        for(int j = 0;j<n;j++)
        {
            arr[i][j] = 0;
        }


    values.fillingArray(arr,n);
    values.convertToSparse(arr, n, n);

  /*  for(int i = 0; i<n; i++)
    {
        for(int j = 0;j<n;j++)
        {
            cout << arr[i][j] << "   " ;
        }
     cout << endl;
    }*/
    float *previousVariableValues = new float[n];
    for(int i = 0;i<n;i++)
        previousVariableValues[i] = 0;

    float *currentVariableValues = new float[n];


    values.zeidel(arr,previousVariableValues,currentVariableValues);

cout<<"Vector: ";

    for (int i = 0; i < n; i++)
        {

            printf("%.4f\t", previousVariableValues[i]);
        }

    //printf("\n%i\t", values.h);

    Widget w;
    w.setH(previousVariableValues,values.h);
    w.show();

    return a.exec();
}



