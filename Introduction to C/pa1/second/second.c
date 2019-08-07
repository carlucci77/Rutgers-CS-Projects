#include<stdio.h>
#include<stdlib.h>
typedef struct Node{ 
    int data;
    struct Node* next;
} Node;
void second (char* file){
    FILE *doc = fopen(file, "r");
    if(doc == NULL){
        printf("error");
        return;
    }else{
        char optr = 'a';
        int num = 0;
        while(optr != 'i'){
            if(fscanf(doc, "%c", &optr) == EOF){
                optr = 'a';
                break;
            }
        }
        if(optr == 'a'){
            printf("%d\n", 0);
            return;
        }
        fscanf(doc, "%d", &num);
        Node* front = NULL;
        Node* ptr = NULL;
        Node* temp = NULL;
        front = (Node*)malloc(sizeof(Node));
        front->data = num; 
        front->next = NULL;
        ptr = front;
        temp = front;
        int check = 0;
        while(fscanf(doc, " %c", &optr) != EOF){
            if(optr == 'i'){
                check = 0;
                fscanf(doc, "%d", &num);
                if(front->next == NULL && front->data < num){
                    front->next = (Node*)malloc(sizeof(Node));
                    front->next->data = num;
                    front->next->next = NULL;
                    continue;
                }else if(front->next == NULL && front->data > num){
                    front->next = (Node*)malloc(sizeof(Node));
                    front->next->data = front->data;
                    front->data = num;
                    front->next->next = NULL;
                    continue;
                }
                while(ptr->next != NULL){
                    if(front->data > num){
                        front = (Node*)malloc(sizeof(Node));
                        front-> data = num;
                        front-> next = ptr;
                        check = 1;
                        break;
                    }else{
                        if(ptr->next->data > num){
                            temp = (Node*)malloc(sizeof(Node));
                            temp->next = ptr->next;
                            ptr->next = temp;
                            temp->data = num;
                            check = 1;
                            break;
                        }
                    }
                    ptr = ptr->next;
                }
                if(check == 0 && ptr->next == NULL){
                    temp = (Node*)malloc(sizeof(Node));
                    temp->data = num;
                    temp->next = NULL;
                    ptr->next = temp;
                    ptr = front;
                    continue;
                }
            }else if(optr == 'd'){
                check = 0;
                fscanf(doc, "%d", &num);
                if(front->data == num && front->next != NULL){
                    ptr = front->next;
                    front->next = NULL;
                    free(front);
                    front = ptr;
                    continue;
                }else if(front->data == num && front->next == NULL){
                    front->data = 0;
                    optr = 'a';
                    while(optr != 'i'){
                        if(fscanf(doc, "%c", &optr) == EOF){
                            optr = 'a';
                            break;
                        }
                    }
                    if(optr == 'a'){
                        printf("%d\n", 0);
                        free(front);
                        return;
                    }else{
                        fscanf(doc, "%d", &num);
                        front->data = num;
                        front->next = NULL;
                        ptr = front;
                        continue;
                    }
                }
                while(ptr->next != NULL){
                    if(ptr->next->data == num){
                        if(ptr->next->next != NULL){
                            temp = ptr->next;
                            ptr->next = temp->next;
                            free(temp);
                            ptr = front;
                            check = 1;
                            break;
                        }else{
                            temp = ptr->next;
                            ptr->next = NULL;
                            free(temp);
                            ptr = front;
                            check = 1;
                            break;
                        }
                    }
                    ptr = ptr->next;
                }
                if(ptr->data == num && check == 0){
                    temp = front;
                    while(temp->next != ptr){
                        temp = temp->next;
                    }
                    temp->next = NULL;
                    temp = temp->next;
                    free(temp);
                    free(ptr);
                }
                ptr = front;
            }
            ptr = front;
        }
        fclose(doc);
        int count = 0;
        ptr = front;
        while(ptr != NULL){
            count++;
            ptr = ptr->next;
        }
        printf("%d\n", count);
        int checker[count];
        int k = 0;
        ptr = front;
        int c = 0;
        for(k = 0; k < count; k++){
            checker[k] = -989000;
        }
        while(ptr != NULL){
            c = 0;
            for(k = 0; k < count; k++){
                if(checker[k] == ptr->data){
                    c = 1;
                    break;
                }
            }
            if(c == 0){
                printf("%d\t", ptr->data);
                for(k = 0; k < count; k++){
                    if(checker[k] == -989000){
                        checker[k] = ptr->data;
                        break;
                    }
                }
            }
            ptr = ptr->next;
        }
        ptr = front;
        while(ptr!= NULL){
            ptr = ptr->next;
            free(front);
            front = ptr;
        }
    }
}
int main(int argc, char** argv){
    second(argv[1]);
    return 0;
}