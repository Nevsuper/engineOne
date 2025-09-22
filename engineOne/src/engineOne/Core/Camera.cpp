#include "pch.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
Camera::Camera(float fov, float aspect, float zNear, float zFar) noexcept
	:m_Position(0.0f, 0.0f, 3.0f),

	m_Projection(1.0f), m_View(1.0f)
	, m_Orientation(1.0f, 0.0f, 0.0f, 0.0f)

{
	UpdateProjection(fov, aspect, zNear, zFar);

}

void Camera::MoveTo(const glm::vec3& position) noexcept
{
	m_Position = position;
	RecalculateView();
}

void Camera::Translate(const glm::vec3& displacement) noexcept
{
	m_Position += displacement;
	RecalculateView();
}



void Camera::Yaw(float angle) noexcept
{
	glm::quat q = glm::angleAxis(angle, m_Worldup);
	m_Orientation = m_Orientation = glm::normalize(q * m_Orientation);
	RecalculateView();
}

void Camera::Pitch(float angle) noexcept
{
	glm::quat q = glm::angleAxis(angle, GetRight());
	m_Orientation = glm::normalize(q * m_Orientation);
	RecalculateView();
}


void Camera::MoveForward(float distance) noexcept
{
	Translate(GetForward() * distance);
}

void Camera::MoveRight(float distance) noexcept
{
	Translate(GetRight() * distance);
}

void Camera::MoveUp(float distance) noexcept
{
	Translate(m_Worldup * distance);
}

void Camera::UpdateProjection(float fov, float aspect, float zNear, float zFar) noexcept
{
	assert(aspect > 0.0f && "Aspect ratio must be greater than zero");
	m_Projection = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::RestPosAndOrient() noexcept
{
	m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
	m_Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	RecalculateView();
}

inline glm::vec3 Camera::GetForward() const noexcept
{
	glm::vec3 forward = glm::rotate(m_Orientation, m_WorldFront);
	forward.y = 0;
	return glm::normalize(forward);
}

inline glm::vec3 Camera::GetRight() const noexcept
{
	glm::vec3 right = glm::rotate(m_Orientation, m_WorldRight);
	right.y = 0;
	return glm::normalize(right);
}



void Camera::RecalculateView() noexcept
{

	glm::vec3 forward = glm::rotate(m_Orientation, m_WorldFront);
	glm::vec3 up = glm::rotate(m_Orientation, m_Worldup);
	m_View = glm::lookAt(m_Position, m_Position + forward, up);
}
