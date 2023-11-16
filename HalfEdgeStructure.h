#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>

namespace HalfEdgeStructure {

	class Solid;
	class Face;
	class Loop;
	class HalfEdge;
	class Edge;
	class Vertex;
	class Point;

	class Solid {
	public:
		Solid();

		Solid* prev();
		Solid* next();
		Face* face();
		Edge* edge();

		void set_prev(Solid* solid);
		void set_next(Solid* solid);
		void set_face(Face* face);
		void set_edge(Edge* edge);

	private:
		Solid* prevSolid;
		Solid* nextSolid;
		Face* firstFace;
		Edge* firstEdge;
	};

	class Face {
	public:
		Face();

		Face* prev();
		Face* next();
		Solid* solid();
		Loop* loop();

		void set_prev(Face* face);
		void set_next(Face* face);
		void set_solid(Solid* solid);
		void set_loop(Loop* loop);

		bool hasInnerLoop();

	private:
		Face* prevFace;
		Face* nextFace;
		Solid* fromSolid;
		Loop* firstLoop;
	};

	class Loop {
	public:
		Loop();

		Loop* prev();
		Loop* next();
		Face* face();
		HalfEdge* half_edge();

		void set_prev(Loop* loop);
		void set_next(Loop* loop);
		void set_face(Face* face);
		void set_half_edge(HalfEdge* half_edge);

		bool check_inner_loop();
		void set_is_inner_loop(bool flag);

	private:
		bool isInnerLoop;
		Loop* prevLoop;
		Loop* nextLoop;
		Face* fromFace;
		HalfEdge* firstHalfEdge;
	};

	class HalfEdge {
	public:
		HalfEdge();

		HalfEdge* prev();
		HalfEdge* next();
		HalfEdge* partner();
		Loop* loop();
		Edge* edge();
		Vertex* start();
		Vertex* end();

		void set_prev(HalfEdge* half_edge);
		void set_next(HalfEdge* half_edge);
		void set_partner(HalfEdge* partner);
		void set_loop(Loop* loop);
		void set_edge(Edge* edge);
		void set_start(Vertex* start_vertex);
		void set_end(Vertex* end_vertex);

	private:
		HalfEdge* prevHE;
		HalfEdge* nextHE;
		HalfEdge* partnerHE;
		Loop* fromLoop;
		Edge* firstEdge;
		Vertex* startVertex;
		Vertex* endVertex;
	};

	class Edge {
	public:
		Edge();

		Edge* prev();
		Edge* next();
		HalfEdge* first_half_edge();
		HalfEdge* second_half_edge();

		void set_prev(Edge* edge);
		void set_next(Edge* edge);
		void set_first_half_edge(HalfEdge* first_half_edge);
		void set_second_half_edge(HalfEdge* second_half_edge);

	private:
		Edge* prevEdge;
		Edge* nextEdge;
		HalfEdge* firstHE;
		HalfEdge* secondHE;
	};

	class Vertex {
	public:
		Vertex();

		Vertex* prev();
		Vertex* next();
		Point* point();

		void set_prev(Vertex* v);
		void set_next(Vertex* v);
		void set_point(Point* p);

	private:
		Vertex* prevVertex;
		Vertex* nextVertex;
		Point* p;
	};

	// º∏∫Œ¿‡
	class Point {
	public:
		Point();
		Point(float x, float y, float z);

		glm::vec3 get_point() const;
		void set_point(float x, float y, float z);
		void set_point(glm::vec3 new_point);
		void set_point(const Point& p);

	private:
		glm::vec3 point;
	};
} // namespace HalfEdgeStructure