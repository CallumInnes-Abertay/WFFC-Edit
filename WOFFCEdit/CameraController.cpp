#include "CameraController.h"
#include "InputCommands.h"

CameraController::CameraController()
{
	Init();
}

// Initalise camera settings
void CameraController::Init()
{
	//Default values
	m_moveSpeed = 0.30;
	m_camRotRate = 3.0;

	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;

	//CreateUpdate();
}

void CameraController::CreateUpdate()
{
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}


void CameraController::Update(const InputCommands& input_commands)
{
	HandleMouse(input_commands);


	if (input_commands.rotRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (input_commands.rotLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}

	//create look direction from Euler angles 
	m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180);

	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	// Update camera position based on input commands (basic WASD movement)
	if (input_commands.forward)
	{
		m_camPosition += m_camLookDirection * m_moveSpeed;
	}
	if (input_commands.back)
	{
		m_camPosition -= m_camLookDirection * m_moveSpeed;
	}
	if (input_commands.right)
	{
		m_camPosition += m_camRight * m_moveSpeed;
	}
	if (input_commands.left)
	{
		m_camPosition -= m_camRight * m_moveSpeed;
	}

	m_camLookAt = m_camPosition + m_camLookDirection;


	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}

void CameraController::HandleMouse(const InputCommands& input_commands)
{
	m_newMouse = Vector2(input_commands.mouseX, input_commands.mouseY);

	//Only allow mouse movement if holding right mouse button
	if (input_commands.RMB)
	{
		Vector2 Difference;

		// Calculate mouse movement difference
		Difference.x = m_newMouse.x - m_oldMouse.x;
		Difference.y = m_newMouse.y - m_oldMouse.y;

		Difference.Normalize();

		// Adjust camera orientation based on mouse movement
		if (Difference.x != 0 || Difference.y != 0)
		{
			// yaw
			m_camOrientation.y -= m_camRotRate * Difference.x;
			// pitch
			m_camOrientation.x -= m_camRotRate * Difference.y;
		}

		float cosR, cosP, cosY;
		float sinR, sinP, sinY;

		// Calculate new look direction based on Euler angles
		cosP = cosf(m_camOrientation.x * (3.1415 / 180));
		cosY = cosf(m_camOrientation.y * (3.1415 / 180));
		cosR = cosf(m_camOrientation.z * (3.1415 / 180));

		sinP = sinf(m_camOrientation.x * (3.1415 / 180));
		sinY = sinf(m_camOrientation.y * (3.1415 / 180));
		sinR = sinf(m_camOrientation.z * (3.1415 / 180));

		m_camLookDirection.x = sinY * cosP;
		m_camLookDirection.y = sinP;
		m_camLookDirection.z = cosP * -cosY;

		m_camLookAt = m_camPosition + m_camLookDirection;

		m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
	}


	m_oldMouse = m_newMouse;
}

void CameraController::FocusCamera(const Vector3& targetPosition)
{
	// Move the camera to look down on and to the side of the object
	m_camPosition.x = targetPosition.x;
	m_camPosition.y = targetPosition.y + 1;
	m_camPosition.z = targetPosition.z - 3;

	// Then change rotation
	LookAt(targetPosition);
}

// Set camera orientation to look at a specific target (the object)
void CameraController::LookAt(const Vector3& target)
{
	// Calculate differences in coordinates
	float dx = target.x - m_camPosition.x;
	float dy = target.y - m_camPosition.y;
	float dz = target.z - m_camPosition.z;

	// Calculate yaw 
	m_camOrientation.y = atan2(dx, dz) * (180.0f / 3.1415);

	// Calculate pitch 
	const float distance = sqrt(dx * dx + dz * dz);
	m_camOrientation.x = -atan2(dy, distance) * (180.0f / 3.1415);
}
