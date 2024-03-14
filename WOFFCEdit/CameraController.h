#pragma once
#include "DisplayChunk.h"


using namespace DirectX::SimpleMath;

class InputCommands;


class CameraController
{
public:
	CameraController();
	void Update(const InputCommands& input_commands);

protected:
	void Init();
	void CreateUpdate();
	void HandleMouse(InputCommands input_commands);


	//Floats
	float m_moveSpeed;
	float m_camRotRate;

	//Vectors
	Vector3 m_camPosition;
	Vector3 m_camOrientation;
	Vector3 m_camLookAt;
	Vector3 m_camLookDirection;
	Vector3 m_camRight;

	Vector2 m_oldMouse;
	Vector2 m_newMouse;


	//Matrices
	Matrix m_view;

public:
	Matrix GetViewMatix() const { return m_view; }
	Vector3 GetPosition() const { return m_camPosition; }
};
