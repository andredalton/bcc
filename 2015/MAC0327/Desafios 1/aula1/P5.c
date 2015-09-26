/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio E
 * MAC0327 -- 08/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

struct actor
{
    unsigned int start;
    unsigned int end;
    unsigned int rooms;
} typedef Actor;

struct part
{
    unsigned int start;
    unsigned int end;
    int actor;
} typedef Part;

int backtrack(int part, int nparts, Part *parts, int nactors, Actor *actors)
{
    int i;

    if (part == nparts)
        return 1;

    for (i=0; i<nactors; i++)
    {
        if (parts[part].start<actors[i].start || parts[part].end>actors[i].end || actors[i].rooms==0)
            continue;
        actors[i].rooms--;
        parts[part].actor = i + 1;
        if (!backtrack(part+1, nparts, parts, 0, nactors, actors))
            actors[i].rooms++;
        else
            return 1;
    }
    return 0;
}

int main(int argc, const char* argv[])
{
    int nparts,
        nactors,
        i;
    Part *parts;
    Actor *actors;

    scanf("%d", &nparts);
    parts = (Part *) malloc(nparts*sizeof(Part));
    for (i=0; i<nparts; i++)
        scanf("%u %u", &parts[i].start, &parts[i].end);

    scanf("%d", &nactors);
    actors = (Actor *) malloc(nactors*sizeof(Actor));
    for (i=0; i<nactors; i++)
        scanf("%u %u %u", &actors[i].start, &actors[i].end, &actors[i].rooms);

    if (!backtrack(0, nparts, parts, nactors, actors)) printf("NO\n");
    else {
        printf("YES\n");
        for (i=0; i<nparts; i++)
            printf("%d ", parts[i].actor);
    }

    free(parts);
    free(actors);
    return 0;
}
