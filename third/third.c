#include<stdlib.h>
#include<stdio.h>
void printMatrix(double **, int, int);
double ** makeMatrix(int r, int c)
{
  double ** m = malloc(r * sizeof(double *));
  int i;
  for (i = 0; i < r; i++)
    {
      m[i] = malloc(c * sizeof(double));
    }
  return m;
}
void freeMatrix(double ** m, int r)
{
  int i;
  for (i = 0; i < r; i++)
    {
      free(m[i]);
    }
  free(m);
}
double ** multiply(double ** m1, double ** m2, int r1, int c1, int c2)
{
  double ** mult = makeMatrix(r1, c2);
  int i;
  int j;
  for (i = 0; i < r1; i++)
    {
      for (j = 0; j < c2; j++)
	{
	  mult[i][j]=0;
	}
    }
  int k;
  for (i = 0; i < r1; i++)
    {
      for (j = 0; j < c2; j++)
	{
	  for (k = 0; k < c1; k++)
	    {
	      mult[i][j] += m1[i][k] * m2[k][j];
	    }
	}
    }
  return mult;
}
double ** transpose(double ** m, int r, int c)
{
  double ** trans = makeMatrix(c, r);
  int i;
  int j;
  for (i = 0; i < r; i++)
    {
      for (j = 0; j < c; j++)
	{
	  trans[j][i] = m[i][j];
	}
    }
  return trans;
}
double ** inverse(double ** m, int r)
{
  double ** aug = makeMatrix(r, 2*r);
  int i;
  int j;
  int x;
  //filling augmented matrix
  for (i = 0; i < r; i++)
    {
      for (j = 0; j < r; j++)
	{
	  aug[i][j] = m[i][j];
	  if (i == j)
	    {
	      aug[i][j + r] = 1;
	    }
	  else
	    {
	      aug[i][j + r] = 0;
	    }
	}
    }
  //making all entries below diagonal 0
  for (j = 0; j < r; j++)
    {
      for (i = j + 1; i < r; i++)
	{
	  if (aug[i][j] != 0)
	    {
	      double factor1 = aug[j][j]/aug[i][j];
	      if (factor1 < 0)
		{
		  factor1 = factor1 * -1;
		}
	      if ((aug[j][j] > 0 && aug[i][j] > 0) || (aug[j][j] < 0 && aug[i][j] < 0))
		{
		  for (x = 0; x < 2*r; x++)
		    {
		      aug[i][x] = factor1*aug[i][x]-aug[j][x];
		    }
		}
	      else
		{
		  for (x=0; x < 2*r; x++)
		    {
		      aug[i][x] = factor1*aug[i][x]+aug[j][x];
		    }
		}
	      
	    }
	} 
    }
  //making pivots 1
  for (i = 0; i < r; i++)
    {
      if (aug[i][i] != 1)
	{
	  double factor2 = aug[i][i];
	  for (x = 0; x < 2*r; x++)
	    {
	      aug[i][x] = aug[i][x] / factor2;
	    }
	}
    }
  //making entires above diagonal 0
  for (j = r-1; j >=0; j--)
    {
      for (i = j - 1; i >= 0; i--)
	{
	  if (aug[i][j] != 0 && aug[i][j] != -0)
	    {
	      double factor3 = aug[i][j];
	      for (x = 0; x < 2*r; x++)
		{
		  aug[i][x] = aug[i][x]- factor3*aug[j][x];
		}
	    }
	} 
    }
  //taking the inverse from the augmented matrix
  double ** inverseMatrix = makeMatrix(r, r);
  for (i = 0; i < r; i++)
    {
      for (j = 0; j < r; j++)
	{
	  inverseMatrix[i][j]=aug[i][j+r];
	}
    }
  freeMatrix(aug,r);
  return inverseMatrix;
}
void printMatrix(double ** m, int r, int c)
{
  int i;
  int j;
  for (i = 0; i < r; i++)
    {
      for (j = 0; j < c; j++)
	{
	  if (j == c-1)
	    {
	      printf("%lf\n", m[i][j]);
	    }
	  else
	    {
	      printf("%lf, ", m[i][j]);
	    }
	}
    }
}
int main(int argc, char ** argv)
{
  FILE * train = fopen(argv[1], "r");
  FILE * test = fopen(argv[2], "r");
  int trainRows;
  int trainCols;
  fscanf(train, "%d\n", &trainCols);
  fscanf(train, "%d\n", &trainRows);
  trainCols++;
  double ** X = makeMatrix(trainRows, trainCols);
  double ** Y = makeMatrix(trainRows, 1);
  int i;
  int j;
  for (i = 0; i < trainRows; i++)
    {
      for (j = 0; j < trainCols; j++)
	{
	  if (j == 0)
	    {
	      X[i][j] = 1;
	    }
	  if (j == trainCols-1)
	    {
	      fscanf(train, "%lf\n", &Y[i][0]);
	    }
	  else
	    {
	      fscanf(train, "%lf,", &X[i][j+1]);
	    }
	}
    }
  double ** XTranspose = transpose(X, trainRows, trainCols);
  double ** first = multiply(XTranspose, X, trainCols, trainRows, trainCols);
  freeMatrix(X, trainRows);
  double ** second = inverse(first, trainCols);
  freeMatrix(first, trainCols);
  double ** third = multiply(second, XTranspose, trainCols, trainCols, trainRows);
  freeMatrix(XTranspose, trainCols);
  freeMatrix(second, trainCols);
  double ** W = multiply(third, Y, trainCols, trainRows, 1);
  freeMatrix(third, trainCols);
  freeMatrix(Y, trainRows);
  int predictions;
  fscanf(test, "%d\n", &predictions);
  for (i = 0; i < predictions; i++)
    {
      double ** house = makeMatrix(1, trainCols);
      house[0][0] = 1;
      for (j = 1; j < trainCols; j++)
	{
	  if (j == trainCols - 1)
	    {
	      fscanf(test, "%lf\n", &house[0][j]);
	    }
	  else
	    {
	      fscanf(test, "%lf,", &house[0][j]);
	    }
	}
      double ** result = multiply(house, W, 1, trainCols, 1);
      printf("%0.0lf\n", result[0][0]);
      freeMatrix(house, 1);
      freeMatrix(result, 1);
    }
  //free matrices
  freeMatrix(W, trainCols);
  return 0;

}
