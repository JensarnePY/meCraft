
#include"renderClasses/Mesh.h"
#include "world.h"

int width = 1200;
int height = 800;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	//make a mesh


	std::vector<Vertex> vertices =
	{ //               COORDINATES       /           NORMALS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(0.0f,   100.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), float(0.0f)},
		Vertex{glm::vec3(0.0f,   0.0f,   0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), float(0.0f)},
		Vertex{glm::vec3(100.0f, 0.0f,   0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), float(0.0f)},
		Vertex{glm::vec3(100.0f, 100.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), float(0.0f)},
	};
	
	std::vector<GLuint>indices{
		0, 1, 2,
		0, 2, 3,

	};

	std::vector<Texture> textures{
		Texture("res/test_img.png", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("res/default.vert", "res/default.frag");
	Mesh floor(vertices, indices, textures);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 1.0f));

	world World;
	World.pre_load_chunk(camera.Position, 10);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);

	int cn = 0;
	double time = glfwGetTime();
	double time2 = glfwGetTime();

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{	

		cn++;
		double timediff = glfwGetTime() - time2;
		double dt = glfwGetTime() - time;
		time = glfwGetTime();
		if (glfwGetTime() - time2 >= 0.25)
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

		camera.Inputs(window, dt);
		camera.updateMatrix(45.0f, 0.1f, 100000.0f);

		

		//World.update(camera, 5);
		World.render(shaderProgram, camera);

		glDisable(GL_DEPTH_TEST);
		floor.Draw(shaderProgram, camera);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
