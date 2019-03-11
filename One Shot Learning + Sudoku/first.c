#include<stdio.h>
#include<stdlib.h>

double** wcal(double**matrix,double** Y, int row, int col);
double** transpose(double** matrix, int rows, int cols);
double** XTmatmult(double** matrix1, double** matrix2,int rows,int cols);
double** inverse(double** matrix, int rows, int cols);
double** ones(double** matrix, int rows, int cols,int x,int y);
double** zeros(double** matrix, int rows, int cols, int x, int y, int level);
double** mult1(double** matrix1, double** matrix2,int rows,int cols);
double** mult2(double** matrix1, double** matrix2,int row, int col);
double** printsums(double** test, double** w,int row,int col);

int main(int argc, char** argv){

	FILE* train;
	train=fopen(argv[1],"r");
	FILE* test;
	test=fopen(argv[2],"r");

	if(train==NULL){
		printf("No file\n");
		return 0;
	}

	if(test==NULL){
		printf("No file\n");
		return 0;
	}

	int attribute;
	int cases;
	int tests;

	fscanf(train,"%d\n",&attribute);
	fscanf(train,"%d\n",&cases);
	fscanf(test,"%d\n",&tests);

	//establish all our matrixes we will be using
	//we have train and the Y matrix here
	double** trainmatrix=(double**)malloc(cases*sizeof(double*));
	double** Y=(double**)malloc(cases*sizeof(double*));
	for(int i=0;i<cases;i++){
		trainmatrix[i]=(double*)malloc((attribute+1)*sizeof(double));
		Y[i]=(double*)malloc(1*sizeof(double));
	}
	//establish the test matrix and the final result matrix
	double** testmatrix=(double**)malloc(tests*sizeof(double*));
	double** result=(double**)malloc(tests*sizeof(double*));
	for(int i=0;i<tests;i++){
		testmatrix[i]=(double*)malloc((attribute)*sizeof(double));
		result[i]=(double*)malloc(1*sizeof(double));
	}
	//result before our final result
	double** final=(double**)malloc((attribute+1)*sizeof(double*));
	for(int i=0;i<=attribute;i++){
		final[i]=(double*)malloc(1*sizeof(double));
	}
	//appedn values into our train and test matrices
	for(int i=0;i<cases;i++){
        for(int j=1;j<=attribute;j++){
            fscanf(train,"%lf%*[,]",&trainmatrix[i][j]);
        }
        fscanf(train,"%lf",&Y[i][0]);
        fscanf(train,"\n");
  }
  for(int i=0;i<cases;i++){
      trainmatrix[i][0]=1;
  }
  for(int i=0;i<tests;i++){
      for(int j=0;j<attribute;j++){
          fscanf(test,"%lf%*[,]",&testmatrix[i][j]);
      }
      fscanf(test,"\n");
  }

  final=wcal(trainmatrix,Y,cases,attribute+1);
  result=printsums(testmatrix,final,tests,attribute);

	return 0;
}

/////////////////////////////////////////////////////////////////
double** wcal(double** matrix,double** Y,int row, int col){

	double** XT=(double**)malloc(col*sizeof(double*));
	for(int i=0;i<col;i++){
		XT[i]=(double*)malloc((row)*sizeof(double));
	}
    XT=transpose(matrix,row,col);
		//print(XT,col,row);

    double** XTX=(double**)malloc(col*sizeof(double*));
	for(int i=0;i<col;i++){
		XTX[i]=(double*)malloc((col)*sizeof(double));
	}
    //times
    XTX=XTmatmult(XT,matrix,row,col);
		//print(XTX,col,col);

	double** XTXinv=(double**)malloc(col*sizeof(double*));
	for(int i=0;i<col;i++){
		XTXinv[i]=(double*)malloc((col)*sizeof(double));
	}
    XTXinv=inverse(XTX,col,col);
		//print(XTXinv,col,col);

	double** TIMES1X=(double**)malloc(col*sizeof(double*));
	for(int i=0;i<col;i++){
		TIMES1X[i]=(double*)malloc(row*sizeof(double));
	}
    TIMES1X=mult1(XTXinv,XT,row,col);
		//print(finalx,col,row);

	double** final=(double**)malloc(col*sizeof(double*));
	for(int i=0;i<col;i++){
		final[i]=(double*)malloc(1*sizeof(double));
	}
    final=mult2(TIMES1X,Y,row,col);

    return final;

}

double** transpose(double** matrix, int rows, int cols){

    double** tr=(double**)malloc(cols*sizeof(double*));
    for (int i=0;i<cols;i++){
        tr[i]=(double*)malloc(rows*sizeof(double));
    }

    for(int i=0;i<cols;i++){
        for(int j=0;j<rows;j++){
            tr[i][j]=matrix[j][i];
        }
    }
    return tr;
}

double** XTmatmult(double** matrix1, double** matrix2,int rows,int cols){
    int row1=cols;
    int row2=rows;
    //int col1=rows;
    int col2=cols;
	double** fin=(double**)malloc(row1*sizeof(double*));
	for(int i=0;i<row1;i++){
		fin[i]=(double*)malloc((col2)*sizeof(double));
	}

    double sum=0;
	for(int i=0; i<row1;i++){
		for(int j=0; j<col2;j++){
			for(int k=0; k<row2;k++){
				sum=sum+matrix1[i][k]*matrix2[k][j];
			}
			fin[i][j]=sum;
			sum=0;
		}

	}

	return fin;

}

double** inverse(double** matrix, int rows, int cols){
    //identity and matrix combination
    double** final=(double**)malloc(rows*sizeof(double*));
    for(int i=0;i<rows;i++){
        final[i]=(double*)malloc((2*cols)*sizeof(double));
    }
		//create dupe matrix
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            final[i][j]=matrix[i][j];
        }
    }
    //positioning of 1
    for(int i=0;i<rows;i++){
        for(int j=cols;j<(2*cols);j++){

            if(j-cols==i){
                final[i][j]=1;
            }
            else{
                final[i][j]=0;
            }
        }
    }

    for(int i=0;i<rows;i++){
	    for(int j=0;j<cols;j++){
            if(i==j){
								//printf("%d, %d \n",i,j);
                final=ones(final,rows,cols*2,i,j);
								//print(final,rows,cols*2);
								//printf("\n");
                for(int k=i+1;k<rows;k++){
                    final=zeros(final,rows,cols*2,k,j,0);
                }
								//print(final,rows,cols*2);
								//printf("\n");
            }
		}

	}
	//print(final,rows,cols*2);
	//printf("\n");

    for(int i=rows-1;i>=0;i--){
        for(int j=cols-1;j>=0;j--){
            if(final[i][j]!=0 && i!=j){
                final=zeros(final,rows,cols*2,i,j,1);
            }
        }
    }

    double** ivmatrix=(double**)malloc(rows*sizeof(double*));
    for(int i=0;i<rows;i++){
        ivmatrix[i]=(double*)malloc(cols*sizeof(double));
    }

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            ivmatrix[i][j]=final[i][j+cols];
        }
    }

    return ivmatrix;
}

double** ones(double** matrix, int rows, int cols, int x, int y){
    if(matrix[x][y]==1){return matrix;}

    double** temp=(double**)malloc(rows*sizeof(double*));
    for(int i=0;i<rows;i++){
        temp[i]=(double*)malloc((cols*2)*sizeof(double));
    }

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            temp[i][j]=matrix[i][j];
        }
    }

    if(temp[x][y]==0){
        //add
        for(int i=1;i<rows;i++){
            if(temp[i][y]!=0){

                for(int j=0;j<cols;j++){
                    temp[x][j]=temp[x][j]+matrix[i][j];
                }
								return temp;
            }
        }
				return temp;

    }

    double id=temp[x][y];
    for(int j=0;j<cols;j++){
        temp[x][j]= (matrix[x][j])/id;
    }

    return temp;
}

//level means new or old: old zeros=0 new zeroes =1
double** zeros(double** matrix, int rows, int cols, int x, int y, int level){
    if(matrix[x][y]==0){return matrix;}

    if(level==0){
        double** temp=(double**)malloc(rows*sizeof(double*));
        for(int i=0;i<rows;i++){
            temp[i]=(double*)malloc(cols*sizeof(double));
        }

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                temp[i][j]=matrix[i][j];
            }
        }
        //divide by number
        double id=temp[x][y];
        for(int j=0;j<cols;j++){
            temp[x][j]=temp[x][j]/id;
        }
        for(int j=0;j<cols;j++){
            temp[x][j]=temp[x][j]-temp[y][j];
        }
        return temp;
    }

    if(level==1){
        double** temp=(double**)malloc(rows*sizeof(double*));
        for(int i=0;i<rows;i++){
            temp[i]=(double*)malloc(cols*sizeof(double));
        }

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                temp[i][j]=matrix[i][j];
            }
        }

        double id=temp[x][y];
        for(int j=0;j<cols;j++){
            temp[x][j]=temp[x][j]-(temp[y][j]*id);

        }
        return temp;
    }

    return matrix;
}

double** mult1(double** matrix1, double** matrix2,int rows,int cols){
	int row1=cols;
	int row2=cols;
	int col=rows;
	double sum=0;

	double** temp=(double**)malloc(row1*sizeof(double*));

	for(int i=0;i<row1;i++){
		temp[i]=(double*)malloc((col)*sizeof(double));
	}

	for(int i=0; i<row1;i++){
		for(int j=0; j<col;j++){
			for(int k=0; k<row2;k++){
				sum=sum+matrix1[i][k]*matrix2[k][j];
			}
			temp[i][j]=sum;
			sum=0;
		}
	}
	return temp;

}

double** mult2(double** matrix1, double** matrix2,int row, int col1){
	int row1=col1;
	int row2=row;
	int col2=1;
	double sum=0;

  double** temp=(double**)malloc(row1*sizeof(double*));
	for(int i=0;i<row1;i++){
		temp[i]=(double*)malloc((col2)*sizeof(double));
	}

	for(int i=0; i<row1;i++){
		for(int j=0; j<col2;j++){
			for(int k=0; k<row2;k++){
				sum=sum+matrix1[i][k]*matrix2[k][j];
			}
			temp[i][j]=sum;

			sum=0;

		}

	}

	return temp;

}

double** printsums(double** test, double**weight,int row,int col){

	double** final=(double**)malloc(row*sizeof(double*));
	for(int i=0;i<row;i++){
		final[i]=(double*)malloc(1*sizeof(double));
	}

	double sum=0;

	for(int i=0;i<row;i++){
			sum=weight[0][0];
			for(int j=0;j<col;j++){
					sum=sum+(test[i][j]*weight[j+1][0]);
	        }
      final[i][0]=sum;
      printf("%0.0lf\n", final[i][0]);
    }

    return final;
}
