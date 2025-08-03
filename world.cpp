
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

static bool s_getSideData(const float x, const float y, const float z, const FastNoiseLite& Noise) {
	float h = (Noise.GetNoise(x, z) * 50);// +pow((Noise.GetNoise(x / 10, z / 10) * 5), 2);
	//float c = s_GetNoise3D(x, y, z, Noise);
	return h > y;//&& c < 0.4f;
};

void static_gen_chunkdata(glm::vec3 pos, std::vector <Vertex>* vertices, std::vector <GLuint>* indices, float chunkSize, const FastNoiseLite *Noise, bool* ready) {

	int ind_i = 0;
	for (float x = 0; x < chunkSize; x++) {
		for (float y = 0; y < chunkSize; y++) {
			for (float z = 0; z < chunkSize; z++) {
				if (s_getSideData(pos.x + x, pos.y + y, pos.z + z, *Noise) == true) {

					float endres_x = pos.x + x;
					float endres_y = pos.y + y;
					float endres_z = pos.z + z;


					if (s_getSideData(endres_x, endres_y + 1, endres_z, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(0.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(0.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(0.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(0.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x, endres_y - 1, endres_z, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(1.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x + 1, endres_y, endres_z, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x - 1, endres_y, endres_z, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x, endres_y, endres_z + 1, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);																												 
						indices->push_back(3 + ind_i);																												 
						indices->push_back(1 + ind_i);																												 
						indices->push_back(0 + ind_i);																												 
						indices->push_back(2 + ind_i);																												 
						indices->push_back(3 + ind_i);																												 
						ind_i += 4;																																	 
					}																																				 
				
					if (s_getSideData(endres_x, endres_y, endres_z - 1, *Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 0.0f,-1.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 0.0f,-1.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 0.0f,-1.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 0.0f,-1.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
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

	*ready = true;
	//std::cout << "ready ";
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
		for (int y = -renderDistent / 3; y <= renderDistent / 3; y++) {
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
		std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, pos, &chunk_ref.vertices, &chunk_ref.indices, chunkSize, &Noise, &chunk_ref.gen);
	}

	for (auto& chunk : chunk) {
		chunk.mesh.clear();
		if (chunk.indices.size() != 0) {
			chunk.mesh.makeMash(chunk.vertices, chunk.indices, textures);
		}			
		chunk.render = true;
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

	//auto start = glfwGetTime();
	std::cout << ChunkToLoad.size() << "\n";
	for (auto& pos : ChunkToLoad) {
		
		chunk.emplace_back(glm::vec3(pos)); 
		chunkdata& chunk_ref = chunk.back();
		std::future<void> mesh_fut = std::async(std::launch::async, static_gen_chunkdata, pos, &chunk_ref.vertices, &chunk_ref.indices, chunkSize, &Noise, &chunk_ref.gen);
	}

	//std::cout << (glfwGetTime() - start) * 1000 << "ms\n";

	for (auto& chunk : chunk) {
		if (chunk.gen == true && chunk.render == false) {
			threads_gen--;

			chunk.mesh.clear();
			//if(chunk.indices.size()
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