#include "Construct.h"


std::vector<glm::vec3> Construct::convert(Solid* solid)
{

	std::vector<glm::vec3> temp_vec;
	std::vector<glm::vec3> res_vec;
	Face* iface = solid->face();

	

	do {
		// One inner loop
		if (iface->hasInnerLoop()) {

			Loop* inner_loop = nullptr;
			Loop* outer_loop = nullptr;
			Loop* iloop = iface->loop();

			do {
				if (iloop->check_inner_loop()) {
					inner_loop = iloop;
				}
				else {
					outer_loop = iloop;
				}

				iloop = iloop->next();
			} while (iloop != iface->loop());

			// [debug]
			printf("[DEBUG] iface->hasInnerLoop() == true\n");

			if (inner_loop == nullptr) {
				printf("ERROR in convert: inner_loop == nullptr\n");
				return std::vector<glm::vec3>();
			}

			if (outer_loop == nullptr) {
				printf("ERROR in convert: outer_loop == nullptr\n");
				return std::vector<glm::vec3>();
			}
			// [debug] end

			// Solve this case
			temp_vec.clear();

			HalfEdge* outer_half_edge = outer_loop->half_edge();
			do {
				HalfEdge* inner_half_edge = inner_loop->half_edge();

				temp_vec.emplace_back(outer_half_edge->start()->point()->get_point());

				glm::vec3 now_point(9999.0, 9999.0, 9999.0);

				do {
					glm::vec3 temp_point = inner_half_edge->start()->point()->get_point();

					if (glm::distance(now_point, temp_vec.back()) > glm::distance(temp_point, temp_vec.back())) {
						now_point = temp_point;
					}

					inner_half_edge = inner_half_edge->next();
				} while (inner_half_edge != inner_loop->half_edge());

				// [debug]
					
				if (glm::distance(now_point, temp_vec.back()) > 17000.0) {
					printf("ERROR in convert: glm::distance(now_point, temp_vec.back()) > 17000.0\n");
					return std::vector<glm::vec3>();
				}
				// [debug] end

				temp_vec.emplace_back(now_point);

				outer_half_edge = outer_half_edge->next();
			} while (outer_half_edge != outer_loop->half_edge());

			unsigned int temp_vec_size = temp_vec.size();
			for (unsigned int i = 0;i< temp_vec_size; i += 2) {
				res_vec.emplace_back(temp_vec[i]);
				res_vec.emplace_back(temp_vec[(i+2)%temp_vec_size]);
				res_vec.emplace_back(temp_vec[(i+1)%temp_vec_size]);

				res_vec.emplace_back(temp_vec[(i + 1) % temp_vec_size]);
				res_vec.emplace_back(temp_vec[(i + 2) % temp_vec_size]);
				res_vec.emplace_back(temp_vec[(i + 3) % temp_vec_size]);
			}


			// next face
			iface = iface->next();
			continue;
		}

		// Retangle
		Loop* iloop = iface->loop();
		do {
			temp_vec.clear();
			HalfEdge* half_edge = iloop->half_edge();
			do {

				temp_vec.emplace_back(half_edge->start()->point()->get_point());


				half_edge = half_edge->next();
			} while (half_edge != iloop->half_edge());


			res_vec.emplace_back(temp_vec[0]);
			res_vec.emplace_back(temp_vec[1]);
			res_vec.emplace_back(temp_vec[2]);

			res_vec.emplace_back(temp_vec[0]);
			res_vec.emplace_back(temp_vec[2]);
			res_vec.emplace_back(temp_vec[3]);


			iloop = iloop->next();
		} while (iloop != iface->loop());
		iface = iface->next();
	} while (iface != solid->face());

	return res_vec;

}

std::vector<glm::vec3> Construct::test1()
{
	Point p1(1.0, 1.0, 1.0);
	Point p2(1.0, 0.0, 1.0);
	Point p3(0.0, 1.0, 1.0);
	Point p4(0.0, 0.0, 1.0);

	Vertex* v1 = nullptr;
	Solid* solid = mvfs(p1, v1);
	Loop* loop = solid->face()->loop();
	HalfEdge* half_edge2 = mev(v1, p2, loop);
	HalfEdge* half_edge3 = mev(v1, p3, loop);
	HalfEdge* half_edge4 = mev(half_edge3->end(), p4, loop);
	mef(half_edge2->end(), half_edge4->end(), loop);

	solid = sweep(solid->face(), glm::vec3(1.0, 1.0, 1.0), 0.3);

	return convert(solid);
}

std::vector<glm::vec3> Construct::test2()
{
	printf("[DEBUG] test2 \n");


	Point p1(-1.0, 1.0, 1.0);
	Point p2(-1.0, 1.0, -1.0);
	Point p3(1.0, 1.0, 1.0);
	Point p4(1.0, 1.0, -1.0);

	Point p11(-1.0, -1.0, 1.0);
	Point p12(-1.0, -1.0, -1.0);
	Point p13(1.0, -1.0, 1.0);
	Point p14(1.0, -1.0, -1.0);

	Point p21(-0.5, 1.0, 0.5); //内缩点
	Point p22(-0.5, 1.0, -0.5); 
	Point p23(0.5, 1.0, 0.5); 
	Point p24(0.5, 1.0, -0.5); 

	Point p31(-0.5, -1.0, 0.5); //内缩点
	Point p32(-0.5, -1.0, -0.5);
	Point p33(0.5, -1.0, 0.5);
	Point p34(0.5, -1.0, -0.5);

	// 上
	Vertex* v1;
	Solid* solid = mvfs(p1, v1);
	Loop* loop1 = solid->face()->loop();

	HalfEdge* half_edge2 = mev(v1, p2, loop1);
	Vertex* v2 = half_edge2->end();
	HalfEdge* half_edge3 = mev(v1, p3, loop1);
	Vertex* v3 = half_edge3->end();
	HalfEdge* half_edge4 = mev(v2, p4, loop1);
	Vertex* v4 = half_edge4->end();

	// （下外环）
	Loop* loop2 = mef(v3,v4,loop1);


	// 下
	HalfEdge* half_edge11 = mev(v1, p11, loop2);
	Vertex* v11 = half_edge11->end();
	HalfEdge* half_edge12 = mev(v2, p12, loop2);
	Vertex* v12 = half_edge12->end();
	HalfEdge* half_edge13 = mev(v3, p13, loop2);
	Vertex* v13 = half_edge13->end();
	HalfEdge* half_edge14 = mev(v4, p14, loop2);
	Vertex* v14 = half_edge14->end();

	Loop* loop11 = mef(v11, v12, loop2);
	Loop* loop12 = mef(v12, v14, loop2);
	Loop* loop13 = mef(v14, v13, loop2);
	Loop* loop14 = mef(v13, v11, loop2);
	
	// 上内
	HalfEdge* half_edge5 = mev(v1, p21, loop1);
	Vertex* v21 = half_edge5->end();
	HalfEdge* half_edge22 = mev(v21, p22, loop1);
	Vertex* v22 = half_edge22->end();
	HalfEdge* half_edge23 = mev(v21, p23, loop1);
	Vertex* v23 = half_edge23->end();
	HalfEdge* half_edge24 = mev(v22, p24, loop1);
	Vertex* v24 = half_edge24->end();

	// （下内环）
	Loop* loop3 = mef(v23, v24, loop1);
	// （上内环）
	Loop* loop4 = kemr(v1, v21, loop1);

	// 下内
	HalfEdge* half_edge31 = mev(v21, p31, loop3);
	Vertex* v31 = half_edge31->end();
	HalfEdge* half_edge32 = mev(v22, p32, loop3);
	Vertex* v32 = half_edge32->end();
	HalfEdge* half_edge33 = mev(v23, p33, loop3);
	Vertex* v33 = half_edge33->end();
	HalfEdge* half_edge34 = mev(v24, p34, loop3);
	Vertex* v34 = half_edge34->end();

	Loop* loop31 = mef(v31, v32, loop3);
	Loop* loop32 = mef(v32, v34, loop3);
	Loop* loop33 = mef(v34, v33, loop3);
	Loop* loop34 = mef(v33, v31, loop3);

	kfmrh(loop2, loop3);

	loop3->set_is_inner_loop(true);
	loop4->set_is_inner_loop(true);
	
	printf("[DEBUG] test2 end \n");

	return convert(solid);
}

std::vector<glm::vec3> Construct::test3() 
{
	Point p1(-1.0, 1.0, 1.0);
	Point p2(-1.0, 1.0, -1.0);
	Point p3(1.0, 1.0, 1.0);
	Point p4(1.0, 1.0, -1.0);

	Point p11(-1.0, -1.0, 1.0);
	Point p12(-1.0, -1.0, -1.0);
	Point p13(1.0, -1.0, 1.0);
	Point p14(1.0, -1.0, -1.0);

	Point p21(-0.5, 1.0, 0.5); //内缩点
	Point p22(-0.5, 1.0, -0.5);
	Point p23(0.5, 1.0, 0.5);
	Point p24(0.5, 1.0, -0.5);

	Point p31(-0.5, -1.0, 0.5); //内缩点
	Point p32(-0.5, -1.0, -0.5);
	Point p33(0.5, -1.0, 0.5);
	Point p34(0.5, -1.0, -0.5);

	// 上
	Vertex* v1;
	Solid* solid = mvfs(p1, v1);
	Loop* loop1 = solid->face()->loop();

	HalfEdge* half_edge2 = mev(v1, p2, loop1);
	Vertex* v2 = half_edge2->end();
	HalfEdge* half_edge3 = mev(v1, p3, loop1);
	Vertex* v3 = half_edge3->end();
	HalfEdge* half_edge4 = mev(v2, p4, loop1);
	Vertex* v4 = half_edge4->end();

	// （下外环）
	Loop* loop2 = mef(v3, v4, loop1);


	// 下
	HalfEdge* half_edge11 = mev(v1, p11, loop2);
	Vertex* v11 = half_edge11->end();
	HalfEdge* half_edge12 = mev(v2, p12, loop2);
	Vertex* v12 = half_edge12->end();
	HalfEdge* half_edge13 = mev(v3, p13, loop2);
	Vertex* v13 = half_edge13->end();
	HalfEdge* half_edge14 = mev(v4, p14, loop2);
	Vertex* v14 = half_edge14->end();

	Loop* loop11 = mef(v11, v12, loop2);
	Loop* loop12 = mef(v12, v14, loop2);
	Loop* loop13 = mef(v14, v13, loop2);
	Loop* loop14 = mef(v13, v11, loop2);

	// 上内
	HalfEdge* half_edge5 = mev(v1, p21, loop1);
	Vertex* v21 = half_edge5->end();
	HalfEdge* half_edge22 = mev(v21, p22, loop1);
	Vertex* v22 = half_edge22->end();
	HalfEdge* half_edge23 = mev(v21, p23, loop1);
	Vertex* v23 = half_edge23->end();
	HalfEdge* half_edge24 = mev(v22, p24, loop1);
	Vertex* v24 = half_edge24->end();

	// （下内环）
	Loop* loop3 = mef(v23, v24, loop1);
	// （上内环）
	Loop* loop4 = kemr(v1, v21, loop1);

	// 下内
	HalfEdge* half_edge31 = mev(v21, p31, loop3);
	Vertex* v31 = half_edge31->end();
	HalfEdge* half_edge32 = mev(v22, p32, loop3);
	Vertex* v32 = half_edge32->end();
	HalfEdge* half_edge33 = mev(v23, p33, loop3);
	Vertex* v33 = half_edge33->end();
	HalfEdge* half_edge34 = mev(v24, p34, loop3);
	Vertex* v34 = half_edge34->end();

	Loop* loop31 = mef(v31, v32, loop3);
	Loop* loop32 = mef(v32, v34, loop3);
	Loop* loop33 = mef(v34, v33, loop3);
	Loop* loop34 = mef(v33, v31, loop3);

	kfmrh(loop2, loop3);

	loop3->set_is_inner_loop(true);
	loop4->set_is_inner_loop(true);

	// 扫掠
	sweep(solid->face(), glm::vec3(-0.5, 1.0, 0.0), 2.0);

	return convert(solid);

}
