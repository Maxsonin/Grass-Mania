#ifndef OBJ_LOADER
#define OBJ_LOADER

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class OBJLoader
{
public:
    void LoadOBJ(const std::string& filepath, std::vector<Vertex>& vertices)
    {
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec3> tempNormals;

        std::ifstream fileStream(filepath);
        if (!fileStream.is_open())
        {
            std::cerr << "ERROR: Failed to open file: " << filepath << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string line;
        while (std::getline(fileStream, line))
        {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            // Vertex
            if (prefix == "v")
            {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                tempVertices.push_back(vertex);
            }
            // Normals
            else if (prefix == "vn")
            {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            }

            // Faces
            else if (prefix == "f")
            {
                unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
                for (int i = 0; i < 3; ++i)
                {
                    std::string vertexData;
                    ss >> vertexData;
                    std::replace(vertexData.begin(), vertexData.end(), '/', ' ');

                    std::stringstream vss(vertexData);
                    vss >> vertexIndex[i] >> texCoordIndex[i] >> normalIndex[i];
                }

                for (int i = 0; i < 3; ++i)
                {
                    Vertex vertex;
                    vertex.position = tempVertices[vertexIndex[i] - 1];
                    vertex.normal = tempNormals[normalIndex[i] - 1];

                    vertices.push_back(vertex);
                }
            }
        }

        fileStream.close();
    }
};

#endif