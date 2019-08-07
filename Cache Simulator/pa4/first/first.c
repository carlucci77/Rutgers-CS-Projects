#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct cache{
    int v;
    long tag;
    int last;
} cache;
void first(int cacheSize, char* assoc, char* cachePolicy, int blockSize, char* file){
    if(cacheSize%2 != 0){
        printf("error\n");
        return;
    }else if(blockSize%2 != 0){
        printf("error\n");
        return;
    }else if(strcmp(cachePolicy, "lru") != 0){
        printf("error\n");
        return;
    }else if(blockSize > cacheSize){
        printf("error\n");
        return;
    }
    FILE *doc = fopen(file,"r");
    if(doc == NULL){
        printf("error\n");
        fclose(doc);
        return;
    }
    char optr = 'a';
    int reads = 0;
    int writes = 0;
    int readsp = 0;
    int writesp = 0;
    int hits = 0;
    int misses = 0;
    int hitsp = 0;
    int missesp = 0;
    int sets = cacheSize/blockSize;
    int lines = 0;
    int temp = 0;
    int setBits = 0;
    int blockBits = blockSize;
    while(blockSize >>= 1){
        temp++;
    }
    blockSize = blockBits;
    blockBits = temp;
    if(strcmp(assoc, "direct") == 0){
        lines = 1;
        sets = sets/lines;
        setBits = sets;
        temp = 0;
        while(sets >>= 1){
            temp++;
        }
        sets = setBits;
        setBits = temp;
    }else if(strcmp(assoc, "assoc") == 0){
        sets = 1;
        setBits = 0;
        lines = cacheSize/blockSize;
    }else{
        lines = assoc[6] - '0';
        if(strlen(assoc) - 1 != 6){
            lines *= 10;
            lines+= (assoc[7] - '0');
        }
        sets = sets/lines;
        setBits = sets;
        temp = 0;
        while(sets >>= 1){
            temp++;
        }
        sets = setBits;
        setBits = temp;
    }
    int i = 0;
    cache*** myCache = (cache***)malloc(sets*sizeof(cache**));
    for (i=0; i<sets; i++){ 
        myCache[i] = (cache**)malloc(lines * sizeof(cache*));
    }
    int k = 0;
    for(k = 0; k < sets; k++){
        for(i = 0; i < lines; i++){
            myCache[k][i] = (cache*)malloc(sizeof(cache));
            myCache[k][i]-> v = 0;
            myCache[k][i]-> tag = 0;
            myCache[k][i]-> last = 0;
        }
    }
    unsigned int memory = 0;
    long set = 0;
    long tag1 = 0;
    long tempTag = 0;
    int l = 0;
    while(fscanf(doc, "%x", &memory) != EOF){
        fscanf(doc, " %c", &optr);
        fscanf(doc, " %c", &optr);
        int bo = 1;
        if(optr == 'R'){
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            if(strcmp(assoc, "assoc") == 0){
                set = 0;
            }
            tag1 = memory >> (setBits + blockBits);
            for(k = 0; k < lines; k++){
                if(myCache[set][k]-> v == 0){
                    misses++;
                    reads++;
                    myCache[set][k]-> v = 1;
                    myCache[set][k]-> tag = tag1;
                    bo = 0;
                    for(i = 0; i < lines; i++){
                        myCache[set][i]-> last = k;
                    }
                    break;
                }else{
                    if(myCache[set][k]->tag == tag1){
                        hits++;
                        bo = 0;
                        l = myCache[set][k]-> last;
                        while(l >= 0){
                            if(myCache[set][l]->tag == myCache[set][k]->tag){
                                myCache[set][l]->tag = tag1;
                                break;
                            }else{
                                tempTag = myCache[set][l]->tag;
                                myCache[set][l]->tag = tag1;
                                tag1 = tempTag;
                                l--;
                            }
                        }
                        break;
                    }
                }
            }
            if(bo == 1){
                misses++;
                reads++;
                l = lines - 1;
                while(l >= 0){
                    tempTag = myCache[set][l]->tag;
                    myCache[set][l]->tag = tag1;
                    tag1 = tempTag;
                    l--;
                }
            }
        }else if(optr == 'W'){
            writes++;
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            if(strcmp(assoc, "assoc") == 0){
                set = 0;
            }
            tag1 = memory >> (setBits + blockBits);
            for(k = 0; k < lines; k++){
                if(myCache[set][k]->v == 0){
                    reads++;
                    misses++;
                    myCache[set][k]->v = 1;
                    myCache[set][k]->tag = tag1;
                    bo = 0;
                    for(i = 0; i < lines; i++){
                        myCache[set][i]-> last = k;
                    }
                    break;
                }else{
                    if(myCache[set][k]->tag == tag1){
                        hits++;
                        bo = 0;
                        l = myCache[set][k]-> last;
                        while(l >= 0){
                            if(myCache[set][l]->tag == myCache[set][k]->tag){
                                myCache[set][l]->tag = tag1;
                                break;
                            }else{
                                tempTag = myCache[set][l]->tag;
                                myCache[set][l]->tag = tag1;
                                tag1 = tempTag;
                                l--;
                            }
                        }
                        break;
                    }
                }
            }
            if(bo == 1){
                misses++;
                reads++;
                l = lines - 1;
                while(l >= 0){
                    tempTag = myCache[set][l]->tag;
                    myCache[set][l]->tag = tag1;
                    tag1 = tempTag;
                    l--;
                }
            }
        }
    }
    for(k = 0; k < sets; k++){
        for(i = 0; i < lines; i++){
            myCache[k][i]->v = 0;
            myCache[k][i]->tag = 0;
            myCache[k][i]-> last = 0;
        }
    }
    fseek(doc, 0, SEEK_SET);
    while(fscanf(doc, "%x", &memory) != EOF){
        fscanf(doc, " %c", &optr);
        fscanf(doc, " %c", &optr);
        int bo = 1;
        int co = 1;
        if(optr == 'R' && strcmp(assoc, "direct") != 0){
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            if(strcmp(assoc, "assoc") == 0){
                set = 0;
            }
            tag1 = memory >> (setBits + blockBits);
            for(k = 0; k < lines; k++){
                if(myCache[set][k]->v == 0){
                    missesp++;
                    readsp++;
                    myCache[set][k]-> v = 1;
                    myCache[set][k]-> tag = tag1;
                    bo = 0;
                    for(i = 0; i < lines; i++){
                        myCache[set][i]-> last = k;
                    }
                    memory = memory + blockSize;//Prefetch start
                    set = ((1 << setBits) - 1) & (memory >> blockBits);
                    if(strcmp(assoc, "assoc") == 0){
                        set = 0;
                    }
                    tag1 = memory >> (setBits + blockBits);
                    for(i = 0; i < lines; i++){
                        if(myCache[set][i]-> v == 0){
                            readsp++;
                            myCache[set][i]-> v = 1;
                            myCache[set][i]-> tag = tag1;
                            co = 0;
                            int m = 0;
                            for(m = 0; m < lines; m++){
                                myCache[set][m]-> last = i;
                            }   
                            break;
                        }else{
                            if(myCache[set][i]->tag == tag1){
                                co = 0;
                                break;
                            }
                        }
                    }
                    if(co == 1){
                        readsp++;
                        l = lines - 1;
                        while(l >= 0){
                            tempTag = myCache[set][l]->tag;
                            myCache[set][l]->tag = tag1;
                            tag1 = tempTag;
                            l--;
                        }
                    }
                    break;//prefetch end
                }else{
                    if(myCache[set][k]->tag == tag1){
                        hitsp++;
                        bo = 0;
                        l = myCache[set][k]-> last;
                        while(l >= 0){
                            if(myCache[set][k]->tag == myCache[set][l]->tag){
                                myCache[set][l]->tag = tag1;
                                break;
                            }else{
                                tempTag = myCache[set][l]-> tag;
                                myCache[set][l]->tag = tag1;
                                tag1 = tempTag;
                                l--;
                            }
                        }
                        break;
                    }
                }
            }
            if(bo == 1){
                missesp++;
                readsp++;
                l = lines - 1;
                while(l >= 0){
                    tempTag = myCache[set][l]->tag;
                    myCache[set][l]->tag = tag1;
                    tag1 = tempTag;
                    l--;
                }
                memory = memory + blockSize;//Prefetch start
                set = ((1 << setBits) - 1) & (memory >> blockBits);
                if(strcmp(assoc, "assoc") == 0){
                    set = 0;
                }
                tag1 = memory >> (setBits + blockBits);
                for(i = 0; i < lines; i++){
                    if(myCache[set][i]-> v == 0){
                        readsp++;
                        myCache[set][i]-> v = 1;
                        myCache[set][i]-> tag = tag1;
                        co = 0;
                        int m = 0;
                        for(m = 0; m < lines; m++){
                            myCache[set][m]-> last = i;
                        }   
                        break;
                    }else{
                        if(myCache[set][i]->tag == tag1){
                            co = 0;
                            break;
                        }
                    }
                }
                if(co == 1){
                    readsp++;
                    l = lines - 1;
                    while(l >= 0){
                        tempTag = myCache[set][l]->tag;
                        myCache[set][l]->tag = tag1;
                        tag1 = tempTag;
                        l--;
                    }
                } //Prefetch ends
            }
        }else if(optr == 'W' && strcmp(assoc, "direct") != 0){
            writesp++;
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            if(strcmp(assoc, "assoc") == 0){
                set = 0;
            }
            tag1 = memory >> (setBits + blockBits);
            for(k = 0; k < lines; k++){
                if(myCache[set][k]->v == 0){
                    readsp++;
                    missesp++;
                    myCache[set][k]->v = 1;
                    myCache[set][k]->tag = tag1;
                    bo = 0;
                    for(i = 0; i < lines; i++){
                        myCache[set][i]-> last = k;
                    }
                    memory+= blockSize;//Prefetch start
                    set = ((1 << setBits) - 1) & (memory >> blockBits);
                    if(strcmp(assoc, "assoc") == 0){
                        set = 0;
                    }
                    tag1 = memory >> (setBits + blockBits);
                    for(k = 0; k < lines; k++){
                        if(myCache[set][k]-> v == 0){
                            readsp++;
                            myCache[set][k]-> v = 1;
                            myCache[set][k]-> tag = tag1;
                            bo = 0;
                            for(i = 0; i < lines; i++){
                                myCache[set][i]-> last = k;
                            }   
                            break;
                        }else{
                            if(myCache[set][k]->tag == tag1){
                                bo = 0;
                                break;
                            }
                        }
                    }
                    if(bo == 1){
                        readsp++;
                        l = lines - 1;
                        while(l >= 0){
                            tempTag = myCache[set][l]->tag;
                            myCache[set][l]->tag = tag1;
                            tag1 = tempTag;
                            l--;
                        }
                    }
                    break; //Prefetch ends
                }else{
                    if(myCache[set][k]->tag == tag1){
                        hitsp++;
                        bo = 0;
                        l = myCache[set][k]-> last;
                        while(l >= 0){
                            if(myCache[set][l]->tag == myCache[set][k]->tag){
                                myCache[set][l]->tag = tag1;
                                break;
                            }else{
                                tempTag = myCache[set][l]->tag;
                                myCache[set][l]->tag = tag1;
                                tag1 = tempTag;
                                l--;
                            }
                        }
                        break;
                    }
                }
            }
            if(bo == 1){
                missesp++;
                readsp++;
                l = lines - 1;
                while(l >= 0){
                    tempTag = myCache[set][l]->tag;
                    myCache[set][l]->tag = tag1;
                    tag1 = tempTag;
                    l--;
                }
                memory += blockSize; //Prefetch starts
                set = ((1 << setBits) - 1) & (memory >> blockBits);
                if(strcmp(assoc, "assoc") == 0){
                    set = 0;
                }
                tag1 = memory >> (setBits + blockBits);
                for(k = 0; k < lines; k++){
                    if(myCache[set][k]-> v == 0){
                        readsp++;
                        myCache[set][k]-> v = 1;
                        myCache[set][k]-> tag = tag1;
                        bo = 0;
                        for(i = 0; i < lines; i++){
                            myCache[set][i]-> last = k;
                        }
                        break;
                    }else{
                        if(myCache[set][k]->tag == tag1){
                            bo = 0;
                            break;
                        }
                    }
                }
                if(bo == 1){
                    readsp++;
                    l = lines - 1;
                    while(l >= 0){
                        tempTag = myCache[set][l]->tag;
                        myCache[set][l]->tag = tag1;
                        tag1 = tempTag;
                        l--;
                    }
                }//Prefetch ends
            }
        }else if(optr == 'R' && strcmp(assoc, "direct") == 0){
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            tag1 = memory >> (setBits + blockBits);
            if(myCache[set][0]->v == 0){
                missesp++;
                readsp++;
                myCache[set][0]->v = 1;
                myCache[set][0]->tag = tag1;
                memory += blockSize;
                set = ((1 << setBits) - 1) & (memory >> blockBits);
                tag1 = memory >> (setBits + blockBits);
                if(myCache[set][0]->v == 0){
                    readsp++;
                    myCache[set][0]->v = 1;
                    myCache[set][0]->tag = tag1;
                }else{
                    if(myCache[set][0]->tag != tag1){
                        readsp++;
                        myCache[set][0]->tag = tag1;
                    }
                }
            }else{
                if(myCache[set][0]->tag == tag1){
                    hitsp++;
                }else{
                    readsp++;
                    missesp++;
                    myCache[set][0]->tag = tag1;
                    memory += blockSize;
                    set = ((1 << setBits) - 1) & (memory >> blockBits);
                    tag1 = memory >> (setBits + blockBits);
                    if(myCache[set][0]->v == 0){
                        readsp++;
                        myCache[set][0]->v = 1;
                        myCache[set][0]->tag = tag1;
                    }else{
                        if(myCache[set][0]->tag != tag1){
                            readsp++;
                            myCache[set][0]->tag = tag1;
                        }
                    }
                }
            }
        }else if(optr == 'W' && strcmp(assoc, "direct") == 0){
            writesp++;
            fscanf(doc, "%x", &memory);
            set = ((1 << setBits) - 1) & (memory >> blockBits);
            tag1 = memory >> (setBits + blockBits);
            if(myCache[set][0]->v == 0){
                missesp++;
                readsp++;
                myCache[set][0]->v = 1;
                myCache[set][0]->tag = tag1;
                memory += blockSize;
                set = ((1 << setBits) - 1) & (memory >> blockBits);
                tag1 = memory >> (setBits + blockBits);
                if(myCache[set][0]->v == 0){
                    readsp++;
                    myCache[set][0]->v = 1;
                    myCache[set][0]->tag = tag1;
                }else{
                    if(myCache[set][0]->tag != tag1){
                        readsp++;
                        myCache[set][0]->tag = tag1;
                    }
                }
            }else{
                if(myCache[set][0]->tag == tag1){
                    hitsp++;
                }else{
                    readsp++;
                    missesp++;
                    myCache[set][0]->tag = tag1;
                    memory += blockSize;
                    set = ((1 << setBits) - 1) & (memory >> blockBits);
                    tag1 = memory >> (setBits + blockBits);
                    if(myCache[set][0]->v == 0){
                        readsp++;
                        myCache[set][0]->v = 1;
                        myCache[set][0]->tag = tag1;
                    }else{
                        if(myCache[set][0]->tag != tag1){
                            readsp++;
                            myCache[set][0]->tag = tag1;
                        }
                    }
                }
            }
        }
    }
    for(k = 0; k < sets; k++){
        for(i = 0; i < lines; i++){
            free(myCache[k][i]);
        }
        free(myCache[k]);
    }
    free(myCache);
    printf("no-prefetch\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
    printf("with-prefetch\n");
    printf("Memory reads: %d\n", readsp);
    printf("Memory writes: %d\n", writesp);
    printf("Cache hits: %d\n", hitsp);
    printf("Cache misses: %d\n", missesp);
    fclose(doc);
    return;
}
int main(int argc, char** argv){
    first(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), argv[5]);
    return 0;
}