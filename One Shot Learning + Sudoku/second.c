#include<stdio.h>
#include<stdlib.h>

int solver(int(*matrix)[9], int x, int y);

int main(int argc, char** argv){

  FILE* txt;
  txt=fopen(argv[1],"r");


  if(txt==NULL){
    printf("error");
    return 0;
  }

  //char matrix1[9][9];
  int matrix[9][9];
  char** matrix1=(char**)malloc(9*sizeof(char*));
	for(int i=0;i<9;i++){
		matrix1[i]=(char*)malloc(9*sizeof(char));
	}

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      fscanf(txt,"%c",&matrix1[i][j]);
      fscanf(txt," ");
      //printf("this is i: %d and this is j: %d \n", i,j);
      //printf("%c \n",matrix[i][j]);
      if(matrix1[i][j]=='-'){
        matrix1[i][j]='0';
      }
    }
  }

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      matrix[i][j]=matrix1[i][j]-'0';
    }
  }

  /*or(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      printf("%d",matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");*/
  solver(matrix,0,0);
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      printf("%d  ",matrix[i][j]);
    }
    printf("\n");
  }
}


int sameRow(int(*matrix)[9],int x, int y, int num){

  for(int i=0;i<9;i++){
    if(matrix[i][y]==num){
      return 1;
    }
  }
  return 0;

}

int sameColumn(int(*matrix)[9],int x, int y, int num){

  for(int i=0;i<9;i++){
    if(matrix[x][i]==num){
      return 1;
    }
  }
  return 0;

}

int sameBox(int(*matrix)[9], int x, int y, int num){
  if(x<3){
    x=0;
  } else if(x<6){
    x=3;
  } else{
    x=6;
  }

  if(y<3){
    y=0;
  } else if(y<6){
    y=3;
  } else{
    y=6;
  }

  for(int i=x;i<x+3;i++){
    for(int j=y;j<y+3;j++){
      if(matrix[i][j]==num){
        return 1;
      }
    }
  }
  return 0;

}

int solver(int(*matrix)[9], int x, int y){

  int tx=0;
  int ty=0;
  int num=1;

  if(matrix[x][y] !=0){
    if(x==8 && y==8){
      return 1;
    }
    if(x<8){
      x++;
    }
    else{
      if(y<8){
        x=0;
        y++;
      }
    }
    if(solver(matrix,x,y)){
      return 1;
    } else{
      return 0;
    }
  }

  if(matrix[x][y]==0){
    while(num<10){
      if(!sameRow(matrix,x,y,num) && !sameColumn(matrix,x,y,num) && !sameBox(matrix,x,y,num)){
        matrix[x][y]=num;

        if(x==8 && y==8){
          return 1;
        }
        if(x<8){
          tx=x+1;
        } else{
          if(y<8){
            tx=0;
            ty=y+1;
          }
        }
        if(solver(matrix,tx,ty)){
          return 1;
        }
      }
      num++;
    }
    if(x==0 && y==0){
      printf("No Soltion");
      return 0;
    }
    matrix[x][y]=0;
    return 0;
  }



  return 0;


}
