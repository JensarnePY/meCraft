#include "Mesh.h"

Mesh::Mesh(){

}

const void Mesh::makeMash(std::vector <Vertex>& vertices, std::vector <Texture>& textures) {

	Mesh::vertices = vertices;
	Mesh::textures = textures;


	glGenVertexArrays(1, &EmptyVAOID);
	glBindVertexArray(EmptyVAOID);

	// Create SSBO vertex binding = 0
	glCreateBuffers(1, &SSBOvert);
	glNamedBufferStorage(
		SSBOvert,
		sizeof(Vertex) * Mesh::vertices.size(),
		Mesh::vertices.data(),
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOvert);

}

void Mesh::clear() {
	glDeleteVertexArrays(1, &EmptyVAOID);
	glDeleteBuffers(1, &SSBOvert);
	glDeleteBuffers(1, &SSBOind);
	SSBOvert = 0;
	SSBOind = 0;
	EmptyVAOID = 0;
	vertices.clear();
	textures.clear();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Activate();

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i].texUnit(shader, ("tex" + std::to_string(i)).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glBindVertexArray(EmptyVAOID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOvert);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size()*6);
}