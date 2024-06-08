#include "events.h"
#include "glm/glm.hpp"

bool events::isMouseActive = false;
int events::pressedKey = 0;
float events::xyCusorPos[2] = { 0, 0 };
float events::lastXYCusorPos[2] = { 0, 0 };
int events::pressedMouseButton = -1;
MOUSE_STATE events::mouse_state = RELEASED;

float lastX = 1000.0f / 2.0f;
float lastY = 1000.0f / 2.0f;
bool firstMouse = true;

static void setKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		events::pressedKey = key;
	}
}

static void setCusorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	events::xyCusorPos[0] = xoffset;
	events::xyCusorPos[1] = yoffset;
}


static void setMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		events::mouse_state = PRESSED;
		events::pressedMouseButton = button;
	}

	else if (action == GLFW_RELEASE)
	{
		events::mouse_state = RELEASED;
		events::pressedMouseButton = -1;
	}
}


events::events(GLFWwindow* window)
{
	glfwSetKeyCallback(window, (GLFWkeyfun)setKey);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)setCusorPos);
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)setMouseButtonCallBack);
}


bool events::mouseMoved()
{
	if (events::lastXYCusorPos[0] != events::xyCusorPos[0])
	{
		events::lastXYCusorPos[0] = events::xyCusorPos[0];
		events::lastXYCusorPos[1] = events::xyCusorPos[1];

		return true;
	}
	else
	{
		return false;
	}
}

bool events::checkKey(int key)
{
	if (key == pressedKey)
	{
		pressedKey = GL_ZERO;
		return true;
	}
	return false;
}

MOUSE_STATE events::checkmouseButtons(int button)
{
	if (pressedMouseButton == button && mouse_state == PRESSED)
	{
		return PRESSED;
	}

	else if (pressedMouseButton == button && mouse_state == MOUSE_STATE::RELEASED)
	{
		return RELEASED;
	}

	else
	{
		return INVALID;
	}
}

float const* events::getCusorOffset()
{
	return xyCusorPos;
}

glm::vec2 events::getCursorPos()
{
	return { lastX, lastY };
}
