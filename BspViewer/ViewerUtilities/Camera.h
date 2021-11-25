#ifndef CAMERA_H
#define CAMERA_H

#include "glew.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <bitset>
#include "WorldSpawn.h"

namespace Camera {

	enum Input : std::size_t {
		FWRD = 0,
		BACK = 1,
		LEFT = 2,
		RGHT = 3,
		SPCE = 4,
		CTRL = 5
	};

	struct MoveInputs {
		int mouseXMove;
		int mouseYMove;
		std::bitset<6> moveKeys;
		bool speedKey;
	};

	struct MoveParams {
		glm::vec3 velocity, target_velocity;
	};

	struct MoveOpts {
		float acceleration_speed;
	};

	struct LookParams {
		float yaw, pitch;

		glm::vec3 position;			// View origin
		glm::vec3 front, up, right;	// Directions
	};

	struct LookOpts {
		float mouse_x_sensitivity,  mouse_y_sensitivity;
		float field_of_view;

		glm::vec3 focus;			// Focus of view; norm ( focus - position ) should be front
	};

	struct WorldOpts {
		glm::vec3 world_up;
	};

	const float DEFAULT_YAW = -90.0f;
	const float DEFAULT_PITCH = 0.0f;
	const float DEFAULT_MOUSE_X_SENSITIVITY = 0.5f;
	const float DEFAULT_MOUSE_Y_SENSITIVITY = DEFAULT_MOUSE_X_SENSITIVITY;
	const float DEFAULT_FOV = 100.0f;

	// Speed change val such that it takes that amount of seconds to change course from going one way to completely opposite way.
	const float SPEED = 1.0f;
	const float VEL_CHANGE_SPEED = 0.25f;

	class Camera
	{
		public:

			MoveParams move;
			LookParams look;
			LookOpts look_opts;
			MoveOpts move_opts;
			WorldOpts world_opts;

			Camera ( glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f) )
			{
				move_opts.acceleration_speed = VEL_CHANGE_SPEED;

				look_opts.field_of_view = DEFAULT_FOV;
				look_opts.focus = glm::vec3(0.0);
				look_opts.mouse_x_sensitivity = DEFAULT_MOUSE_X_SENSITIVITY;
				look_opts.mouse_y_sensitivity = DEFAULT_MOUSE_Y_SENSITIVITY;

				world_opts.world_up = glm::vec3(0.0f, 1.0f, 0.0f),

				move.target_velocity = move.velocity = glm::vec3(0.0);

				look.yaw = DEFAULT_YAW;
				look.pitch = DEFAULT_PITCH;
				look.position = pos;

				updateCameraVectors();
			}

			// returns the view matrix calculated using Euler Angles and the LookAt Matrix
			glm::mat4 GetViewMatrix()
			{
				return glm::lookAt(look.position, look.position + look.front, look.up);
			}

			void ResetZoom() {
				look_opts.field_of_view = DEFAULT_FOV;
			}

			void ProcessKeyboardAccel(MoveInputs input, float deltaTime)
			{
				if (input.moveKeys.any())
				{
					move.target_velocity =  (float)(input.moveKeys.test(Input::FWRD) - input.moveKeys.test(Input::BACK)) * look.front;
					move.target_velocity += (float)(input.moveKeys.test(Input::RGHT) - input.moveKeys.test(Input::LEFT)) * look.right;
					move.target_velocity += (float)(input.moveKeys.test(Input::SPCE) - input.moveKeys.test(Input::CTRL)) * world_opts.world_up;
					move.target_velocity *= SPEED;
					move.target_velocity += move.target_velocity * static_cast < float > ( input.speedKey ) * 10.0f;
				}
				else
					move.target_velocity = glm::vec3(0.0);

				float multiplier = (SPEED / VEL_CHANGE_SPEED * 2) * deltaTime;
				glm::vec3 DiffVelocity = move.target_velocity - move.velocity;
				if (glm::length(DiffVelocity) > multiplier)
					move.velocity += multiplier * glm::normalize(DiffVelocity);
				else
					move.velocity = move.target_velocity;

				look.position += move.velocity * deltaTime;
			}

			void ProcessMouse(float xoffset, float yoffset)
			{
				look.yaw	+= xoffset * look_opts.mouse_x_sensitivity;
				look.pitch	-= yoffset * look_opts.mouse_y_sensitivity;
				
				// make sure that when pitch is out of bounds, screen doesn't get flipped
				look.pitch = fmin(look.pitch, 89.0f);
				look.pitch = fmax(look.pitch, -89.0f);

				updateCameraVectors();
			}

		private:

			// calculates the front vector from the Camera's (updated) Euler Angles
			void updateCameraVectors()
			{
				// calculate the new Front vector
				look.front.x = cos(glm::radians(look.yaw)) * cos(glm::radians(look.pitch));
				look.front.y = sin(glm::radians(look.pitch));
				look.front.z = sin(glm::radians(look.yaw)) * cos(glm::radians(look.pitch));
				look.front = glm::normalize(look.front);

				// also re-calculate the Right and Up vector
				look.right = glm::normalize(glm::cross(look.front, world_opts.world_up));
				look.up = glm::normalize(glm::cross(look.right, look.front));
			}
	};
}
#endif