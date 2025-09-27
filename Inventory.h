#pragma once
#include "renderClasses/Mesh.h"

struct item{
	blockID block = blockID::air;
	short Count = 0;
};

class Inventory{
private:
	GLFWwindow* m_window;
public:
	int hotbar_selected = 1;
	std::vector<item> hotbarItem;

	Inventory(GLFWwindow* window);
	void update();
};
