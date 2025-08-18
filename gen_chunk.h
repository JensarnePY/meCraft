#pragma once
#include "world.h"
#include "math.h"

struct chunkdata;

namespace gen_chunk {

	static float GetNoise3D(float x, float y, float z, const FastNoiseLite& Noise);

	static int get_block_pos(int x, int y, int z);

	static int get_block_pos(float x, float y, float z);

	const float noise(float x, float z, const FastNoiseLite* Noise);

	void make_noiselist(bool* is_nedad, chunkdata* chunk, const FastNoiseLite* Noise);

	void make_blockIdList(chunkdata* chunk);

	void make_tree(chunkdata* chunk, const FastNoiseLite* Noise);

	void make_vertices(chunkdata* chunk);

	void gen_chunkdata(chunkdata* chunk, const FastNoiseLite* Noise);

	void reload_chunkdata(chunkdata* chunk);

}

