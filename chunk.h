#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "world.h"
#include "math.h"

struct chunkdata {
	glm::vec3 pos;
	Mesh mesh;
	WaterMesh watermesh;
	bool gen = false;
	bool render = false;
	bool loaded_to_gpu = false;

	bool waterrender = false;
	bool water_loaded_to_gpu = false;
	std::vector <float> blockIdList;
	std::vector <bool> waterList;
	const int chunkSize = 32;
	const float F_chunkSize = 32;

	chunkdata(glm::vec3 position) : pos(position) {}

	int getpos(int x, int y, int z);
	int getpos(float x, float y, float z);

	void reload();

	const float noise(float x, float z, const FastNoiseLite* Noise);


	void make_noiselist(bool* is_nedad, const FastNoiseLite* Noise);
	void make_water_noiselist(bool* is_nedad, const FastNoiseLite* Noise);

	void make_blockId(const FastNoiseLite* Noise);

	void make_tree(const FastNoiseLite* Noise);

	void make_vertices();
	void make_water_vertices();

	void gen_chunkdata(const FastNoiseLite* Noise);

};

#endif