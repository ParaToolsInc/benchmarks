%module mm
%{
extern double** allocateMatrix(int rows, int cols);
extern void freeMatrix(double** matrix, int rows, int cols);
extern void initialize(double **matrix, int rows, int cols);
extern char * mm(void);
void timestep(double time);
%}

extern double** allocateMatrix(int rows, int cols);
extern void freeMatrix(double** matrix, int rows, int cols);
extern void initialize(double **matrix, int rows, int cols);
extern char * mm(void);
void timestep(double time);
