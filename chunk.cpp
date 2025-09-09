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

int chunkdata::getpos(float x, float y, float z) {
	const float size = F_chunkSize + 2.0f;
	return (int)(x + 1.0f) + (z + 1.0f) * size + (y + 1.0f) * size * size;
}

const float chunkdata::noise(float x, float z, const FastNoiseLite* Noise) {
	return Noise->GetNoise(x / 4, z / 4) * 50 + Noise->GetNoise(x / 15, z / 15) * 300;
}

void chunkdata::make_noiselist(bool* is_nedad, const FastNoiseLite* Noise) {
	blockIdList.resize(pow(chunkSize + 2, 3));
	int i = 0;
	for (float z = 0; z < chunkSize + 2; z++) {
		for (float x = 0; x < chunkSize + 2; x++) {

			const float h = noise(x + pos.x, z + pos.z, Noise);

			for (float y = 0; y < chunkSize + 2; y++) {
				if (h > y + pos.y) {
					blockIdList[getpos(x - 1.0f, y - 1.0f, z - 1.0f)] = blockID::defalt_stone;
					i++;
				}
			}
		}
	}
	if (i == 0 || i == pow(chunkSize + 2, 3)) *is_nedad = false;
}

void chunkdata::make_water_noiselist(bool* is_nedad, const FastNoiseLite* Noise) {
	waterList.resize(pow(chunkSize + 2, 3));
	int i = 0;
	for (float z = 0; z < chunkSize + 2; z++) {
		for (float x = 0; x < chunkSize + 2; x++) {

			const float h = noise(x + pos.x, z + pos.z, Noise);

			for (float y = 0; y < chunkSize + 2; y++) {


				if (y + pos.y <= 0 && y + pos.y >= h) {
					waterList[getpos(x - 1.0f, y - 1.0f, z - 1.0f)] = true;
					i++;
				}
			}
		}
	}
	if (i == 0 || i == pow(chunkSize + 2, 3)) *is_nedad = false;
}

void chunkdata::make_blockId(const FastNoiseLite* Noise) {
	for (int z = 0; z < chunkSize; z++) {
		for (int x = 0; x < chunkSize; x++) {

			const float h = noise(x + pos.x + 1, z + pos.z + 1, Noise);

			for (int y = 0; y < chunkSize; y++) {

				float block = blockID::air;
				if (blockIdList[getpos(x, y, z)] == blockID::defalt_stone) block = blockID::grass_block;
				if (blockIdList[getpos(x, y + 1, z)] == blockID::defalt_stone) block = blockID::dirt_block;
				
				if (blockIdList[getpos(x, y, z)] == blockID::defalt_stone) {

					if (waterList[getpos(x + 1, y, z)]
						|| waterList[getpos(x - 1, y, z)]
						|| waterList[getpos(x, y + 1, z)]
						|| waterList[getpos(x, y - 1, z)]
						|| waterList[getpos(x, y, z + 1)]
						|| waterList[getpos(x, y, z - 1)]
						|| y + pos.y < 3 && h > y + pos.y - 5) block = blockID::sand;
				}
				if (blockIdList[getpos(x, y, z)] == blockID::defalt_stone && h > y + pos.y + 3) block = blockID::defalt_stone;

				blockIdList[getpos(x, y, z)] = block;
			}
		}
	}
}

void chunkdata::make_tree(const FastNoiseLite* Noise) {

	for (int z = 0; z < chunkSize; z++) {
		for (int x = 0; x < chunkSize; x++) {
			int h = (int)noise((float)x + pos.x, (float)z + pos.z, Noise);
			for (int y = 0; y < chunkSize; y++) {

				if (h == y + pos.y && rand() % 200 == 1 && h > 1) {


					//chanse the data to a tree
					for (int tree_wood = 0; tree_wood < 5; tree_wood++) {
						if (y >= 0 && y + tree_wood <= 31) {
							blockIdList[getpos(x, tree_wood + y, z)] = blockID::wood_log;
						}
					}
					for (int l_y = 0; l_y < 4; l_y++) {
						for (int l_z = z - 2; l_z < z + 3; l_z++) {
							for (int l_x = x - 2; l_x < x + 3; l_x++) {
								if (l_y >= 0 && l_y <= 31 && l_z >= 1 && l_z <= 31 && y + 3 > 0 && y + l_y + 3 <= 31) {
					
									blockIdList[getpos(l_x, l_y + y + 3, l_z)] = blockID::leaves;
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
				if (blockIdList[getpos(x, y, z)] != blockID::air && blockIdList[getpos(x, y, z)] != blockID::water) {

					float endres_x = pos.x + x;
					float endres_y = pos.y + y;
					float endres_z = pos.z + z;
					float block = blockIdList[getpos(x, y, z)];

					if (blockIdList[getpos(x, y + 1, z)] == blockID::air || blockIdList[getpos(x, y + 1, z)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::top, block });
					}

					if (blockIdList[getpos(x, y - 1, z)] == blockID::air || blockIdList[getpos(x, y - 1, z)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::bottom, block });
					}

					if (blockIdList[getpos(x + 1, y, z)] == blockID::air || blockIdList[getpos(x + 1, y, z)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::right, block });
					}

					if (blockIdList[getpos(x - 1, y, z)] == blockID::air || blockIdList[getpos(x - 1, y, z)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::left, block });
					}

					if (blockIdList[getpos(x, y, z + 1)] == blockID::air || blockIdList[getpos(x, y, z + 1)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::back, block });
					}

					if (blockIdList[getpos(x, y, z - 1)] == blockID::air || blockIdList[getpos(x, y, z - 1)] == blockID::water) {
						mesh.vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::front, block });
					}
				}
			}
		}
	}
}

void chunkdata::make_water_vertices() {

	for (int z = 0; z < chunkSize; z++) {
		for (int x = 0; x < chunkSize; x++) {
			for (int y = 0; y < chunkSize; y++) {
				if (waterList[getpos(x, y, z)] == true) {

					const float endres_x = pos.x + x;
					const float endres_y = pos.y + y;
					const float endres_z = pos.z + z;

					if (waterList[getpos(x, y + 1, z)] == false && blockIdList[getpos(x, y + 1, z)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::top });
					}

					if (waterList[getpos(x, y - 1, z)] == false && blockIdList[getpos(x, y - 1, z)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::bottom });
					}

					if (waterList[getpos(x + 1, y, z)] == false && blockIdList[getpos(x + 1, y, z)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::right });
					}

					if (waterList[getpos(x - 1, y, z)] == false && blockIdList[getpos(x - 1, y, z)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::left });
					}

					if (waterList[getpos(x, y, z + 1)] == false && blockIdList[getpos(x, y, z + 1)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::back });
					}

					if (waterList[getpos(x, y, z - 1)] == false && blockIdList[getpos(x, y, z - 1)] == blockID::air) {
						watermesh.vertices.push_back(WaterVertex{ endres_x, endres_y, endres_z, face::front });
					}
				}
			}
		}
	}
}

void chunkdata::gen_chunkdata(const FastNoiseLite* Noise) {

	bool is_nedad = true;
	make_noiselist(&is_nedad, Noise);

	bool is_water_nedad = true;
	make_water_noiselist(&is_water_nedad, Noise);


	if (is_nedad) {
		make_blockId(Noise);
		make_tree(Noise);
		make_vertices();
	}
	if (is_water_nedad) {
		make_water_vertices();
	}

	gen = true;
}

static void gen_chunk(chunkdata* chunk, const FastNoiseLite* Noise) {
	chunk->gen_chunkdata(Noise);
}

void chunkdata::reload() {

	mesh.clear();
	watermesh.clear();

	std::vector<Texture> textures;
	make_vertices();
	make_water_vertices();
	mesh.makeMash(textures);
	watermesh.makeMash(textures);
	gen = true;

}