#include<stdlib.h>
#include<stdio.h>
int checkRow(char **, int, int,  char);
int checkCol(char **, int, int,  char);
int checkSect(char **, int, int, char);

int checkInit(char ** g)
{
  int x;
  int y;
  for (x = 0; x < 16; x++)
    {
      for (y = 0; y < 16; y++)
	{
	  if (g[x][y] != '-')
	    {
	      if (checkRow(g, x,y, g[x][y]) == 1 || checkCol(g,x, y, g[x][y]) == 1 || checkSect(g, x, y, g[x][y]) == 1)
		{
		  return 1;
		}
	    }
	}
    }
  return 0;
}
int checkRow(char ** g, int r, int c, char hex)
{
  int i;
  for (i = 0; i < 16; i++)
    {
      if (g[r][i] == hex && i != c)
	{
	  return 1;
	}
    }
  return 0;
}
int checkCol(char ** g,int r,  int c, char hex)
{
  int i;
  for (i = 0; i < 16; i++)
    {
      if (g[i][c] == hex && i != r)
	{
	  return 1;
	}
    }
  return 0;
}
int checkSect(char ** g, int r, int c, char hex)
{
  int i;
  int j;
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
	{
	  if (g[r-r%4+i][c-c%4+j] == hex && r-r%4+i != r && c-c%4+j != c)
	    {
	      return 1;
	    }
	}
    }
  return 0;
}
int countBlanks(char ** g)
{
  int x;
  int y;
  int count = 0;
  for (x = 0; x < 16; x++)
    {
      for(y = 0; y < 16; y++)
	{
	  if (g[x][y] == '-')
	    {
	      count++;
	    }
	}
    }
  return count;
}
int solveLogically(char ** g, char * values)
{
  int numberOfBlanks = countBlanks(g);
  int x;
  int y;
  int hex;
  int counter = 0;
  char h = 'z';
  for (x = 0; x < 16; x++)
    {
      for (y = 0; y < 16; y++)
	{
	  if (numberOfBlanks == 0)
	    {
	      return 1;
	    }
	  if (g[x][y] == '-')
	    {
	      for (hex = 0; hex < 16; hex++)
		{
		  if (checkRow(g, x, y, values[hex]) == 0 && checkCol(g, x, y, values[hex]) == 0 && checkSect(g, x, y, values[hex]) == 0)
		    {
		      counter++;
		      h = values[hex];
		    }
		}
	      if (counter == 1)
		{
		  g[x][y] = h;
		  numberOfBlanks--;
		  x = 0;
		  y = 0;
		}
	      counter = 0;
	    }
	}
    }
  return 0;
}
int solve(char ** g, char * values)
{
  int x;
  int y;
  int checker = 0;
  int cycleValues;
  for (x = 0; x < 16; x++)
    {
      for (y = 0; y < 16; y++)
	{
	  if (g[x][y] == '-')
	    {
	      checker = 1;
	      break;
	    }
	}
      if (checker == 1)
	{
	  break;
	}
    }
  if (checker == 0)
    {
      return 1;
    }
  for (cycleValues = 0; cycleValues < 16; cycleValues++)
    {
      if (checkRow(g, x,y,  values[cycleValues]) == 0 && checkCol(g,x, y, values[cycleValues]) == 0 && checkSect(g, x, y, values[cycleValues]) == 0)
	{
	  g[x][y] = values[cycleValues];
	  if (solve(g, values) == 1)
	    {
	      return 1;
	    }
	  else
	    {
	      g[x][y] = '-';
	    }
	}
    }
  return 0;
}
int main(int argc, char ** argv)
{
  FILE *f = fopen(argv[1], "r"); 
  char ** grid = malloc(16 * sizeof(char *));
  char * hexValues = malloc(16 * sizeof(char));
  int i;
  int j;
  hexValues[0] = '0';
  hexValues[1] = '1';
  hexValues[2] = '2';
  hexValues[3] = '3';
  hexValues[4] = '4';
  hexValues[5] = '5';
  hexValues[6] = '6';
  hexValues[7] = '7';
  hexValues[8] = '8';
  hexValues[9] = '9';
  hexValues[10] = 'A';
  hexValues[11] = 'B';
  hexValues[12] = 'C';
  hexValues[13] = 'D';
  hexValues[14] = 'E';
  hexValues[15] = 'F';
  for (i = 0; i < 16; i++)
    {
      grid[i] = malloc(16 * sizeof(char));
      for (j = 0; j < 16; j++)
	{
	  if (j == 15)
	    {
	      fscanf(f, "%c\n", &grid[i][j]);
	      //  printf("%c\n", grid[i][j]);
	    }
	  else
	    {
	      fscanf(f, "%c\t", &grid[i][j]);
	      //  printf("%c ", grid[i][j]);
	    }
	}
    }
  if (checkInit(grid) == 1)
    {
      printf("no-solution\n");
    }
  else
    {
      solveLogically(grid, hexValues);
      int solution = solve(grid, hexValues);
      if (solution == 1)
	{
	  for (i = 0; i < 16; i++)
	    {
	      for (j = 0; j < 16; j++)
		{
		  if (j == 15)
		    {
		      printf("%c\n", grid[i][j]);
		    }
		  else
		    {
		      printf("%c\t", grid[i][j]);
		    }
		}
	    }
	}
      else
	{
	  printf("no-solution\n");
	}
    }
  fclose(f);
  for (i = 0; i < 16; i++)
  {
    free(grid[i]);
  }
  free(grid);
  free(hexValues);
  return 0;
}
