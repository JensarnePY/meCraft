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
	right = 2,
	left = 3,
	back = 4,
	front = 5
};

enum blockID
{
	air = 0,
	grass_block = 1,
	dirt_block = 2,
	defalt_stone = 3
};

//struct Vertex
//{
//	float position[3];
//	float texUV[2];
//	float face[1];
//	float blockID[1];
//};


struct Vertex
{
	float position[3];
	float face;
	float blockID;
};



class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <Texture> textures;

	GLuint EmptyVAOID = 0;
	GLuint SSBOvert = 0;
	GLuint SSBOind = 0;

	Mesh();
	const void makeMash(std::vector <Vertex>& vertices, std::vector <Texture>& textures);
	void clear();
	void Draw(Shader& shader, Camera& camera);
};
#endif