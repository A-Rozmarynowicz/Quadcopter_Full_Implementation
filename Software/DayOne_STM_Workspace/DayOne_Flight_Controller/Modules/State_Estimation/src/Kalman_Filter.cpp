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

	X_k1_optimal_past = {0.0f, 0.0f, 0.0f};
}

void Kalman_Filter::Measurement_Received(
		float measurement,
		float dt,
		float variance)
{
}
