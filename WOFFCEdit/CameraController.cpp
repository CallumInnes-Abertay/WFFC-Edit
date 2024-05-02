#include "CameraController.h"
#include "InputCommands.h"

CameraController::CameraController()
{
	Init();
}

void CameraController::Init()
{
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

	if (input_commands.RMB)
	{
		Vector2 Difference;

		Difference.x = m_newMouse.x - m_oldMouse.x;
		Difference.y = m_newMouse.y - m_oldMouse.y;

		Difference.Normalize();

		if (Difference.x != 0 || Difference.y != 0)
		{
			// yaw
			m_camOrientation.y -= m_camRotRate * Difference.x;
			// pitch
			m_camOrientation.x -= m_camRotRate * Difference.y;
		}

		float cosR, cosP, cosY;
		float sinR, sinP, sinY;

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
