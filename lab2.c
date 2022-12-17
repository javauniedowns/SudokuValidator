#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "lab2.h"


#define MWIDTH  100
#define MHEIGHT 100

#define ROW_SIZE 9
#define COL_SIZE ROW_SIZE

pthread_mutex_t lock;
static char* FileName;
static bool FirstRun = true;
static int matrixValid = 1;


struct MyStruc
{
  int arr[ROW_SIZE][COL_SIZE];
  int arr2[ROW_SIZE][COL_SIZE];
  int arr3[ROW_SIZE][COL_SIZE];
};

int** read_board_from_file(char* filename)
{

    if (FirstRun == true)
    {
        FileName = malloc(100);
        strncat(FileName, filename, 100);
        FirstRun = false;
    }

    int** sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
        sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    }

    char *pointer = malloc(100);

    FILE *pointer2 = fopen(filename, "r");
    char *pointer3 = malloc(100);

    int arr[ROW_SIZE][COL_SIZE];

    int count = 0;
    for(int i = 0; i<COL_SIZE; i++)
    {
        fgets(pointer, 100, pointer2);
        strncat(pointer3, pointer, 17);
        if (i!=ROW_SIZE-1)
        {
            strncat(pointer3, ",", 2);
        }
        //printf("%s\n",pointer3);
        for (int j = 0; j < COL_SIZE; j++)
        {
            arr[i][j] = atoi(&pointer3[count]);
            count = count + 2;
        }
        /*
        sudoku_board[0] = arr[0];
        sudoku_board[1] = arr[1];
        sudoku_board[2] = arr[2];
        sudoku_board[3] = arr[3];
        sudoku_board[4] = arr[4];
        sudoku_board[5] = arr[5];
        sudoku_board[6] = arr[6];
        sudoku_board[7] = arr[7];
        sudoku_board[8] = arr[8];
        */
        for(int i = 0; i<ROW_SIZE; i++)
        {
            for (int j = 0; j<COL_SIZE; j++)
            {
               sudoku_board[i][j] = arr[i][j];
            }
        }
    }

    return sudoku_board;
}

struct MyStruc Array_From_3X3()
{


    struct MyStruc cArray;

    int n = 0;

    int rowMin = 0;
    int rowMax = 3;

    int collMin = 0;
    int collMax = 3;


    for(int k = 0; k<ROW_SIZE; k++)
    {

        for(int i = rowMin; i<rowMax; i++)
        {
            for(int j = collMin; j<collMax; j++)
            {
                cArray.arr3[k][n] = read_board_from_file(FileName)[i][j];
                n++;
            }
        }
        collMin += 3;
        collMax += 3;

        if(collMax > COL_SIZE)
        {
            collMax = 3;
            collMin = 0;
            rowMax += 3;
            rowMin += 3;
        }
        n = 0;
    }

    return cArray;

}

struct MyStruc Array_From_RowOrColl(int value)
{

    //Create and fill array
    struct MyStruc nArray;

    for(int i = 0; i<ROW_SIZE; i++)
    {
        for(int j = 0; j<COL_SIZE; j++)
        {
            if (value == 0)
            {
                nArray.arr[i][j] = read_board_from_file(FileName)[i][j];

            }else{

                nArray.arr2[j][i] = read_board_from_file(FileName)[i][j];
            }
        }
    }

    return nArray;
}

void *Array_Check(void *nValue)
{
    pthread_mutex_lock(&lock);

    int *nVal = (int*)(nValue);

    struct MyStruc Array;

    if(*nVal == 0)
    {
        //printf("--------------------------Matrix of the rows\n");
        Array = Array_From_RowOrColl(0);
    }
    else if(*nVal == 1)
    {
        //printf("--------------------------Matrix of the collums\n");
        Array = Array_From_RowOrColl(1);
    }
    else
    {
        //printf("--------------------------Matrix of each 3X3 matrix\n");
        Array = Array_From_3X3();
    }

    //Create 2d array of zeros
    int arr[ROW_SIZE][COL_SIZE];
    for(int i = 0; i<ROW_SIZE; i++)
    {
        for (int j = 0; j<COL_SIZE; j++)
        {
            arr[i][j] = 0;
        }
    }

    //Replace zeros with 1 if value is found
    for(int i = 0; i<ROW_SIZE; i++)
    {
        for (int j = 0; j<COL_SIZE; j++)
        {
            if(*nVal == 0)
            {
                arr[i][Array.arr[i][j]-1] = (arr[i][Array.arr[i][j]-1]) + 1;
            }
            else if(*nVal == 1)
            {
                arr[i][Array.arr2[i][j]-1] = (arr[i][Array.arr2[i][j]-1]) + 1;
            }
            else
            {
                arr[i][Array.arr3[i][j]-1] = (arr[i][Array.arr3[i][j]-1]) + 1;
            }
        }
    }

    //Print original 2d array
    /*
    for(int i = 0; i<ROW_SIZE; i++)
    {
        for (int j = 0; j<COL_SIZE; j++)
        {
            if(*nVal == 0)
            {
                printf("%d ", Array.arr[i][j]);
            }
            else if(*nVal == 1)
            {
                printf("%d ", Array.arr2[i][j]);
            }
            else
            {
                printf("%d ", Array.arr3[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
    */


    //Print 2d array of found values & Check for non-zero values
    for(int i = 0; i<ROW_SIZE; i++)
    {
        for (int j = 0; j<COL_SIZE; j++)
        {
            //printf("%d ", arr[i][j]);
            if (arr[i][j] != 1)
            {
                matrixValid = 0;
            }
        }
        //printf("\n");
    }

    //printf("\n");
    pthread_mutex_unlock(&lock);
}

int is_board_valid(){
    //pthread_t* tid;  /* the thread identifiers */
    //pthread_attr_t attr;
    //param_struct* parameter;

    if (pthread_mutex_init(&lock, NULL) !=0)
    {
        printf("Mutex initilization failed.\n");
    }

    int val1 = 0;
    int val2 = 1;
    int val3 = 2;

    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, Array_Check, (void*) &val1);
    pthread_create(&thread2, NULL, Array_Check, (void*) &val2);
    pthread_create(&thread3, NULL, Array_Check, (void*) &val3);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return matrixValid;
}

/*
int main()
{

    read_board_from_file("invalid_board.txt");
    is_board_valid();
    //printf("%d\n", is_board_valid());
    remove("run");
}
*/
