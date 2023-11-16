#include "EulerOperation.h"

HalfEdgeStructure::Solid* EulerOperation::mvfs(Point p, Vertex*& v)
{
	Solid* solid = new Solid();
	Face* face = new Face();
	Loop* loop = new Loop();
	v = new Vertex();

	// ���õ������
	v->set_point(new Point(p));

	// ���ù�ϵ
	solid->set_face(face);

	face->set_solid(solid);
	face->set_loop(loop);

	loop->set_face(face);

	return solid;
}

HalfEdgeStructure::HalfEdge* EulerOperation::mev(Vertex* v, Point p, Loop* lp)
{
	Edge* edge = new Edge();
	HalfEdge* half_edge1 = new HalfEdge();
	HalfEdge* half_edge2 = new HalfEdge();
	Vertex* v2 = new Vertex();

	// 1. �����µ�v2
	v2->set_point(new Point(p));

	// 2.1 ���ù�ϵ
	half_edge1->set_loop(lp);
	half_edge2->set_loop(lp);
	half_edge1->set_edge(edge);
	half_edge2->set_edge(edge);
	edge->set_first_half_edge(half_edge1);
	edge->set_second_half_edge(half_edge2);

	half_edge1->set_partner(half_edge2);
	half_edge2->set_partner(half_edge1);

	half_edge1->set_start(v);
	half_edge1->set_end(v2);
	half_edge2->set_start(v2);
	half_edge2->set_end(v);

	// 2.2 ����loop
	if (lp->half_edge() == nullptr) {
		lp->set_half_edge(half_edge1);

		half_edge1->set_prev(half_edge2);
		half_edge1->set_next(half_edge2);

		half_edge2->set_prev(half_edge1);
		half_edge2->set_next(half_edge1);
	}
	else {
		HalfEdge* temp_loop_half_edge = lp->half_edge();

		while (temp_loop_half_edge->start() != v) {
			temp_loop_half_edge = temp_loop_half_edge->next();
		}

		HalfEdge* temp_loop_half_edge_prev = temp_loop_half_edge->prev();

		temp_loop_half_edge_prev->set_next(half_edge1);

		half_edge1->set_prev(temp_loop_half_edge_prev);
		half_edge1->set_next(half_edge2);

		half_edge2->set_prev(half_edge1);
		half_edge2->set_next(temp_loop_half_edge);

		temp_loop_half_edge->set_prev(half_edge2);
	}

	// 2.3 ����solid�е�edge
	if (lp->face() != nullptr && lp->face()->solid() != nullptr) {
		Solid* solid = lp->face()->solid();

		if (solid->edge()==nullptr) {
			solid->set_edge(edge);
		}
		else {
			// ����Ҹ��ط�����edge
			Edge* temp_solid_edge = solid->edge();
			Edge* temp_solid_edge_next = temp_solid_edge->next();

			temp_solid_edge->set_next(edge);
			temp_solid_edge_next->set_prev(edge);
			edge->set_prev(temp_solid_edge);
			edge->set_next(temp_solid_edge_next);
		}
	}

	return half_edge1;
}

HalfEdgeStructure::Loop* EulerOperation::mef(Vertex* v1, Vertex* v2, Loop* lp)
{
	HalfEdge* half_edge1 = new HalfEdge();
	HalfEdge* half_edge2 = new HalfEdge();
	Edge* edge = new Edge();

	// 1. ���ù�ϵ
	edge->set_first_half_edge(half_edge1);
	edge->set_second_half_edge(half_edge2);
	half_edge1->set_edge(edge);
	half_edge2->set_edge(edge);

	half_edge1->set_start(v1);
	half_edge1->set_end(v2);
	half_edge2->set_start(v2);
	half_edge2->set_end(v1);

	half_edge1->set_partner(half_edge2);
	half_edge2->set_partner(half_edge1);

	// 2. �ҵ��ؼ���
	HalfEdge* temp_half_edge_v1 = nullptr;
	HalfEdge* temp_half_edge_v2 = nullptr;
	HalfEdge* i_half_edge = lp->half_edge();
	do {
		if (i_half_edge->start() == v1) {
			temp_half_edge_v1 = i_half_edge;
		}
		if (i_half_edge->start() == v2) {
			temp_half_edge_v2 = i_half_edge;
		}

		i_half_edge = i_half_edge->next();
	} while (i_half_edge != lp->half_edge());

	// [debug]
	if (temp_half_edge_v1 == nullptr) {
		printf("ERROR: temp_half_edge_v1 == nullptr\n");

		delete half_edge1;
		delete half_edge2;
		delete edge;

		return nullptr;
	}

	if (temp_half_edge_v2 == nullptr) {
		printf("ERROR: temp_half_edge_v2 == nullptr\n");

		delete half_edge1;
		delete half_edge2;
		delete edge;

		return nullptr;
	} 
	// [debug] end

	// 3. �޸Ĺ�ϵ��Ϊ�˱�֤������������������Ҫ�޸ĵĶ�����ǰ�ó�����
	HalfEdge* temp_half_edge_v1_prev = temp_half_edge_v1->prev();
	HalfEdge* temp_half_edge_v2_prev = temp_half_edge_v2->prev();

	half_edge1->set_prev(temp_half_edge_v1_prev);
	half_edge1->set_next(temp_half_edge_v2);
	half_edge2->set_prev(temp_half_edge_v2_prev);
	half_edge2->set_next(temp_half_edge_v1);

	temp_half_edge_v1->set_prev(half_edge2);
	temp_half_edge_v1_prev->set_next(half_edge1);

	temp_half_edge_v2->set_prev(half_edge1);
	temp_half_edge_v2_prev->set_next(half_edge2);

	// 4. ����solid�е�edge
	Solid* solid = lp->face()->solid();

	if (solid->edge() == nullptr) {
		solid->set_edge(edge);
	}
	else {
		// ����Ҹ��ط�����edge
		Edge* temp_solid_edge = solid->edge();
		Edge* temp_solid_edge_next = temp_solid_edge->next();

		temp_solid_edge->set_next(edge);
		temp_solid_edge_next->set_prev(edge);
		edge->set_prev(temp_solid_edge);
		edge->set_next(temp_solid_edge_next);
	}


	// 5. ������loop����face������loop����ͬһ�����ϣ�
	Loop* new_loop = new Loop();
	Face* new_face = new Face();

	lp->set_half_edge(half_edge1);
	new_loop->set_half_edge(half_edge2);
	new_loop->set_face(new_face);

	new_face->set_loop(new_loop);
	new_face->set_solid(solid);

	// 5.1 ������face��solid��
	Face* temp_solid_face = solid->face();
	if (temp_solid_face == nullptr) {
		solid->set_face(new_face);
	}
	else {
		Face* temp_solid_face_next = temp_solid_face->next();

		temp_solid_face->set_next(new_face);
		temp_solid_face_next->set_prev(new_face);
		new_face->set_prev(temp_solid_face);
		new_face->set_next(temp_solid_face_next);
	}

	return new_loop;
}

// (��ɾ���Ĳ���)
HalfEdgeStructure::Loop* EulerOperation::kemr(Vertex* v1, Vertex* v2, Loop* lp)
{
	HalfEdge* half_edge_v1 = nullptr;
	HalfEdge* i_half_edge = lp->half_edge();
	do {

		if (i_half_edge->start()==v1 && i_half_edge->end()==v2) {
			half_edge_v1 = i_half_edge;
			break;
		}

		i_half_edge = i_half_edge->next();
	} while (i_half_edge != lp->half_edge());

	// [debug]
	if (half_edge_v1 == nullptr) {
		printf("ERROR: half_edge_v1 == nullptr\n");

		return nullptr;
	}
	// [debug] end

	HalfEdge* half_edge_v2 = half_edge_v1->partner();
	Edge* edge = half_edge_v1->edge();


	// 1. �ı��ϵ
	HalfEdge* half_edge_v1_prev = half_edge_v1->prev();
	HalfEdge* half_edge_v1_next = half_edge_v1->next();
	HalfEdge* half_edge_v2_prev = half_edge_v2->prev();
	HalfEdge* half_edge_v2_next = half_edge_v2->next();

	half_edge_v1_prev->set_next(half_edge_v2_next);
	half_edge_v1_next->set_prev(half_edge_v2_prev);
	half_edge_v2_prev->set_next(half_edge_v1_next);
	half_edge_v2_next->set_prev(half_edge_v1_prev);

	// 2. �����»�
	Loop* new_loop = new Loop();

	lp->set_half_edge(half_edge_v1_prev);
	new_loop->set_half_edge(half_edge_v2_prev);

	// 3. �޸�����

	Face* face = lp->face();
	Solid* solid = face->solid();

	new_loop->set_face(face);

	if (face->loop() == nullptr) {
		face->set_loop(new_loop);
	}
	else {
		// ������һ����loop��face��
		Loop* temp_face_loop = face->loop();
		Loop* temp_face_loop_next = temp_face_loop->next();

		new_loop->set_prev(temp_face_loop);
		new_loop->set_next(temp_face_loop_next);
		temp_face_loop->set_next(new_loop);
		temp_face_loop_next->set_prev(new_loop);
	}

	// ���������Ƴ�edge
	Edge* edge_prev = edge->prev();
	Edge* edge_next = edge->next();

	if (solid->edge()==edge) {
		if (edge_next != edge) {
			solid->set_edge(edge_next);
		}
		else if (edge_prev != edge) {
			solid->set_edge(edge_prev);
		}
		else {
			solid->set_edge(nullptr);
		}
	}

	edge_next->set_prev(edge_prev);
	edge_prev->set_next(edge_next);

	// ɾ��ָ��
	delete edge;
	delete half_edge_v1;
	delete half_edge_v2;

	return new_loop;
}

// (��ɾ���Ĳ���)
void EulerOperation::kfmrh(Loop* outer_loop, Loop* inner_loop)
{
	Face* outer_face = outer_loop->face();
	Face* inner_face = inner_loop->face();

	// 1. �޸����滷������
	inner_loop->set_face(outer_face);

	if (outer_face->loop()==nullptr) {
		outer_face->set_loop(inner_loop);
	}
	else {
		// ������һ����loop��face��
		Loop* temp_face_loop = outer_face->loop();
		Loop* temp_face_loop_next = temp_face_loop->next();

		inner_loop->set_prev(temp_face_loop);
		inner_loop->set_next(temp_face_loop_next);
		temp_face_loop->set_next(inner_loop);
		temp_face_loop_next->set_prev(inner_loop);
	}

	// 2. ɾ��inner_face
	Face* inner_face_prev = inner_face->prev();
	Face* inner_face_next = inner_face->next();
	Solid* solid = inner_face->solid();

	if (solid->face() == inner_face) {
		if (inner_face_prev != inner_face) {
			solid->set_face(inner_face_prev);
		}
		else if (inner_face_next != inner_face) {
			solid->set_face(inner_face_next);
		}
		else {
			solid->set_face(nullptr);
		}
	}

	inner_face_prev->set_next(inner_face_next);
	inner_face_next->set_prev(inner_face_prev);

	delete inner_face;
}

HalfEdgeStructure::Solid* EulerOperation::sweep(Face* face, glm::vec3 dir, float dis)
{
	Solid* solid = face->solid();

	Loop* iloop = face->loop();
	do {
		HalfEdge* half_edge = iloop->half_edge();
		Vertex* v_start = half_edge->start();
		// ɨ�Ӻ�ĵ�
		Point sweep_point(
			v_start->point()->get_point().x + dir.x * dis,
			v_start->point()->get_point().y + dir.y * dis,
			v_start->point()->get_point().z + dir.z * dis
		);

		HalfEdge* new_half_edge = mev(v_start, sweep_point, iloop);
		Vertex* sweep_v = new_half_edge->end();

		Vertex* sweep_v_last = sweep_v;

		half_edge = half_edge->next();
		while (half_edge->start() != v_start) {
			Vertex* v_start_now = half_edge->start();
			// ɨ�Ӻ�ĵ�
			Point sweep_point_now(
				v_start_now->point()->get_point().x + dir.x * dis,
				v_start_now->point()->get_point().y + dir.y * dis,
				v_start_now->point()->get_point().z + dir.z * dis
			);
			HalfEdge* new_half_edge_now = mev(v_start_now, sweep_point_now, iloop);
			Vertex* sweep_v_now = new_half_edge_now->end();
			mef(sweep_v_last, sweep_v_now, iloop);

			// update
			sweep_v_last = sweep_v_now;

			half_edge = half_edge->next();
		}

		mef(sweep_v_last, sweep_v, iloop);

		iloop = iloop->next();
	} while (iloop != face->loop());

	return solid;
}
