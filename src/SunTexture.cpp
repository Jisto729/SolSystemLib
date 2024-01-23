#include "../include/SunTexture.h"
#include <glm/gtx/vector_angle.hpp>
#include "libs/PerlinNoise/PerlinNoise.hpp"

#include <iostream>
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
			baseTexture[faceIndex][i].emplace_back(getBaseColor(value));
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
				float factor;
				//making the seams less noticable
				if (j < 3 || j > texHeight - 3 || k < 3 || k > texWidth - 3)
				{
					factor = 0.0;
				} else if (j < 6 || j > texHeight - 6 || k < 6 || k > texWidth - 6)
				{
					factor = 0.3;
				} else
				{
					factor = 0.6;
				}
				newTexture[i][j][k] = baseTexture[i][j][k] + getAnimationColor(animationNoise[j / sizeRatio][k / sizeRatio], factor);// glm::vec3(0.0, 0.7 * animationNoise[j / sizeRatio][k / sizeRatio], 0.0);
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

//TODO probably add temperature
glm::vec3 SunTexture::getBaseColor(float noiseValue)
{
	//R = 1, G = 0 - 0.85, B = 0 (gradient from red to yellow)
	return glm::vec3(1.0, 0.85 * noiseValue, 0.0);
}

glm::vec3 SunTexture::getAnimationColor(float noiseValue, float factor)
{
	return glm::vec3(0.0, factor * noiseValue, 0.0);
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