
#ifndef _PRONTO_SOCORRO_H
#define _PRONTO_SOCORRO_H

#include <stdbool.h>

#define MAX_FPA 50

typedef int filaprio_info;

typedef struct {
    char sobrenome[41];
    char medico[41];
    int gravidade;
    int tolerancia;
    int temp_espera;
    int chegada;
} paciente;

typedef struct {
    char nome[41];
    int situacao;
    int tempo_atend;
} medico;

typedef struct {
    paciente itens[MAX_FPA];
    int n;
} fpa;

 // inicizaliza a fila de prioridade ascendente
 void fpa_init(fpa *);

 // verifica se a fila está cheia
 bool fpa_isempty(fpa);

 // verifica se a fila está cheia
 bool fpa_isfull(fpa);

 // insere um valor na fila de prioridade ascendente
 void fpa_ins(fpa *, paciente);

 // remove e devolve o valor de menor prioridade na fila
 paciente fpa_rem_min(fpa *);

 // verifica a gravidade em que o paciente se encaixa dada as respostas do questionario
 void verifica_gravidade(int, paciente *);

 // calcula o tempo que levara o atendimento do paciente
 void tempo_atendimento(medico *, int);

 // ordena o vetor de medicos em ordem crescente em relação ao tempo levado para realizar o atendimento
 void orderna_vet_doc (medico *, int);

 #endif // _PRONTO_SOCORRO_H
