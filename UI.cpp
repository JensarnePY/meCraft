#include "UI.h"

UI::UI() : VAOID(0), VBOID(0), EBOID(0)
{
}

void UI::toGPU(std::vector<Texture>& textures) {

    GLfloat vertices[] = {
        // Positions     // TexCoords
        -1.0f, -1.0f,  0.0f, 0.0f, // Lower left
        -1.0f,  1.0f,  0.0f, 1.0f, // Upper left
         1.0f,  1.0f,  1.0f, 1.0f, // Upper right
         1.0f, -1.0f,  1.0f, 0.0f  // Lower right
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // EBO stays bound to VAO
}

void UI::update(int win_width, int win_height, glm::vec2 pos, glm::vec2 size, float rotate) {
    this->pos = pos;
    this->size = size;
    this->rotate = rotate;

    this->win_width = win_width;
    this->win_height = win_height;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));
    projection = glm::ortho(0.0f, (float)win_width, (float)win_height, 0.0f, -1.0f, 1.0f);
}

void UI::render(Shader& shader) {

	shader.Activate();
	glUniform1f(glGetUniformLocation(shader.ID, "scale"), 0.05f);
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i].texUnit(shader, ("tex" + std::to_string(i)).c_str(), i);
		textures[i].Bind();
	}

    
	glBindVertexArray(VAOID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}