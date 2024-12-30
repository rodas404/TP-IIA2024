//
// Created by footr on 24/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algoritmos.h"
#include "utils.h"

#include "utilsEvolutivo.h"
#include "algoritmoEvoluido.h"

#define DEFAULT_RUNS 30

void mainLocal(ProblemData dados, int runs, float mbf){
    int *sol, *best, i;
    int cost, best_cost = 0, num_iter = 10000;

    sol = malloc(sizeof(int) * dados.n_coins);
    best = malloc(sizeof(int) * dados.n_coins);

    if (sol == NULL || best == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }
    for (i = 0; i < runs; i++) {
        gera_sol_inicial(sol, dados);
        printf("Solucao inicial: ");
        escreve_sol(sol, dados);
        cost = trepa_colinas(sol, dados, num_iter);

        printf("\nRepeticao %d: ", i+1);
        escreve_sol(sol, dados);
        printf("Custo final: %d\n", cost);
        mbf += (float)cost;
        if(i==0 || cost < best_cost){
            best_cost = cost;
            substitui(best, sol, dados.n_coins);
        }
    }
    printf("\n\nMBF: %.3f\n", mbf/(float)i);
    printf("Melhor solucao obtida: ");
    escreve_sol(best, dados);
    printf("Melhor custo: %d\n", best_cost);
    free(sol);
    free(best);
}



void mainEvolutivo(int runs, float mbf, ProblemData data){
    struct info EA_param;
    pchrom pop = NULL, parents = NULL;
    chrom best_run, best_ever;
    int gen_actual, r, inv, i;

    EA_param.popsize = 150; //tamanho da população
    EA_param.pr = 0.5;     //probabilidade de recombinação
    EA_param.pm = 0.1;    //probabilidade de mutação
    EA_param.tsize = 2;    //tamanho do torneio
    EA_param.numGenes = data.n_coins; // tamanho da solucao
    EA_param.numGenerations = 10000;  //numero de geracoes

    for(r=0; r<runs; r++){
        printf("Repeticao %d\n", r+1);
        pop = init_pop(EA_param, data);
        evaluate(pop, EA_param, data);
        gen_actual = 1;
        best_run = pop[0];
        best_run = get_best(pop, EA_param, best_run);
        parents = malloc(sizeof(chrom) * EA_param.popsize);
        if(parents == NULL){
            printf("Erro na alocacao de memoria.\n");
            exit(1);
        }
        while(gen_actual <= EA_param.numGenerations){
            tournament(pop, EA_param, parents);
            //roleta(pop, EA_param, parents);
            genetic_operators(parents, EA_param, pop);
            evaluate(pop, EA_param, data);
            best_run = get_best(pop, EA_param, best_run);
            gen_actual++;
        }

        // Contagem das soluções inválidas
        for (inv=0, i=0; i<EA_param.popsize; i++) {
            if (pop[i].valido == 0)
                inv++;
        }

        // Escreve resultados da repetição que terminou
        write_best(best_run, EA_param);
        printf("Percentagem Invalidos: %.3f\n\n", 100*(float)inv/(float)EA_param.popsize);
        mbf += (float) best_run.fitness;

        if (r == 0 || best_run.fitness < best_ever.fitness)
            best_ever = best_run;
    }

    printf("\nMBF: %.3f\n", mbf / (float)r);
    printf("\nMelhor solucao encontrada:\n");
    write_best(best_ever, EA_param);

    // Liberta a memória usada
    free(parents);
    free(pop);
}



void mainHibrido(int runs, float mbf, ProblemData data){
    struct info EA_param;
    pchrom pop = NULL, parents = NULL;
    chrom best_run, best_ever;
    int gen_actual, r, inv, i, num_iter = 10000;

    EA_param.popsize = 100; //tamanho da população
    EA_param.pr = 0.5;     //probabilidade de recombinação
    EA_param.pm = 0.1;    //probabilidade de mutação
    EA_param.tsize = 2;    //tamanho do torneio
    EA_param.numGenes = data.n_coins; // tamanho da solucao
    EA_param.numGenerations = 1000;  //numero de geracoes

    for(r=0; r<runs; r++){
        printf("Repeticao %d\n", r+1);
        pop = init_pop(EA_param, data);
        evaluate(pop, EA_param, data);
        trepa_colinas_hibrido(pop, EA_param, data, num_iter);
        best_run = pop[0];
        best_run = get_best(pop, EA_param, best_run);
        parents = malloc(sizeof(chrom) * EA_param.popsize);
        if(parents == NULL){
            printf("Erro na alocacao de memoria.\n");
            exit(1);
        }
        gen_actual = 1;
        while(gen_actual <= EA_param.numGenerations){
            //tournament(pop, EA_param, parents);
            roleta(pop, EA_param, parents);
            genetic_operators(parents, EA_param, pop);
            evaluate(pop, EA_param, data);
            best_run = get_best(pop, EA_param, best_run);
            gen_actual++;
        }
        trepa_colinas_hibrido(pop, EA_param, data, num_iter);
        // Contagem das soluções inválidas
        for (inv=0, i=0; i<EA_param.popsize; i++)
            if (pop[i].valido == 0)
                inv++;

        // Escreve resultados da repetição que terminou
        write_best(best_run, EA_param);
        printf("Percentagem Invalidos: %.3f\n\n", 100*(float)inv/(float)EA_param.popsize);
        mbf += (float) best_run.fitness;
        if (r == 0 || best_run.fitness < best_ever.fitness)
            best_ever = best_run;
    }

    printf("\nMBF: %.3f\n", mbf / (float)r);
    printf("\nMelhor solucao encontrada:\n");
    write_best(best_ever, EA_param);

    // Liberta a memória usada
    free(parents);
    free(pop);
}

int main(int argc, char *argv[]) {
    char nome_fich[100];
    int runs, i;
    float mbf = 0.0;
    ProblemData dados;

    if (argc == 3) {
        runs = atoi(argv[2]);
        strcpy(nome_fich, argv[1]);
    } else if (argc == 2) {
        runs = DEFAULT_RUNS;
        strcpy(nome_fich, argv[1]);
    } else {
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro:");
        gets(nome_fich);
    }

    if (runs <= 0)
        return 0;

    init_rand();

    dados = init_dados(nome_fich);

    printf("\nObjetivo: %.2f\n", dados.goal);
    printf("Moedas disponiveis: ");
    for (i = 0; i < dados.n_coins; i++)
        printf("%.2f ", dados.coins[i]);
    printf("\n\n");

    int num;
    printf("Algoritmos disponiveis:\n");
    printf("1 - Algoritmo Local\n");
    printf("2 - Algoritmo Evolutivo\n");
    printf("3 - Algoritmo Hibrido\n");
    printf("Opcao:");
    scanf("%d", &num);
    switch (num) {
        case 1:
            mainLocal(dados, runs, mbf);
            break;
        case 2:
            mainEvolutivo(runs, mbf, dados);
            break;
        case 3:
            mainHibrido(runs, mbf, dados);
            break;
        default:
            printf("Invalido\n");
            break;
    }

    free(dados.coins);
    return 0;
}