#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
CameraFPS::CameraFPS(float fov, float aspect, float zNear, float zFar) noexcept
	:m_Position(0.0f, 0.0f, 3.0f),
	m_Front(0.0f, 0.0f, -1.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Right(1.0f, 0.0f, 0.0f),
	m_Projection(1.0f), m_View(1.0f)

{
	UpdateProjection(fov, aspect, zNear, zFar);

}

void CameraFPS::MoveTo(const glm::vec3& position) noexcept
{
	m_Position = position;
	RecalculateView();
}

void CameraFPS::Translate(const glm::vec3& displacement) noexcept
{
	m_Position += displacement;
	RecalculateView();
}



void CameraFPS::Yaw(float angle) noexcept
{
	m_Front = glm::rotate(m_Front, glm::radians(angle), m_Up);
	m_Right = glm::cross(m_Front, m_Up);
	RecalculateView();
}

void CameraFPS::Pitch(float angle) noexcept
{
	m_Front = glm::rotate(m_Front, glm::radians(angle), m_Right);
	m_Up = glm::cross(m_Right, m_Front);
	RecalculateView();
}

void CameraFPS::Roll(float angle) noexcept
{
	m_Up = glm::rotate(m_Up, glm::radians(angle), m_Front);
	m_Right = glm::cross(m_Front, m_Up);
	RecalculateView();
}

void CameraFPS::MoveForward(float distance) noexcept
{
	Translate(m_Front * distance);
}

void CameraFPS::MoveRight(float distance) noexcept
{
	Translate(m_Right * distance);
}

void CameraFPS::MoveUp(float distance) noexcept
{
	Translate(m_Up * distance);
}

void CameraFPS::UpdateProjection(float fov, float aspect, float zNear, float zFar) noexcept
{
	m_Projection = glm::perspective(fov, aspect, zNear, zFar);
}

void CameraFPS::RestPosAndOrient() noexcept
{
	m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
	RecalculateView();
}

void CameraFPS::RecalculateView() noexcept
{
	m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
