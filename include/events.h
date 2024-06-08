#pragma once
#include "glm/glm.hpp"
#include "error.h"

enum MOUSE_STATE : int
{
	PRESSED,
	RELEASED,
	INVALID = -1
};

class events
{
public:
	events() = default;
	events(GLFWwindow* window);
	~events() = default;

	bool mouseMoved();
	bool checkKey(int key);
	MOUSE_STATE checkmouseButtons(int button);
	float const* getCusorOffset();
	glm::vec2 getCursorPos();

	inline int getMouseButton()
	{
		return pressedMouseButton;
	}

public:
	static bool isMouseActive;
	static int pressedKey;
	static int pressedMouseButton;
	static float xyCusorPos[2];
	static float lastXYCusorPos[2];
	static MOUSE_STATE mouse_state;
};


