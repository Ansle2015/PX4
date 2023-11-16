/****************************************************************************
 *
 *   Copyright (c) 2023 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#pragma once

#include <matrix/matrix/math.hpp>

class DifferentialDriveKinematics
{
public:
	DifferentialDriveKinematics() = default;
	~DifferentialDriveKinematics() = default;

	void computeInverseKinematics();
	void computeForwardsKinematics();
	void setWheelBase(float wheel_base);
	void setWheelRadius(float wheel_radius);
	void setInput(const matrix::Vector2f& input, bool inverse);

	matrix::Vector2f getOutput(bool inverse) const;

private:

	matrix::Vector2f _input{0.0f, 0.0f};
	matrix::Vector2f _output_inverse{0.0f, 0.0f}; // _output(0) -> Right Motor Speed [rad/s], _output(1) -> Left Motor Speed [rad/s]
	matrix::Vector2f _output_forwards{0.0f, 0.0f}; // _output[0] -> Linear Velocity in X [rad/s], _output[1] -> Angular Velocity in Z [rad/s]

	float _linear_vel_x{0.0f};
	float _yaw_rate{0.0f};

	float _motor_vel_right{0.0f};
	float _motor_vel_left{0.0f};

	float _wheel_base{1.0f};
	float _wheel_radius{1.0f};

};