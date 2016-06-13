#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

extern "C" {
   void timestep_(const double&);
}

#define MATRIX_SIZE 1024
#define NRA MATRIX_SIZE
#define NCA MATRIX_SIZE
#define NCB MATRIX_SIZE

double** allocateMatrix(int rows, int cols) {
  int i;
  double **matrix = (double**)malloc((sizeof(double*)) * rows);
  for (i=0; i<rows; i++) {
    matrix[i] = (double*)malloc((sizeof(double)) * cols);
  }
  return matrix;
}

void freeMatrix(double** matrix, int rows, int cols) {
  int i;
  for (i=0; i<rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void initialize(double **matrix, int rows, int cols) {
  int i,j;
  for (i=0; i<rows; i++) {
    for (j=0; j<cols; j++) {
      matrix[i][j]= i+j;
    }
  }
}

void timestep(double time){
    timestep_(time);
}

char * mm(void)
{

    int i, j, k;
    double **a, **b, **c;
    char *msg;
    msg = (char*)malloc(5*sizeof(char));
    a = allocateMatrix(NRA, NCA);
    b = allocateMatrix(NCA, NCB);
    c = allocateMatrix(NRA, NCB);

    initialize(a, NRA, NCA);
    initialize(b, NCA, NCB);
    initialize(c, NRA, NCB);


    for(i = 0; i < NRA; i++) {
      for(k = 0; k < NCA; k++) {
        for(j = 0; j < NCB; j++) {
          c[i][j] = a[i][k]*b[k][j];
        }
      }
    }

   printf("C[0:4,0:4]=\n");
   for(i = 0; i < 4; i++) {
     for(j = 0; j < 4; j++)
       printf("%f ", c[i][j]);
     printf("\n");
   }

    freeMatrix(a, NRA, NCA);
    freeMatrix(b, NCA, NCB);
    freeMatrix(c, NRA, NCB);

    msg = "done";

    return msg;
}
