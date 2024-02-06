#include "../include/SunColors.h"

using namespace ssl;

glm::vec3 SunColors::getSunColor(float noiseValue, int temperature)
{
	//the temperature value puts the color between two possible gradients 
	glm::vec3 begLow, begHigh, endLow, endHigh;

	float begImportance = 1.0;
	const int tempRange = 5000;
	if (temperature < 5000)
	{
		begLow = endLow = colorsLow[0];
		begHigh = endHigh = colorsHigh[0];
	} else if (temperature > 30000)
	{
		begLow = endLow = colorsLow[6];
		begHigh = endHigh = colorsHigh[6];
	} else
	{
		begLow = colorsLow[(temperature / tempRange) - 1];
		begHigh = colorsHigh[(temperature / tempRange) - 1];
		endLow = colorsLow[temperature / tempRange];
		endHigh = colorsHigh[temperature / tempRange];
		if (temperature % tempRange == 0)
		{
			begImportance = 1.0;
		} else
		{
			begImportance = (temperature % tempRange) / (float)tempRange;
		}
	}

	glm::vec3 low = (begImportance * begLow + (1.0f - begImportance) * endLow);
	glm::vec3 high = (begImportance * begHigh + (1.0f - begImportance) * endHigh);

	return getColorFromGradient(low, high, noiseValue);
}

glm::vec3 SunColors::getColorFromGradient(glm::vec3 low, glm::vec3 high, float value)
{
	if (value > 1.0 || value < 0.0) return glm::vec3();
	return low + ((high - low) * value);
}

const glm::vec3 SunColors::colorsLow[7] = {glm::vec3(0.6, 0.0, 0.0), 
	glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.65, 0.0), glm::vec3(1.0, 1.0, 1.0), 
	glm::vec3(0.6, 0.75, 1.0), glm::vec3(0.4, 0.6, 1.0), glm::vec3(0.0, 0.0, 1.0)};

const glm::vec3 SunColors::colorsHigh[7] = { glm::vec3(0.8, 0.3, 0.0), 
	glm::vec3(1.0, 0.85, 0.0), glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 1.0),
	glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)};