#pragma once
#include "world.h"
#include "math.h"

struct chunkdata {
	glm::vec3 pos;
	Mesh mesh;
	bool gen = false;
	bool render = false;
	bool loaded_to_gpu = false;
	std::vector <float> blockIdList;
	const int chunkSize = 32;

	chunkdata(glm::vec3 position) : pos(position) {}

	int getpos(int x, int y, int z);

	int getpos(glm::vec3);

	void reload();

	const float noise(float x, float z, const FastNoiseLite* Noise);

	void make_noiselist(bool* is_nedad, const FastNoiseLite* Noise);

	void make_blockIdList();

	void make_tree(const FastNoiseLite* Noise);

	void make_vertices();

	void gen_chunkdata(const FastNoiseLite* Noise);

};
