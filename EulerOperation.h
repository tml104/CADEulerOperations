#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "HalfEdgeStructure.h"


namespace EulerOperation {
	using namespace HalfEdgeStructure;
	
	/*
		mvfs: ����һ���塢һ���桢һ���⻷��һ����
	*/
	Solid* mvfs(Point p, Vertex*& v);

	/*
	*	mev:����һ���µ㣬ͬʱ����һ�������µ�����һ������ı�
	*/
	HalfEdge* mev(Vertex* v, Point p, Loop* lp);

	/*
		mef:����������Ϊ�˵㶨��һ���µıߣ�ͬʱ����һ���µ��棬�µĻ���
	*/
	Loop* mef(Vertex* v1, Vertex* v2, Loop* lp);

	/*
		kemr: ��ȥ���е�һ���ߣ�����һ���ڻ���
	*/
	Loop* kemr(Vertex* v1, Vertex* v2, Loop* lp);

	/*
		kfmrh:ɾ��һ���棬�����䶨��Ϊ��һ������ڻ�����������������һ��ͨ�׻�������ϲ���һ������
	*/
	void kfmrh(Loop* outer_loop, Loop* inner_loop);

	/*
	*	sweep: ɨ�Ӳ���
	*/
	Solid* sweep(Face* face, glm::vec3 dir, float dis);

}//namespace EulerOperation