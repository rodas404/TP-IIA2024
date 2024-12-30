//
// Created by footr on 08/12/2024.
//
#include <math.h>
#define EPSILON 0.0001
#include <stdio.h>
#include <stdlib.h>
#include "utilsEvolutivo.h"

pchrom init_pop(struct info d, ProblemData data){
    int i;
    pchrom  indiv;

    indiv = malloc(sizeof(chrom)*d.popsize);
    if (indiv==NULL)
    {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }
    for (i=0; i<d.popsize; i++){
        gera_sol_inicial(indiv[i].p, data);
    }
    return indiv;
}

chrom get_best(pchrom pop, struct info d, chrom best) {
    for (int i = 0; i < d.popsize; i++) {
        if (pop[i].valido == 1 &&  pop[i].fitness < best.fitness)
            best = pop[i];
    }
    return best;
}

int eval_individual(int *sol,ProblemData data, int *v)
{
    int fitness;
    float soma;

    fitness = calcula_fit(sol, data);
    soma = calcula_soma(sol, data);

    if(fabs(soma - data.goal) > EPSILON)  //por causa da precisão dos numeros de ponto flutuante (file5.txt)
        *v = 0;
    else
        *v = 1;
    return fitness;
}

void evaluate(pchrom pop, struct info d, ProblemData data){
    int i;
    for (i=0; i<d.popsize; i++)
        pop[i].fitness = eval_individual(pop[i].p, data, &pop[i].valido);
}


void write_best(chrom x, struct info d) {
    int i;
    printf("Best individual: %d\n", x.fitness);
    for (i = 0; i < d.numGenes; i++)
        printf("%d ", x.p[i]);
    putchar('\n');
}