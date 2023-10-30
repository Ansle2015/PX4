#include "differential_drive_control_guidance.hpp"


matrix::Vector2f differential_drive_control_guidance::computeGuidance(const matrix::Vector2f& current_pos, const matrix::Vector2f& waypoint, const matrix::Vector2f& previous_waypoint, const matrix::Vector2f& next_waypoint, float vehicle_yaw, float dt)
{		
		_global_position = current_pos;
		_current_waypoint = waypoint; 
		_previous_waypoint = previous_waypoint;
		_next_waypoint = next_waypoint;
		_dt = dt; 

		float desired_heading = computeAdvancedBearing(_global_position, _current_waypoint, _previous_waypoint);

		float distance_to_next_wp = get_distance_to_next_waypoint(_global_position(0), _global_position(1),
									_current_waypoint(0), _current_waypoint(1));

		// if (distance_to_next_wp < 2) {
		// 	matrix::Vector2f output; 
		// 	output(0) = 0;
		// 	output(1) = 0;
		// 	return output;  // We are closer than loiter radius to waypoint, stop.

		// }

		float heading_error = normalizeAngle(desired_heading - vehicle_yaw);

		float align_error = computeAlignment(_global_position, _current_waypoint, _previous_waypoint);

		float desired_angular_rate = _yaw_rate_point_pid.pid(heading_error, 0, _dt, 200, true, 1, 0.0, 0) + _yaw_rate_align_pid.pid(align_error, 0, _dt, 200, true, 1, 0.0, 0);

		// change this, make it a parameter or something
		float desired_linear_velocity = 2;

		// initialize this at the start of the function 
		_forwards_velocity_smoothing.setMaxJerk(22);
		_forwards_velocity_smoothing.setMaxAccel(2);
		_forwards_velocity_smoothing.setMaxVel(4);

		const float max_velocity = math::trajectory::computeMaxSpeedFromDistance(22, 1, distance_to_next_wp, 0.0f);

		_forwards_velocity_smoothing.updateDurations(max_velocity);

		_forwards_velocity_smoothing.updateTraj(_dt);

		desired_linear_velocity = _forwards_velocity_smoothing.getCurrentVelocity();

		matrix::Vector2f output; 

		if ((_current_waypoint == _next_waypoint) && (double)distance_to_next_wp < 0.2) {
			output(0) = 0;
			output(1) = 0;
		} else {
			output(0) = desired_linear_velocity;
			output(1) = desired_angular_rate;
		}

		return output; 
}


float differential_drive_control_guidance::computeAdvancedBearing(const matrix::Vector2f& current_pos, const matrix::Vector2f& waypoint, const matrix::Vector2f& previous_waypoint) {

		// PX4_ERR("two waypoints: %f %f", (double)previous_waypoint(0), (double)previous_waypoint(1));

		matrix::Vector2f wanted_path = waypoint - previous_waypoint;
		matrix::Vector2f current_path = current_pos - previous_waypoint;

		// Normalize the vectors
		matrix::Vector2f wanted_path_normalized = wanted_path;
		matrix::Vector2f current_path_normalized = current_path;

		wanted_path_normalized.normalize();
		current_path_normalized.normalize();

		float dot = wanted_path_normalized.dot(current_path_normalized);

		float theta = acos(dot);

		matrix::Vector2f p1 = wanted_path_normalized * cos(theta) * current_path.norm() + previous_waypoint;

		matrix::Vector2f v1 = current_pos - p1;

		matrix::Vector2f new_waypoint = -v1*10 + waypoint;

		// PX4_ERR("two waypoints: %f %f and %f %f", (double)current_pos(0), (double)current_pos(1), (double)new_waypoint(0), (double)new_waypoint(1));

		return computeBearing(current_pos, new_waypoint);
}

float differential_drive_control_guidance::computeBearing(const matrix::Vector2f& current_pos, const matrix::Vector2f& waypoint) {
        float delta_x = waypoint(0) - current_pos(0);
        float delta_y = waypoint(1) - current_pos(1);
        return std::atan2(delta_y, delta_x);
}

float differential_drive_control_guidance::normalizeAngle(float angle) {
	// wtf, i hope no one sees this for now lol
        while (angle > (float)M_PI) angle -= (float)2.0 * (float)M_PI;
        while (angle < -(float)M_PI) angle += (float)2.0 * (float)M_PI;
        return angle;
}

float differential_drive_control_guidance::computeAlignment(const matrix::Vector2f& current_pos, const matrix::Vector2f& waypoint, const matrix::Vector2f& previous_waypoint)
{
    matrix::Vector2f wanted_path = waypoint - previous_waypoint;
    matrix::Vector2f current_path = current_pos - previous_waypoint;

    // Normalize the vectors
    wanted_path.normalize();
    current_path.normalize();

    float result = -(1 - wanted_path.dot(current_path));

    // Check if result is finite, and return 0 if not
    if (!PX4_ISFINITE(result)) {
        return 0.0f;
    }

    return result;
}
