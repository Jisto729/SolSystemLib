#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace ssl
{
	class GLObject
	{
	public:
		virtual std::vector<float> getVertices();
		virtual std::vector<float> getColors();
		virtual std::vector<float> getNormals();
		virtual std::vector<int> getIndices();
		virtual glm::vec3 getCenter();
	};
}