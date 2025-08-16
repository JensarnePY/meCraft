#include "UI.h"

void UI::toGPU() {

	GLfloat vertices[] =
	{ //     COORDINATES    /   TexCoord  //
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f  // Lower right corner
	};

	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2 
	};


	Shader shaderProgram("UI.vert", "UI.frag");

	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);

	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	tex.
	tex.texUnit(shaderProgram, "crossair_tex", 0);

}

void UI::render(Shader& shader) {

	shader.Activate();
	glUniform1f(shader.ID, 0.5f);
	tex.Bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}