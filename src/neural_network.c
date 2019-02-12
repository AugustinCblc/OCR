# include <stdlib.h>
# include <stdio.h>
# include <math.h>

#define Xi	4
#define Xj	2

#define Yi	4
#define Yj	1

FILE *f = NULL;

double **init_tab(double **tab, int m, int n)
{
	tab = malloc(sizeof(double *) * m);
	for (int i = 0; i < m; i++)
	{
		tab[i] = malloc(sizeof(double) * n);
	}

	return tab;
}

double **minus_mat(double **A, double **B, size_t N, size_t M)
{
	size_t I, J;

    double **C = malloc(0);
	C = init_tab(C, N, M);

        for (I=0; I < N; I++)
        {
                for (J=0; J < M; J++)
                {
                        C[I][J]=0;
                        C[I][J] += A[I][J] - B[I][J];
                }
        }

        return C;
}

double **mult_mat(double **A, double **B, size_t N, size_t M)
{
	size_t I, J;

        double **C = malloc(0);
	C = init_tab(C, N, M);

        for (I=0; I < N; I++)
        {
                for (J=0; J < M; J++)
                {
                        C[I][J]=0;
                        C[I][J] += A[I][J] * B[I][J];
                }
        }

        return C;
}

double **dot_mat(double **A, double **B, size_t N, size_t M, size_t P)
{
	size_t I, J, K;

	double **C = malloc(0);
	C = init_tab(C, N, P);

	for (I=0; I<N; I++)
	{
     		for (J=0; J<P; J++)
        	{
          		C[I][J]=0;
          		for (K=0; K<M; K++)
			{
        	       		C[I][J] += A[I][K]*B[K][J];
			}
		}
        }

	return C;
}

double **transp_mat(double**A, size_t m, size_t n)
{
	double **C = malloc(0);
	C = init_tab(C, n, m);

        size_t i, j;
        for(i = 0; i < m; i++)
        {
                for(j = 0; j < n; j++)
                        C[j][i] = A[i][j];
        }

	return C;
}


double **sigmo(double **X, size_t m, size_t n, int deriv)
{
	double **A = malloc(0);
	A = init_tab(A, m, n);

	for(size_t i = 0; i < m; i++)
	{
		for(size_t j = 0; j < n; j++)
		{
			if(deriv == 1)
				A[i][j] = X[i][j] * (1 - X[i][j]);
			else
				A[i][j] = 1/(1 + exp(-X[i][j]));
		}
	}

	return A;
}

double RandomEqualREAL(double Low, double High)
{
  	return ((double) rand() / RAND_MAX) * (High-Low) + Low;
}

double **init_w(double **w, int m, int n)
{
	int i, j;
        for(i = 0; i < m; i++)
        {
                for(j = 0; j < n; j++)
				{
					double randN = RandomEqualREAL(-1, 0.5);
                    w[i][j] = randN;  //W1 = 2, 3
				}
        }

	return w;
}

void print_tab(double **X, size_t m, size_t n)
{
	for(size_t i = 0; i < m; i++)
	{
		for(size_t j = 0; j < n; j++)
			printf("| %lf |", X[i][j]);
		printf("\n");
	}
}

double array_max(double **A, size_t m, size_t n)
{
	double a = fabs(A[0][0]);

	for(size_t i = 0; i < m; i++)
	{
		for(size_t j = 0; j < n; j++)
		{
			if (a < fabs(A[i][j]))
				a = fabs(A[i][j]);
		}
	}

	return a;
}

void write_tab(double **X, size_t m, size_t n)
{
	for(size_t i = 0; i < m; i++)
	{
		for(size_t j = 0; j < n; j++)
			fprintf(f, "| %lf |", X[i][j]);
		fprintf(f, "\n");
	}
}

int main()
{
	f = fopen("XOR.txt", "w");

	double **X = malloc(0);
	X = init_tab(X, Xi, Xj);
		X[0][0] = 0; X[0][1] = 0;
		X[1][0] = 0; X[1][1] = 1;
		X[2][0] = 1; X[2][1] = 0;
		X[3][0] = 1; X[3][1] = 1;


	double **Y = malloc(0);
	Y = init_tab(Y, Yi, Yj);
		Y[0][0] = 0;
		Y[1][0] = 1;
		Y[2][0] = 1;
		Y[3][0] = 0;

	fprintf(f, "Inputs :\n\n           X\n\n");
	write_tab(X, Xi, Xj);
	fprintf(f, "\n      Y\n\n");
	write_tab(Y, Yi, Yj);

	double **W1 = malloc(0);
	W1 = init_tab(W1, Xj, Xi);
	W1 = init_w(W1, Xj, Xi);

	double **W2 = malloc(0);
	W2 = init_tab(W2, Yi, Yj);
	W2 = init_w(W2, Yi, Yj);

	double **L1;
	double **L2;
	double error = 1;

	int compt = 0;

	//for (int i = 0; i < 8000; i++)
	while(error > 0.01)
	{
		double **L0 = malloc(0);
		init_tab(L0, Xi, Xj);
		L0 = X;
		L1 = malloc(0);
		init_tab(L1, Yi, Yj);
		L1 = sigmo(dot_mat(L0, W1, Xi, Xj, Xi), Xi, Xi, 0);

		L2 = sigmo(dot_mat(L1, W2, Xi, Xi, Yj), Yi, Yj, 0);

		double **L2_error;
		L2_error = minus_mat(Y, L2, Yi, Yj);
		error = array_max(L2_error, Yi, Yj);


		double **L2_delta;
		L2_delta = mult_mat(L2_error, sigmo(L2, Yi, Yj, 1), Yi, Yj);

		double **L1_error;
		L1_error = dot_mat(L2_delta, transp_mat(W2, Yi, Yj), Yi, Yj, Yi);

		double **L1_delta;
		L1_delta = mult_mat(L1_error, sigmo(L1, Xi, Xi, 1), Xi, Xi);

		double **res;
		double **t = transp_mat(L0, Xi, Xj);
		res = dot_mat(t, L1_delta, Xj, Xi, Xi);

		for(int i = 0; i < Xj; i++)
		{
			for(int j = 0; j < Xi; j++)
				W1[i][j] += res[i][j];
		}

		double **res2 = malloc(0);
		res2 = init_tab(res2, Xi, Yj);
		res2 = dot_mat(transp_mat(L1, Xi, Yi), L2_delta, Xi, Yi, Yj);

		for(int i = 0; i < Xi; i++)
		{
			for(int j = 0; j < Yj; j++)
				W2[i][j] += res2[i][j];
		}

		compt++;
		if(compt % 10000 == 0 || compt == 1)
		{
			printf("Après %d exécution :\n", compt);
			print_tab(L2, Yi, Yj);
		}
	}

	printf("Final : (après %d exécution)\n", compt);
	print_tab(L2, Yi, Yj);

	fprintf(f, "\n\n\nOutput :\n\n");
	write_tab(L2, Yi, Yj);

	fclose(f);
	return 0;
}
