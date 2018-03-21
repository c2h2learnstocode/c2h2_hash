#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  int lines;
  char ** words;
} Lines;

void read_file_to_lines(char*, Lines *);
void print_words( Lines *);




int main(int argc, char **argv){
    char ** words;
    Lines l;
    read_file_to_lines(argv[1], &l);
    print_words(&l);

}

void print_words(Lines *l){
    int i=0;
    for(i=0; i<l->lines; i++){
        printf("%s\n", l->words[i]);
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
