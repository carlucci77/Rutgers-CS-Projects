#include<stdio.h>
void fifth(char* sentence){
    int k = 0;
    while(sentence[k] != '\0'){
        char temp = sentence[k];
        if(temp == 'a' || temp == 'A' || temp == 'e' || temp == 'E' || temp == 'i' || temp == 'I' || temp == 'u' || temp == 'U' || temp == 'o' || temp == 'O'){
            printf("%c", temp);
        }
        k++;
    }
}
int main(int argc, char** argv){
    fifth(argv[1]);
    return 0;
}