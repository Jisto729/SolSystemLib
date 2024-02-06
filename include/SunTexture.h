#pragma once

#ifdef SSL_EXPORTS
#define SSL_EXPORT __declspec(dllexport)
#else
#define SSL_EXPORT __declspec(dllimport)
#endif

#include<vector>
#include<glm/glm.hpp>

namespace ssl
{
	class SunTexture
	{
	public:
		SSL_EXPORT SunTexture(int seed);
		SSL_EXPORT std::vector<std::vector<float>> getCurrentTexture();
		SSL_EXPORT void nextFrame();
		SSL_EXPORT int getHeight();
		SSL_EXPORT int getWidth();
	private:
		std::vector<std::vector<std::vector<glm::vec3>>> animateTexture();
		void loadTextureFrames();
		std::vector<std::vector<float>> ConvertTexture(std::vector<std::vector<std::vector<glm::vec3>>> *texture);
		glm::vec3 getBaseColor(float noiseValue, int temperature);
		glm::vec3 getAnimationColor(float noiseValue, float factor);
		void fillSunTextureFace(float size, glm::vec3 orientation, int faceIndex);
		std::vector<std::vector<std::vector<glm::vec3>>> baseTexture;
		int texHeight;
		int texWidth;
		float time = 0.0;
		int seed;

		std::vector<std::vector<std::vector<float>>> textureFrames;
		int frameCount = 100;
		int currentFrame = 0;

		bool isReversed = false;
	};
}