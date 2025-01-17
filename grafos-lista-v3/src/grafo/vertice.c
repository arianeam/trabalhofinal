/*
 * vertice.c
 *
 *  Created on: 29 de abr de 2019
 *      Author: ednei.freiberger
 */


/*
 * vertice.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdlib.h>
#include <stdio.h>

#include "vertice.h"
#include "../lista_enc/lista_enc.h"



struct vertices {
	int id;                /*!< Identifica��o num�rica do v�rtice  */
	lista_enc_t *arestas;  /*!< Listas encadeada das arestas pertencentes ao v�rtice  */

	/* Informa��es para componentes conexos */
	int id_grupo;
	vertice_t* pai;
	int visitado;  /*Busca em Profundidade*/
	int distancia;   /*<Busca em largura*/
    int dist;       /*bellman-ford*/
	int predec;     /*bellman-ford*/

};

struct arestas {
	int peso;          /*!< Identifica��o num�rica da aresta  */
	vertice_t *fonte;  /*!< V�rtice fonte. Geralmente v�rtice pertencente a lista de arestas do v�rtice */
	vertice_t *dest;   /*!< V�rtice destino */

	status_aresta_t status;     /*!< Estado de exporta��o. Utilizado na fun��o de exporta��o para dot. */
};

/**
  * @brief  Cria uma novo v�rtice
  * @param	id: Identifica��o num�rica do v�rtice
  *
  * @retval vertice_t: ponteiro para um novo v�rtice
  */
vertice_t *cria_vertice(int id)
{
	vertice_t *p = NULL;

	p = malloc(sizeof(vertice_t));

	if (p == NULL) {
		perror("cria_vertice:");
		exit(EXIT_FAILURE);
	}

	p->id = id;
	p->arestas = cria_lista_enc();
	p->id_grupo = -1;
	p->pai = NULL;
	p->distancia = 0;

	return p;
}

/**
  * @brief  Obt�m o identificador num�rico do v�rtice
  * @param	vertice: ponteiro do v�rtice
  *
  * @retval int: identificador num�rico do v�rtice
  */
int vertice_get_id(vertice_t *vertice)
{
	if (vertice == NULL)
	{
		fprintf(stderr, "vertice_obter_id: vertice invalido!\n");
		exit(EXIT_FAILURE);
	}

	return vertice->id;
}
/**
  * @brief  Cria uma aresta entre dois v�rtices
  * @param	fonte: ponteiro do v�rtice fonte
  * @param	destino: ponteiro do v�rtice destino
  * @param  peso: peso da aresta
  *
  * @retval arestas_t: ponteiro da nova aresta criada
  */
arestas_t *cria_aresta(vertice_t *fonte, vertice_t *destino, int peso)
{
	arestas_t *p;

	p = (arestas_t*)malloc(sizeof(arestas_t));

	if (p == NULL) {
		perror("cria_aresta:");
		exit(EXIT_FAILURE);
	}

	p->peso = peso;
	p->fonte = fonte;
	p->dest = destino;

	return p;
}

/**
  * @brief  Adiciona uma nova aresta na lista de arestas de um v�rtice
  * @param	vertice: ponteiro do v�rtice
  * @param	aresta: ponteiro da aresta
  *
  * @retval Nenhum
  */
void adiciona_aresta(vertice_t *vertice, arestas_t *aresta)
{
	no_t *no;

	if (vertice == NULL || aresta == NULL)	{
		fprintf(stderr, "adiciona_aresta: dados invalidos\n");
		exit(EXIT_FAILURE);
	}

	no = cria_no(aresta);
	add_cauda(vertice->arestas, no);

}

/**
  * @brief  Obt�m a lista encadadeada de arestas: �til para obter todos v�rtices adjacentes
  * @param	vertice: ponteiro do v�rtice
  *
  * @retval lista_enc_t: lista encadeada contendas as arestas
  */
lista_enc_t *vertice_get_arestas(vertice_t *vertice)
{
	if (vertice == NULL){
		fprintf(stderr, "vertice_get_arestas: vertice invalido\n");
		exit(EXIT_FAILURE);
	}

	return vertice->arestas;
}




/**
  * @brief  Obt�m a lista encadadeada de arestas: �til para obter todos v�rtices adjacentes
  * @param	vertice: ponteiro do v�rtice
  *
  * @retval int: retorna a distancia do vertice
  */
int vertice_get_distancia(vertice_t *vertice)
{
	if (vertice == NULL){
		fprintf(stderr, "vertice_get_distancia: vertice invalido\n");
		exit(EXIT_FAILURE);
	}

	return vertice->distancia;
}





/**
  * @brief  Obt�m o peso se uma aresta
  * @param	aresta: ponteiro da aresta
  *
  * @retval int: peso num�rico da aresta
  */
int aresta_get_peso (arestas_t *aresta) {
	if (aresta == NULL){
		fprintf(stderr, "aresta_get_peso: aresta invalido\n");
		exit(EXIT_FAILURE);
	}

	return aresta->peso;
}



/**
  * @brief  Obt�m o destino de uma aresta
  * @param	aresta: ponteiro da aresta
  *
  * @retval vertice_t: destino da aresta
  */
vertice_t *aresta_get_destino (arestas_t *aresta) {
	if (aresta == NULL){
		fprintf(stderr, "aresta_get_peso: aresta invalido\n");
		exit(EXIT_FAILURE);
	}

	return aresta->dest;
}


/**
  * @brief  Obt�m o v�rtice adjacente de um aresta
  * @param	aresta: ponteiro da aresta
  *
  * @retval vertice_t: v�rtice adjacente ou destino
  */
vertice_t *aresta_get_adjacente(arestas_t *aresta)
{
	if (aresta == NULL){
		fprintf(stderr, "aresta_get_adjacente: aresta invalido\n");
		exit(EXIT_FAILURE);
	}

	return aresta->dest;
}

/**
  * @brief  Retorna uma aresta se um v�rtice for adjacente a outro: busca linear na lista de arestas
  * @param	vertice: v�rtice de refer�ncia
  * @param  adjacente: v�rtice que se deseja verificar se � adjacente � refer�ncia
  *
  * @retval arestas_t: aresta adjacente. NULL se n�o for adjacente
  */
arestas_t *procurar_adjacente(vertice_t *vertice, vertice_t *adjacente)
{
	no_t *no;
	arestas_t *aresta;

	if (vertice == NULL){
		fprintf(stderr, "procurar_adjacente: aresta invalido\n");
		exit(EXIT_FAILURE);
	}

	no = obter_cabeca(vertice->arestas);

	while (no){

		aresta = obter_dado(no);

		if (aresta->dest == adjacente || aresta->fonte == adjacente)
			return aresta;

		no = obtem_proximo(no);
	}

	return NULL;
}

/**
  * @brief  Retorna o estado de exporta��o de uma aresta. Utilizada para gera��o do dot
  * @param	aresta: aresta em quest�o
  *
  * @retval status_aresta_t: estado atual: conforme status_aresta_t
  */
status_aresta_t aresta_get_status (arestas_t *aresta)
{
	if (aresta == NULL){
		fprintf(stderr, "aresta_get_status: aresta invalida\n");
		exit(EXIT_FAILURE);
	}

	return aresta->status;
}

/**
  * @brief  Altera o de exporta��o de uma aresta. Utilizada para gera��o do dot
  * @param	aresta: aresta em quest�o
  * @param  status: novo estado: conforme status_aresta_t
  *
  * @retval Nenhum
  */
void aresta_set_status(arestas_t *aresta, status_aresta_t status)
{
	if (aresta == NULL){
		fprintf(stderr, "aresta_set_status: aresta invalida\n");
		exit(EXIT_FAILURE);
	}

	aresta->status = status;
}

/**
  * @brief  Altera a propriedade grupo de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  grupo: novo grupo
  *
  * @retval Nenhum
  */
void vertice_set_grupo(vertice_t *vertice, int grupo) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->id_grupo = grupo;
}



/**
  * @brief  Altera a propriedade grupo de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  grupo: novo grupo
  *
  * @retval Nenhum
  */
void vertice_set_visitado(vertice_t *vertice, int visitado) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->visitado = visitado;
}




/**
  * @brief  Obt�m a propriedade grupo de um v�rtice
  * @param	vertice: v�rtice em quest�o
  *
  * @retval int: grupo da aresta
  */
int vertice_get_grupo(vertice_t *vertice) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->id_grupo;
}


/**
  * @brief  Obt�m a propriedade visitado de um v�rtice
  * @param	vertice: v�rtice em quest�o
  *
  * @retval int: grupo da aresta
  */
int vertice_get_visitado(vertice_t *vertice) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->visitado;
}


/**
  * @brief  Obt�m a propriedade fonte de um v�rtice
  * @param	vertice: v�rtice em quest�o
  *
  * @retval int: grupo da aresta
  */
vertice_t* vertice_get_pai(vertice_t *vertice) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->pai;
}


/**
  * @brief  Obt�m a propriedade dist de um v�rtice
  * @param	vertice: v�rtice em quest�o
  *
  * @retval int: dist da aresta
  */
int vertice_get_dist(vertice_t *vertice) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->dist;
}


/**
  * @brief  Obt�m a propriedade predec de um v�rtice
  * @param	vertice: v�rtice em quest�o
  *
  * @retval int: predec da aresta
  */
vertice_t* vertice_get_predec(vertice_t *vertice) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_grupo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->predec;
}

/**
  * @brief  Altera a propriedade pai de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  pai: novo v�rtice adjacente e pai
  *
  * @retval Nenhum
  */
void vertice_set_pai(vertice_t *vertice, vertice_t *pai) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_pai: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->pai = pai;
}






/**
  * @brief  Altera a propriedade distancia de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  distancia: distancia entre vertice fonte
  *
  * @retval Nenhum
  */
void vertice_set_distancia(vertice_t *vertice, int dist) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_distancia: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->distancia = dist;
}



/**
  * @brief  Altera a propriedade dist de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  distancia: dist entre vertice fonte
  *
  * @retval Nenhum
  */
void vertice_set_dist(vertice_t *vertice, int dist) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_distancia: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->dist = dist;
}



/**
  * @brief  Altera a propriedade predec de um v�rtice
  * @param	vertice: v�rtice em quest�o
  * @param  distancia: predec entre vertice fonte
  *
  * @retval Nenhum
  */
void vertice_set_predec(vertice_t *vertice, int predec) {

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_distancia: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->predec = predec;
}


