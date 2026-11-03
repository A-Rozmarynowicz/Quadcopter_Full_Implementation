#ifndef STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_
#define STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_
#include <cstdint>

class Kalman_Filter
{
private:
	static constexpr uint8_t DIM = 3;

	struct Kalman_State
	{
		float d_0th, d_1st, d_2nd;
	}; // value, 1st derivative, 2nd derivative

	Kalman_State X_kk1_predicted;
	Kalman_State X_k1_optimal_past;

	float P_kk1_covariance_matrix_predicted[DIM][DIM];
	float P_k1_covariance_matrix_optimal_past[DIM][DIM];

	float A_state_transition_matrix[DIM][DIM];
	float Q_propagation_covariance_matrix[DIM][DIM];

	float R_last_measurement_variance;

	void Propagate_State();
	void Update_With_Measurement();

public:
	void Initialize();
	void Measurement_Received(float measurement, float dt, float variance);
};

#endif /* STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_ */
