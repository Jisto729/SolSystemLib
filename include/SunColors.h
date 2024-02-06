#pragma once

#include <vector>
#include<glm/glm.hpp>

namespace ssl
{
	class SunColors
	{
	public:
		static glm::vec3 getSunColor(float noiseValue, int temperature);
	private:
		static const glm::vec3 colorsLow[7];
		static const glm::vec3 colorsHigh[7];
		static glm::vec3 getColorFromGradient(glm::vec3 low, glm::vec3 high, float value);
	};
}