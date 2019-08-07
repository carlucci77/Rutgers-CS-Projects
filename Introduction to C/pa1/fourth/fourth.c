#include <stdlib.h>
#include<stdio.h>
void fourth(char* file){
    FILE *doc = fopen(file, "r");
    int columns1 = 0;
    int rows1 = 0;
    fscanf(doc, "%d", &rows1);
    fscanf(doc, "%d", &columns1);
    int matrix1[rows1][columns1];
    int k = 0;
    int i = 0;
    for(k = 0; k < rows1; k++){
        for(i = 0; i < columns1; i++){
            fscanf(doc, "%d", &matrix1[k][i]);
        }
    }
    int rows2 = 0;
    int columns2 = 0;
    fscanf(doc, "%d", &rows2);
    fscanf(doc, "%d", & columns2);
    if(columns1 != rows2){
        printf("bad-matrices");
        return;
    }
    int matrix2[rows2][columns2];
    for(k = 0; k < rows2; k++){
        for(i = 0; i < columns2; i++){
            fscanf(doc, "%d", &matrix2[k][i]);
        }
    }
    int result[rows1][columns2];
    int holder = 0;
    int j = 0;
    for(k = 0; k < rows1; k++){
        for(i = 0; i < columns2; i++){
            holder = 0;
           for(j = 0; j < rows2; j++){
               holder += matrix1[k][j] * matrix2[j][i];
           }
            result[k][i] = holder;
        }
    } 
    for(k = 0; k < rows1; k++){
        for(i = 0; i < columns2; i++){
            if(i != columns2 - 1){
                printf("%d\t", result[k][i]);
            }else{
                printf("%d\n", result[k][i]);
            }
        }
    }
    fclose(doc);
}
int main(int argc, char** argv){
    fourth(argv[1]);
    return 0;
}