#include "pch.h"
#include "Graph.h"
#include <queue>
// Salesman
// 
// 
// Greedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits &visits)
{
	CTrack TrackFull(&graph);

	CVertex* v = visits.m_Vertices.front();
	boolean ultimo_añadido = false;
	list<CVertex*> Candidats =  visits.m_Vertices;

	Candidats.pop_front();
	Candidats.pop_back();
	
	while (!Candidats.empty()) {
		Dijkstra(graph, v);

		CVertex* v1 = Candidats.front();
		for (CVertex* possible_min : Candidats) {
			if (v1==v || v1->m_DijkstraDistance > possible_min->m_DijkstraDistance) {
				v1 = possible_min;
			}
		}

		CTrack TrackParcial(&graph);
		CVertex* v1_tmp = v1;

		while (v1_tmp != v) {
			TrackParcial.AddFirst(v1_tmp->m_pDijkstraPrevious);
			CVertex* aux = v1_tmp->m_pDijkstraPrevious->m_pOrigin;
			v1_tmp = aux;
		}
			
		TrackFull.Append(TrackParcial);

		Candidats.remove(v1);
		v = v1;
	}
	
	Dijkstra(graph, v);

	CVertex* final = visits.m_Vertices.back();
	CTrack TrackParcialFinal(&graph);
	CVertex* v1_tmp = final;

	while (v1_tmp != v) {
		TrackParcialFinal.AddFirst(v1_tmp->m_pDijkstraPrevious);
		CVertex* aux = v1_tmp->m_pDijkstraPrevious->m_pOrigin;
		v1_tmp = aux;
	}

	TrackFull.Append(TrackParcialFinal);


	return TrackFull;
}
