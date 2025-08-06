#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>
#include<vector>

#include"../Camera.h"
#include"Texture.h"

enum face
{
	top = 0,
	bottom = 1,
	front = 2,
	back = 3,
	left = 4,
	right = 5
};

enum blockID
{
	grass_block = 0,
	dirt_block = 1
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texUV;
	float face;
	float blockID;
};


class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	GLuint VAOID = 0;
	GLuint VBOID = 0;
	GLuint EBOID = 0;

	Mesh();
	//Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	const void makeMash(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	void clear();
	void Draw(Shader& shader, Camera& camera);
};
#endif