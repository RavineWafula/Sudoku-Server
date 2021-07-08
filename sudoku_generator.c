#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


typedef struct sudoku {
   int grid[9][9];
}Sudoku;

void Printer(Sudoku sudoku){
   for(int i = 0; i<9; i++){
      for(int j = 0; j<9; j++){
         printf("%d ", sudoku.grid[i][j]);
      }printf("\n");
   }
}

int generator(int start, int end){
   return (rand()%(end-start+1))+start;
}

void swap_row(Sudoku *sudoku, int rep[2]){
   
   int tp[9];
   for(int k = 0; k<9; k++){
      tp[k] = sudoku->grid[rep[0]][k];
   }
   for(int k = 0; k<9; k++){
      sudoku->grid[rep[0]][k] = sudoku->grid[rep[1]][k];
      sudoku->grid[rep[1]][k] = tp[k];
   }
}
void swap_col(Sudoku *sudoku, int rep[2]){
   
   int tp[9];
   for(int k = 0; k<9; k++){
      tp[k] = sudoku->grid[k][rep[0]];
   }
   for(int k = 0; k<9; k++){
      sudoku->grid[k][rep[0]] = sudoku->grid[k][rep[1]];
      sudoku->grid[k][rep[1]] = tp[k];
   }
}

int main(){
   Sudoku sudoku;
   srand(time(0));
   int grid[9][9] = {{1,2,3,4,5,6,7,8,9},
                     {4,5,6,7,8,9,1,2,3},
                     {7,8,9,1,2,3,4,5,6},
                     {2,3,1,5,6,4,8,9,7},
                     {5,6,4,8,9,7,2,3,1},
                     {8,9,7,2,3,1,5,6,4},
                     {3,1,2,6,4,5,9,7,8},
                     {6,4,5,9,7,8,3,1,2},
                     {9,7,8,3,1,2,6,4,5}};
                     
   for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
         sudoku.grid[i][j] = grid[i][j];
      }
   }
   
   //swapping rows
   for(int i = 0; i<3; i++){
      int limit = generator(7, 16);
      //printf("%d %d\n", limit, 9898);
      for(int j = 0; j<limit; j+=3){
         int a, b;
         
         while(1){
            a = generator(0, 2);
            b = generator(0, 2);
            if(a!=b) break;
         }
         //printf("%d %d\n", a, b);
         int rep[2] = {a+j, b+j};
         //Printer(sudoku);
         swap_row(&sudoku, rep);
         
         //printf("\n");
      }
   }  
   // swapping columns
   for(int i = 0; i<3; i++){
      int limit = generator(7, 16);
      for(int j = 0; j<limit; j+=3){
         int a, b;
         
         while(1){
            a = generator(0, 2);
            b = generator(0, 2);
            if(a!=b) break;
         }
         //printf("%d %d\n", a, b);
         int rep[2] = {a+j, b+j};
         //Printer(sudoku);
         swap_col(&sudoku, rep);
         
         //printf("\n");
      }
   }  
   Printer(sudoku);
   printf("\n");
   int mask[9][9];
   char final[] = "";
   for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
         mask[i][j] = sudoku.grid[i][j];
         int remove = generator(1,9);
         if(remove<mask[i][j]){
            printf("X");
         }
         else {printf("%d", mask[i][j]);}
      }printf("\n");
   }
   printf("\n");
   return 0;
}
