/*
 * vertice.h
 *
 *  Created on: 29 de abr de 2019
 *      Author: ednei.freiberger
 */
/*
 * vertice.h
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#ifndef GRAFO_VERTICE_H_
#define GRAFO_VERTICE_H_

#include "../lista_enc/lista_enc.h"

/* Exporta os tipos de dados do vertice e arestas */
typedef struct vertices vertice_t;
typedef struct arestas arestas_t;

/* Tipo enum o status das arestas: utilizado para exportação do DOT */
typedef enum status_aresta { VAZIO, EXPORTADA } status_aresta_t;

/* Cria um novo vertice com id */
vertice_t *cria_vertice(int id);

/* Cria uma nova aresta */
arestas_t *cria_aresta(vertice_t *fonte, vertice_t *destino, int peso);

/* Obtem id de um vertice */
int vertice_get_id(vertice_t *vertice);

/* Obtem distancia de um vertice */
int vertice_get_distancia(vertice_t *vertice);

/* Adiciona uma aresta a um grafo */
void adiciona_aresta(vertice_t *vertice, arestas_t *aresta);

/* Obtem a lista encadeada das arestas */
lista_enc_t *vertice_get_arestas(vertice_t *vertice);

/* Retorna o peso de um aresta */
int aresta_get_peso (arestas_t *aresta);

/* Obtem o destinho de uma aresta */
vertice_t *aresta_get_adjacente(arestas_t *aresta);

/* Retorna uma aresta caso seja fonte ou destino */
arestas_t *procurar_adjacente(vertice_t *vertice, vertice_t *adjacente);

/* Configura status de uma aresta */
void aresta_set_status(arestas_t *aresta, status_aresta_t status);

/* Obtem o status de uma aresta */
status_aresta_t aresta_get_status (arestas_t *aresta);


/* Obtem o destino de uma aresta */
vertice_t* aresta_get_destino (arestas_t *aresta);



/* Configura membro id_grupo da struct vertice
 * Ver: struct vertices */
void vertice_set_grupo(vertice_t *vertice, int grupo);

/* Configura membro distancia da struct vertice
 * Ver: struct vertices */
void vertice_set_distancia(vertice_t *vertice, int distancia);

/* Configura membro pai da struct vertice
 * Ver: struct vertices */
void vertice_set_pai(vertice_t *vertice, vertice_t *pai);

/* Configura membro dist da struct vertice
 * Ver: struct vertices */
void vertice_set_dist(vertice_t *vertice, int dist);

/* Configura membro predec da struct vertice
 * Ver: struct vertices */
void vertice_set_predec(vertice_t *vertice, int predec);


/* Obtem membro id_grupo da struct vertice
 * Ver: struct vertices */
int vertice_get_grupo(vertice_t *vertice);


/* Configura membro visitado da struct vertice */
void vertice_set_visitado(vertice_t *vertice, int visitado);


/* Obtem membro id_grupo da struct vertice
 * Ver: struct vertices */
int vertice_get_visitado(vertice_t *vertice);

/* Obtem membro fonte_grupo da struct vertice
 * Ver: struct vertices */
vertice_t* vertice_get_pai(vertice_t *vertice);

/* Obtem membro dist da struct vertice
 * Ver: struct vertices */
int vertice_get_dist(vertice_t *vertice);

/* Obtem membro predec da struct vertice
 * Ver: struct vertices */
vertice_t* vertice_get_predec(vertice_t *vertice);






#endif /* GRAFO_VERTICE_H_ */
