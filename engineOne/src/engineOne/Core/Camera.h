#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
class Camera
{
public:

	Camera(float fov = 60.0f, float aspect = 4.0f/3.0f, float zNear = 0.1f, float zFar = 100.0f) noexcept;

	void MoveTo(const glm::vec3& position) noexcept;
	void Translate(const glm::vec3& displacement) noexcept;

	void Yaw(float angle) noexcept;  
	void Pitch(float angle) noexcept; 
	

	void MoveForward(float distance) noexcept;
	void MoveRight(float distance) noexcept;
	void MoveUp(float distance) noexcept;

	void UpdateProjection(float fov, float aspect, float zNear, float zFar) noexcept;

	const glm::mat4& GetViewMatrix() const noexcept { return m_View; }
	const glm::mat4& GetProjectionMatrix() const noexcept { return m_Projection; }

	void RestPosAndOrient() noexcept;

	const glm::vec3& GetPos() const noexcept {	return m_Position;}
private:

	glm::vec3 GetForward() const noexcept;
	glm::vec3 GetRight() const noexcept;



	void RecalculateView() noexcept;
private:
	glm::mat4 m_View, m_Projection;
	glm::vec3 m_Position;
	glm::quat m_Orientation;
	const glm::vec3 m_Worldup = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 m_WorldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 m_WorldRight = glm::vec3(1.0f, 0.0f, 0.0f);
};

