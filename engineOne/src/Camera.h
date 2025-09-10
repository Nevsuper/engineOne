#pragma once
#include<glm/glm.hpp>

class CameraFPS
{
public:

	CameraFPS(float fov, float aspect, float zNear, float zFar) noexcept;

	void MoveTo(const glm::vec3& position) noexcept;
	void Translate(const glm::vec3& displacement) noexcept;

	void Yaw(float angle) noexcept;   // rotate around the up vector
	void Pitch(float angle) noexcept; // rotate around the right vector
	void Roll(float angle) noexcept;  // rotate around the front vector

	void MoveForward(float distance) noexcept;
	void MoveRight(float distance) noexcept;
	void MoveUp(float distance) noexcept;

	void UpdateProjection(float fov, float aspect, float zNear, float zFar) noexcept;

	const glm::mat4& GetViewMatrix() const noexcept { return m_View; }
	const glm::mat4& GetProjectionMatrix() const noexcept { return m_Projection; }

	void RestPosAndOrient() noexcept;
private:
	void RecalculateView() noexcept;
private:
	glm::mat4 m_View, m_Projection;
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

};

