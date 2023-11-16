#include "HalfEdgeStructure.h"

HalfEdgeStructure::Point::Point() : point(0.0, 0.0, 0.0)
{
}

HalfEdgeStructure::Point::Point(float x, float y, float z): point(x,y,z)
{
}

glm::vec3 HalfEdgeStructure::Point::get_point() const
{
	return this->point;
}

void HalfEdgeStructure::Point::set_point(float x, float y, float z)
{
	this->point = glm::vec3(x, y, z);
}

void HalfEdgeStructure::Point::set_point(glm::vec3 new_point)
{
	this->point = new_point;
}

void HalfEdgeStructure::Point::set_point(const Point& p)
{
	this->point = p.get_point();
}

HalfEdgeStructure::Edge::Edge(): prevEdge(this), nextEdge(this), firstHE(nullptr), secondHE(nullptr)
{
}

HalfEdgeStructure::Edge* HalfEdgeStructure::Edge::prev()
{
	return this->prevEdge;
}

HalfEdgeStructure::Edge* HalfEdgeStructure::Edge::next()
{
	return this->nextEdge;
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::Edge::first_half_edge()
{
	return this->firstHE;
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::Edge::second_half_edge()
{
	return this->secondHE;
}

void HalfEdgeStructure::Edge::set_prev(Edge* edge)
{
	this->prevEdge = edge;
}

void HalfEdgeStructure::Edge::set_next(Edge* edge)
{
	this->nextEdge = edge;
}

void HalfEdgeStructure::Edge::set_first_half_edge(HalfEdge* first_half_edge)
{
	this->firstHE = first_half_edge;
}

void HalfEdgeStructure::Edge::set_second_half_edge(HalfEdge* second_half_edge)
{
	this->secondHE = second_half_edge;
}

HalfEdgeStructure::HalfEdge::HalfEdge(): prevHE(this), nextHE(this), partnerHE(nullptr), fromLoop(nullptr), firstEdge(nullptr), startVertex(nullptr), endVertex(nullptr)
{
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::HalfEdge::prev()
{
	return this->prevHE;
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::HalfEdge::next()
{
	return this->nextHE;
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::HalfEdge::partner()
{
	return this->partnerHE;
}

HalfEdgeStructure::Loop* HalfEdgeStructure::HalfEdge::loop()
{
	return this->fromLoop;
}

HalfEdgeStructure::Edge* HalfEdgeStructure::HalfEdge::edge()
{
	return this->firstEdge;
}

HalfEdgeStructure::Vertex* HalfEdgeStructure::HalfEdge::start()
{
	return this->startVertex;
}

HalfEdgeStructure::Vertex* HalfEdgeStructure::HalfEdge::end()
{
	return this->endVertex;
}

void HalfEdgeStructure::HalfEdge::set_prev(HalfEdge* half_edge)
{
	this->prevHE = half_edge;
}

void HalfEdgeStructure::HalfEdge::set_next(HalfEdge* half_edge)
{
	this->nextHE = half_edge;
}

void HalfEdgeStructure::HalfEdge::set_partner(HalfEdge* half_edge)
{
	this->partnerHE = half_edge;
}

void HalfEdgeStructure::HalfEdge::set_loop(Loop* loop)
{
	this->fromLoop = loop;
}

void HalfEdgeStructure::HalfEdge::set_edge(Edge* edge)
{
	this->firstEdge = edge;
}

void HalfEdgeStructure::HalfEdge::set_start(Vertex* start_vertex)
{
	this->startVertex = start_vertex;
}

void HalfEdgeStructure::HalfEdge::set_end(Vertex* end_vertex)
{
	this->endVertex = end_vertex;
}

HalfEdgeStructure::Loop::Loop(): isInnerLoop(false), prevLoop(this), nextLoop(this), fromFace(nullptr), firstHalfEdge(nullptr)
{
}

HalfEdgeStructure::Loop* HalfEdgeStructure::Loop::prev()
{
	return this->prevLoop;
}

HalfEdgeStructure::Loop* HalfEdgeStructure::Loop::next()
{
	return this->nextLoop;
}

HalfEdgeStructure::Face* HalfEdgeStructure::Loop::face()
{
	return this->fromFace;
}

HalfEdgeStructure::HalfEdge* HalfEdgeStructure::Loop::half_edge()
{
	return this->firstHalfEdge;
}

void HalfEdgeStructure::Loop::set_prev(Loop* loop)
{
	this->prevLoop = loop;
}

void HalfEdgeStructure::Loop::set_next(Loop* loop)
{
	this->nextLoop = loop;
}

void HalfEdgeStructure::Loop::set_face(Face* face)
{
	this->fromFace = face;
}

void HalfEdgeStructure::Loop::set_half_edge(HalfEdge* half_edge)
{
	this->firstHalfEdge = half_edge;
}

bool HalfEdgeStructure::Loop::check_inner_loop()
{
	return this->isInnerLoop;
}

void HalfEdgeStructure::Loop::set_is_inner_loop(bool flag)
{
	this->isInnerLoop = flag;
}

HalfEdgeStructure::Face::Face(): prevFace(this), nextFace(this), fromSolid(nullptr), firstLoop(nullptr)
{
}

HalfEdgeStructure::Face* HalfEdgeStructure::Face::prev()
{
	return this->prevFace;
}

HalfEdgeStructure::Face* HalfEdgeStructure::Face::next()
{
	return this->nextFace;
}

HalfEdgeStructure::Solid* HalfEdgeStructure::Face::solid()
{
	return this->fromSolid;
}

HalfEdgeStructure::Loop* HalfEdgeStructure::Face::loop()
{
	return this->firstLoop;
}

void HalfEdgeStructure::Face::set_prev(Face* face)
{
	this->prevFace = face;
}

void HalfEdgeStructure::Face::set_next(Face* face)
{
	this->nextFace = face;
}

void HalfEdgeStructure::Face::set_solid(Solid* solid)
{
	this->fromSolid = solid;
}

void HalfEdgeStructure::Face::set_loop(Loop* loop)
{
	this->firstLoop = loop;
}

/*
	检查是否有“内环”
*/
bool HalfEdgeStructure::Face::hasInnerLoop()
{
	Loop* first_loop = this->loop();
	Loop* iloop = first_loop;

	if (iloop == nullptr) return false;

	bool flag = false;

	do {

		if (iloop->check_inner_loop()) {
			flag = true;
			break;
		}

		iloop = iloop->next();
	}
	while (iloop != first_loop && iloop!=nullptr);

	return flag;
}

HalfEdgeStructure::Solid::Solid(): prevSolid(this), nextSolid(this), firstFace(nullptr), firstEdge(nullptr)
{
}

HalfEdgeStructure::Solid* HalfEdgeStructure::Solid::prev()
{
	return this->prevSolid;
}

HalfEdgeStructure::Solid* HalfEdgeStructure::Solid::next()
{
	return this->nextSolid;
}

HalfEdgeStructure::Face* HalfEdgeStructure::Solid::face()
{
	return this->firstFace;
}

HalfEdgeStructure::Edge* HalfEdgeStructure::Solid::edge()
{
	return this->firstEdge;
}

void HalfEdgeStructure::Solid::set_prev(Solid* solid)
{
	this->prevSolid = solid;
}

void HalfEdgeStructure::Solid::set_next(Solid* solid)
{
	this->nextSolid = solid;
}

void HalfEdgeStructure::Solid::set_face(Face* face)
{
	this->firstFace = face;
}

void HalfEdgeStructure::Solid::set_edge(Edge* edge)
{
	this->firstEdge = edge;
}

HalfEdgeStructure::Vertex::Vertex(): prevVertex(this), nextVertex(this), p(nullptr)
{
}

HalfEdgeStructure::Vertex* HalfEdgeStructure::Vertex::prev()
{
	return this->prevVertex;
}

HalfEdgeStructure::Vertex* HalfEdgeStructure::Vertex::next()
{
	return this->nextVertex;
}

HalfEdgeStructure::Point* HalfEdgeStructure::Vertex::point()
{
	return this->p;
}

void HalfEdgeStructure::Vertex::set_prev(Vertex* v)
{
	this->prevVertex = v;
}

void HalfEdgeStructure::Vertex::set_next(Vertex* v)
{
	this->nextVertex = v;
}

void HalfEdgeStructure::Vertex::set_point(Point* p)
{
	this->p = p;
}