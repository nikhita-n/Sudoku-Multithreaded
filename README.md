# Sudoku-Multithreaded Program
Assignment submission for CS252(Minor Course) 

Submitted by 

Nikhita Neelakanta, 191EC227

### Description

A C program to check the validity of a given Sudoku puzzle(can be user input) using threads. If invalid, the program also reports why the puzzle fails to be a Sudoku puzzle.

#### Criteria for checking Validity

- All entries in the puzzle to be within the range [0-9]
- No presence of duplicate entries in rows
- No presence of duplicate entries in columns
- No presence of duplicate entries in subgrids


### Libraries
- ```<pthread.h>```  - for handling thread-related operations
- ```<stdio.h>```    
- ```<stdlib.h>```   
- ```<time.h>```
### Execution
After cloning the repository, go into the local directory and enter the following command on the terminal

```
gcc pthreads_multithreaded.c -o sudoku.out -lpthread
```
### References
- https://man7.org/linux/man-pages/man3/pthread_create.3.html
- https://man7.org/linux/man-pages/man3/pthread_join.3.html

### Future Work

Use various combinations of assigning threads to different regions and compare the time elapsed
