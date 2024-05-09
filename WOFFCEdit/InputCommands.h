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

	//Mouse Controls
	float mouseX;
	float mouseY;

	bool LMB;
	bool RMB;

	//Non Camera Controls
	bool shiftDown;
	bool controlDown;

	// Arrow key controls 
	bool upArrowDown;
	bool downArrowDown;
	bool leftArrowDown;
	bool rightArrowDown;
};
