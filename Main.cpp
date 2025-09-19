
#include"renderClasses/Mesh.h"
#include "world.h"
#include "UI.h"
#include "chunk.h"

int width = 1200;
int height = 800;


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
	Shader watershader("res/water.vert", "res/water.frag");
	Shader uishader("res/UI.vert", "res/UI.frag");
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	watershader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(watershader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 1.0f));

	/*
	5 chunks    650ms
	10 chunks   5100ms
	20 chunks  38200ms
	40 chunks N/Ams
	to pre load
	*/

	world World;
	World.pre_load_chunk(glm::vec3(0.0f), 5);

	

	UI ui(window);

	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

		

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && lastmou_right == GLFW_RELEASE) {
			lastmou_right = GLFW_PRESS;
			raycastRES result = World.raycast(camera.Position, camera.Orientation, 50.0f, false);
			if (result.hit) {

				result.chunk->blockIdList[result.chunk->getpos(result.pos.x, result.pos.y, result.pos.z)] = blockID::air;

				//if (result.pos.x - 1) {
				//	chunkdata* temp_chunk = World.getchunk(result.pos.x - 1, result.pos.y, result.pos.z);
				//	temp_chunk->blockIdList[temp_chunk->getpos(result.pos.x - 1, result.pos.y, result.pos.z)] = blockID::air;
				//}

				result.chunk->reload();
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) lastmou_right = GLFW_RELEASE;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && lastmou_left == GLFW_RELEASE) {
			lastmou_left = GLFW_PRESS;
			raycastRES result = World.raycast(camera.Position, camera.Orientation, 50.0f, true);
			if (result.hit) {

				result.chunk->blockIdList[result.chunk->getpos(result.pos.x, result.pos.y, result.pos.z)] = (float)blockID::defalt_stone;
				result.chunk->reload();
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) lastmou_left = GLFW_RELEASE;

		if (camera.exit()) return 0;
		

		camera.Inputs(window, dt);
		camera.updateMatrix(45.0f, 0.1f, 100000.0f);

		shaderProgram.Activate();
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "time"), (float)glfwGetTime());
		watershader.Activate();
		glUniform1f(glGetUniformLocation(watershader.ID, "time"), (float)glfwGetTime());
		glUniform3f(glGetUniformLocation(watershader.ID, "camDir"), camera.Orientation.x,
																	camera.Orientation.y,
																	camera.Orientation.z);

		World.update(camera, 3);

		auto start = glfwGetTime();

		
		World.render(shaderProgram, watershader, camera);
		
		ui.render(uishader);
		//std::cout << (glfwGetTime() - start) * 1000 << "ms rendering \n";


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}