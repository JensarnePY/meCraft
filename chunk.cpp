#include "chunk.h"



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

int chunkdata::getpos(int x, int y, int z) {
	const int size = chunkSize + 2;
	return (x + 1) + (z + 1) * size + (y + 1) * size * size;
}

int chunkdata::getpos(glm::vec3) {
	const int size = chunkSize + 2;
	return (pos.x + 1) + (pos.z + 1) * size + (pos.y + 1) * size * size;
}

const float chunkdata::noise(float x, float z, const FastNoiseLite* Noise) {
	return Noise->GetNoise(x / 4, z / 4) * 30 + Noise->GetNoise(x / 15, z / 15) * 200;
}

void chunkdata::make_noiselist(bool* is_nedad, const FastNoiseLite* Noise) {
	bool emty = false;
	bool full = false;
	blockIdList.resize(pow(chunkSize + 2, 3));

	for (float z = 0; z < chunkSize + 2; z++) {
		for (float x = 0; x < chunkSize + 2; x++) {

			const float h = noise(x + pos.x, z + pos.z, Noise);

			for (float y = 0; y < chunkSize + 2; y++) {

				if ((h > y + pos.y) == true) {
					blockIdList[getpos(x - 1.0f, y - 1.0f, z - 1.0f)] = blockID::defalt_stone;
					full = true;
				}
				else {
					blockIdList[getpos(x - 1.0f, y - 1.0f, z - 1.0f)] = blockID::air;
					emty = true;
				}

			}
		}
	}
	if (emty == true && full == false || emty == false && full == true) *is_nedad = false;
}

void chunkdata::make_blockIdList() {
	for (int z = 0; z < chunkSize; z++) {
		for (int x = 0; x < chunkSize; x++) {
			for (int y = 0; y < chunkSize; y++) {

				float block = blockID::air;
				if (blockIdList[getpos(x, y, z)] == 3) block = blockID::grass_block;
				if (blockIdList[getpos(x, y + 1, z)] == 3) block = blockID::dirt_block;

				blockIdList[getpos(x, y, z)] = block;


			}
		}
	}
}

void chunkdata::make_tree(const FastNoiseLite* Noise) {

	for (float z = 0; z < chunkSize; z++) {
		for (float x = 0; x < chunkSize; x++) {
			if (rand() % 70 == 1) {

				float float_h = noise(x + pos.x, z + pos.z, Noise);
				int h = (int)float_h;
				h = h % 32;

				if (float_h > 0 && blockIdList[getpos(x, (float)h - 1, z)] == blockID::grass_block) {


					//chanse the data to a tree
					for (int tree_wood = 0; tree_wood < 5; tree_wood++) {
						if (h >= 0 && h + tree_wood <= 31) {
							blockIdList[getpos(x, tree_wood + h, z)] = blockID::wood_log;
						}
					}
					for (int l_y = 0; l_y < 4; l_y++) {
						for (int l_z = z - 2; l_z < z + 2; l_z++) {
							for (int l_x = x - 2; l_x < x + 2; l_x++) {
								if (l_y >= 0 && l_y <= 31 && l_z >= 0 && l_z <= 31 && h + 4 > 0 && h + l_y + 4 <= 31) {

									blockIdList[getpos(l_x, l_y + h + 4, l_z)] = blockID::leaves;
								}
							}
						}
					}
				}
			}
		}
	}
}

void chunkdata::make_vertices() {

	for (int z = 0; z < chunkSize; z++) {
		for (int x = 0; x < chunkSize; x++) {
			for (int y = 0; y < chunkSize; y++) {
				if (this->blockIdList[getpos(x, y, z)] != blockID::air) {

					float endres_x = pos.x + x;
					float endres_y = pos.y + y;
					float endres_z = pos.z + z;
					float block = blockIdList[getpos(x, y, z)];

					if (blockIdList[getpos(x, y + 1, z)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::top, block });
					}

					if (blockIdList[getpos(x, y - 1, z)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::bottom, block });
					}

					if (blockIdList[getpos(x + 1, y, z)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::right, block });
					}

					if (blockIdList[getpos(x - 1, y, z)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::left, block });
					}

					if (blockIdList[getpos(x, y, z + 1)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::back, block });
					}

					if (blockIdList[getpos(x, y, z - 1)] == blockID::air) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::front, block });
					}
				}
			}
		}
	}
}

void chunkdata::gen_chunkdata(const FastNoiseLite* Noise) {

	bool is_nedad = true;
	make_noiselist(&is_nedad, Noise);


	if (is_nedad) {

		make_blockIdList();
		make_tree(Noise);
		make_vertices();

	}
	gen = true;
}

static void gen_chunk(chunkdata* chunk, const FastNoiseLite* Noise) {
	chunk->gen_chunkdata(Noise);
}

void chunkdata::reload() {

	mesh.clear();

	std::vector<Texture> textures;
	make_vertices();
	mesh.makeMash(textures);
	gen = true;

}