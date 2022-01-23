#include "input_handling.h"

int handle_input(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) return -1;
	return 0;
}
