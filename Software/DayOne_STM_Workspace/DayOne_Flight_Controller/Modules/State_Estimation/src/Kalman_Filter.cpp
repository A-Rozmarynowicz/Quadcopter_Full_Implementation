#include "State_Estimation/inc/Kalman_Filter.hpp"


void Kalman_Filter::Propagate_State()
{
}

void Kalman_Filter::Update_With_Measurement()
{
}

void Kalman_Filter::Initialize()
{
	for (uint8_t i = 0; i < DIM; i++)
	{
		for (uint8_t j = 0; j < DIM; j++)
		{
			if (i == j)
			{
				P_k1_covariance_matrix_optimal_past[i][j] = 10e9;
			}
			else
			{
				P_k1_covariance_matrix_optimal_past[i][j] = 0.0f;
			}
		}
	}

	for (uint8_t i = 0; i < DIM; i++)
	{
		for (uint8_t j = 0; j < DIM; j++)
		{
			if (i == j)
			{
				A_state_transition_matrix[i][j] = 1.0;
			}
			else
			{
				A_state_transition_matrix[i][j] = 0.0f;
			}
		}
	}

	X_k1_optimal_past = {0.0f, 0.0f, 0.0f};


}

void Kalman_Filter::Measurement_Received(
		float measurement,
		float dt,
		float variance)
{

}

void Kalman_Filter::_build_A(float dt)
{
	A_state_transition_matrix[0][1] = dt;
	A_state_transition_matrix[1][2] = dt;
	A_state_transition_matrix[0][2] = 0.5*dt*dt;
}

void Kalman_Filter::_build_Q(float dt, float acc_variance)
{
	Q_propagation_covariance_matrix[0][0] = acc_variance * dt * dt * dt * dt * dt / 20.0;
	Q_propagation_covariance_matrix[0][1] = acc_variance * dt * dt * dt * dt / 8.0;
	Q_propagation_covariance_matrix[0][2] = acc_variance * dt * dt * dt / 6.0;

	Q_propagation_covariance_matrix[1][0] = acc_variance * dt * dt * dt * dt / 8.0;
	Q_propagation_covariance_matrix[1][1] = acc_variance * dt * dt * dt / 3.0;
	Q_propagation_covariance_matrix[1][2] = acc_variance * dt * dt / 2.0;

	Q_propagation_covariance_matrix[2][0] = acc_variance * dt * dt * dt / 6.0;
	Q_propagation_covariance_matrix[2][1] = acc_variance * dt * dt / 2.0;
	Q_propagation_covariance_matrix[2][2] = acc_variance * dt;
}

void Kalman_Filter::_calculate_X_kk1()
{
	X_kk1_predicted[0] =
			X_k1_optimal_past[0] +
			A_state_transition_matrix[0][1] * X_k1_optimal_past[1] +
			A_state_transition_matrix[0][2] * X_k1_optimal_past[2];

	X_kk1_predicted[1] =
			X_k1_optimal_past[1] +
			A_state_transition_matrix[1][2] * X_k1_optimal_past[2];

	X_kk1_predicted[2] = X_k1_optimal_past[2];

}

void Kalman_Filter::_calculate_P_kk1()
{

}

void Kalman_Filter::Perform_Cycle()
{
}
