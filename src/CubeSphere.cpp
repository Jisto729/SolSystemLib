//TODO change maybe
#include "../include/CubeSphere.h"
#include <iostream>
#include <tgmath.h>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace ssl;

CubeSphere::CubeSphere(float size, int subdivisions)
{
    center = glm::vec3(0.0, 0.0, 0.0);
    CubeSphere::Sphere(size, subdivisions);
    setUniqueColors();
}

CubeSphere::CubeSphere(float size, float posX, float posY, float posZ, int subdivisions)
{
    std::cout << "here" << std::endl;
    center = glm::vec3(posX, posY, posZ);
    CubeSphere::Sphere(size, subdivisions);
    //TODO probably wrong here
    int vertSize = vertices.size();
    for (int i = 0; i < vertSize; i++)
    {
        vertices[i] += glm::vec3(posX, posY, posZ);
    }
    setUniqueColors();
}


void CubeSphere::Sphere(float size, int subdivisions)
{
    SphereSide(size, subdivisions, glm::vec3(0.0, 0.0, 1.0));
    SphereSide(size, subdivisions, glm::vec3(0.0, 0.0, -1.0));
    SphereSide(size, subdivisions, glm::vec3(0.0, 1.0, 0.0));
    SphereSide(size, subdivisions, glm::vec3(0.0, -1.0, 0.0));
    SphereSide(size, subdivisions, glm::vec3(1.0, 0.0, 0.0));
    SphereSide(size, subdivisions, glm::vec3(-1.0, 0.0, 0.0));
}

void CubeSphere::SphereSide(float size, int subdivisions, glm::vec3 orientation)
{
    int startIndex = vertices.size();
    int verticesNum = pow(2, subdivisions) + 1;
    //distance between the vertices on a unit cube
    float vertexDiff = 1.0 / (verticesNum - 1);
    float angleDiff = glm::radians(90.0) / (verticesNum - 1);
    float cornerDist = atan(1); //= pi / 4
    //commputing rotation angle and axis to be able to make all 6 sides of cube
    glm::vec3 rotationAxis = orientation.y != 0.0 ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(0.0, 1.0, 0.0);
    float rotationAngle = glm::orientedAngle(glm::vec3(0.0, 0.0, 1.0), orientation, rotationAxis);

    for (int i = 0; i < verticesNum; i++)
    {
        for (int j = 0; j < verticesNum; j++)
        {
            //the base face is in the +z orientation
            glm::vec3 pointDir = glm::vec3(glm::radians(-45.0) + i * angleDiff, glm::radians(-45.0) + j * angleDiff, cornerDist);
            glm::vec3 point = size * glm::normalize(pointDir);

            point = glm::vec4(point, 1.0) * glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);
            vertices.push_back(point);

            glm::vec3 texCoord = glm::vec3(glm::normalize(point).x, glm::normalize(point).y, glm::normalize(point).z);
            texCoords.push_back(texCoord);
        }
    }

    for (int i = 0; i < verticesNum - 1; i++)
    {
        for (int j = 0; j < verticesNum - 1; j++)
        {
            //adding clockwise indices
            indices.push_back(glm::vec3(startIndex, startIndex, startIndex) + glm::vec3(i + j * verticesNum, i + j * verticesNum + 1, i + ((j + 1) * verticesNum)));
            indices.push_back(glm::vec3(startIndex, startIndex, startIndex) + glm::vec3(i + j * verticesNum + 1, i + ((j + 1) * verticesNum) + 1, i + ((j + 1) * verticesNum)));
        }
    }
}

std::vector<float> CubeSphere::getVertices() 
{ 
    return convertToGLData(vertices);
}

std::vector<float> CubeSphere::getNormals() 
{ 
    return convertToGLData(computeNormals());
}

std::vector<float> CubeSphere::getColors() { return colors; }

std::vector<int> CubeSphere::getIndices() 
{ 
    std::vector<float> flIndices = convertToGLData(indices);
    return std::vector<int>(flIndices.begin(), flIndices.end());
}

std::vector<float> CubeSphere::getTexCoords()
{
    return convertToGLData(texCoords);
}


glm::vec3 CubeSphere::getCenter() { return center; }

void CubeSphere::setColor(glm::vec3 color)
{
    colors.clear();
    for (glm::vec3 vertex : vertices)
    {
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
    }
}

void CubeSphere::setUniqueColors()
{
    colors.clear();
    float color = 0;
    float diff = 1.0 / vertices.size();
    for (glm::vec3 vertex : vertices)
    {
        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);
        color += diff;
    }
}

std::vector<float> CubeSphere::convertToGLData(std::vector<glm::vec3> data)
{
    std::vector<float> returnData;
    for (glm::vec3 item : data)
    {
        returnData.push_back(item[0]);
        returnData.push_back(item[1]);
        returnData.push_back(item[2]);
    }
    return returnData;
}

std::vector<glm::vec3> CubeSphere::computeNormals()
{
    //TODO check if normals are facing the correct way
    std::vector<glm::vec3> returnNormals;
    for (glm::vec3 point : vertices)
    {
        returnNormals.push_back(glm::vec3(0.0f));
    }

    for (glm::vec3 triangle : indices)
    {
        glm::vec3 direction = glm::cross(vertices[triangle[1]] - vertices[triangle[0]], vertices[triangle[2]] - vertices[triangle[0]]);
        glm::vec3 normal = glm::normalize(direction);
        returnNormals[triangle[0]] += normal;
        returnNormals[triangle[1]] += normal;
        returnNormals[triangle[2]] += normal;
    }
    return returnNormals;
}