#pragma once
#include <future>
#include <thread>
#include "renderClasses/Mesh.h"
#include "FastNoiseLite.h"
#include "chunk.h"

class chunkdata;

struct raycastRES
{
	glm::vec3 pos;
	chunkdata* chunk;
	bool hit;
	int i;
};

class world {
public:
	const int chunkSize = 32;
	int threads_gen = 0;
	int MAX_threads_gen = 12;

	std::vector<Texture> textures{
			Texture("res/grass_top.png",    0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png",   2, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/stone.png",        3, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/log.png",          4, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/leaves.png",       5, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/water.png",        6, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/sand.png",         7, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	std::vector<Texture> watertextures{
			Texture("res/water.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	FastNoiseLite Noise;
	bool thr_ready = true;
	std::vector<chunkdata> chunk;
	world();
	raycastRES raycast(const glm::vec3 start, const glm::vec3 dir, const float max_distan, bool the_hit_bafore);
	chunkdata* getchunk(int x, int y, int z);
	void pre_load_chunk(glm::vec3 pos, int renderDistent);
	void update(Camera& camera, int renderDistent);
	void render(Shader& shader, Shader& watershader, Camera& camera);
};