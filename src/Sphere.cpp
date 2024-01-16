//TODO change maybe
#include "../include/Sphere.h"
#include <iostream>

using namespace ssl;

Sphere::Sphere(int size, float posX, float posY, float posZ)
{
    Sphere::icosahedron(posX, posY, posZ);
    //Sphere::setUniqueColors();
    Sphere::setColor(glm::vec3(0.5f, 0.7f, 0.0f));
}

void Sphere::icosahedron(float posX, float posY, float posZ)
{

    center = glm::vec3(posX, posY, posZ);
    //from https://danielsieger.com/blog/2021/01/03/generating-platonic-solids.html
    float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
    float a = 1.0f;
    float b = 1.0f / phi;

    // adding vertices
    vertices.push_back(glm::vec3(posX, posY + b, posZ - a));
    vertices.push_back(glm::vec3(posX + b, posY + a, posZ));
    vertices.push_back(glm::vec3(posX - b, posY + a, posZ));
    vertices.push_back(glm::vec3(posX, posY + b, posZ + a));
    vertices.push_back(glm::vec3(posX, posY - b, posZ + a));
    vertices.push_back(glm::vec3(posX - a, posY, posZ + b));
    vertices.push_back(glm::vec3(posX, posY - b, posZ - a));
    vertices.push_back(glm::vec3(posX + a, posY, posZ - b));
    vertices.push_back(glm::vec3(posX + a, posY, posZ + b));
    vertices.push_back(glm::vec3(posX - a, posY, posZ - b));
    vertices.push_back(glm::vec3(posX + b, posY - a, posZ));
    vertices.push_back(glm::vec3(posX - b, posY - a, posZ));

    // adding triangles
    //indices.push_back(glm::vec3(2, 1, 0));
    //indices.push_back(glm::vec3(1, 2, 3));
    //indices.push_back(glm::vec3(5, 4, 3));
    //indices.push_back(glm::vec3(4, 8, 3));
    //indices.push_back(glm::vec3(7, 6, 0));
    //indices.push_back(glm::vec3(6, 9, 0));
    //indices.push_back(glm::vec3(11, 10, 4));
    //indices.push_back(glm::vec3(10, 11, 6));
    //indices.push_back(glm::vec3(9, 5, 2));
    //indices.push_back(glm::vec3(5, 9, 11));
    //indices.push_back(glm::vec3(8, 7, 1));
    //indices.push_back(glm::vec3(7, 8, 10));
    //indices.push_back(glm::vec3(2, 5, 3));
    //indices.push_back(glm::vec3(8, 1, 3));
    //indices.push_back(glm::vec3(9, 2, 0));
    //indices.push_back(glm::vec3(1, 7, 0));
    //indices.push_back(glm::vec3(11, 9, 6));
    //indices.push_back(glm::vec3(7, 10, 6));
    //indices.push_back(glm::vec3(5, 11, 4));
    //indices.push_back(glm::vec3(10, 8, 4));

    indices.push_back(glm::vec3(2, 1, 0));
    indices.push_back(glm::vec3(1, 2, 3));
    indices.push_back(glm::vec3(5, 4, 3));
    indices.push_back(glm::vec3(4, 8, 3));
    indices.push_back(glm::vec3(7, 6, 0));
    indices.push_back(glm::vec3(6, 9, 0));
    indices.push_back(glm::vec3(11, 10, 4));
    indices.push_back(glm::vec3(10, 11, 6));
    indices.push_back(glm::vec3(9, 5, 2));
    indices.push_back(glm::vec3(5, 9, 11));
    indices.push_back(glm::vec3(8, 7, 1));
    indices.push_back(glm::vec3(7, 8, 10));
    indices.push_back(glm::vec3(2, 5, 3));
    indices.push_back(glm::vec3(8, 1, 3));
    indices.push_back(glm::vec3(9, 2, 0));
    indices.push_back(glm::vec3(1, 7, 0));
    indices.push_back(glm::vec3(11, 9, 6));
    indices.push_back(glm::vec3(7, 10, 6));
    indices.push_back(glm::vec3(5, 11, 4));
    indices.push_back(glm::vec3(10, 8, 4));

    //TODO remove Sphere:: where can
    GLvertices = Sphere::convertToGLData(vertices);
    std::vector<float> flIndices = Sphere::convertToGLData(indices);
    GLindices = std::vector<int>(flIndices.begin(), flIndices.end());
    GLnormals = convertToGLData(computeNormals());
}

std::vector<float> Sphere::getVertices() { return GLvertices; }

std::vector<float> Sphere::getNormals() { return GLnormals; };

std::vector<float> Sphere::getColors() { return colors; }

std::vector<int> Sphere::getIndices() { return GLindices; }

glm::vec3 Sphere::getCenter() { return center; }

void Sphere::setColor(glm::vec3 color)
{
    colors.clear();
    for (glm::vec3 vertex : vertices) {
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
    }
}

void Sphere::setUniqueColors()
{
    colors.clear();
    float color = 0;
    float diff = 1.0 / vertices.size();
    for (glm::vec3 vertex : vertices) {
        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);
        color += diff;
    }
}

std::vector<float> Sphere::convertToGLData(std::vector<glm::vec3> data)
{
    std::vector<float> returnData;
    for (glm::vec3 item : data) {
        returnData.push_back(item[0]);
        returnData.push_back(item[1]);
        returnData.push_back(item[2]);
    }
    return returnData;
}

std::vector<glm::vec3> Sphere::computeNormals()
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