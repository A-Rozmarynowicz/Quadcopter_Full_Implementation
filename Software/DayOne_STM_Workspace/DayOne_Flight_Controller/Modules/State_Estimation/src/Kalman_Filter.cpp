#include "State_Estimation/inc/Kalman_Filter.hpp"



void Kalman_Filter::Measurement_Received(
		float measurement_value,
		MEASUREMENT_ORDER measurement_order,
		float variance,
		float (&dt_array)[DT_POWER_ARRAY_LENGTH] )
{
	Propagate_State(dt_array);
	Update_With_Measurement(measurement_value, measurement_order, variance, dt_array);
}

void Kalman_Filter::Propagate_State(float (&dt_array)[DT_POWER_ARRAY_LENGTH] )
{
	_build_A(dt_array[0], dt_array[1]);
	_build_Q(ACCURACY_VARIANCE_CONFIDENCE, dt_array[0], dt_array[1], dt_array[2], dt_array[3], dt_array[4]);
	_calculate_X_kk1();
	_calculate_P_kk1();
}

void Kalman_Filter::Update_With_Measurement(
		float measurement_value,
		MEASUREMENT_ORDER measurement_order,
		float variance,
		float (&dt_array)[DT_POWER_ARRAY_LENGTH] )
{
	_build_R(variance);
	float gain_K = _calculate_gain_K(measurement, measurement_order);
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

#pragma region Propagate_State

void Kalman_Filter::_build_A(float dt1, float dt2)
{
	A_state_transition_matrix[0][1] = dt1;
	A_state_transition_matrix[1][2] = dt1;
	A_state_transition_matrix[0][2] = 0.5*dt2;
}

void Kalman_Filter::_build_Q(float acc_variance, float dt1, float dt2, float dt3, float dt4, float dt5)
{
	Q_propagation_covariance_matrix[0][0] = acc_variance * dt5 / 20.0;
	Q_propagation_covariance_matrix[0][1] = acc_variance * dt4 / 8.0;
	Q_propagation_covariance_matrix[0][2] = acc_variance * dt3 / 6.0;

	Q_propagation_covariance_matrix[1][0] = acc_variance * dt4 / 8.0;
	Q_propagation_covariance_matrix[1][1] = acc_variance * dt3 / 3.0;
	Q_propagation_covariance_matrix[1][2] = acc_variance * dt2 / 2.0;

	Q_propagation_covariance_matrix[2][0] = acc_variance * dt3 / 6.0;
	Q_propagation_covariance_matrix[2][1] = acc_variance * dt2 / 2.0;
	Q_propagation_covariance_matrix[2][2] = acc_variance * dt1;
}

void Kalman_Filter::_calculate_X_kk1()
{
	X_kk1_predicted.d_0th =
			X_k1_optimal_past.d_0th +
			A_state_transition_matrix[0][1] * X_k1_optimal_past.d_1st +
			A_state_transition_matrix[0][2] * X_k1_optimal_past.d_2nd;

	X_kk1_predicted.d_1st =
			X_k1_optimal_past.d_1st +
			A_state_transition_matrix[1][2] * X_k1_optimal_past.d_2nd;

	X_kk1_predicted.d_2nd = X_k1_optimal_past.d_2nd;
}

void Kalman_Filter::_calculate_P_kk1()
{
	float AP[3][3] = {};

	for (uint8_t i = 0; i < 3; i++)
	{
	    for (uint8_t j = 0; j < 3; j++)
	    {
	        for (uint8_t t = 0; t < 3; t++)
	        {
	            AP[i][j] += A_state_transition_matrix[i][t] *
	                        P_k1_covariance_matrix_optimal_past[t][j];
	        }
	    }
	}

	float APAT[3][3] = {};

	for (uint8_t i = 0; i < 3; i++)
	{
	    for (uint8_t j = 0; j < 3; j++)
	    {
	        for (uint8_t t = 0; t < 3; t++)
	        {
	            APAT[i][j] += AP[i][t] *
	                          A_state_transition_matrix[j][t];
	        }
	    }
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			P_kk1_covariance_matrix_predicted[i][j] = APAT[i][j] + Q_propagation_covariance_matrix[i][j];
		}
	}

}

#pragma endregion

#pragma region Update_With_Measurement

void Kalman_Filter::_build_R(float variance)
{
	R_last_measurement_variance = variance;
}

float Kalman_Filter::_calculate_gain_K(float measurement_value, MEASUREMENT_ORDER measurement_order)
{

}

void Kalman_Filter::_fuse_estimate_with_measurement(float gain_K, MEASUREMENT_ORDER measurement_order)
{
}

void Kalman_Filter::_update_uncertainty(MEASUREMENT_ORDER measurement_order)
{
}

#pragma endregion
