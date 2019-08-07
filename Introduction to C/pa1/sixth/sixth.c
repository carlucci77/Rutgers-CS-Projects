#include<stdlib.h>
#include<stdio.h>
typedef struct Node{
    int data;
    struct Node* left;
    struct Node* right;
}Node;
void printTree(Node* head){
    if(head == NULL){
        return;
    }
    printTree(head->left);
    free(head->left);
    printf("%d\t", head->data);
    printTree(head->right);
    free(head->right);
}
void sixth(char* file){
    FILE *doc = fopen(file, "r");
    int num = 0;
    if(doc == NULL){
        printf("error");
        return;
    }else{
        char optr = 'a';
        Node* head = NULL;
        Node* ptr = NULL;
        head = (Node*)malloc(sizeof(Node));
        ptr = head;
        if(fscanf(doc, "%c", &optr) == EOF){
            printf("\n");
            return;
        }
        fscanf(doc, "%d", &num);
        head->data = num;
        head->right = NULL;
        head->left = NULL;
        fscanf(doc, "%c", &optr);
        while(fscanf(doc, "%d", &num) != EOF){
            fscanf(doc, "%c", &optr);
            while(ptr!= NULL){
                if(ptr->data == num){
                    break;
                }
                if(ptr->data > num){
                    if(ptr->left == NULL){
                        ptr->left = (Node*)malloc(sizeof(Node));
                        ptr->left->data = num;
                        ptr->left->left = NULL;
                        ptr->left->right = NULL;
                        break;
                    }else{
                        ptr = ptr->left;
                        continue;
                    }
                }else{
                    if(ptr->right == NULL){
                        ptr->right = (Node*)malloc(sizeof(Node));
                        ptr->right->data = num;
                        ptr->right->right = NULL;
                        ptr->right->left = NULL;
                        break;
                    }else{
                        ptr = ptr->right;
                        continue;
                    }
                }
            }
            ptr = head;
        }
        printTree(head);
        free(head);
    }
    fclose(doc);
}
int main(int argc, char** argv){
    sixth(argv[1]);
    return 0;
}