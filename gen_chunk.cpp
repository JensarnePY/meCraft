#include "gen_chunk.h"


namespace gen_chunk {

	static float GetNoise3D(float x, float y, float z, const FastNoiseLite& Noise) {
		float ab = Noise.GetNoise(x, y);
		float bc = Noise.GetNoise(y, z);
		float ac = Noise.GetNoise(x, z);
		float ba = Noise.GetNoise(y, x);
		float cb = Noise.GetNoise(z, y);
		float ca = Noise.GetNoise(z, x);
		float abc = ab + bc + ac + ba + cb + ca;
		return abc / 6.0f;
	}

	static int get_block_pos(int x, int y, int z) {
		const int size = 32 + 2;
		x += 1;
		y += 1;
		z += 1;
		return x + z * size + y * size * size;
	}

	static int get_block_pos(float x, float y, float z) {
		const float size = 32 + 2;
		x += 1;
		y += 1;
		z += 1;
		return x + z * size + y * size * size;
	}

	const float noise(float x, float z, const FastNoiseLite* Noise) {
		return Noise->GetNoise(x / 4, z / 4) * 30 + Noise->GetNoise(x / 15, z / 15) * 200;
	}

	void make_noiselist(bool* is_nedad, chunkdata* chunk, const FastNoiseLite* Noise) {
		bool emty = false;
		bool full = false;
		chunk->is_solid_list.resize(pow(chunk->chunkSize + 2, 3));
		chunk->blockIdList.resize(pow(chunk->chunkSize, 3));

		for (float z = 0; z < chunk->chunkSize + 2; z++) {
			for (float x = 0; x < chunk->chunkSize + 2; x++) {

				const float h = noise(x + chunk->pos.x, z + chunk->pos.z, Noise);

				for (float y = 0; y < chunk->chunkSize + 2; y++) {
					
					 if ((h > y + chunk->pos.y) == true) {
						 chunk->is_solid_list[get_block_pos(x - 1.0f, y - 1.0f, z - 1.0f)] = true;
						 full = true;
					 }
					 else {
						 chunk->is_solid_list[get_block_pos(x - 1.0f, y - 1.0f, z - 1.0f)] = false;
						 emty = true;
					 }
					 
				}
			}
		}
		if (emty == true && full == false || emty == false && full == true) *is_nedad = false;
	}

	void make_blockIdList(chunkdata* chunk) {
		for (int z = 0; z < chunk->chunkSize; z++) {
			for (int x = 0; x < chunk->chunkSize; x++) {
				for (int y = 0; y < chunk->chunkSize; y++) {

					float block = blockID::air;
					if (chunk->is_solid_list[get_block_pos(x, y, z)] == true) block = blockID::grass_block;
					if (chunk->is_solid_list[get_block_pos(x, y + 1, z)] == true) block = blockID::dirt_block;

					chunk->blockIdList[chunk->getBlockIDPos(x, y, z)] = block;
					
					
				}
			}
		}
	}

	void make_tree(chunkdata* chunk, const FastNoiseLite* Noise) {

		for (float z = 0; z < chunk->chunkSize; z++) {
			for (float x = 0; x < chunk->chunkSize; x++) {
				if (rand() % 70 == 1) {

					float float_h = noise(x + chunk->pos.x, z + chunk->pos.z, Noise);
					int h = (int)float_h;
					h = h % 32;

					if (float_h > 0 && chunk->is_solid_list[get_block_pos(x, (float)h - 1, z)] == true) {


						//chanse the data to a tree
						for (int tree_wood = 0; tree_wood < 5; tree_wood++) {
							if (h >= 0 && h + tree_wood <= 31) {
								chunk->blockIdList[chunk->getBlockIDPos(x, tree_wood + h, z)] = blockID::wood_log;
								chunk->is_solid_list[get_block_pos(x, tree_wood + (float)h, z)] = true;
							}
						}
						for (int l_y = 0; l_y < 4; l_y++) {
							for (int l_z = z - 2; l_z < z + 2; l_z++) {
								for (int l_x = x - 2; l_x < x + 2; l_x++) {
									if (l_y >= 0 && l_y <= 31 && l_z >= 0 && l_z <= 31 && h + 4 > 0 && h + l_y + 4 <= 31) {
										
										chunk->blockIdList[chunk->getBlockIDPos(l_x, l_y + h + 4, l_z)] = blockID::leaves;
										chunk->is_solid_list[get_block_pos(l_x, l_y + (int)h + 4, l_z)] = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void make_vertices(chunkdata* chunk) {

		for (int z = 0; z < chunk->chunkSize; z++) {
			for (int x = 0; x < chunk->chunkSize; x++) {
				for (int y = 0; y < chunk->chunkSize; y++) {
					if (chunk->is_solid_list[get_block_pos(x, y, z)] == true) {

						float endres_x = chunk->pos.x + x;
						float endres_y = chunk->pos.y + y;
						float endres_z = chunk->pos.z + z;
						float block = chunk->blockIdList[chunk->getBlockIDPos(x, y, z)];

						if (chunk->is_solid_list[get_block_pos(x, y + 1, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::top, block });
						}

						if (chunk->is_solid_list[get_block_pos(x, y - 1, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::bottom, block });
						}

						if (chunk->is_solid_list[get_block_pos(x + 1, y, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::right, block });
						}

						if (chunk->is_solid_list[get_block_pos(x - 1, y, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::left, block });
						}

						if (chunk->is_solid_list[get_block_pos(x, y, z + 1)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::back, block });
						}

						if (chunk->is_solid_list[get_block_pos(x, y, z - 1)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::front, block });
						}
					}
				}
			}
		}
	}

	void gen_chunkdata(chunkdata* chunk, const FastNoiseLite* Noise) {

		bool is_nedad = true;
		make_noiselist(&is_nedad, chunk, Noise);
		

		if (is_nedad) {

			make_blockIdList(chunk);
			make_tree(chunk, Noise);
			make_vertices(chunk);

		}
		chunk->gen = true;
	}

	void reload_chunkdata(chunkdata* chunk) {


		make_vertices(chunk);
		chunk->gen = true;
	}

}