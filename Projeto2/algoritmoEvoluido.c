//
// Created by footr on 08/12/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include "utilsEvolutivo.h"
#include "algoritmoEvoluido.h"
#include "algoritmos.h"

void tournament(pchrom pop, struct info d, pchrom parents){
    int i, x1, x2;

    // Realiza popsize torneios
    for(i=0; i<d.popsize;i++)
    {
        x1 = random_l_h(0, d.popsize-1);
        do
            x2 = random_l_h(0, d.popsize-1);
        while(x1==x2);
        if(pop[x1].fitness < pop[x2].fitness)
            parents[i]=pop[x1];
        else
            parents[i]=pop[x2];
    }
}

void genetic_operators(pchrom parents, struct info d, pchrom offspring){
    // Recombinação com um ponto de corte
    //crossover(parents, d, offspring);
    crossover_dois_pontos_corte(parents, d, offspring);
    // Mutação
    //mutation(offspring, d);
    mutacao_por_troca(offspring, d);
}

void crossover(pchrom parents, struct info d, pchrom offspring){
    int i, j, point;

    for (i=0; i<d.popsize; i+=2)
    {
        if (rand_01() < d.pr)
        {
            point = random_l_h(0, d.numGenes-1);
            for (j=0; j<point; j++)
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (j=point; j<d.numGenes; j++)
            {
                offspring[i].p[j]= parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];
        }
    }
}

void crossover_dois_pontos_corte(pchrom parents, struct info d, pchrom offspring) {
    int i, j, point1, point2;

    for (i = 0; i < d.popsize; i += 2) {
        if (rand_01() < d.pr) {
            point1 = random_l_h(0, d.numGenes - 2);
            point2 = random_l_h(point1 + 1, d.numGenes - 1);

            for (j = 0; j < point1; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
            for (j = point1; j < point2; j++) {
                offspring[i].p[j] = parents[i + 1].p[j];
                offspring[i + 1].p[j] = parents[i].p[j];
            }
            for (j = point2; j < d.numGenes; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
        } else {
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}


void mutation(pchrom offspring, struct info d) {
    int i, j;

    for (i = 0; i < d.popsize; i++)
        for (j = 0; j < d.numGenes; j++)
            if (rand_01() < d.pm) {
                if (rand_01() < 0.5 && offspring[i].p[j] != 0)
                    offspring[i].p[j]--;
                else
                    offspring[i].p[j]++;
            }
}


void mutacao_por_troca(pchrom offspring, struct info d) {
    int i, pos1, pos2, aux;

    for (i = 0; i < d.popsize; i++) {
        if (rand_01() < d.pm) {
            pos1 = random_l_h(0, d.numGenes - 1);
            do {
                pos2 = random_l_h(0, d.numGenes - 1);
            }while (pos2 == pos1);
            aux = offspring[i].p[pos1] = offspring[i].p[pos2];
            offspring[i].p[pos2] = aux;
        }
    }
}

void roleta(pchrom pop, struct info d, pchrom parents) {
    int i, j;
    float soma = 0.0;
    float *prob_acumulada = malloc(d.popsize * sizeof(float));

    // Calcula a soma dos fitness
    for (i = 0; i < d.popsize; i++) {
        soma += (float)pop[i].fitness;
    }

    // Calcula a probabilidade acumulada
    prob_acumulada[0] = (float)pop[0].fitness / soma;
    for (i = 1; i < d.popsize; i++) {
        prob_acumulada[i] = prob_acumulada[i - 1] + ((float)pop[i].fitness / soma);
    }

    // Seleciona os pais usando a roleta
    for (j = 0; j < d.popsize; j++) {
        float x = rand_01();
        i = 0;
        while (i < d.popsize - 1 && x > prob_acumulada[i]) {
            i++;
        }
        parents[j] = pop[i];
    }

    free(prob_acumulada);
}

void trepa_colinas_hibrido(pchrom pop, struct info d, ProblemData data, int num_iter) {
    int *nova_sol, custo_viz, custo, dummy;

    nova_sol = malloc(sizeof(int) * d.numGenes);
    if (nova_sol == NULL) {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    for (int i = 0; i < d.popsize; i++) {
        custo = eval_individual(pop[i].p, data, &dummy);
        pop[i].fitness = custo;

        for (int j = 0; j < num_iter; j++) {
            gera_vizinho(pop[i].p, nova_sol, data);
            custo_viz = eval_individual(nova_sol, data, &dummy);

            if (custo_viz <= custo) {
                substitui(pop[i].p, nova_sol, d.numGenes);
                custo = custo_viz;
                pop[i].fitness = custo;
            }
        }
    }

    free(nova_sol);
}