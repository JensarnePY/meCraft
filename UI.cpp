#include "UI.h"

UI::UI()
    : VAOID(0), VBOID(0), EBOID(0)
{
}

void UI::toGPU(std::vector<Texture>& textures) {

    GLfloat vertices[] = {
        // Positions         // TexCoords
        -0.03f, -0.03f, 0.0f,  0.0f, 0.0f, // Lower left
        -0.03f,  0.03f, 0.0f,  0.0f, 1.0f, // Upper left
         0.03f,  0.03f, 0.0f,  1.0f, 1.0f, // Upper right
         0.03f, -0.03f, 0.0f,  1.0f, 0.0f  // Lower right
    };

    GLuint indices[] = {
        0, 2, 1,
        0, 3, 2
    };

    this->textures = textures;

    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);

    // Vertex buffer
    glGenBuffers(1, &VBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer
    glGenBuffers(1, &EBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // EBO stays bound to VAO
}

void UI::render(Shader& shader, Camera& camera) {

	shader.Activate();
	glUniform1f(glGetUniformLocation(shader.ID, "scale"), 0.5f);

	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i].texUnit(shader, ("tex" + std::to_string(i)).c_str(), i);
		textures[i].Bind();
	}

    

	glBindVertexArray(VAOID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}