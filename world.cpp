
#include "world.h"

world::world() {

	Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise.SetSeed(glfwGetTime());
	Noise.SetFrequency(0.02f);
	MAX_threads_gen = std::thread::hardware_concurrency() / 2;
}

static float s_GetNoise3D(float x, float y, float z, const FastNoiseLite& Noise) {
	float ab = Noise.GetNoise(x, y);
	float bc = Noise.GetNoise(y, z);
	float ac = Noise.GetNoise(x, z);
	float ba = Noise.GetNoise(y, x);
	float cb = Noise.GetNoise(z, y);
	float ca = Noise.GetNoise(z, x);
	float abc = ab + bc + ac + ba + cb + ca;
	return abc / 6.0f;
}

const int getpos(int x, int y, int z) {
	const int size = 32 + 2;
	return x + z * size + y * size * size;
}

static void static_gen_chunkdata(glm::vec3 pos, std::vector <Vertex>* vertices, std::vector <GLuint>* indices, float chunkSize, const FastNoiseLite* Noise, bool* ready) {

	std::vector <bool> noiselist;
	noiselist.resize(pow(chunkSize + 2, 3));

	bool emty = false;
	bool foll = false;

	for (float z = 0; z < chunkSize + 2; z++) {
		for (float x = 0; x < chunkSize + 2; x++) {

			float h = (Noise->GetNoise(x + pos.x, z + pos.z) * 50);

			for (float y = 0; y < chunkSize + 2; y++) {

				noiselist[getpos(x, y, z)] = (h > y + pos.y);

				if ((h > y + pos.y) == true) foll = true;
				else emty = true;

			}
		}
	}

	if (emty == true && foll == false || emty == false && foll == true);
	{
		int ind_i = 0;
		for (int z = 0; z < chunkSize; z++) {
			for (int x = 0; x < chunkSize; x++) {
				for (int y = 0; y < chunkSize; y++) {
					if (noiselist[getpos(x + 1, y + 1, z + 1)] == true) {

						float endres_x = pos.x + x + 1;
						float endres_y = pos.y + y + 1;
						float endres_z = pos.z + z + 1;

						float block = blockID::grass_block;
						if (noiselist[getpos(x + 1, y + 2, z + 1)] == true) {

							block = blockID::dirt_block;
						}
						//std::cout << block;
						if (noiselist[getpos(x + 1, y + 2, z + 1)] == false) {
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 0.0f), int(face::top), block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 0.0f), int(face::top), block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 1.0f), int(face::top), block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 1.0f), int(face::top), block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

						if (noiselist[getpos(x + 1, y, z + 1)] == false) {
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 0.0f), face::bottom, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 0.0f), face::bottom, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 1.0f), face::bottom, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 1.0f), face::bottom, block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

						if (noiselist[getpos(x + 2, y + 1, z + 1)] == false) {
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 0.0f), face::right, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 0.0f), face::right, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 1.0f), face::right, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 1.0f), face::right, block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

						if (noiselist[getpos(x, y + 1, z + 1)] == false) {
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 0.0f), face::left, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 0.0f), face::left, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 1.0f), face::left, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 1.0f), face::left, block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

						if (noiselist[getpos(x + 1, y + 1, z + 2)] == false) {
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 0.0f), face::back, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 0.0f), face::back, block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(0.0f, 1.0f), face::back, block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec2(1.0f, 1.0f), face::back, block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

						if (noiselist[getpos(x + 1, y + 1, z)] == false) {
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 0.0f), int(face::front), block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 0.0f), int(face::front), block });
							vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(0.0f, 1.0f), int(face::front), block });
							vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec2(1.0f, 1.0f), int(face::front), block });
							indices->push_back(0 + ind_i);
							indices->push_back(3 + ind_i);
							indices->push_back(1 + ind_i);
							indices->push_back(0 + ind_i);
							indices->push_back(2 + ind_i);
							indices->push_back(3 + ind_i);
							ind_i += 4;
						}

					}
				}
			}
		}
	}

	
	noiselist.clear();
	*ready = true;
}

static bool chunkExists(const std::vector<chunkdata>& chunks, const glm::vec3& pos)
{
	for (const chunkdata& ch_pos : chunks) {
		if (ch_pos.pos == pos) {
			return true;
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

				glm::vec3 newChunkPos(
					(current_chunkX + x) * chunkSize,
					(current_chunkY + y) * chunkSize,
					(current_chunkZ + z) * chunkSize);

				ChunkToLoad.push_back(newChunkPos);
				
			}
		}
	}

	
	std::cout << ChunkToLoad.size() << " ChunkToLoad\n";
	for (auto& pos : ChunkToLoad) {

		chunk.emplace_back(glm::vec3(pos));
		chunkdata& chunk_ref = chunk.back();
		//std::cout << chunk.size() << "\n";
		std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, pos, &chunk_ref.vertices, &chunk_ref.indices, chunkSize, &Noise, &chunk_ref.gen);
	}

	for (auto& chunk : chunk) {
		chunk.mesh.clear();
		if (chunk.indices.size() != 0) {
			chunk.mesh.makeMash(chunk.vertices, chunk.indices, textures);
			chunk.render = true;
			//chunk.indices.clear();
			//chunk.vertices.clear();
		}			
		
	}
	std::cout << (glfwGetTime() - start) * 1000 << "ms to preload\n";
}

void world::update(Camera& camera, int renderDistent) {

	std::vector<glm::vec3>ChunkToLoad;
	int current_chunkX = static_cast<int>(std::floor(camera.Position.x / chunkSize));
	int current_chunkY = static_cast<int>(std::floor(camera.Position.y / chunkSize));
	int current_chunkZ = static_cast<int>(std::floor(camera.Position.z / chunkSize));
	for (int x = -renderDistent; x <= renderDistent; x++) {
		for (int y = -renderDistent / 3; y <= renderDistent / 3; y++) {
			for (int z = -renderDistent; z <= renderDistent; z++) {

				glm::vec3 newChunkPos(
					(current_chunkX + x) * chunkSize,
					(current_chunkY + y) * chunkSize,
					(current_chunkZ + z) * chunkSize);

				if (!chunkExists(chunk, newChunkPos) && ChunkToLoad.size() <= MAX_threads_gen && threads_gen < MAX_threads_gen) {
					ChunkToLoad.push_back(newChunkPos);
					threads_gen++;
				}
			}
		}
	}

	for (auto& pos : ChunkToLoad) {
		
		chunk.emplace_back(glm::vec3(pos)); 
		chunkdata& chunk_ref = chunk.back();
		std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, pos, &chunk_ref.vertices, &chunk_ref.indices, chunkSize, &Noise, &chunk_ref.gen);
	}

	for (auto& chunk : chunk) {
		if (chunk.gen == true && chunk.render == false) {
			threads_gen--;

			chunk.mesh.clear();
			chunk.mesh.makeMash(chunk.vertices, chunk.indices, textures);
			chunk.render = true;
			break;

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