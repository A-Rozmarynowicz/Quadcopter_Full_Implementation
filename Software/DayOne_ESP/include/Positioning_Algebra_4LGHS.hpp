#pragma once

#include "Configuration.hpp"

namespace ALGEBRA_4LGHS {

extern float A_matrix[3][3];
extern float AT_matrix[3][3];
extern float ATA_matrix[3][3];
extern float ATA_inv_matrix[3][3];
extern float B_vector[3];
extern float solution_vector[3];
extern float ATB_vector[3];

void Build_Constant_Matrices();

void _build_a_matrix();
void _build_at_matrix();
void _build_ata_matrix();
uint8_t _build_ata_inv_matrix();
void _build_b_vector_constants();
void _build_b_vector();
void _calculate_atb_vector();
void _calculate_solution();
}