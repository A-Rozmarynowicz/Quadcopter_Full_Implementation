
#ifndef POSITIONING_ALGEBRA_5LGHS_HPP
#define POSITIONING_ALGEBRA_5LGHS_HPP

#include "Configuration.hpp"
#include "UWB.hpp"

namespace ALGEBRA_5LGHS {

extern float B_vector_constants[5];
extern float A_matrix[4][3];
extern float AT_matrix[3][4];
extern float ATA_matrix[3][3];
extern float ATA_inv_matrix[3][3];
extern float B_vector[4];
extern float ATB_vector[3];

bool Build_Constant_Matrices(Position (&lghs_positions)[5]);

void Estimate_Position(
    UWB_Measurement (&distances_to_lghs)[5],
    Position &position
);

void _build_a_matrix(Position (&lghs_positions)[5]);
void _build_at_matrix();
void _build_ata_matrix();
uint8_t _build_ata_inv_matrix();
void _build_b_vector_constants(Position (&lghs_positions)[5]);
void _build_b_vector(UWB_Measurement (&distances_to_lghs)[5]);
void _calculate_atb_vector();
void _calculate_solution(Position &position);

}

#endif
