#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>


//---------------------------------------- FUNCTIONS DECLARATIONS --------------------------------------
void checkParam(int argc, char** argv);
char** openFile(char** argv, int* numCols, int* numRows, int* numItrs);

void runWrap(char** argv, char** currentGrid, int* numCols, int* numRows, int* numItrs);
void runNoWrap(char** argv, char** currentGrid, int* numCols, int* numRows, int* numItrs);

int numNeighborWrap(char** currentGrid, int* numCols, int* numRows, int r, int c);
int numNeighborNoWrap(char ** currentGrid, int* numCols,int* numRows, int row, int col);

void updateGridWrap (char** currentGridptr, char** nextGridptr, int* numCols, int* numRows);
void updateGridNoWrap(char** currentGridptr, char** nextGridptr, int* numCols, int* numRows);

void printGrid(char **currentGrid, int* numCols, int* numRows);
//-------------------------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
  int numCols, numRows, numItrs;
  char** currentGrid;

  checkParam(argc, argv);

  currentGrid = openFile(argv, &numCols, &numRows, &numItrs);

  if(strcmp(argv[2], "wrap") == 0){
    runWrap(argv, currentGrid, &numCols, &numRows, &numItrs);
  }
  else if(strcmp(argv[2], "nowrap") == 0){
    runNoWrap(argv, currentGrid, &numCols, &numRows, &numItrs);
  }
}



//----------------------------------- CheckParam ------------------------------------------
void checkParam(int argc, char** argv){
  if(argc == 5 || argc == 4){

    if(strcmp(argv[2],"wrap") != 0 && strcmp(argv[2],"nowrap") != 0){
      printf("Error!\n");
      printf("Please type <file name> wrap/nowrap hide/show slow/med/fast\n");
    }
    else if(strcmp(argv[3],"show") != 0 && strcmp(argv[3],"hide") != 0){
      printf("Error!\n");
      printf("Please type <file name> wrap/nowrap hide/show slow/med/fast\n");
    }
  }
  else{
    printf("Error!\n");
    printf("Please type <file name> wrap/nowrap hide/show slow/med/fast\n");
  }

  if(strcmp(argv[3],"show") == 0){
    if(argc != 5){
      printf("Error!\n");
      printf("Please type <file name> wrap/nowrap hide/show slow/med/fast\n");
    }

    if(strcmp(argv[4],"slow") != 0 && strcmp(argv[4],"med") != 0 && strcmp(argv[4],"fast") != 0){
      printf("Error!\n");
      printf("Please type <file name> wrap/nowrap hide/show slow/med/fast\n");
    }
  }
}
//-------------------------------------------------------------------------------------------


//--------------------------------------- OpenFile -------------------------------------------
char** openFile(char** argv, int* numCols, int* numRows, int* numItrs){
  FILE* infile;
  int i, j, k;
  char ret;
  char** currentGrid;
  char** nextGrid;

  k = 0;

  infile = fopen(argv[1], "r");
  
  if(!infile){
    perror("Bad file");
    exit(1);
  }

  fscanf(infile, "%d", numRows);
  fscanf(infile, "%d", numCols);
  fscanf(infile, "%d", numItrs);

  currentGrid = (char**)malloc((*numRows)*sizeof(char*));

  for(i = 0; i < (*numRows); i++){
    currentGrid[i] = (char*)malloc((*numCols)*sizeof(char));
  }

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      currentGrid[i][j] = '-';
    }
  }

  while(k == 0){
    ret =  fscanf(infile, "%d" "%d", &i, &j);
    
    currentGrid[i][j] = '@';

    if(ret == EOF){
      k = 1;
    }
  }

  fclose(infile);

  return currentGrid;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------- RunWrap --------------------------------------------
void runWrap(char** argv, char** currentGrid, int* numCols, int* numRows, int* numItrs){
  int i, j;
  float runTime;

  char** nextGrid;

  struct timeval tvStart;
  struct timeval tvEnd;

  nextGrid = (char**)malloc((*numRows)*sizeof(char*));
  
  for(i = 0; i < (*numRows); i++){
    nextGrid[i] = (char*)malloc((*numCols)*sizeof(char));
  }

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      nextGrid[i][j] = currentGrid[i][j];
    }
  }

  if(strcmp(argv[3],"show") == 0){

    if(strcmp(argv[4],"slow") == 0){

      gettimeofday(&tvStart, NULL);

      for(i = 0; i < (*numItrs); i++){
        updateGridWrap(currentGrid, nextGrid, numCols, numRows);

	      printGrid(currentGrid, numCols, numRows);

	      usleep(1000000/3);
	      system("clear");
      }
      
      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);

    } 
    else if(strcmp(argv[4],"med") == 0){

      gettimeofday(&tvStart, NULL);

      for(i = 0; i < (*numItrs); i++){
	       updateGridWrap(currentGrid, nextGrid, numCols, numRows);

	       printGrid(currentGrid, numCols, numRows);

	       usleep(100000);
	       system("clear");
      }

      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
    }
    else if(strcmp(argv[4], "fast") == 0){

      gettimeofday(&tvStart, NULL);
	
      for(i = 0; i < (*numItrs); i++){
	       updateGridWrap(currentGrid, nextGrid, numCols, numRows);
	  
	       printGrid(currentGrid, numCols, numRows);

	       usleep(100000/3);
	       system("clear");
      }

      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
    }
  }
  else if(strcmp(argv[3], "hide") == 0){

    gettimeofday(&tvStart, NULL);
      
    for(i = 0; i < (*numItrs); i++){
      updateGridWrap(currentGrid, nextGrid, numCols, numRows);
    }
      
    printGrid(currentGrid, numCols, numRows);

    gettimeofday(&tvEnd, NULL);

    runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

    printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
  }

  for(i = 0; i < *numRows; i++){
    free(currentGrid[i]);
    free(nextGrid[i]);
  }

  free(currentGrid);
  free(nextGrid);
}
//-------------------------------------------------------------------------------------------


//------------------------------------- RunNoWrap -------------------------------------------
void runNoWrap(char** argv, char** currentGrid, int* numCols, int* numRows, int* numItrs){
  int i, j;
  float runTime;

  char** nextGrid;

  struct timeval tvStart;
  struct timeval tvEnd;
  
  nextGrid = (char**)malloc((*numRows)*sizeof(char*));
  
  for(i = 0; i < (*numRows); i++){
    nextGrid[i] = (char*)malloc((*numCols)*sizeof(char));
  }

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      nextGrid[i][j] = currentGrid[i][j];
    }
  }

  if(strcmp(argv[3],"show") == 0){

    if(strcmp(argv[4],"slow") == 0){

      gettimeofday(&tvStart, NULL);
       
      for(i = 0; i < (*numItrs); i++){
        updateGridNoWrap(currentGrid, nextGrid, numCols, numRows);

        printGrid(currentGrid, numCols, numRows);

        usleep(1000000/3);
        system("clear");
      }
      
      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
    } 
    else if(strcmp(argv[4],"med") == 0){

      gettimeofday(&tvStart, NULL);

      for(i = 0; i < (*numItrs); i++){
         updateGridNoWrap(currentGrid, nextGrid, numCols, numRows);

         printGrid(currentGrid, numCols, numRows);

         usleep(100000);
         system("clear");
      }

      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
    }
    else if(strcmp(argv[4], "fast") == 0){

      gettimeofday(&tvStart, NULL);
  
      for(i = 0; i < (*numItrs); i++){
         updateGridNoWrap(currentGrid, nextGrid, numCols, numRows);
    
         printGrid(currentGrid, numCols, numRows);

         usleep(100000/3);
         system("clear");
      }

      printGrid(currentGrid, numCols, numRows);

      gettimeofday(&tvEnd, NULL);

      runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

      printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
    }
  }
  else if(strcmp(argv[3], "hide") == 0){

    gettimeofday(&tvStart, NULL);
      
    for(i = 0; i < (*numItrs); i++){
      updateGridNoWrap(currentGrid, nextGrid, numCols, numRows);
    }
      
    printGrid(currentGrid, numCols, numRows);

    gettimeofday(&tvEnd, NULL);

    runTime = tvEnd.tv_sec - tvStart.tv_sec + ((float)tvEnd.tv_usec - tvStart.tv_usec)/1000000;

    printf("\n\nTotal time for %d iterations of %dx%d is %f secs.\n\n", *numItrs, *numRows, *numCols, runTime);
  }

  for(i = 0; i < *numRows; i++){
    free(currentGrid[i]);
    free(nextGrid[i]);
  }

  free(currentGrid);
  free(nextGrid);
}

//-------------------------------------------------------------------------------------------


//----------------------------------- NumNeighborWrap ----------------------------------------
int numNeighborWrap(char** currentGrid, int* numCols, int* numRows, int row, int col){
  int i, j;
  int neighbors = 0;

  for(i = row -1; i <= row + 1; i++){
    for(j = col - 1; j <= col + 1; j++){

      if((i != row || j != col) && currentGrid[(i + (*numRows))%(*numRows)][(j + (*numCols))%(*numCols)] == '@'){
	       neighbors++;
      }
    }
  }

  return neighbors;
}
//-------------------------------------------------------------------------------------------


//---------------------------------- NumNeighborNoWrap ---------------------------------------
int numNeighborNoWrap(char** currentGrid, int* numCols, int* numRows, int row, int col){
  int i, j;
  int neighbors = 0;

  if(row == 0 && col == 0){

    for(i = row; i <= row + 1; i++){
      for(j = col; j <= col + 1; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row == 0 && col != 0 && col != (*numCols-1)){

    for(i = row; i <= row + 1; i++){
      for(j = col - 1; j <= col + 1; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row == 0 && col == (*numCols-1)){

    for(i = row; i <= row + 1; i++){
      for(j = col - 1; j <= col; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row != 0 && row != (*numRows-1) && col == (*numCols-1)){

    for(i = row - 1; i <= row + 1; i++){
      for(j = col - 1; j <= col; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row == (*numRows-1) && col == (*numCols-1)){

    for(i = row - 1; i <= row; i++){
      for(j = col - 1; j <= col; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row == (*numRows-1) && col != 0 && col != (*numCols-1)){

    for(i = row - 1; i <= row; i++){
      for(j = col - 1; j <= col + 1; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row == (*numRows-1) && col == 0){

    for(i = row - 1; i <= row; i++){
      for(j = col; j <= col + 1; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else if(row != 0 && row != (*numRows-1) && col == 0){

    for(i = row - 1; i <= row + 1; i++){
      for(j = col; j <= col + 1; j++){

        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }
  else{
    for(i = row -1; i <= row + 1; i++){
      for(j = col - 1; j <= col + 1; j++){
      
        if((i != row || j != col) && currentGrid[i][j] == '@'){
          neighbors++;
        }
      }
    }
  }

  return neighbors;
}
//-------------------------------------------------------------------------------------------


//------------------------------------ UpdateGridWrap ---------------------------------------
void updateGridWrap (char** currentGrid, char** nextGrid, int* numCols, int* numRows){
  int neighbors;
  int row, col;
  int i, j;

  for(row = 0; row < (*numRows); row++){
    for(col = 0; col < (*numCols); col++){
      
      neighbors = numNeighborWrap(currentGrid, numCols, numRows, row, col);

      if(currentGrid[row][col] == '-' && neighbors == 3){
	       nextGrid[row][col] = '@';
      }
      else if(currentGrid[row][col] == '@' && (neighbors < 2 || neighbors > 3)){
	       nextGrid[row][col] = '-';
      }
      else if(currentGrid[row][col] == '@' && (neighbors ==  2 || neighbors == 3)){
	       nextGrid[row][col] = '@';
      }
      else{
	       nextGrid[row][col] = currentGrid[row][col];
      }
    }
  }

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      currentGrid[i][j] = nextGrid[i][j];
    }
  }
}
//-------------------------------------------------------------------------------------------


//------------------------------------ UpdateGridNoWrap -------------------------------------
void updateGridNoWrap(char** currentGrid, char** nextGrid, int* numCols, int* numRows){
  int neighbors;
  int row, col;
  int i, j;

  for(row = 0; row < (*numRows); row++){
    for(col = 0; col < (*numCols); col++){
      
      neighbors = numNeighborNoWrap(currentGrid, numCols, numRows, row, col);

      if(currentGrid[row][col] == '-' && neighbors == 3){
         nextGrid[row][col] = '@';
      }
      else if(currentGrid[row][col] == '@' && (neighbors < 2 || neighbors > 3)){
         nextGrid[row][col] = '-';
      }
      else if(currentGrid[row][col] == '@' && (neighbors ==  2 || neighbors == 3)){
         nextGrid[row][col] = '@';
      }
      else{
         nextGrid[row][col] = currentGrid[row][col];
      }
    }
  }

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      currentGrid[i][j] = nextGrid[i][j];
    }
  }
}
//-------------------------------------------------------------------------------------------


//---------------------------------------- PrintGrid ----------------------------------------
void printGrid(char **currentGrid, int* numCols, int* numRows){
  int i, j;

  for(i = 0; i < (*numRows); i++){
    for(j = 0; j < (*numCols); j++){
      printf("%c ", currentGrid[i][j]);
    }
    
    printf("\n");
  }
}
//-------------------------------------------------------------------------------------------
