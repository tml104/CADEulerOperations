#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "HalfEdgeStructure.h"
#include "EulerOperation.h"

namespace Construct {

	using namespace HalfEdgeStructure;
	using namespace EulerOperation;

	std::vector<glm::vec3> convert(Solid* solid);

	std::vector<glm::vec3> test1();
	std::vector<glm::vec3> test2();
	std::vector<glm::vec3> test3();


} // namespace Construct