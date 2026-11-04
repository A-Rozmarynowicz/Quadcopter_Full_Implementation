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

	void _build_A(float dt1, float dt2);
	void _build_Q(float acc_variance, float dt1, float dt2, float dt3, float dt4, float dt5);
	void _calculate_X_kk1();
	void _calculate_P_kk1();

	void Propagate_State();
	void Update_With_Measurement();

public:
	void Initialize();
	void Measurement_Received(float measurement, float variance, float (&dt_array)[DT_POWER_ARRAY_LENGTH]);
	void Perform_Cycle();
};

#endif /* STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_ */
