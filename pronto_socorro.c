#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pronto_socorro.h"

void fpa_init(fpa * f) {
    f->n = 0;
}

bool fpa_isempty(fpa f) {
    return f.n == 0;
}

bool fpa_isfull(fpa f) {
    return f.n == MAX_FPA;
}

void fpa_ins(fpa * f, paciente x)
{
    if (!fpa_isfull(*f))
    {
        int p, pai;
        f->itens[(f->n)++] = x; // insere no final do heap
        p = f->n-1;
        // índice do último
        pai = (p - 1) / 2; // pai de p
        while (p > 0 && f->itens[pai].gravidade > x.gravidade)
        {
            f->itens[p] = f->itens[pai];
            p = pai;
            pai = (p - 1) / 2;
        }
        f->itens[p] = x;
    }
    else
    {
        fprintf(stderr, "Fila de prioridade cheia!\n");
        exit(1);
    }

}

// Funcao para remover o heap minimo
paciente fpa_rem_min(fpa * f)
{
    if (!fpa_isempty(*f))
    {
        int i = 0, j = 1, d; // índices do heap
        paciente menor = f->itens[0], x;
        f->itens[0] = f->itens[--(f->n)]; // passa o último para a raiz
        x = f->itens[0]; // novo elemento
        d = f->n-1;
        // índice do último
        if(j < d && f->itens[j+1].gravidade < f->itens[j].gravidade) j++;
        while(j <= d && f->itens[j].gravidade < x.gravidade)
        {
            f->itens[i] = f->itens[j];
            i = j;
            j = 2 * j + 1;
            if(j < d && f->itens[j+1].gravidade < f->itens[j].gravidade) j++;
        }
        f->itens[i] = x;
        return menor;
    }
    else
    {
        fprintf(stderr, "Fila de prioridade vazia!\n");
        exit(1);
    }
}

// Função para verificar a gravidade do paciente
void verifica_gravidade (int i, paciente * p) {
    if (i < 3) {
        p->gravidade = 0;
        p->tolerancia = 0;
    }
    if (i > 2 && i < 6) {
        p->gravidade = 1;
        p->tolerancia = 10;
    }
    if (i > 5 && i < 9) {
        p->gravidade = 2;
        p->tolerancia = 20;
    }
    if (i > 8 && i < 12) {
        p->gravidade = 3;
        p->tolerancia = 60;
    }
}

// Função para calcular o tempo de atendimento de cada paciente
void tempo_atendimento(medico * n, int i) {
    switch (i) {
    case 0:
        n->tempo_atend = 40 + (rand() % 21);
        break;
    case 1:
        n->tempo_atend = 15 + (rand() % 11);
        break;
    case 2:
        n->tempo_atend = 10 + (rand() % 11);
        break;
    case 3:
        n->tempo_atend = 6 + (rand() % 5);
        break;
    case 4:
        n->tempo_atend = 1 + (rand() % 10);
        break;
    }
}

// Função de ordenação do vetor de medico em ordem crescente em relação ao tempo de etendimento
void orderna_vet_doc(medico * m, int n) {
    int i, j;
    medico aux;
    for (i = 1; i < n; i++) {
        aux = m[i];
        for (j = i; (j > 0) && (aux.tempo_atend < m[j - 1].tempo_atend); j--) {
            m[j] = m[j - 1];
        }
        m[j] = aux;
    }
}

