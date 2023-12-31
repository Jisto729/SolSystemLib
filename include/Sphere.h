#pragma once

#ifdef SSL_EXPORTS
#define SSL_EXPORT __declspec(dllexport)
#else
#define SSL_EXPORT __declspec(dllimport)
#endif

#include <vector>
#include <glm/glm.hpp>

namespace ssl
{
	class Sphere
	{
	public:
		SSL_EXPORT Sphere(int size, float posX, float posY, float posZ);
		SSL_EXPORT void setColor(glm::vec3 color);
		SSL_EXPORT std::vector<float> getVertices();
		SSL_EXPORT std::vector<float> getColors();
		SSL_EXPORT std::vector<int> getIndices();
		SSL_EXPORT glm::vec3 getCenter();

	private:
		std::vector<float> GLvertices;
		std::vector<glm::vec3> vertices;
		std::vector<float> colors;
		std::vector<int> GLindices;
		std::vector<glm::vec3> indices;
		glm::vec3 center;

		void Sphere::icosahedron(float posX, float posY, float posZ);
		void Sphere::setUniqueColors();
		std::vector<float> Sphere::convertToGLData(std::vector<glm::vec3> data);
	};
}