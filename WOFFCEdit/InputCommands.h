#pragma once

struct InputCommands
{
	//Camera Controls
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;
	bool rotUp;
	bool rotDown;

	float mouseX;
	float mouseY;

	bool LMB;
	bool RMB;

	//Non Camera Controls
	bool shiftDown;
	bool shiftUp;
};
