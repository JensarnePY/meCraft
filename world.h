#pragma once

#include <algorithm>
#include <future>
#include <thread>
#include "renderClasses/Mesh.h"
#include "FastNoiseLite.h"

struct chunkdata {
	glm::vec3 pos;
	Mesh mesh;
	bool gen = false;
	bool render = false;
	std::vector <GLuint> indices;
	std::vector <Vertex> vertices;

	chunkdata(glm::vec3 position) : pos(position) {}
};

//truct threadata{
//	glm::vec3 pos;
//	//void gen_thread_chunk(glm::vec3 pos, const FastNoiseLite Noise);
//	
//	bool ready = false;
//	bool runing = false;
//	
//	std::future<void> ftr;
//	std::vector <GLuint> indices = {};
//	std::vector <Vertex> vertices = {};
//rivate:
//	
//	int chunkSize = 32;
//;



class world {
	const float chunkSize = 32;
	int threads_gen = 0;
	int MAX_threads_gen = 0;

	std::vector<Texture> textures{
			Texture("res/grass_top.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
	};


	std::vector<chunkdata> chunk;
	FastNoiseLite Noise;
	bool thr_ready = true;
public:
	world();
	void pre_load_chunk(glm::vec3 pos, int renderDistent);
	void update(Camera& camera, int renderDistent);
	void render(Shader& shader, Camera& camera);
};