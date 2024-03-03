#define N 4
#include <iostream>
#include "mpi.h"

using namespace std;

void print_results(int a[N][N])
{
    int i, j;
    cout << "C = " << endl;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cout << " " << a[i][j] << endl; 
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    int rank, numtasks;
    int i, j, sum = 0;
    int a[N][N] = { {1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7,} };
    int b[N][N] = { {1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7,} };
    int c[N][N]; //результирующая матрица
    int aa[N], cc[N]; //буфферы используемые при распределнении и сборе
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    

    //раcпределение строк первой матрицы по процессам   
    MPI_Scatter(a, N * N / numtasks, MPI_INT, aa, N * N / numtasks, MPI_INT, 0, MPI_COMM_WORLD);

    //передача второй матрицы всем процессам
    MPI_Bcast(b, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    //векторное умножение
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            sum = sum + aa[j] * b[j][i];
        }
        cc[i] = sum;
        sum = 0;
    }

    MPI_Gather(cc, N * N / numtasks, MPI_INT, c, N * N / numtasks, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    if (rank == 0)         
        print_results(c);
}

