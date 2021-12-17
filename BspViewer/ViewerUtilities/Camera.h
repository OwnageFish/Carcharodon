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
		std::bitset<6> moveKeys;
		bool speedKey;

		MoveInputs() : moveKeys(0b000000), speedKey(false) {}
	};

	struct MoveParams {
		glm::vec3 velocity, target_velocity;
	};

	struct MoveOpts {
		float acceleration_value;
	};

	struct LookInputs {
		int mouseXMove, mouseYMove;
	};

	const float DEFAULT_YAW = -90.0f;
	const float DEFAULT_PITCH = 0.0f;

	struct LookParams {
		float yaw, pitch;

		glm::vec3 position;			// View origin
		glm::vec3 front, up, right;	// Directions
		glm::vec3 focus;			// Focus of view; norm ( focus - position ) should be front
	};

	const float DEFAULT_MOUSE_X_SENSITIVITY = 0.35f;
	const float DEFAULT_MOUSE_Y_SENSITIVITY = DEFAULT_MOUSE_X_SENSITIVITY;
	const float DEFAULT_NEAR_Z_PLANE = 0.01f;
	const float DEFAULT_FAR_Z_PLANE = 1000.0f;
	const float DEFAULT_FOV = 100.0f;

	struct LookOpts {
		float mouse_x_sensitivity,  mouse_y_sensitivity;
		float near_z_plane, far_z_plane;
		float field_of_view;
	};

	const glm::vec3 DEFAULT_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	struct WorldOpts {
		glm::vec3 world_up;
	};

	// Speed change val such that it takes that amount of seconds to change course from going one way to completely opposite way.

	// 
	// 
	// 
	// !!!! CHANGE THIS !!!!
	// 
	// 
	// 
	const float CAM_MOVE_SPEED_MULT = 4.0f;
	const float CAM_MOVE_ACCEL = 10.0f;

	class Object
	{
		public:

			MoveParams move;
			LookParams look;
			LookOpts look_opts;
			MoveOpts move_opts;
			WorldOpts world_opts;

			Object ( glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float y = DEFAULT_YAW, float p = DEFAULT_PITCH )
			{
				move_opts.acceleration_value = CAM_MOVE_ACCEL;

				move.target_velocity = move.velocity = glm::vec3(0.0);

				look.yaw = y;
				look.pitch = p;
				look.position = pos;
				look.focus = glm::vec3(0.0);

				look_opts.mouse_x_sensitivity = DEFAULT_MOUSE_X_SENSITIVITY;
				look_opts.mouse_y_sensitivity = DEFAULT_MOUSE_Y_SENSITIVITY;
				look_opts.near_z_plane = DEFAULT_NEAR_Z_PLANE;
				look_opts.far_z_plane = DEFAULT_FAR_Z_PLANE;
				look_opts.field_of_view = DEFAULT_FOV;

				world_opts.world_up = DEFAULT_WORLD_UP;

				updateCameraVectors();
			}

			void ProcessKeyboardAccel(MoveInputs input, float deltaTime)
			{
				// Find target velocity and direction
				if (input.moveKeys.any())
				{
					move.target_velocity =  (float)(input.moveKeys.test(Input::FWRD) - input.moveKeys.test(Input::BACK)) * look.front;
					move.target_velocity += (float)(input.moveKeys.test(Input::RGHT) - input.moveKeys.test(Input::LEFT)) * look.right;
					move.target_velocity += (float)(input.moveKeys.test(Input::SPCE) - input.moveKeys.test(Input::CTRL)) * world_opts.world_up;
					if (input.speedKey)
						move.target_velocity *= CAM_MOVE_SPEED_MULT;
				}
				else
					move.target_velocity = glm::vec3(0.0);

				// Adjust current velocity by resulting speed change over time (given acceleration and time difference)
				float multiplier = move_opts.acceleration_value * deltaTime;
				if (input.speedKey)
					multiplier *= CAM_MOVE_SPEED_MULT;
				
				glm::vec3 DiffVel = move.target_velocity - move.velocity;
				if (glm::length(DiffVel) > multiplier) // Adjust velocity to approach target as much as possible given accel
					move.velocity += multiplier * glm::normalize(DiffVel);
				else // We can reach target velocity immediately
					move.velocity = move.target_velocity;

				// Adjust position due to the newly selected velocity
				look.position += move.velocity * deltaTime;
			}


			// returns the view matrix calculated using Euler Angles and the LookAt Matrix
			glm::mat4 GetViewMatrix()
			{
				return glm::lookAt(look.position, look.position + look.front, look.up);
			}

			void ProcessMouse(float xoffset, float yoffset)
			{
				SetPitchYaw( look.yaw   + xoffset * look_opts.mouse_x_sensitivity,
							 look.pitch - yoffset * look_opts.mouse_y_sensitivity );
			}

			void SetPitchYaw(float y, float p) {
				look.yaw = y;
				look.pitch = p;

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