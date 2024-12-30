//
// Created by footr on 24/11/2024.
//

#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int n_coins;
    float *coins;
    float goal;
} ProblemData;
ProblemData init_dados(char* nome);

//local
void gera_sol_inicial(int *sol, ProblemData data);
int random_l_h(int min, int max);
float rand_01();
void init_rand();
void escreve_sol(int *sol, ProblemData data);
int calcula_fit(int s[], ProblemData data);
void substitui(int s[], const int ns[], int n);
int reparaSol(int *sol, ProblemData data, float *sum);
float calcula_soma(const int s[], ProblemData data);
int calcula_total(int s[], int n_coins);



#endif //UTILS_H
