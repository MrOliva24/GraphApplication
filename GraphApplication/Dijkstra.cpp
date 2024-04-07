#include "pch.h"
#include "Graph.h"
#include <queue>

// =============================================================================
// Dijkstra ====================================================================
// =============================================================================


void Dijkstra(CGraph& graph, CVertex *pStart)
{	
	
	for (CVertex& vertex : graph.m_Vertices) {
		vertex.m_DijkstraDistance = std::numeric_limits<double>::max();  //infinity()
		vertex.m_DijkstraVisit = false; //inicalitzar en false
		vertex.m_pDijkstraPrevious = NULL;
	}
	pStart->m_DijkstraDistance = 0;
	CVertex* pActual = pStart;
	do  {
		pActual->m_DijkstraVisit = true;
		for (CEdge* v : pActual->m_Edges) {
			if (pActual->m_DijkstraDistance + v->m_Length < v->m_pDestination->m_DijkstraDistance) {
				v->m_pDestination->m_DijkstraDistance = pActual->m_DijkstraDistance + v->m_Length;
				v->m_pDestination->m_pDijkstraPrevious = v;
			}
		}
		pActual = NULL;
		double min_dist = std::numeric_limits<double>::max();
		for (CVertex& v : graph.m_Vertices) {
				if (min_dist > v.m_DijkstraDistance && !v.m_DijkstraVisit) {
					min_dist = v.m_DijkstraDistance;
					pActual = &v;
				}
		}
	} while (pActual != NULL);
}


// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void DijkstraQueue(CGraph& graph, CVertex* pStart)
{
	/*
	struct comparator {
		bool operator()(CVertex* pE1, CVertex* pE2) {
			return pE1->m_DijkstraDistance > pE2->m_DijkstraDistance;
		}
	};

	for (CVertex& vertex : graph.m_Vertices) {
		vertex.m_DijkstraDistance = std::numeric_limits<double>::max();
		vertex.m_DijkstraVisit = false;
	}

	std::priority_queue<CVertex*, std::vector<CVertex*>, comparator> queue;


	pStart->m_DijkstraDistance = 0;
	queue.push(pStart);
	CVertex* pActual = pStart;

	while (!queue.empty()) {


		pActual = queue.top(); queue.pop();

		for (CEdge* vEdge : pActual->m_Edges) {

			double distance = pActual->m_DijkstraDistance + vEdge->m_Length;
			if (distance < vEdge->m_pDestination->m_DijkstraDistance && !vEdge->m_pDestination->m_DijkstraVisit) {
				vEdge->m_pDestination->m_DijkstraDistance = distance;
			}
			queue.push(vEdge->m_pDestination);

		}
	}*/
	struct comparator {
		bool operator()(CVertex* pE1, CVertex* pE2) {
			return pE1->m_DijkstraDistance > pE2->m_DijkstraDistance;
		}
	};

	std::priority_queue<std::pair<double, CVertex*>, std::vector<std::pair<double, CVertex*>>, std::greater<>> queue;

	for (CVertex& vertex : graph.m_Vertices) {
		vertex.m_DijkstraDistance = std::numeric_limits<double>::max();
		vertex.m_DijkstraVisit = false;
		vertex.m_pDijkstraPrevious = NULL;
	}




	pStart->m_DijkstraDistance = 0;
	queue.push({ 0, pStart });
	CVertex* pActual = pStart;
	double dist;

	while (!queue.empty()) {

		dist = queue.top().first;
		pActual = queue.top().second;
		queue.pop();

		if (!pActual->m_DijkstraVisit) {
			pActual->m_DijkstraVisit = true;

			for (CEdge* vEdge : pActual->m_Edges) {

				double distance = pActual->m_DijkstraDistance + vEdge->m_Length;

				if (distance < vEdge->m_pDestination->m_DijkstraDistance) {
					vEdge->m_pDestination->m_DijkstraDistance = distance;
					vEdge->m_pDestination->m_pDijkstraPrevious = vEdge;
					queue.push({ distance, vEdge->m_pDestination });

				}
			}
		}
	}
}