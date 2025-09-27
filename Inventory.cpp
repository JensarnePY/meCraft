#include "Inventory.h"


Inventory::Inventory(GLFWwindow* window) {
	m_window = window;
	hotbarItem.resize(9);
}


void Inventory::update() {

	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) hotbar_selected = 1;
	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) hotbar_selected = 2;
	if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS) hotbar_selected = 3;
	if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS) hotbar_selected = 4;
	if (glfwGetKey(m_window, GLFW_KEY_5) == GLFW_PRESS) hotbar_selected = 5;
	if (glfwGetKey(m_window, GLFW_KEY_6) == GLFW_PRESS) hotbar_selected = 6;
	if (glfwGetKey(m_window, GLFW_KEY_7) == GLFW_PRESS) hotbar_selected = 7;
	if (glfwGetKey(m_window, GLFW_KEY_8) == GLFW_PRESS) hotbar_selected = 8;
	if (glfwGetKey(m_window, GLFW_KEY_9) == GLFW_PRESS) hotbar_selected = 9;
	
	for (auto& i : hotbarItem) {
		if (i.Count == 1) {
			i.block == blockID::air;
		}
	}

}

