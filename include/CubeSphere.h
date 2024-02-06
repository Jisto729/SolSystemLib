#pragma once

#ifdef SSL_EXPORTS
#define SSL_EXPORT __declspec(dllexport)
#else
#define SSL_EXPORT __declspec(dllimport)
#endif

#include <vector>
#include <glm/glm.hpp>
#include "GLObject.h"

namespace ssl
{
	//TODO add from interface GLObject
	class CubeSphere
	{
	public:
		SSL_EXPORT CubeSphere(float size, int subdivisions);
		SSL_EXPORT CubeSphere(float size, float posX, float posY, float posZ, int subdivisions);
		SSL_EXPORT void setColor(glm::vec3 color);
		SSL_EXPORT void CubeSphere::setUniqueColors();
		SSL_EXPORT std::vector<float> getVertices();
		SSL_EXPORT std::vector<float> getColors();
		SSL_EXPORT std::vector<float> getNormals();
		SSL_EXPORT std::vector<int> getIndices();
		SSL_EXPORT std::vector<float> getTexCoords();
		SSL_EXPORT glm::vec3 getCenter();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<float> colors;
		std::vector<glm::vec3> indices;
		std::vector<glm::vec3> texCoords;
		glm::vec3 center;

		void Sphere(float size, int subdivisions);
		void SphereSide(float size, int subdivisions, glm::vec3 orientation);
		//TODO add to base class, so that it is not in multiple classes
		std::vector<float> CubeSphere::convertToGLData(std::vector<glm::vec3> data);
		std::vector<glm::vec3> computeNormals();
	};
}