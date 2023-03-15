#include <stdio.h>
#include <stddef.h>

char memory[25000];

struct block{
    int flag;
    size_t size;
    struct block *next;
};

struct block *startblock=(void*)memory;

void conector(){
    struct block *pptr,*ptr;
    pptr=startblock;
    while(pptr->flag!=0){
        pptr=pptr->next;
    }
    if(pptr->next->flag==0){
        ptr=pptr->next;
        pptr->size=ptr->size+sizeof(struct block)+pptr->size;
        pptr->next=ptr->next;
        conector();
    }
}

void MyFree(void* ptr){
    if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+25000))){
        struct block* current;
        current=ptr;
        --current;
        current->flag=0;
        printf("Freeing memory succesfull.\n");
        conector();
    }
    else{
        printf("This pointer not valid.\n");
    }
}

void *MyMalloc(size_t bytes){
    struct block *current;
    void *result;

    if(!(startblock->size)){
        startblock->size=25000-sizeof(struct block);
        startblock->flag=0;
        startblock->next=NULL;
        printf("Memory initialized.\n");
    }

    current=startblock;

    while(current->next!=NULL){

        if(current->flag==0&&current->size==(bytes)){
            current->size=bytes;
            current->flag=1;
            result=(void*)(++current);
            printf("block allocated.\n");

            return result;
        }

        else if(current->flag==0&&current->size>bytes&&current->size<(sizeof(struct block)+bytes)){
            current->flag=1;
            result=(void*)(++current);
            printf("block allocated.\n");

            return result;
        }
        else if(current->flag==0&&current->size>=(bytes+sizeof(struct block))){
            struct block *newblock=(void*)current+bytes+sizeof(struct block);
            newblock->size=(current->size)-bytes-sizeof(struct block);
            newblock->flag=0;
            newblock->next=current->next;
            current->size=bytes;
            current->flag=1;
            current->next=newblock;
            result=(void*)(++current);
            printf("block allocated.\n");

            return result;
        }
        current=current->next;
    }
    if(current->size>=(bytes+sizeof(struct block))){
        struct block *newblock=(void*)current+bytes+sizeof(struct block);
        newblock->size=(current->size)-bytes-sizeof(struct block);
        newblock->flag=0;
        newblock->next=current->next;
        current->size=bytes;
        current->flag=1;
        current->next=newblock;
        result=(void*)(++current);
        printf("block allocated.\n");

        return result;

    }
    else{
        printf("Memory is not enough.\n");
    }


}
