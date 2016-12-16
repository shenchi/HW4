#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

namespace hw4
{
	class Camera
	{
	public:

		inline void SetPosition(const glm::vec3& pos) { position = pos; }

		inline const glm::vec3& GetPosition() const { return position; }

		inline void SetRotation(const glm::vec3& rot) { rotation = rot; }

		inline const glm::vec3& GetRotation() const { return rotation; }

		inline glm::mat4 GetRotationMatrix() const {
			return glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
		}

		inline void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
		{
			mat_proj = glm::perspective(fov, aspect, zNear, zFar);
		}

		inline glm::mat4 GetViewMatrix() const
		{
			glm::mat3 rot = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
			glm::vec3 target = position + rot * glm::vec3(0.0f, 0.0f, -1.0f);
			//glm::vec3 up = rot * glm::vec3(0.0f, 1.0f, 0.0f);
			return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		inline const glm::mat4& GetProjectionMatrix() const { return mat_proj; }

	private:
		glm::vec3	position;
		glm::vec3	rotation;
		glm::mat4	mat_proj;
	};
}
