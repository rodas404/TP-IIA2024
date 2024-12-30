//
// Created by footr on 24/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.h"

ProblemData init_dados(char* nome) {
    FILE *f;
    ProblemData dados;

    f = fopen(nome, "r");
    if (f == NULL) {
        printf("Erro ao abrir ficheiro.\n");
        exit(1);
    }

    // Número de moedas
    fscanf(f, " %d", &dados.n_coins);

    // Objetivo
    fscanf(f, " %f", &dados.goal);

    // Matriz para as moedas
    dados.coins = malloc(sizeof(float) * dados.n_coins);
    if (dados.coins == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }

    // Preenchimento da matriz
    for (int i = 0; i < dados.n_coins; i++) {
        fscanf(f, " %f", &dados.coins[i]);
    }

    fclose(f);
    return dados;
}


void gera_sol_inicial(int *sol, ProblemData data) {
    float sum = 0.0;

    for (int i = 0; i < data.n_coins; i++)
        sol[i] = 0;

    while (sum < data.goal) {
        int x = random_l_h(0, data.n_coins - 1);
        sol[x]++;
        sum += data.coins[x];
    }
}

void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = random_l_h(0, i);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int reparaSol(int *sol, ProblemData data, float *sum) {
    //versao aleatoria
    /*while(*sum != data.goal){
        int pos;
        if(*sum > data.goal){
            do{
                pos = random_l_h(0, data.n_coins-1);
            }while(sol[pos] == 0);
            sol[pos]--;
            *sum -= data.coins[pos];
        }
        if(*sum < data.goal){
            pos = random_l_h(0, data.n_coins - 1);
            sol[pos]++;
            *sum += data.coins[pos];
        }
    }*/


    //versao hibrida
    int *indices = malloc(data.n_coins * sizeof(int));
    if (indices == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }

    for (int i = 0; i < data.n_coins; i++)
        indices[i] = i;

    shuffle(indices, data.n_coins);
    for (int k = 0; k < data.n_coins; k++) {
        int i = indices[k];
        while (sol[i] > 0 && *sum > data.goal) {
            sol[i]--;
            *sum -= data.coins[i];
        }
    }

    shuffle(indices, data.n_coins);
    for (int k = 0; k < data.n_coins; k++) {
        int i = indices[k];
        while (*sum < data.goal && *sum + data.coins[i] <= data.goal) {
            sol[i]++;
            *sum += data.coins[i];
        }
    }
    free(indices);

    //versao deterministica
   /* for (int i = 0; i < data.n_coins; i++) {
        while (sol[i] > 0 && *sum > data.goal) {
            sol[i]--;
            *sum -= data.coins[i];
        }
    }

    for (int i = data.n_coins - 1; i >= 0; i--) {
        while (*sum < data.goal && *sum + data.coins[i] <= data.goal) {
            sol[i]++;
            *sum += data.coins[i];
        }
    }*/
    return calcula_total(sol, data.n_coins);
}


int random_l_h(int min, int max){
    return min + rand() % (max-min+1);
}

float rand_01(){
    return ((float)rand())/RAND_MAX;
}

void init_rand(){
    srand((unsigned)time(NULL));
}

void escreve_sol(int *sol, ProblemData data) {
    float soma = 0.0;
    for(int i=0; i<data.n_coins; i++){
        printf("%d ", sol[i]);
        soma += data.coins[i] * (float)sol[i];
    }
    printf("-> %.2f (%.2f)\n", soma, data.goal);
}

int calcula_fit(int s[], ProblemData data) {
    int total = calcula_total(s, data.n_coins);
    float soma = calcula_soma(s, data);
    if(soma != data.goal){
        //reparacao
        total = reparaSol(s, data, &soma);
        if(soma != data.goal) {
            //penalizacao
            total = total + fabsf(soma - data.goal) * 1000;
        }
    }
    /*if(soma != data.goal) {
        //apenas penalizacao
        total = total + fabsf(soma - data.goal) * 1000;
    }*/
    return total;
}


int calcula_total(int s[], int n_coins){
    int total = 0;
    for (int i = 0; i < n_coins; i++) {
        total += s[i];
    }
    return total;
}

void substitui(int s[], const int ns[], int n){
    int i;
    for(i=0; i<n; i++)
        s[i]=ns[i];
}

float calcula_soma(const int s[], ProblemData data) {
    float soma = 0.0;
    for (int i = 0; i < data.n_coins; i++) {
        soma += (float)s[i] * data.coins[i];
    }
    return soma;
}







