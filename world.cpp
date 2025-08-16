
#include "world.h"

world::world() {

	Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise.SetSeed(1337);
	Noise.SetFrequency(0.05f);
}



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

inline int getpos(int x, int y, int z) noexcept {
	const int size = 32 + 2;
	x += 1;
	y += 1;
	z += 1;
	return x + z * size + y * size * size;
}

chunkdata* world::getchunk(int x, int y, int z)  {

	x *= chunkSize;
	y *= chunkSize;
	z *= chunkSize;

	for (chunkdata& ch : chunk) {
		if (ch.pos == glm::vec3(x,y,z)) {
			return &ch;
		}
	}
	return nullptr;
}

const float noise(float x, float z, const FastNoiseLite* Noise) {
	return Noise->GetNoise(x / 4, z / 4) * 30 + Noise->GetNoise(x / 15, z / 15) * 200;
}

void static_gen_chunkdata(chunkdata* chunk, const FastNoiseLite* Noise) {

	// 605 ish 306ms

	bool emty = false;
	bool foll = false;
	bool furst = false;
	if (chunk->noiselist.size() == 0) {
		furst = true;
		chunk->noiselist.resize(pow(chunk->chunkSize + 2, 3));
		chunk->blockIdList.resize(pow(chunk->chunkSize, 3));

		for (float z = 0; z < chunk->chunkSize + 2; z++) {
			for (float x = 0; x < chunk->chunkSize + 2; x++) {

				float h = noise(x + chunk->pos.x, z + chunk->pos.z, Noise);

				for (float y = 0; y < chunk->chunkSize + 2; y++) {

					chunk->noiselist[getpos(x - 1, y - 1, z - 1)] = (h > y + chunk->pos.y);

					if ((h > y + chunk->pos.y) == true) foll = true;
					else emty = true;

				}
			}
		}
	}
	else {
		foll = true;
	}
	if (emty == true && foll == false || emty == false && foll == true);
	{
		int ind_i = 0;
		for (int z = 0; z < chunk->chunkSize; z++) {
			for (int x = 0; x < chunk->chunkSize; x++) {
				for (int y = 0; y < chunk->chunkSize; y++) {
					if (chunk->noiselist[getpos(x, y, z)] == true) {

						float endres_x = chunk->pos.x + x;
						float endres_y = chunk->pos.y + y;
						float endres_z = chunk->pos.z + z;

						if (furst) {
							float _block = blockID::air;
							if (chunk->noiselist[getpos(x, y, z)] == true) _block = blockID::grass_block;
							if (chunk->noiselist[getpos(x, y + 1, z)] == true) _block = blockID::dirt_block;
							chunk->blockIdList[chunk->getBlockIDPos(x, y, z)] = _block;
						}

						float block = chunk->blockIdList[chunk->getBlockIDPos(x, y, z)];
						
						if (chunk->noiselist[getpos(x, y + 1, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::top, block });
						}

						if (chunk->noiselist[getpos(x, y - 1, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::bottom, block });
						}

						if (chunk->noiselist[getpos(x + 1, y , z )] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::right, block });
						}

						if (chunk->noiselist[getpos(x - 1, y, z)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::left, block });
						}

						if (chunk->noiselist[getpos(x , y , z + 1)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::back, block });
						}

						if (chunk->noiselist[getpos(x , y , z - 1)] == false) {
							chunk->vertices.push_back(Vertex{ endres_x, endres_y, endres_z, face::front, block });
						}

					}
				}
			}
		}
	}
	chunk->gen = true;
}

void world::gen_chunkdata(chunkdata* chunk, const FastNoiseLite* Noise) {
	static_gen_chunkdata(chunk, Noise);
}

void chunkdata::reload() {

	mesh.clear();
	vertices.clear();

	std::vector<Texture> textures{
			Texture("res/grass_top.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/stone.png", 3, GL_RGBA, GL_UNSIGNED_BYTE),
	};
	FastNoiseLite N;
	static_gen_chunkdata(this, &N);
	mesh.makeMash(vertices, textures);

}

static bool chunkExists(const std::vector<chunkdata>& chunks, const glm::vec3& pos)
{
	for (const chunkdata& ch_pos : chunks) {
		if (ch_pos.pos == pos) {
			return true;
			break;
		}
	}
	return false;
}

void world::pre_load_chunk(glm::vec3 pos, int renderDistent) {
	auto start = glfwGetTime();

	std::vector<glm::vec3>ChunkToLoad;
	int current_chunkX = static_cast<int>(std::floor(pos.x / chunkSize));
	int current_chunkY = static_cast<int>(std::floor(pos.y / chunkSize));
	int current_chunkZ = static_cast<int>(std::floor(pos.z / chunkSize));
	for (int x = -renderDistent; x <= renderDistent; x++) {
		for (int y = -renderDistent / 2; y <= renderDistent / 2; y++) {
			for (int z = -renderDistent; z <= renderDistent; z++) {
					
				ChunkToLoad.push_back(glm::vec3(
												(current_chunkX + x) * chunkSize,
												(current_chunkY + y) * chunkSize,
												(current_chunkZ + z) * chunkSize));
			}
		}
	}

	std::cout << ChunkToLoad.size() << " ChunkToLoad\n";

	// start to load the mesh in threads
	for (auto& pos : ChunkToLoad) {
		chunk.emplace_back(glm::vec3(pos));
		chunkdata& chunk_ref = chunk.back();
		std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, &chunk_ref, &Noise);
	}

	// load the mesh to the GPU
	for (auto& chunk : chunk) {
		if (chunk.noiselist.size() != 0) {
			chunk.mesh.makeMash(chunk.vertices, textures);
			chunk.render = true;
			chunk.loaded_to_gpu = true;
		}
	}
	std::cout << (glfwGetTime() - start) * 1000 << "ms to preload\n";
	std::cout << ((glfwGetTime() - start) * 1000) / ChunkToLoad.size() << "ms per chunk\n";
}

void world::update(Camera& camera, int renderDistent) {

	if (thr_ready == true) {

		std::vector<glm::vec3>ChunkToLoad;
		int current_chunkX = static_cast<int>(std::floor(camera.Position.x / chunkSize));
		int current_chunkY = static_cast<int>(std::floor(camera.Position.y / chunkSize));
		int current_chunkZ = static_cast<int>(std::floor(camera.Position.z / chunkSize));
		for (int x = -renderDistent; x <= renderDistent; x++) {
			for (int y = -renderDistent / 2; y <= renderDistent / 2; y++) {
				for (int z = -renderDistent; z <= renderDistent; z++) {

					glm::vec3 newChunkPos(
						(current_chunkX + x) * chunkSize,
						(current_chunkY + y) * chunkSize,
						(current_chunkZ + z) * chunkSize);

					if (!chunkExists(chunk, newChunkPos) && ChunkToLoad.size() <= 2 && threads_gen < MAX_threads_gen) {
						ChunkToLoad.push_back(newChunkPos);
						threads_gen++;
					}
				}
			}
		}

		for (auto& pos : ChunkToLoad) {
			chunk.emplace_back(glm::vec3(pos));
			chunkdata& chunk_ref = chunk.back();
			std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, &chunk_ref, &Noise);
			threads_gen--;
		}
		
		// load the mesh to the GPU
		for (auto& chunk : chunk) {
			//std::cout << chunk.gen;
			if (chunk.gen == true && chunk.vertices.size() != 0 && chunk.loaded_to_gpu == false) {
				chunk.mesh.clear();
				chunk.mesh.makeMash(chunk.vertices, textures);
				chunk.render = true;
				chunk.loaded_to_gpu = true;
				
			}
		}
	}
}

void  world::render(Shader& shader, Camera& camera) {
	for (auto& chunk : chunk) {
		if (chunk.render == true) {
			chunk.mesh.Draw(shader, camera);
		}
	}
}