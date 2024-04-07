#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 
#include <random>
#include <chrono>
#include <vector>
#include "Matrix.h"

// SalesmanTrackProbabilistic ==================================================
struct path {
	CTrack track;
	double distance;
};

CTrack SalesmanTrackProbabilistic(CGraph& graph, CVisits& visits)
{
	int nVisits = visits.GetNVertices();
	Matrix<path> Matrix(1000, 1000);
	CTrack track(&graph);
	double totalDistance = numeric_limits<double>::max();

	int i = 0, j;
	for (CVertex* v1 : visits.m_Vertices) {
		Dijkstra(graph, v1);
		j = 0;
		for (CVertex* v2 : visits.m_Vertices) {
			Matrix(i, j).distance = v2->m_DijkstraDistance;

			while (v2->m_pDijkstraPrevious) {
				Matrix(i, j).track.m_Edges.push_front(v2->m_pDijkstraPrevious);
				v2 = v2->m_pDijkstraPrevious->m_pOrigin;
			}
			j++;
		}
		i++;
	}

	for (int i = 0; i < 1500 * nVisits; i++) {
		vector<int> index;
		index.push_back(0);


		vector<int> solucio;
		int size = nVisits - 2; //Numero total de visitas excluyendo la primera y la ultima
		for (int j = 0; j < size; j++) {
			solucio.push_back(j + 1);
		}

		for (int k = 0; k < nVisits; k++) {
			int index1 = rand() % (size);
			int index2 = rand() % (size);
			int aux = solucio[index1];
			solucio[index1] = solucio[index2];
			solucio[index2] = aux;
		}
		for (int idx = 0; idx < size; idx++) {
			index.push_back(solucio[i]);
		}
		index.push_back(visits.m_Vertices.size() - 1);

		double dist_local = numeric_limits<double>::max();
		double final_distance = 0;
		for (int caca = 0; caca < index.size(); caca++) {
			final_distance += Matrix(index[caca], index[caca + 1]).distance;
		}
		double dist_auxiliar;
		bool comprobar = true;
		while (comprobar) {
			//dist_auxiliar = dist_local;
			double dist_local = final_distance;
			vector<pair<int, int>> childs;
			for (int row = 1; row < index.size() - 1; row++) {
				for (int col = 1; col < index.size() - 1; col++) {
					if (row != col) {
						vector<int> child = index;
						int aux = child[row];
						child[row] = child[col];
						child[col] = aux;

						dist_auxiliar = 0;
						for (int pos = 0; pos < child.size() - 1; pos++) {
							dist_auxiliar += Matrix(child[pos], child[pos + 1]).distance;
						}
						if (dist_auxiliar <= dist_local) {
							if (dist_auxiliar < dist_local) {
								dist_local = dist_auxiliar;
								childs.clear();
							}
							childs.push_back(make_pair(row, col));
						}

					}
				}
			}
			if (dist_local < final_distance) {
				int ayuda = rand() % (childs.size());
				pair<int, int> best = childs[ayuda];

				int sexo = index[best.first];
				index[best.first] = index[best.second];
				index[best.second] = sexo;
				final_distance = dist_local;
			}
			else {
				comprobar = false;
			}

		}


		if (final_distance < totalDistance) {

			track.m_Edges.clear();
			totalDistance = 0;
			for (int ver = 0; ver < index.size() - 1; ver++) {
				track.Append(Matrix(index[ver], index[ver + 1]).track);
				totalDistance += Matrix(index[ver], index[ver + 1]).distance;
			}
		}

	}

	return track;
}
