/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


package sudoku;

import java.util.Scanner;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


/**
 *
 * @author downs
 */
public class Sudoku 
{  
   static int ROW_SIZE = 9;
   static int COL_SIZE = 9;
   static int nVal = -1;
   static int matrixValid = 1;
   static String FileName;
   
    int[][] read_board_from_file(String filename)
    {
        FileName = filename;
        Scanner sc = new Scanner(Sudoku.class.getResourceAsStream (filename));
        
        int arr1[][] = new int[9][9];
        String line[]; 
                                
        int count = 0;    
        
        for (int i = 0; i<9; i++)
        {
            line = sc.nextLine().trim().split(" ");
            for (int j = 0; j<9; j++)
            {
                arr1[i][j] = Character.getNumericValue(line[0].charAt(count));
                count += 2;
            }
            count = 0;
            
        }
        return arr1;
    }
    


    int[][] Array_From_3X3()
    {
        int n = 0;

        int rowMin = 0;
        int rowMax = 3;
        int collMin = 0;
        int collMax = 3;
        int[][] cArray = new int[ROW_SIZE][COL_SIZE];

        for(int k = 0; k<ROW_SIZE; k++)
        {

        for(int i = rowMin; i<rowMax; i++)
        {
            for(int j = collMin; j<collMax; j++)
            {
                cArray[k][n] = read_board_from_file(FileName)[i][j];
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
    
    int[][] Array_From_RowOrColl(int value)
    {

        //Create and fill array
        int[][] nArray = new int[ROW_SIZE][COL_SIZE];
        for(int i = 0; i<ROW_SIZE; i++)
        {
        for(int j = 0; j<COL_SIZE; j++)
        {
                if (value == 0)
                {
                nArray[i][j] = read_board_from_file(FileName)[i][j];

                }else{

                nArray[j][i] = read_board_from_file(FileName)[i][j];
                }
        }
        }

        return nArray;
    }

    public void printArray(int[][] arr)
    {
        for(int i = 0; i<ROW_SIZE; i++)
	{
            for (int j = 0; j<COL_SIZE; j++)
            {
                 System.out.print(arr[i][j]+" ");                 
            }
            System.out.println();
        }
    }
    
    
    public class Main extends Thread {
        private static Lock lock = new ReentrantLock();
        @Override                  
        public void run() {
            
            lock.lock();
            
            nVal += 1;

            int Array[][] = new int[ROW_SIZE][COL_SIZE];

            if(nVal == 0)
            {
            //System.out.println("--------------------------Matrix of the rows\n");
            Array = Array_From_RowOrColl(0);
            }
            else if(nVal == 1)
            {
            //System.out.println("--------------------------Matrix of the collums\n");
            Array = Array_From_RowOrColl(1);
            }
            else
            {
            //System.out.println("--------------------------Matrix of each 3X3 matrix\n");
            Array = Array_From_3X3();
            }

            //Create 2d array of zeros
            int arr[][] = new int[ROW_SIZE][COL_SIZE];
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
                    arr[i][Array[i][j]-1] = (arr[i][Array[i][j]-1]) + 1;

                }
            }
            
            //Sudoku SInstance = new Sudoku();
            //SInstance.printArray(Array);
            //System.out.println();
            
            //SInstance.printArray(arr);
            //System.out.println();                       

            //Print 2d array of found values & Check for non-zero values
            for(int i = 0; i<ROW_SIZE; i++)
            {
                for (int j = 0; j<COL_SIZE; j++)
                {
                    if (arr[i][j] != 1)
                    {
                        matrixValid = 0;
                    }
                }
            }
            lock.unlock();

        }
        
    }
    
    int is_board_valid() throws InterruptedException
    {
        Sudoku SInstance = new Sudoku();    
        Sudoku.Main thread = SInstance.new Main();
        Sudoku.Main thread1 = SInstance.new Main();
        Sudoku.Main thread2 = SInstance.new Main();
        
        thread.start(); 
        thread1.start();
        thread2.start();
        
        thread.join();
        thread1.join();
        thread2.join();
        
        return matrixValid;                      
    }
    public static void main(String[] args) throws InterruptedException
    {                      
        Sudoku SInstance = new Sudoku(); 
        
        int sudoku_board[][];
        
        sudoku_board = SInstance.read_board_from_file("invalid_board.txt");
        
        if (sudoku_board == null){
            System.out.println("No board available to validate. File %s is empty or unreadable.\n");
        }
            
        if(SInstance.is_board_valid() == 1)
        {
            System.out.println("The board is valid.\n"); 
        }else{
            System.out.println("The board is not valid.\n");
        }
        
    }   
}