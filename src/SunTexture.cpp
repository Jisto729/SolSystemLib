#include "../include/SunTexture.h"
#include "../include/SunColors.h"
#include <glm/gtx/vector_angle.hpp>
#include "libs/PerlinNoise/PerlinNoise.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

using namespace ssl;

SunTexture::SunTexture(int startSeed)
{
	seed = startSeed;
	int size = 256;
	texHeight = size;
	texWidth = size;

	baseTexture.reserve(6);

	for (int i = 0; i < 6; i++)
	{
		baseTexture.emplace_back();
		baseTexture[i].reserve(size);
		for (int j = 0; j < size; j++)
		{
			baseTexture[i].emplace_back();
			baseTexture[i][j].reserve(size);
		}
	}

	fillSunTextureFace(size, glm::vec3(0.0, 0.0, 1.0), 0);
	fillSunTextureFace(size, glm::vec3(0.0, 0.0, -1.0), 1);
	fillSunTextureFace(size, glm::vec3(0.0, 1.0, 0.0), 2);
	fillSunTextureFace(size, glm::vec3(0.0, -1.0, 0.0), 3);
	fillSunTextureFace(size, glm::vec3(1.0, 0.0, 0.0), 4);
	fillSunTextureFace(size, glm::vec3(-1.0, 0.0, 0.0), 5);

	loadTextureFrames();
}

void SunTexture::fillSunTextureFace(float size, glm::vec3 orientation, int faceIndex)
{
	const siv::PerlinNoise noise{ siv::PerlinNoise::seed_type(seed) };
	float value;
	float angleDiff = glm::radians(90.0) / (size - 1);
	float cornerDist = atan(1); //= pi / 4
	//commputing rotation angle and axis to be able to make all 6 sides of cube
	glm::vec3 rotationAxis = orientation.y != 0.0 ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(0.0, 1.0, 0.0);
	float rotationAngle = glm::orientedAngle(glm::vec3(0.0, 0.0, 1.0), orientation, rotationAxis);

	float scale = 0.1;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			//the base face is in the +z orientation
			glm::vec3 pointDir = glm::vec3(glm::radians(-45.0) + i * angleDiff, glm::radians(-45.0) + j * angleDiff, cornerDist);
			glm::vec3 point = size * glm::normalize(pointDir);

			point = scale * (glm::vec4(point, 1.0) * glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis));
			value = noise.octave3D_01(point.x, point.y, point.z, 4, 0.5);
			baseTexture[faceIndex][i].emplace_back(getBaseColor(value, 23000));
		}
	}
}

std::vector<std::vector<float>> SunTexture::getCurrentTexture()
{
	return textureFrames[currentFrame];
}

std::vector<std::vector<std::vector<glm::vec3>>> SunTexture::animateTexture()
{
	std::vector<std::vector<std::vector<glm::vec3>>> newTexture;
	newTexture.reserve(6);
	int textureSize = texHeight;
	for (int i = 0; i < 6; i++)
	{
		newTexture.emplace_back();
		newTexture[i].reserve(textureSize);
		for (int j = 0; j < textureSize; j++)
		{
			newTexture[i].emplace_back();
			newTexture[i][j].reserve(textureSize);
		}
	}


	const siv::PerlinNoise noise{ siv::PerlinNoise::seed_type(seed) };
	//creating the noise which is used to animate the texture
	int size = 256;
	float scale = 0.03;
	std::vector<std::vector<float>> animationNoise;
	animationNoise.reserve(size);
	for (int i = 0; i < size; i++)
	{
		animationNoise.emplace_back();
		animationNoise[i].reserve(size);
		for (int j = 0; j < size; j++)
		{
			animationNoise[i].emplace_back(noise.octave3D_11(i * scale, j * scale, time, 4, 0.5));
		}
	}

	int sizeRatio = texHeight / size;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < texHeight; j++)
		{
			for (int k = 0; k < texWidth; k++)
			{
				//making the seams less noticable
				float nearestEdge = std::min(std::min(j, texHeight - j), std::min(k, texHeight - k));
				float factor = std::min(0.02 * nearestEdge, 0.6);

				newTexture[i][j][k] = baseTexture[i][j][k] + getAnimationColor(animationNoise[j / sizeRatio][k / sizeRatio], factor);
			}
		}
	}

	time += 0.01;
	return newTexture;
}

void SunTexture::nextFrame()
{
	isReversed ? currentFrame-- : currentFrame++;
	if (currentFrame >= frameCount)
	{
		currentFrame--;
		isReversed = true;
	}
	if (currentFrame < 0)
	{
		currentFrame++;
		isReversed = false;
	}
}

glm::vec3 SunTexture::getBaseColor(float noiseValue, int temperature)
{
	return SunColors::getSunColor(noiseValue, temperature);
}

glm::vec3 SunTexture::getAnimationColor(float noiseValue, float factor)
{
	return glm::vec3(factor * noiseValue, factor* noiseValue, factor * noiseValue);
}

std::vector<std::vector<float>> SunTexture::ConvertTexture(std::vector<std::vector<std::vector<glm::vec3>>> *texture)
{ 
	std::vector<std::vector<float>> resultTexture;
	for (int i = 0; i < 6; i++)
	{
		resultTexture.emplace_back();

		int size = (*texture)[i].size();
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				resultTexture[i].push_back((*texture)[i][k][j].x);
				resultTexture[i].push_back((*texture)[i][k][j].y);
				resultTexture[i].push_back((*texture)[i][k][j].z);
			}
		}
	}
	return resultTexture; 
}

int SunTexture::getHeight() 
{ 
	return texHeight; 
}

int SunTexture::getWidth() 
{ 
	return texWidth; 
}

void SunTexture::loadTextureFrames()
{
	textureFrames.reserve(frameCount);
	for (int i = 0; i < frameCount; i++)
	{
		std::vector<std::vector<std::vector<glm::vec3>>> newTexture = animateTexture();
		std::vector<std::vector<float>> newTextureConverted = ConvertTexture(&newTexture);
		textureFrames.emplace_back(newTextureConverted);
	}
}