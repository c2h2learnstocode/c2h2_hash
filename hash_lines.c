#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>


/* this hash table only stores 64bit keys, no value is stored*/
#define SPARSE_RATIO 10

uint64_t divider   =4398046511105; //2*42*+1
uint64_t uint64_max=18446744073709551615; //2**64-1

typedef struct {
  int lines;
  char ** words;
} Lines;

void read_file_to_lines(char*, Lines *);
void print_words( Lines *);
void insert_lines_into_ht(Lines *, unsigned char *);



int main(int argc, char **argv){
    char ** words;
    Lines l;
    read_file_to_lines(argv[1], &l);
    //print_words(&l);
    int ht_len = l.lines*SPARSE_RATIO;
    
    unsigned char ht[uint64_max/divider];
    
    int i;
    for(i=0; i<uint64_max/divider; i++){
        ht[i]=0;
    }

    insert_lines_into_ht(&l, &ht);
}

void print_words(Lines *l){
    int i=0;
    for(i=0; i<l->lines; i++){
        printf("%s\n", l->words[i]);
    }
}



void insert_lines_into_ht(Lines *l, unsigned char * ht){
    int i;
    uint64_t x;
    int hash_idx;

    for(i=0; i<l->lines; i++){
        l->words[i][strlen(l->words[i])]=0;
        x=strtoull(l->words[i], NULL, 10);
        hash_idx = (x/(uint64_t)divider); 
        ht[hash_idx]+=1;
        //printf("%s, %llu\n", l->words[i], x);
        //printf("%llu, %d\n", x, hash_idx );
    }

    for(i=0; i<uint64_max/divider; i++){
        printf("%u\n", ht[i]);
    }

}


void read_file_to_lines(char* file, Lines *l){
    int lines_allocated = 128;
    int max_line_len = 100;

    /* Allocate lines of text */
    l->words = (char **)malloc(sizeof(char*)*lines_allocated);
    if (l->words==NULL){
        fprintf(stderr,"Out of memory (1).\n");
        exit(1);
    }

    FILE *fp = fopen(file, "r");
    if (fp == NULL){
        fprintf(stderr,"Error opening file.\n");
        exit(2);
    }

    int i;
    for (i=0;1;i++){
        int j;

        /* Have we gone over our line allocation? */
        if (i >= lines_allocated){
            int new_size;

            /* Double our allocation and re-allocate */
            new_size = lines_allocated*2;
            l->words = (char **)realloc(l->words,sizeof(char*)*new_size);
            if (l->words==NULL) {
                fprintf(stderr,"Out of memory.\n");
                exit(3);
            }

            lines_allocated = new_size;
            }
        /* Allocate space for the next line */
        l->words[i] = malloc(max_line_len);
        if (l->words[i]==NULL){
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
            }
        if (fgets(l->words[i],max_line_len-1,fp)==NULL){
            break;
        }

        /* Get rid of CR or LF at end of line */
        for (j=strlen(l->words[i])-1;j>=0 && (l->words[i][j]=='\n' || l->words[i][j]=='\r');j--)
            ;
        l->words[i][j+1]='\0';
    }
    fclose(fp);
    l->lines=i;

}
