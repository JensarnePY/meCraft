
#include "world.h"

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

static bool s_getSideData(float x, float y, float z, const FastNoiseLite& Noise) {
	float h = (Noise.GetNoise(x, z) * 10) + pow((Noise.GetNoise(x / 10, z / 10) * 5), 2);
	//float c = s_GetNoise3D(x, y, z, Noise);
	return h > y;//&& c < 0.4f;
};

void world::gen_chunkdata(glm::vec3 pos, std::vector <Vertex>* vertices, std::vector <GLuint>* indices) {
	

	int ind_i = 0;
	for (float x = 0; x < chunkSize; x++) {
		for (float y = 0; y < chunkSize; y++) {
			for (float z = 0; z < chunkSize; z++) {
				if (s_getSideData(pos.x + x, pos.y + y, pos.z + z, Noise) == true) {

					//int dx = pos.x;
					//int dy = pos.y;
					//int dz = pos.z;

					float endres_x = pos.x + x;
					float endres_y = pos.y + y;
					float endres_z = pos.z + z;


					if (s_getSideData(endres_x, endres_y + 1, endres_z, Noise) == false) {
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

					if (s_getSideData(endres_x, endres_y - 1, endres_z, Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(1.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(1.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x + 1, endres_y, endres_z, Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x - 1, endres_y, endres_z, Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);
						indices->push_back(3 + ind_i);
						indices->push_back(1 + ind_i);
						indices->push_back(0 + ind_i);
						indices->push_back(2 + ind_i);
						indices->push_back(3 + ind_i);
						ind_i += 4;
					}

					if (s_getSideData(endres_x, endres_y, endres_z + 1, Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  1.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
						indices->push_back(0 + ind_i);																												 
						indices->push_back(3 + ind_i);																												 
						indices->push_back(1 + ind_i);																												 
						indices->push_back(0 + ind_i);																												 
						indices->push_back(2 + ind_i);																												 
						indices->push_back(3 + ind_i);																												 
						ind_i += 4;																																	 
					}																																				 
																																									 
					if (s_getSideData(endres_x, endres_y, endres_z - 1, Noise) == false) {
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  0.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(1.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f), float(2.0f) });
						vertices->push_back(Vertex{ glm::vec3(0.0f + endres_x,  1.0f + endres_y,  0.0f + endres_z), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f), float(2.0f) });
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

static bool chunkExists(const std::vector<chunkdata>& chunks, const glm::vec3& pos)
{
	return std::any_of(chunks.begin(), chunks.end(), [&pos](const chunkdata& chunk)
		{
			return chunk.pos == pos;
		});
}

void  world::update(Camera& camera, int renderDistent) {
	int i = 0;
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

				if (!chunkExists(chunk, newChunkPos)) {
					ChunkToLoad.push_back(newChunkPos);
				}
				i++;
			}
		}
	}

	//std::cout << i << " ";

	if (ChunkToLoad.size() != 0 && world_vertices.size() < 3000000) {

		auto start = glfwGetTime();

		chunk.push_back(chunkdata{ glm::vec3(ChunkToLoad[0])});
		
		int offset = world_vertices.size();
		gen_chunkdata(ChunkToLoad[0], &world_vertices, &indices);
		for (int i : indices) {
			world_indices.push_back(i + offset);
		}
		if (indices.size() > 0) {
			mesh.clear();
			mesh.makeMash(world_vertices, world_indices, textures);
		}
		//std::cout << indices.size() << " ";
		indices.clear();
		std::cout << world_vertices.size() << " : vert " << world_indices.size() << " : ind ";
		std::cout << chunk.size() << " ";
		

		std::cout << (glfwGetTime() - start) * 1000 << " ms\n";
	}
}

void  world::render(Shader& shader, Camera& camera) {
	mesh.Draw(shader, camera);
}