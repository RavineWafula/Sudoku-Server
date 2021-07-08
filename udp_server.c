#include<stdio.h>
#include<strings.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


#define PORT 5000
#define MAXLINE 1000

typedef struct sudoku {
   int grid[9][9];
}Sudoku;

typedef struct player {
   char sudokuid[100];
   char name[100];
}Player;


int determine(char solution[81], char buffer[120]){
   char type = buffer[0];
   for(int i = 1; i<82; i++){
      if(solution[i-1]!=buffer[i]){return 0;}
   }
   return 1;
}

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
   
   for(int i = 0; i<3; i++){
      int limit = generator(3, 6);
      for(int j = 0; j<limit; j+=3){
         int a, b;
         
         while(1){
            a = generator(0, 2);
            b = generator(0, 2);
            if(a!=b) break;
         }
         int rep[2] = {a+j, b+j};
         swap_row(&sudoku, rep);
      }
   } 
   for(int i = 0; i<3; i++){
      int limit = generator(3, 6);
      for(int j = 0; j<limit; j+=3){
         int a, b;
         
         while(1){
            a = generator(0, 2);
            b = generator(0, 2);
            if(a!=b) break;
         }
         int rep[2] = {a+j, b+j};
         swap_col(&sudoku, rep);
      }
   }  
   int mask[9][9];
   char final[100], solution[100];
   final[0] = '0';
   for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
         mask[i][j] = sudoku.grid[i][j];
         //printf("%d",sudoku.grid[i][j]);
         solution[i*9+j] = '0'+sudoku.grid[i][j];
         int remove = generator(1,9);
         char ch;
         if(remove<mask[i][j]){ ch = 'X';}
         else {ch  = '0'+mask[i][j];}
      	 final[i * 9 + j+1] = ch;
      }
   }
   printf("\n");
   printf("%s\n", solution);
   final[82] ='\0';
   printf("%s",final);
   printf("\n");
   
   // start sending the puzzle
   
   char buffer[200];
   int listenfd, len;
   struct sockaddr_in servaddr, cliaddr;
   bzero(&servaddr, sizeof(servaddr));
   
   // creating a UDP Socket
   listenfd = socket(AF_INET, SOCK_DGRAM, 0);
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(broadcast);
   servaddr.sin_port = htons(PORT);
   servaddr.sin_family = AF_INET;

   // bind server address to socket descriptor
   bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
   // send the puzzle to each connected player
   sendto(listenfd, final, MAXLINE, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
   int lim;
   while(1){
      //receive the solution
      len = sizeof(cliaddr);
      int n = recvfrom(listenfd, buffer, sizeof(buffer),0, (struct sockaddr*)&cliaddr,&len); //receive message from server
      buffer[n] = '\0';
      puts(buffer);
      lim = n;
      printf("%d\n",determine(solution, buffer));
      if(determine(solution, buffer)==1){break;}
      else{
         char wrong[] = "Wrong! Please try again";
         sendto(listenfd, wrong, MAXLINE, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
      }
    }
    
    char name[100];
    for(int i=82; i<lim; i++){
       name[i-82] = buffer[i];
    }
    name[lim-82] = ' ';
    int st = strlen(name);
    char correct[] = " got it right!";
    int size = strlen(correct);
    int j = 0;
    for(int i=st; i<st+size; i++){
       name[i] = correct[j];
       j++;
    }
    name[st+size] = '\0';
    
    printf("%s\n", name);
    sendto(listenfd, name, MAXLINE, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
    
    return 0;
}
   
   
   
   
   
   
   
   
   
   
   
   
