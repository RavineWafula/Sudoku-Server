#include<ESP8266WiFi.h>
#include<WiFiUdp.h>
#include<stdio.h>
#include<strings.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
 
// Set WiFi credentials
#define WIFI_SSID "Hub"
#define WIFI_PASS "ravineWaf"
#define PORT 50000
#define MAXLINE 1000

WiFiUDP Udp;

typedef struct sudoku {
   int grid[9][9];
}Sudoku;

int determine(char solution[81], char type1[200]){
   for(int i = 1; i<82; i++){
      if(solution[i-1]!=type1[i]){return 0;}
   }
   return 1;
}

int generator(int start, int end){
   return (rand()%(end-start+1))+start;
}

void sendPackets(char *puzzle){
   Udp.beginPacket(IPAddress(192,168,43,16),Udp.remotePort()); // The first parameter is the ip address of the client
   Udp.write(puzzle, 200);
   Udp.endPacket();
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

void setup() {
  // Setup serial port
  Serial.begin(115200);
  Serial.println();
 
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(PORT);
}
 
void loop(){
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
   
   for(int i = 0; i<6; i++){
      int limit = generator(3, 7);
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
   for(int i = 0; i<6; i++){
      int limit = generator(3, 7);
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
   char puzzle[100], solution[100];
   puzzle[0] = '0';
   for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
         mask[i][j] = sudoku.grid[i][j];
         solution[i*9+j] = '0'+sudoku.grid[i][j];
         int remove = generator(1,9);
         char ch;
         if(remove<mask[i][j]){ ch = '0';}
         else {ch  = '0'+mask[i][j];}
         puzzle[i * 9 + j+1] = ch;
      }
   }
   Serial.printf("The solution should be:\n%s\n\n", solution);
   puzzle[82] ='\0';
   Serial.printf("The puzzle is:\n%s\n\n",puzzle);
   
   // start sending the puzzle
   sendPackets(puzzle);
   
   char type1[200];
   int lim = 0;
   while(1){
      int len = 0;
      //receive the solution
      while(1){
         int packetSize = Udp.parsePacket();
         len = Udp.read(type1, 200);
         delay(1500);
         if(len==0){continue;}
         else{
            type1[len] = '\0';
            break;
         }
      }   
      
      Serial.printf("UDP packet contents: %s, %d\n", type1, sizeof(type1));
     
      lim = len;
      //Serial.printf("%d\n",determine(solution, type1));
      if(determine(solution, type1)==0){
         char wrong[] = "Wrong! Please try again";
         sendPackets(wrong);         
      }
      else {break;}
    }
    
    char name[100];
    for(int i=82; i<lim; i++){
       name[i-82] = type1[i];
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
    
    Serial.printf("%s\n", name);
    
    sendPackets(name);
}
 
