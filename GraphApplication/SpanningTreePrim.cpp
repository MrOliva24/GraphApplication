#include "pch.h"
#include "Graph.h"
#include <vector>
#include <queue>
using namespace std;

// =============================================================================
// SpanningTreePrim ============================================================
// =============================================================================


CSpanningTree SpanningTreePrim(CGraph & graph)
{
	CSpanningTree spanningTree(&graph);

	//Comprovació de si el graph és buit
	if (graph.m_Edges.empty()) {
		return spanningTree; 
	}

	struct comparator {
			bool operator()(CEdge* pE1, CEdge* pE2) {
				return pE1->m_Length > pE2->m_Length;
			}
		};

	for (CVertex& node : graph.m_Vertices) node.m_PrimInTree = false;


	//Agafem el primer node i l'afegim al tree
	
	graph.m_Vertices.front().m_PrimInTree = true;
	priority_queue<CEdge*, std::vector<CEdge*>, comparator> queue;


	for (CEdge* edgeActual : graph.m_Vertices.front().m_Edges) {
		queue.push(edgeActual);
	}

	while (!queue.empty()) {
		CEdge* edge_Min_Length = queue.top(); 
		queue.pop();
		if (edge_Min_Length->m_pDestination->m_PrimInTree == false) {
			spanningTree.m_Edges.push_back(edge_Min_Length);
			edge_Min_Length->m_pDestination->m_PrimInTree = true;
			for (CEdge* edgeActual : edge_Min_Length->m_pDestination->m_Edges) {
				if(edgeActual->m_pDestination->m_PrimInTree == false) queue.push(edgeActual);
			}
		}
	}
	return spanningTree;
}
