#pragma once
#include "DisplayChunk.h"


using namespace DirectX::SimpleMath;

struct InputCommands;


class CameraController
{
public:
	CameraController();
	void Update(const InputCommands& input_commands);
	void FocusCamera(const Vector3& targetPosition);
	void LookAt(const Vector3& target);

protected:
	void Init();
	void CreateUpdate();
	void HandleMouse(const InputCommands& inputCommands);


	//Floats
	float m_moveSpeed;
	float m_camRotRate;

	//Vectors
	Vector3 m_camPosition;
	Vector3 m_camOrientation;
	Vector3 m_camLookAt;
	Vector3 m_camLookDirection;
	Vector3 m_camRight;

	Vector2 m_oldMouse_pos;
	Vector2 m_newMousePosition;


	//Matrices
	Matrix m_view;

public:
	Matrix GetViewMatix() const { return m_view; }
	Vector3 GetPosition() const { return m_camPosition; }
};
