//
// Created by footr on 08/12/2024.
//

#ifndef PROJETO2_UTILSEVOLUTIVO_H
#define PROJETO2_UTILSEVOLUTIVO_H
#include "algoritmoEvoluido.h"
#include "utils.h"

pchrom init_pop(struct info d, ProblemData data);
chrom get_best(pchrom pop, struct info d, chrom best);
void write_best(chrom x, struct info d);
void evaluate(pchrom pop, struct info d, ProblemData data);
int eval_individual(int *sol, ProblemData data, int *v);

#endif //PROJETO2_UTILSEVOLUTIVO_H
