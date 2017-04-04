#include "matrixtoperation.h"

MatrixOperations::MatrixOperations()
{
    m_IA = 0;
    m_JA = 0;
    m_AN = 0;


}



void MatrixOperations::convertToSparse(float **arr, int n, int m)
{
    QVector<int> IA;
    QVector<int> JA;
    QVector<float> AN;
    m_elements_count = 0;
    m_M = m;
    m_N = n;
    if(m_IA)
        delete[] m_IA;
    if(m_JA)
        delete[] m_JA;
    if(m_AN)
        delete[] m_AN;
    for(int i = 0; i < n; i++)
       {
        IA.append(AN.size());
        for(int j = 0;j < m;j++)
        {
            if(arr[i][j] != 0)
            {
                AN.append(arr[i][j]);
                JA.append(j);
                ++m_elements_count;
            }
        }

       }
    m_IA = new int[IA.size()];
    m_JA = new int[JA.size()];
    m_AN = new float[AN.size()];
    for(int i = 0;i < IA.size();++i)
    {
        m_IA[i] = IA[i];
    }
    for(int i = 0;i < JA.size();++i)
    {
        m_JA[i] = JA[i];
        m_AN[i] = AN[i];
    }
}

float MatrixOperations::getVal(int i, int j)
{
    if (m_IA[i] >= m_elements_count)
        return 0.0;

    if (m_N  - 1 != i)
    {
        if (m_IA[i + 1] == m_IA[i])
            return 0.0;
    }

    for (int ind = m_IA[i]; ind < m_elements_count; ++ind)
    {
        if (m_N - 1 != i)
        {
            if (ind == m_IA[i + 1])
                return 0.0;
        }

        if (m_JA[ind] == j)
        {
            return m_AN[ind];
        }
    }

    return 0.0;
}

void MatrixOperations::setVal(int i, int j, float val)
{
    bool is_new_element = false;

    if (m_IA[i] >= m_elements_count)
        is_new_element = true;

    if (m_N  - 1 != i)
    {
        if (m_IA[i + 1] == m_IA[i])
            is_new_element = true;
    }

    for (int ind = m_IA[i]; !is_new_element && ind < m_elements_count; ++ind)
    {
        if (m_N - 1 != i)
        {
            if (ind == m_IA[i + 1])
            {
                is_new_element = true;
                break;
            }
        }

        if (m_JA[ind] == j)
        {
            m_AN[ind] = val;

            return;
        }
    }

    if (is_new_element)
    {
        /* НЕ ЗАКОНЧЕНА */
    }
}

float MatrixOperations::getCurrSum(int i, float *vec)
{
    if (m_IA[i] >= m_elements_count)
        return 0.0;

    if (m_N  - 1 != i)
    {
        if (m_IA[i + 1] == m_IA[i])
            return 0.0;
    }

    float curr_sum = 0.0;

    for (int ind = m_IA[i]; ind < m_elements_count; ++ind)
    {
        if (m_N - 1 != i)
        {
            if (ind == m_IA[i + 1])
            {
                return curr_sum;
            }
        }

        if (m_JA[ind] >= i)
        {
            return curr_sum;
        }

        curr_sum += m_AN[ind]*vec[m_JA[ind]];
    }

    return curr_sum;
}

float MatrixOperations::getPrevSum(int i, float *vec)
{
    if (m_IA[i] >= m_elements_count)
        return 0.0;

    if (m_N  - 1 != i)
    {
        if (m_IA[i + 1] == m_IA[i])
            return 0.0;
    }

    float curr_sum = 0.0;

    for (int ind = m_IA[i]; ind < m_elements_count; ++ind)
    {
        if (m_N - 1 != i)
        {
            if (ind == m_IA[i + 1])
            {
                return curr_sum;
            }
        }

        if (m_JA[ind] > i)
        {
            curr_sum += m_AN[ind]*vec[m_JA[ind]];
        }
    }

    return curr_sum;
}

void MatrixOperations::fillingValues()
{
    L = 1.0/h;
    L_sqv = L*L;
    A = (L_sqv/2)+4;
    B = (L_sqv/12)-1;
    A1 = (L_sqv/4)+2;
    A2 = (L_sqv/6)+1;
    A3 = (L_sqv/12)+1;
    B1 = (L_sqv/24)-0.5;
    C = (L_sqv/12);
    eps = 0.00001;
    Meth = true;
    if(Meth) n = (h - 1)*(h - 1);
       else  n = (h+1)*(h+1);

}

void  MatrixOperations::fillingArray(float **arr,int n)
{

    if(Meth)
    {
        for(int i = 0; i < n; i++ )
        {
        int j;

        arr[i][i]= A;

        j= i - h + 1;
        if(j>=0)
        {
            arr[i][j] = B;
            arr[j][i] = B;
        }

        j= i - 1;
        if(i%(h-1)!=0)
        {
            arr[i][j] = B;
            arr[j][i] = B;
        }

        j= i - h + 2;
        if(j>0 && j%(h-1)!=0)
        {
            arr[i][j] = C;
            arr[j][i] = C;
        }
        }
     }
    else
    {
        for(int i = 0; i < n; i++ )
           {
            int j;

            if(i==0 || i==n-1)
            {
                arr[i][i]=A3;
            }

            if(i==h || i==n-h-1)
            {
                arr[i][i]=A2;
            }

            if(i>0 && i<h)
            {
                arr[i][i]=A1;
            }
            if(i>h && i<n-h-1)
            {
                if(i%(h+1)==0 || i%(h+1)==h) arr[i][i]=A1;
                else arr[i][i]=A;
            }
            if(i>=n-h && i<n-1)
            {
                arr[i][i]=A1;
            }

            j= i - h - 1;
            if(j>=0)
            {
                arr[i][j] = B;
                arr[j][i] = B;
                if(j%(h+1)==0 || j%(h+1)==h)
                {
                    arr[i][j] = B1;
                    arr[j][i] = B1;
                }
            }

            j= i - 1;
            if(j>=0)
            {
                if(i%(h+1)!=0)
                {
                    arr[i][j] = B;
                    arr[j][i] = B;
                }
                if((i<=h || i>=n-h-1) && i%(h+1)!=0)
                {
                    arr[i][j] = B1;
                    arr[j][i] = B1;
                }
            }


            j= i - h;
            if(j>0 && j%(h+1)!=0)
            {
                arr[i][j] = C;
                arr[j][i] = C;
            }
           }
    }
}

void MatrixOperations::zeidel(float **arr, float *previousVariableValues, float *currentVariableValues)
{
    while (true)
         {
             // Введем вектор значений неизвестных на текущем шаге



            for (int i = 0; i < n; i++)
             {

                 currentVariableValues[i] = 0;//спросить по этому поводу

                 // Вычитаем сумму по всем отличным от i-ой неизвестным
               /*  for (int j = 0; j < n; j++)
                 {

                     if (j < i)
                     {
                         currentVariableValues[i] -= arr[i][j] * currentVariableValues[j];
                     }

                     // При j > i используем значения с прошлой итерации
                     if (j > i)
                     {
                         currentVariableValues[i] -= arr[i][j] * previousVariableValues[j];
                     }


                 }*/

                currentVariableValues[i] -= getCurrSum(i, currentVariableValues);
                currentVariableValues[i] -= getPrevSum(i, previousVariableValues);

                currentVariableValues[i] += L_sqv;

                 // Делим на коэффициент при i-ой неизвестной
                 currentVariableValues[i] /= arr[i][i];//arr[i][i];
             }

             // Посчитаем текущую погрешность относительно предыдущей итерации
             long double error = 0.0;

             for (int i = 0; i < n; i++)
             {
                 error += fabs(currentVariableValues[i] - previousVariableValues[i]);
             }

             // Если необходимая точность достигнута, то завершаем процесс
             if (error < eps)
             {
                 break;
             }

             // Переходим к следующей итерации, так
             // что текущие значения неизвестных
             // становятся значениями на предыдущей итерации


             for (int j = 0; j < n; ++j)
                 previousVariableValues[j] = currentVariableValues[j];


         }


}


