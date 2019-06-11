/*
	Apuntadores y Arreglos
	Valter Núñez - A01206138
	22 de enero de 2019
*/

// Library
#include <stdio.h>
#include <stdlib.h>

//
typedef unsigned int uint;

//Declaration data structure
typedef struct {
	double *data;
	uint nrows, ncols;
} Matrix;

//Print Matrix procedure -- DONE
void printm(Matrix *M) {
	//Declaration variables for
	int i,j;
	
	//FOR cycles to print Matrix
	for (i=0; i<M->nrows; i++)
	{
		for (j=0; j<M->ncols; j++)
		{
			printf(" %.2f ", M-> data[i*(M->ncols) + j]);
		};
		//Print skip line
		printf("\n");
	};
	//Print skip line
	printf("\n");
}

//Memory allocation for Matrix -- DONE
Matrix* alloc_matrix(uint nrows, uint ncols) {
	
	//Validation that Rows and Columns are bigger than 0
	if(nrows<1 || ncols<1)
	{
		printf("Nrows and ncols must be greater than zero. \n");
		return NULL;
	};
	
	//Memory allocation for Matrix
	Matrix *M = malloc(sizeof(Matrix));

	//Error if can't be allocated
	if (M == NULL)
	{
		return NULL;
	}
	
	//Asign values
	M->nrows = nrows;
	M->ncols = ncols;
	M->data = malloc(sizeof(double) * nrows * ncols);

	//Frees memory if allocated and has an error
	if (M->data == NULL)
	{
		free(M->data);
		return NULL;
	}
  
	return M;

}

void set(Matrix *M, uint row, uint col, double val) {
	
	//Validates Matrix
	if (M != NULL)
	{
		if(row <= (M->nrows - 1) && col <= (M->ncols -1))
		{
			M->data [row *(M->ncols) + col] = val;
		}
	}
	//Returns error message
	else
	{
		printf("The matrix must be valid. \n");
	}
	
}

//Multiplication of 2 Matrixes
void matrix_mult(Matrix *A, Matrix *B, Matrix *C) {
	
	//Declaration variable for 
	int i, j, z;
	
	//Validates Matrix
	if (A!=NULL && B!=NULL && C!=NULL)
	{
		//Validates the multiplication can be made
		if (A->ncols == B->nrows && A->nrows == C->nrows && B-> ncols == C->ncols)
		{
			//Multiplies the matrix
			for (i=0; i < A->nrows; i++)
			{
				for (j=0; j < B->ncols; j++)
				{
					for (z=0; z < B->nrows; z++)
					{
						C->data [i*(C->ncols) +j] += (A->data[i*(A->ncols) +z] * B->data[z*(B->ncols) +j]);
					}
				}
			}
		}
		
		//Error Messages
		else if (A->ncols != B->nrows)
		{
			printf("A->ncols must ve equal to B->nrows. \n");
		}
		else if (A->nrows != C->nrows)
		{
			printf("A->nrows must be equal to C->nrows. \n");
		}
		else if(B->ncols != C->ncols)
		{
    		printf("B->ncols must be equal to C->ncols. \n");
		}
		else
		{
    		 printf("A->ncols must be equal to B->nrows. \n");
		}
	}
	else
	{
		printf("The matrixes must be valid. \n");
	}	
	}
	


//Free memory allocated to the matrix -- DONE
void free_matrix(Matrix *M) {
	
	//Validates Matrix
	if (M != NULL)
	{
		free(M->data);
		free(M);
	}
	//In case of error in last validation
	else
	{
		printf("The matrix must be a valid one. \n");
	}
	
}


int main(int argc, char* argv[]) {
	printf("Creating the matrix A:\n");
	Matrix *A = alloc_matrix(3, 2);
	
	printf("Setting the matrix A:\n");
	set(A, 0, 0, 1.2);
	set(A, 0, 1, 2.3);
	set(A, 1, 0, 3.4);
	set(A, 1, 1, 4.5);
	set(A, 2, 0, 5.6);
	set(A, 2, 1, 6.7);
	printf("Printing the matrix A:\n");
	printm(A);
	
	printf("Creating the matrix B:\n");
	Matrix *B = alloc_matrix(2, 3);
	printf("Setting the matrix B:\n");
	set(B, 0, 0, 5.5);
	set(B, 0, 1, 6.6);
	set(B, 0, 2, 7.7);
	set(B, 1, 0, 1.2);
	set(B, 1, 1, 2.1);
	set(B, 1, 2, 3.3);
	printf("Printing the matrix B:\n");
	printm(B);
	
	printf("Creating the matrix C:\n");
	Matrix *C = alloc_matrix(3, 3);
	printf("A x B = C:\n");
	matrix_mult(A, B, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	printf("B x A = C:\n");
	matrix_mult(B, A, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	Matrix *D = NULL;
	printf("Setting a NULL matrix (D):\n");
	set(D, 0, 0, 10);
	
	printf("A x D(NULL) = C:\n");
	matrix_mult(A, D, C);
	printf("D(NULL) x A = C:\n");
	matrix_mult(D, A, C);
	
	printf("Allocating E with (0,0): \n");
	Matrix *E = alloc_matrix(0, 0);
	
	printf("Freeing A:\n");
	free_matrix(A);
	printf("Freeing B:\n");
	free_matrix(B);
	printf("Freeing C:\n");
	free_matrix(C);
	printf("Freeing D(NULL):\n");
	free_matrix(D);
	printf("Freeing E(NULL):\n");
	free_matrix(E);

	return 0;
}
