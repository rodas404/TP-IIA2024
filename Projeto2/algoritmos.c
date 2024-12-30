//
// Created by footr on 25/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"
#include "utils.h"

void gera_vizinho(int s[], int ns[], ProblemData data) {
    int p1, p2;

    //copia solução original para a nova
    for (int i = 0; i < data.n_coins; i++)
        ns[i] = s[i];

    do{
        p1 = random_l_h(0, data.n_coins-1);
    } while(ns[p1] == 0);

    do{
        p2 = random_l_h(0, data.n_coins-1);
    } while(p2 == p1);

    // troca posicoes
    int temp = ns[p1];
    ns[p1] = ns[p2];
    ns[p2] = temp;

}



int trepa_colinas(int sol[], ProblemData data, int n_iter){
    int *nova_sol, cost, cost_viz;
    nova_sol = malloc(sizeof(int) * data.n_coins);

    if(nova_sol == NULL){
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }
    cost = calcula_fit(sol, data);
    for(int i=0; i<n_iter; i++){
        gera_vizinho2(sol, nova_sol, data);
        cost_viz = calcula_fit(nova_sol, data);
        if(cost_viz <= cost){
            substitui(sol, nova_sol, data.n_coins);
            cost = cost_viz;
        }
    }

    free(nova_sol);
    return cost;
}

void gera_vizinho2(int s[], int ns[], ProblemData data) {
    int p1, p2, p3, p4;
    float sum;

    // Copia a solução original para a nova
    for (int i = 0; i < data.n_coins; i++)
        ns[i] = s[i];

    // Primeira troca de posições
    do {
        p1 = random_l_h(0, data.n_coins - 1);
    } while (ns[p1] == 0);

    do {
        p2 = random_l_h(0, data.n_coins - 1);
    } while (p2 == p1);

    int temp = ns[p1];
    ns[p1] = ns[p2];
    ns[p2] = temp;

    // Segunda troca de posições
    do {
        p3 = random_l_h(0, data.n_coins - 1);
    } while (ns[p3] == 0);

    do {
        p4 = random_l_h(0, data.n_coins - 1);
    } while (p4 == p3);

    temp = ns[p3];
    ns[p3] = ns[p4];
    ns[p4] = temp;
}



