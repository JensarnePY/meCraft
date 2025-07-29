#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>
#include<vector>

#include"../Camera.h"
#include"Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUV;
	glm::float32 dir;
};

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;

	Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	void makeMash(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	void clear();
	void Draw(Shader& shader, Camera& camera);
};
#endif