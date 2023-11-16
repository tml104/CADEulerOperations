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
		mvfs: 定义一个体、一个面、一个外环、一个点
	*/
	Solid* mvfs(Point p, Vertex*& v);

	/*
	*	mev:定义一个新点，同时定义一条连接新点与另一给定点的边
	*/
	HalfEdge* mev(Vertex* v, Point p, Loop* lp);

	/*
		mef:以两给定点为端点定义一条新的边，同时定义一个新的面，新的环。
	*/
	Loop* mef(Vertex* v1, Vertex* v2, Loop* lp);

	/*
		kemr: 消去环中的一条边，定义一个内环。
	*/
	Loop* kemr(Vertex* v1, Vertex* v2, Loop* lp);

	/*
		kfmrh:删除一个面，并将其定义为另一个面的内环，进而在体中生成一个通孔或将两物体合并成一个物体
	*/
	void kfmrh(Loop* outer_loop, Loop* inner_loop);

	/*
	*	sweep: 扫掠操作
	*/
	Solid* sweep(Face* face, glm::vec3 dir, float dis);

}//namespace EulerOperation