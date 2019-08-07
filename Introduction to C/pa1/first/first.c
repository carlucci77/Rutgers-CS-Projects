#include<stdio.h>
void first(char* file){
	int length = 0;
	FILE *doc = fopen(file,"r");
	fscanf(doc, "%d", &length);
	int sorted[length];
    int check[length];
	int k = 0;
	int temp = 0;
    for(k = 0; k < length; k++){
        check[k] = 0;
    }
	for(k = 0; k < length; k++){
		fscanf(doc,"%d",&temp);
		if(check[0] == 0){
            check[0] = 1;
			sorted[0] = temp;
		}else{
			if(temp%2 == 0){
				int i = 0;
				for(i = 0; i < length; i++){
					if(check[i] != 0){    
						if(sorted[i]% 2 != 0 || sorted[i] > temp){
                            int j = i;
                            for(j = i; j < length; j++){
                                if(check[j] != 0){
                                    int temp2 = sorted[j];
                                    sorted[j] = temp;
                                    /*printf("%d", sorted[j]);*/
                                    temp = temp2;
                                }else{
                                    check[j] = 1;
                                    sorted[j] = temp;
                                    break;
                                }
                            }
                            break;
						} 
					}else{
                        check[i] = 1;
						sorted[i] = temp;
						break;
					}
				} 
			}else{
				int i = 0;
				for(i = 0; i < length; i++){
                    if(check[i] != 0){
                        if(sorted[i] < temp && sorted[i]%2 != 0){
                            int j = i;
                            for(j = i; j < length; j++){
                                if(check[j] != 0){
                                    int temp2 = sorted[j];
                                    sorted[j] = temp;
                                    temp = temp2;
                                }else{
                                    check[j] = 1;
                                    sorted[j] = temp;
                                    break;
                                }
                            }
                            break;
                        }
                    }else{
                        check[i] = 1;
                        sorted[i] = temp;
                        break;
                    }
				}
			} 
		}
	}
    int j = 0;
    for(j = 0; j < length; j++){
        printf("%d\t", sorted[j]);
    }
    fclose(doc);
}
int main(int argc, char** argv){
    first(argv[1]);
    return 0;
}