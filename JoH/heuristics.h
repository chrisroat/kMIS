#ifndef KMIS_HEURISTICS_H
#define KMIS_HEURISTICS_H

#include "graph.h"
#include "grasp.h"

using namespace std;

/**
Heur�stica kInter (Artigo do sbpo 2013).
**/
Solucao heuristica_kinter(Graph &graph);

/**
Heur�stica kInter estendida.
**/
Solucao heuristica_kinter_estendida(Graph &graph);

/**
Heur�stica kInter estendida sendo que sempre gero a solu��o mesmo que a solu��o parcial tenha valor
menor que a melhor at� agora encotrado.
**/
Solucao heuristica_kinter_estendida_path_relinking(Graph &graph);

#endif // KMIS_HEURISTICS_H
