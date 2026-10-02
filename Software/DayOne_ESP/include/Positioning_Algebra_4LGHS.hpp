#pragma once

#include "Configuration.hpp"
#include "UWB.hpp"

namespace ALGEBRA_4LGHS {

extern float B_vector_constants[NUMBER_OF_LIGHTHOUSES];
extern float A_matrix[3][3];
extern float AT_matrix[3][3];
extern float ATA_matrix[3][3];
extern float ATA_inv_matrix[3][3];
extern float B_vector[3];
extern float ATB_vector[3];

bool Build_Constant_Matrices(Position (&lghs_positions)[3]);
void Estimate_Position(UWB_Measurement (&distances_to_lghs)[4], Position &position);

void _build_a_matrix(Position (&lghs_positions)[3]);
void _build_at_matrix();
void _build_ata_matrix();
uint8_t _build_ata_inv_matrix();
void _build_b_vector_constants(Position (&lghs_positions)[3]);
void _build_b_vector(UWB_Measurement (&distances_to_lghs)[4]);
void _calculate_atb_vector();
void _calculate_solution(Position &position);
}