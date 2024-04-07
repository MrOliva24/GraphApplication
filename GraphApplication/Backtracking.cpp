#include "pch.h"
#include "Graph.h"
#include <set>
#include <stack>



CVertex* vertexDesti;
CTrack solucioOptima(NULL);
CVisits visites(NULL);
list<list<int>> cami;
double LCamiMesCurt;
double LCamiActual;
double distancia;
double distanciaOptima;
list<CVertex*> llistaV;
list<CVertex*> llistaVdesordenada;

CTrack Cami_MesCurt(NULL);
double Longitud_CamiMesCurt;


struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};


void RecursivaBT(NodeCami* pAnterior, CVertex* pActual);

bool TotsVisitats() { //Comprovem si tots els nodes son visitats
	int sum;
	for (CVertex* vertexAux : visites.m_Vertices) {

		sum = 0;

		for (CEdge* edgeAux : vertexAux->m_Edges) {

			if (edgeAux->m_Processed != false) {
				++sum;
			}
		}

		if (sum == 0) {
			return false;
		}
	}
	return true;
}




CTrack SalesmanTrackBacktracking(CGraph& graph, CVisits& visits)
{
	visites = visits;

	// distOptima = MAX
	distanciaOptima = numeric_limits<double>::max();
	// dist = 0
	distancia = 0;

	//Guardamos el primer nodo
	CVertex* vertexOrigen = visites.m_Vertices.front();

	//Guardamos y quietamos el ultimo nodo
	vertexDesti = visites.m_Vertices.back();
	visites.m_Vertices.pop_back();

	//Definim totes les arestes com a processades
	for (CEdge& edge : graph.m_Edges) {
		edge.m_Processed = false;
	}

	//Llamamos a la funcion de arriba
	RecursivaBT(NULL, vertexOrigen);

	return solucioOptima;
}


void RecursivaBT(NodeCami* pAnterior, CVertex* pActual)
{
	// Miramos si es el ultimo vertice
	if (pActual == vertexDesti) {
		if (TotsVisitats()) {
			if (distanciaOptima > distancia) {

				solucioOptima.Clear();

				while (pAnterior) {

					solucioOptima.m_Edges.push_front(pAnterior->m_pEdge);
					pAnterior = pAnterior->m_pAnterior;

				}

				distanciaOptima = distancia;
			}
			return;

		}
	}

	NodeCami nodeAux;
	nodeAux.m_pAnterior = NULL;

	for (CEdge* edgeAux : pActual->m_Edges) {
		nodeAux.m_pAnterior = pAnterior;

		if (distanciaOptima > distancia + edgeAux->m_Length) {

			if (!edgeAux->m_Processed) {

				nodeAux.m_pEdge = edgeAux;
				edgeAux->m_Processed = true;
				distancia = distancia + edgeAux->m_Length;

				RecursivaBT(&nodeAux, edgeAux->m_pDestination);

				distancia = distancia - edgeAux->m_Length;
				edgeAux->m_Processed = false;
				nodeAux.m_pEdge = NULL;

			}
		}
	}
	nodeAux.m_pAnterior = NULL;

}





// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================
struct element {
	double dist;
	list<CEdge*> CT;
};

element matriu[20][20];
int ordre[20];
int ordre2[20];


void SalesmanTrackBacktrackingGreedyRec(int i)
{
	if (LCamiActual < LCamiMesCurt) {
		if (i == llistaV.size() - 1) {
			LCamiActual += abs(matriu[ordre[i - 1]][ordre[i]].dist);
			if (LCamiActual < LCamiMesCurt) {
				LCamiMesCurt = LCamiActual;
				for (int l = 0; l < llistaV.size(); l++) {
					ordre2[l] = ordre[l];
				}
			}
			LCamiActual -= abs(matriu[ordre[i - 1]][ordre[i]].dist);
		}
		else {
			for (int j = 1; j < llistaV.size() - 1; j++) {
				bool found = false;
				for (int k = 1; k < i && !found; k++) {
					found = (j == ordre[k]);
				}

				if (!found) {
					ordre[i] = j;
					LCamiActual += abs(matriu[ordre[i - 1]][ordre[i]].dist);
					i++;
					SalesmanTrackBacktrackingGreedyRec(i);
					i--;
					LCamiActual -= abs(matriu[ordre[i - 1]][ordre[i]].dist);
				}
			}
		}
	}
}


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	llistaV = visits.m_Vertices;
	llistaVdesordenada = llistaV;

	int i = 0;
	int j;
	for (CVertex* vertex : llistaV) {
		Dijkstra(graph, vertex);
		j = 0;
		for (CVertex* vertex2 : llistaVdesordenada) {
			matriu[i][j].dist = vertex2->m_DijkstraDistance;
			while (vertex2->m_pDijkstraPrevious) {
				matriu[i][j].CT.push_front(vertex2->m_pDijkstraPrevious);
				vertex2 = vertex2->m_pDijkstraPrevious->m_pOrigin;
			}
			j++;
		}
		i++;
	}

	LCamiMesCurt = numeric_limits<double>::max();
	LCamiActual = 0.0;
	ordre[0] = 0;
	ordre[llistaV.size() - 1] = llistaV.size() - 1;
	SalesmanTrackBacktrackingGreedyRec(1);

	CTrack t(&graph);

	for (int m = 0; m < llistaV.size() - 1; m++) {
		while (!matriu[ordre2[m]][ordre2[m + 1]].CT.empty()) {
			t.m_Edges.push_back(matriu[ordre2[m]][ordre2[m + 1]].CT.front());
			matriu[ordre2[m]][ordre2[m + 1]].CT.pop_front();
		}
	}

	return t;
}