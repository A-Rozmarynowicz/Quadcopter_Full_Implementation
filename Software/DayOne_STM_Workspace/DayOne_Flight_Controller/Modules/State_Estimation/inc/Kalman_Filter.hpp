#ifndef STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_
#define STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_
#include <cstdint>

#include "State_Estimation/inc/Measurement_Utils.hpp"

class Kalman_Filter
{
private:
	static constexpr uint8_t DIM = 3;

	const float ACCURACY_VARIANCE_CONFIDENCE = 1.0;

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

	void _build_R(float variance);
	float _calculate_gain_K(float measurement_value, MEASUREMENT_ORDER measurement_order);
	void _fuse_estimate_with_measurement(float gain_K, MEASUREMENT_ORDER measurement_order);
	void _update_uncertainty(float gain_K, MEASUREMENT_ORDER measurement_order);

	void Propagate_State(float (&dt_array)[DT_POWER_ARRAY_LENGTH]);
	void Update_With_Measurement(float measurement_value, MEASUREMENT_ORDER measurement_order, float variance, float (&dt_array)[DT_POWER_ARRAY_LENGTH]);

public:
	void Initialize();
	void Measurement_Received(float measurement_value, MEASUREMENT_ORDER measurement_order, float variance, float (&dt_array)[DT_POWER_ARRAY_LENGTH]);
};

#endif /* STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_ */
