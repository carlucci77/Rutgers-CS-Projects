#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    int data;
    struct Node* next;
} Node;
void third(char* file){
    FILE *doc = fopen(file, "r");
    char optr = 'a';
    int check = 0;
    int length = 10000;
    Node* table[length];
    int k = 0;
    for(k = 0; k < length; k++){
        table[k] = NULL;
    }
    Node* ptr = NULL;
    Node* head = NULL;
    int key = 0;
    int insertCount = 0;
    int searchCount = 0;
    int bucket = 0;
    while(fscanf(doc, "%c", &optr) != EOF){
        if(optr == 'i'){
            break;
        }
    }
    while(fscanf(doc, "%d", &key) != EOF){
        if(key < 0){
            key = key + length;
            bucket = key%length;
            key -= length;
        }else{
            bucket = key%length;
        }
        if(optr == 'i'){
            if(table[bucket] == NULL){
                table[bucket] = (Node*)malloc(sizeof(Node));
                table[bucket]->data = key;
                table[bucket]->next = NULL;
            }else{
                ptr = table[bucket];
                head = table[bucket];
                check = 0;
                insertCount++;
                while(ptr != NULL){
                    if(ptr->data == key){
                        check = 1;
                        break;
                    }else{
                        ptr = ptr->next;
                    }
                }
                if(check == 0){
                    ptr = head;
                    head = (Node*)malloc(sizeof(Node));
                    head->data = key;
                    head->next = ptr;
                    table[bucket] = head;
                }
            }
        }else{
            if(table[bucket] == NULL){
                continue;
            }else{
                head = table[bucket];
                ptr = head;
                while(ptr != NULL){
                    if(ptr->data == key){
                        searchCount++;
                        break;
                    }else{
                        ptr = ptr->next;
                    }
                }
            }
        }
        fscanf(doc, " %c", &optr);
    }
    printf("%d\n", insertCount);
    printf("%d", searchCount);
    fclose(doc);
    for(k = 0; k < length; k++){
        if(table[k] != NULL){
            ptr = table[k];
            head = ptr;
            while(ptr != NULL){
                ptr = head->next;
                free(head);
                head = ptr;
            }
        }
    }
}
int main(int argc, char** argv){
    third(argv[1]);
    return 0;
}