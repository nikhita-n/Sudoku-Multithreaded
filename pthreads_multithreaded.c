#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define NUM_THREADS 9
//data
int puzzle[9][9] = {{1,2,3,6,7,8,9,4,5},
                    {5,8,4,2,3,9,7,6,1},
                    {9,6,7,1,4,5,3,2,8},
                    {3,7,2,4,6,1,5,8,9},
                    {6,9,1,5,8,3,2,7,4},
                    {4,5,8,7,9,2,6,1,3},
                    {8,3,6,9,2,4,1,5,7},
                    {2,1,9,8,5,7,4,3,6},
                    {7,4,5,3,1,6,8,9,2}};
int var[9] = {0,1,2,3,4,5,6,7,8}; //this is the row/column/grid identifier we pass to the threads
int rows[9] = {0};
int column[9] = {0};
int grid_33[9] = {0};
int in_range[9] = {0};
int result = 0;
//User defined function to handle input and output of the Sudoku Puzzle
void input ();
void output ();
//threads call these functions
void * row_check_runner (void *param);
void * col_check_runner (void *param);
void * grid_check_runner (void *param);
void * check_if_in_range_runner (void *param);
//User defined functions to handle thread related operations
void create_threads (pthread_t *,pthread_attr_t,void *param);
void all_check (pthread_t *,pthread_attr_t);
int check_thread_return (int *);

int main ()
{
  int c;
  printf ("\nSudoku Validity checking program using multiple threads.\n\n\nChoose\n1.Input a puzzle\n2.Default Puzzle\n");
  scanf ("%d",&c);
  switch (c)
    {
      case 1:
        input ();
        break;
      case 2:
      default:
        break;
    }
  output ();
  pthread_t workers[NUM_THREADS];   //thread identifier
  pthread_attr_t attr;   //set of thread attributes
  pthread_attr_init (&attr); //setting default attributes of the thread
  printf ("\n\nResults\n");
  all_check (workers,attr);
  if (result)
    {
      printf ("\n\nYes, the given puzzle is a sudoku puzzle\n");
    }
  else
    {
      printf ("\n\nNo, the given puzzle is not a sudoku puzzle\n");
    }
  return 0;
}

void all_check (pthread_t *workers,pthread_attr_t attr)
{
  //CHECKING IF THE PUZZLE HAS ENTRIES IN THE RANGE, 1 to 9
  create_threads (workers,attr,check_if_in_range_runner);
  if (check_thread_return (in_range))
    {
      printf ("\nIn Range [1-9] -CHECK");
      //ROW_CHECK
      create_threads (workers,attr,row_check_runner);
      if (check_thread_return (rows))
        {
          printf ("\nRows -CHECK");
          //COLUMN CHECK
          create_threads (workers,attr,col_check_runner);
          if (check_thread_return (column))
            {
              printf ("\nColumns -CHECK");
              //GRID CHECK
              create_threads (workers,attr,grid_check_runner);
              if (check_thread_return (grid_33))
                {
                  printf ("\n3x3 Grids -CHECK");
                  result = 1;
                }
              else
                {
                  result = 0;
                  printf ("\nDuplicate entries found in 3x3 grids");
                }
            }
          else
            {
              result = 0;
              printf ("\nDuplicate entries found in columns");
            }
        }
      else
        {
          result = 0;
          printf ("\nDuplicate entries found in rows");
        }
    }
  else
    {
      printf ("\nOut of range[1-9] entries found");
      result = 0;
    }
}
void create_threads (pthread_t *workers,pthread_attr_t attr,void *param)
{
  //creating threads
  for (int i = 0; i < NUM_THREADS; i++)
    {
      pthread_create (&workers[i],&attr,param,&var[i]);
    }

  //waiting for threads to exit
  for (int i = 0; i < NUM_THREADS; i++)
    {
      pthread_join (workers[i],NULL);
    }
}
void * row_check_runner (void *param)
{
  int *temp = (int*)param;
  int row = *temp;
  int flag = 0;
  //checking duplicate entries
  for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 9; j++)
        {
          if (puzzle[row][i] == puzzle[row][j])
            {
              flag = 1;
              break;
            }
        }
    }
  if (flag == 0)
    {
      rows[row] = 1;
    }
  else
    {
      rows[row] = 0;
    }
  //printf(" rows array %d ",rows[row]);
  pthread_exit (0);
}

void * col_check_runner (void *param)
{
  int col = *((int*)param);
  int flag = 0;
  //checking duplicate entries
  for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 9; j++)
        {
          if (puzzle[i][col] == puzzle[j][col])
            {
              flag = 1;
              break;
            }
        }
    }
  if (flag == 0)
    {
      column[col] = 1;
    }
  else
    {
      column[col] = 0;
    }
  pthread_exit (0);
}

void * grid_check_runner (void *param)
{
  int grid = *((int*)param);
  int flag = 0;
  int k,l,m,n;
  k = (grid / 3) * 3;
  l = (grid % 3) * 3;
  m = k;
  n = l + 1;
  //checking duplicate entries
  for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 9; j++)
        {
          if (puzzle[k][l] == puzzle[m][n])
            {
              flag = 1;
              break;
            }
          m += ((n + 1) - (grid % 3) * 3) / 3;
          n = (n + 1) % 3 + (grid % 3) * 3;
        }
      k += ((l + 1) - (grid % 3) * 3) / 3;
      l = (l + 1) % 3 + (grid % 3) * 3;
      m = k + ((l + 1) - (grid % 3) * 3) / 3;
      n = (l + 1) % 3 + (grid % 3) * 3;

    }
  if (flag == 0)
    {
      grid_33[grid] = 1;
    }
  else
    {
      grid_33[grid] = 0;
    }
  pthread_exit (0);
}

void * check_if_in_range_runner (void *param)
{
  int *temp = ((int*)param);
  int flag = 0;
  int row = *temp;
  //checking if in range
  for (int i = 0; i < 9; i++)
    {
      if (puzzle[row][i] > 9 || puzzle[row][i] < 1)
        {
          flag = 1;
          break;
        }
    }
  if (flag == 0)
    {
      in_range[row] = 1;
    }
  else
    {
      in_range[row] = 0;
    }
  pthread_exit (0);
}

int check_thread_return (int *array)
{
  for (int i = 0; i < NUM_THREADS; i++)
    {
      if (array[i] == 0)
        {
          return 0;
        }
    }
  return 1;
}
void input ()
{
  for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
        {
          scanf ("%d",&puzzle[i][j]);
        }
    }
}
void output ()
{
  printf ("\n\nThe chosen Sudoku Puzzle is:\n\n");
  for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
        {
          printf ("%d ",puzzle[i][j]);
          if (j % 3 == 2)
            {
              printf ("|");
            }
        }
      if (i % 3 == 2)
        {
          printf ("\n---------------------");
        }
      printf ("\n");
    }
}