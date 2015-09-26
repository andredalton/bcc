#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5000
#define LETTERS 26

struct Node {
    struct Node v[LETTERS];
} typedef node;

unsigned long int nodes = 0;

void new_hash() {
    int i;
    node *n = (node*) malloc(sizeof(node));
    for (i=0; i<LETTERS; i++)
}

int main() {
    char *word;

    word = (char*) malloc(MAX_SIZE*sizeof(char));
    fgets(word, MAX_SIZE, stdin);



    return 0;
}
