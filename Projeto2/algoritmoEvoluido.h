//
// Created by footr on 08/12/2024.
//

#ifndef PROJETO2_ALGORITMOEVOLUIDO_H
#define PROJETO2_ALGORITMOEVOLUIDO_H

#include "utils.h"
#define MAX_OBJ 10 // Numero maximo de objectos

// Estrutura para armazenar parametros
struct info {
    int popsize; // Tamanho da população
    float pm; // Probabilidade de mutação
    float pr; // Probabilidade de recombinação
    int tsize; // Tamanho do torneio para seleção do pai da próxima geração
    int numGenes; // Número de moedas diferentes que podem ser usadas
    int numGenerations; // Número de gerações
};

// Individuo (solução)
typedef struct individual chrom, *pchrom;

struct individual {
    int p[MAX_OBJ]; // Solução
    int fitness; // Valor da qualidade da solução
    int valido; // 1 se for uma solução válida e 0 se não for
};


void tournament(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring);
void crossover(pchrom parents, struct info d, pchrom offspring);
void mutation(pchrom offspring, struct info d);
void crossover_dois_pontos_corte(pchrom parents, struct info d, pchrom offspring);
void mutacao_por_troca(pchrom offspring, struct info d);
void roleta(pchrom pop, struct info d, pchrom parents);
void trepa_colinas_hibrido(pchrom pop, struct info d, ProblemData data, int num_iter);


#endif //PROJETO2_ALGORITMOEVOLUIDO_H
