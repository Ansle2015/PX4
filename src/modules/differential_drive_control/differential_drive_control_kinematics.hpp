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

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>

#include <matrix/matrix/math.hpp>



class differential_drive_control_kinematics : public ModuleParams
{
public:
	differential_drive_control_kinematics() : ModuleParams(this) {};
	~differential_drive_control_kinematics() = default;

	void computeControl();
	void setInput(const matrix::Vector2f& input);
	matrix::Vector2f getOutput() const;

private:

	// Input & Output
	matrix::Vector2f _input{0.0f, 0.0f};  // input_[0] -> Vx [m/s], input_[1] -> Omega [rad/s]
	matrix::Vector2f _output{0.0f, 0.0f}; // output_[0] -> Right Motor [rad/s], output_[1] -> Left Motor [rad/s]

	float _vx{0.0f};
	float _omega{0.0f};

	float _motor_vel_left{0.0f};
	float _motor_vel_right{0.0f};


	DEFINE_PARAMETERS(
		(ParamFloat<px4::params::DDC_WHEEL_BASE>) _param_ddc_wheel_base,
		(ParamFloat<px4::params::DDC_WHEEL_RADIUS>) _param_ddc_wheel_radius
	)
};
