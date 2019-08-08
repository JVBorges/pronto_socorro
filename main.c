/* Integrantes: João Vitor Borges C.C - 201625512   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pronto_socorro.h"
#define MAX_PAC 50

int main()
{
    /* Declaração das variaves */
    fpa f;
    fpa_init(&f);
    int n_med, n_pac, i, j, t, ind_paciente = 0, tempo = 0, aux = 0, instante = 0;
    float cont = 0;
    char op[24];
    scanf("%d", &n_med);
    medico m[n_med];
    paciente p_curados[MAX_PAC];
    /* Recebendo os medicos */
    for (i = 0; i < n_med; i++) {
        scanf("%s", m[i].nome);
        m[i].situacao = 0;
        m[i].tempo_atend = 0;
    }
    /* Recebe o tempo */
    scanf("%d", &t);
    int t_total = t;
    while(t >= 0) {
        instante = t_total;;
        scanf("%d", &n_pac);
        paciente p[n_pac];
        /* Recebendo os pacientes */
        for (i = 0; i < n_pac; i++) {
            scanf(" %s", p[i].sobrenome);
            p[i].gravidade = 4;
            p[i].tolerancia = 180;
            p[i].chegada = t_total;
            scanf(" %23[^\n]", op);
            for (j = 0; j < 23; j++) {
                if (op[j] == 'S') {
                    verifica_gravidade(j / 2, &p[i]);
                    break;
                }
            }
            fpa_ins(&f, p[i]);
        }
        /* Alocando os pacientes aos medicos livres */
        for (i = 0; i < n_med; i++) {
            if (m[i].situacao == 0 && !fpa_isempty(f)) {
                p_curados[ind_paciente] = fpa_rem_min(&f);
                strcpy(p_curados[ind_paciente].medico, m[i].nome);
                m[i].situacao = 1;
                p_curados[ind_paciente].temp_espera = 0;
                tempo_atendimento(&m[i], p_curados[ind_paciente].gravidade);
                ind_paciente++;
            }
        }
        /* Recebe o valor do tempo */
        scanf("%d", &t);
        t_total += t;
        /* Inicio do calculo do tempo passado */
        if (t < 0 && !fpa_isempty(f)) {
            /* Realiza o calculo para saber o tempo de espera dos pacientes que estão
               na fila de espera ate a fila de prioridade ficar vazia                 */
            while (!fpa_isempty(f)) {
                orderna_vet_doc(m, n_med);
                while (m[0].tempo_atend-- > 0) {
                    tempo++;
                }
                for (i = 1; i < n_med; i++) {
                    m[i].tempo_atend -= tempo;
                }
                aux += tempo;
                for (j = 0; j < n_med; j++) {
                    if (m[j].tempo_atend <= 0) {
                        if (!fpa_isempty(f)) {
                            p_curados[ind_paciente] = fpa_rem_min(&f);
                            strcpy(p_curados[ind_paciente].medico, m[j].nome);
                            m[j].situacao = 1;
                            p_curados[ind_paciente].temp_espera = (aux + (t_total - p_curados[ind_paciente].chegada)) + 1;
                            p_curados[ind_paciente].chegada = t_total;
                            tempo_atendimento(&m[j], p_curados[ind_paciente].gravidade);
                            ind_paciente++;
                        }
                    }
                }
                tempo = 0;
            }
        } else {
            /* Caso a fila de prioridade esteja vazia não é necessario calcular o tempo de espera
               então só é necessario calcular a diferença do tempo de atendimento pelo tempo passado */
            if (fpa_isempty(f)) {
                for (i = 0; i < n_med; i++) {
                    m[i].tempo_atend -= t;
                    if (m[i].tempo_atend < 0) m[i].situacao = 0;
                }
            } else {
                /* Caso contrario ele tem que descobrir qual foi o tempo de espera
                   do paciente que esta na fila de espera                           */
                orderna_vet_doc(m, n_med);
                while (instante + aux < t_total) {
                    for (i = 0; i < n_med; i++) {
                        if (m[i].tempo_atend - t <= 0) {
                            while (m[i].tempo_atend-- > 0) {
                                tempo++;
                            }
                            aux += tempo;
                            instante += aux;
                            for (j = 1; j < n_med; j++) {
                                m[j].tempo_atend -= tempo;
                                if (m[j].tempo_atend <= 0) m[j].situacao = 0;
                            }
                            if (m[i].tempo_atend <= 0) {
                                if (!fpa_isempty(f)) {
                                    p_curados[ind_paciente] = fpa_rem_min(&f);
                                    strcpy(p_curados[ind_paciente].medico, m[i].nome);
                                    m[i].situacao = 1;
                                    p_curados[ind_paciente].temp_espera = aux + (t_total - p_curados[ind_paciente].chegada);
                                    p_curados[ind_paciente].chegada = t_total;
                                    tempo_atendimento(&m[i], p_curados[ind_paciente].gravidade);
                                    ind_paciente++;
                                } else {
                                    m[i].situacao = 0;
                                }
                            }
                            tempo = 0;
                        } else {
                            m[i].tempo_atend -= (t - aux);
                            instante += (t - aux);
                        }
                    }
                    aux = 0;
                }
            }
        }
    }
    /* Saida do programa em ordem de atendimento */
    for (i = 0; i < ind_paciente; i++) {
        if (p_curados[i].temp_espera > p_curados[i].tolerancia) cont++;
        printf("%s %s %d %d\n", p_curados[i].medico, p_curados[i].sobrenome, p_curados[i].tolerancia, p_curados[i].temp_espera);
    }
    printf("%.1f%%", (cont / ind_paciente) * 100.0);
    return 0;
}

