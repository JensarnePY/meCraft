
#include"renderClasses/Mesh.h"
#include "world.h"
#include "UI.h"
#include "gen_chunk.h"

int width = 1200;
int height = 800;

struct raycastRES
{
	glm::vec3 pos;
	chunkdata* chunk;
	bool hit;
	int i;

};

raycastRES raycast(const glm::vec3 start, const glm::vec3 dir, const float max_distan, world *World, bool the_hit_bafore) {

	float curr_distan = 0.0f;

	raycastRES last_hit{ glm::vec3(0.0f), nullptr, false, 0 };

	while (curr_distan < max_distan) {

		curr_distan += 0.02f;

		glm::vec3 res_pos = start + dir * curr_distan;

		int chunkX = res_pos.x >= 0 ? res_pos.x / 32 : res_pos.x / 32 - 1;
		int chunkY = res_pos.y >= 0 ? res_pos.y / 32 : res_pos.y / 32 - 1;
		int chunkZ = res_pos.z >= 0 ? res_pos.z / 32 : res_pos.z / 32 - 1;
		chunkdata* chunk = World->getchunk(chunkX, chunkY, chunkZ);
		if (!chunk) {
			continue;
		}

		int localBlockX = ((int)floor(res_pos.x) % 32 + 32) % 32;
		int localBlockY = ((int)floor(res_pos.y) % 32 + 32) % 32;
		int localBlockZ = ((int)floor(res_pos.z) % 32 + 32) % 32;

		glm::vec3 pos(localBlockX, localBlockY, localBlockZ);
		int i = chunk->getpos(localBlockX, localBlockY, localBlockZ);

		// Check if block is solid
		if (chunk->is_solid_list[i]) {
			if (the_hit_bafore == false) return { pos, chunk, true, i }; // hit
			else return last_hit;
		}
		last_hit = raycastRES{ pos, chunk, true, i };
	}

	// No hit
	return { glm::vec3(0.0f), nullptr, false, 0 };
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	Shader shaderProgram("res/default.vert", "res/default.frag");
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 1.0f));

	/*
	605 chunks    155ms
	4800 chunks   850ms
	35301 chunks  5300ms
	269001 chunks 41200ms
	to pre load
	*/

	world World;
	World.pre_load_chunk(glm::vec3(0.0f), 10);

	//UI ui;
	//ui.toGPU();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);

	int cn = 0;
	double time = glfwGetTime();
	double time2 = glfwGetTime();

	bool lastmou_right = GLFW_RELEASE;
	bool lastmou_left = GLFW_RELEASE;

	while (!glfwWindowShouldClose(window))
	{	
		cn++;
		double timediff = glfwGetTime() - time2;
		double dt = glfwGetTime() - time;
		time = glfwGetTime();
		if (glfwGetTime() - time2 >= 0.2)
		{
			std::string FPS = std::to_string((1.0 / timediff) * cn);
			std::string ms = std::to_string((timediff / cn) * 1000);
			std::string newTitle = FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			time2 = glfwGetTime();
			cn = 0;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
			camera.setWinSize(width, height);
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			shaderProgram.Delete();
			glfwDestroyWindow(window);
			glfwTerminate();
			return 0;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && lastmou_right == GLFW_RELEASE) {
			lastmou_right = GLFW_PRESS;
			raycastRES result = raycast(camera.Position, camera.Orientation, 10.0f, &World, false);
			if (result.hit) {

				result.chunk->is_solid_list[result.chunk->getpos(result.pos.x, result.pos.y, result.pos.z)] = false;

				result.chunk->mesh.clear();
				result.chunk->vertices.clear();

				gen_chunk::reload_chunkdata(result.chunk);
				result.chunk->mesh.makeMash(result.chunk->vertices, World.textures);
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) lastmou_right = GLFW_RELEASE;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && lastmou_left == GLFW_RELEASE) {
			lastmou_left = GLFW_PRESS;
			raycastRES result = raycast(camera.Position, camera.Orientation, 10.0f, &World, true);
			if (result.hit) {

				result.chunk->is_solid_list[result.chunk->getpos(result.pos.x, result.pos.y, result.pos.z)] = true;
				result.chunk->blockIdList[result.chunk->getBlockIDPos(result.pos)] = blockID::defalt_stone;

				result.chunk->reload();
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) lastmou_left = GLFW_RELEASE;
		
		camera.Inputs(window, dt);
		camera.updateMatrix(45.0f, 0.1f, 100000.0f);


		glUniform1f(glGetUniformLocation(shaderProgram.ID, "time"), glfwGetTime());

		//World.update(camera, 2);

		auto start = glfwGetTime();
		World.render(shaderProgram, camera);
		//ui.render();
		std::cout << (glfwGetTime() - start) * 1000 << "ms rendering \n";


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
