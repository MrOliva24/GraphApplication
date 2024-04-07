#include "pch.h"
#include "Graph.h"
#include "GraphApplicationDlg.h"
#include <set>

// =============================================================================
// CONVEX HULL =================================================================
// =============================================================================

// QuickHull ===================================================================

double PosicioRespeteRecta(CGPoint& a, CGPoint& b, CGPoint& c) {
	return (a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y);
}

double AreaTriangle(CGPoint& a, CGPoint& b, CGPoint c) {
	return abs((a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y)) / 2.0;
}

list<CVertex*> QuickHull(CVertex* a, CVertex* b, list<CVertex*> zone) {

	if (zone.empty()) {
		if (a->m_Point == b->m_Point) return { a };
		return { a, b };
	}

	CVertex* c = zone.front();
	double areaC = 0;
	double area;
	for (CVertex* vertex : zone) {
		area = AreaTriangle(a->m_Point, b->m_Point, vertex->m_Point);
		if (area > areaC) {
			c = vertex;
			areaC = area;
		}
	}
	list<CVertex*> poly_l, poly_r;

	for (CVertex* vertex : zone) {
		if (PosicioRespeteRecta(a->m_Point, c->m_Point, vertex->m_Point) > 0) poly_l.push_back(vertex);
		else if (PosicioRespeteRecta(c->m_Point, b->m_Point, vertex->m_Point) > 0) poly_r.push_back(vertex);
	}

	list<CVertex*> left = QuickHull(a, c, poly_l), right = QuickHull(c, b, poly_r);

	right.pop_front();
	left.splice(left.end(), right);

	return left;
}

CConvexHull QuickHull(CGraph& g) {
	CConvexHull poly(&g);

	if (g.m_Vertices.empty()) {
		CConvexHull emptyHull(&g);
		return emptyHull;
	}

	if (g.GetNVertices() < 3) {
		poly.m_Vertices.push_back(&g.m_Vertices.front());
		if (g.m_Vertices.back().m_Point != g.m_Vertices.front().m_Point) { poly.m_Vertices.push_back(&g.m_Vertices.back()); }
		return poly;
	}

	CVertex* a = &g.m_Vertices.front(), * b = &g.m_Vertices.front();

	for (CVertex& vertex : g.m_Vertices) {
		if (vertex.m_Point.m_X <= a->m_Point.m_X) a = &vertex;
		else if (vertex.m_Point.m_X >= b->m_Point.m_X) b = &vertex;
	}

	list<CVertex*> up_g, down_g;

	for (CVertex& vertex : g.m_Vertices) {
		if (PosicioRespeteRecta(a->m_Point, b->m_Point, vertex.m_Point) > 0) up_g.push_back(&vertex);
		else down_g.push_back(&vertex);
	}
	poly.m_Vertices = QuickHull(a, b, up_g);
	list<CVertex*> down = QuickHull(b, a, down_g);

	poly.m_Vertices.pop_back();
	down.pop_back();
	poly.m_Vertices.splice(poly.m_Vertices.end(), down);
	return poly;
}