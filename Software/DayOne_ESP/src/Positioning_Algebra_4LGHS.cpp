#include "Positioning_Algebra_4LGHS.hpp"

// using namespace ALGEBRA_4LGHS;

void _build_a_matrix(){
    float x1 = lghs_positions[0].x, y1 = lghs_positions[0].y, z1 = lghs_positions[0].z;
    for (uint8_t row=0; row<NUMBER_OF_LIGHTHOUSES-1; row++){
        float xj = lghs_positions[row+1].x, yj = lghs_positions[row+1].y, zj = lghs_positions[row+1].z;
        ALGEBRA_4LGHS::A_matrix[row][0] = -2.0 * xj + 2.0 * x1;
        ALGEBRA_4LGHS::A_matrix[row][1] = -2.0 * yj + 2.0 * y1;
        ALGEBRA_4LGHS::A_matrix[row][2] = -2.0 * zj + 2.0 * z1;
    }
}


void _build_at_matrix(){
    for (uint8_t row=0; row<NUMBER_OF_LIGHTHOUSES-1; row++){
        for (uint8_t column=0; column<3; column++){
            ALGEBRA_4LGHS::AT_matrix[column][row] = ALGEBRA_4LGHS::A_matrix[row][column];
        }
    }
}


void _build_ata_matrix(){
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            ALGEBRA_4LGHS::ATA_matrix[i][j] = 0.0;

            for (uint8_t k = 0; k < NUMBER_OF_LIGHTHOUSES-1; k++) {
                ALGEBRA_4LGHS::ATA_matrix[i][j] += ALGEBRA_4LGHS::AT_matrix[i][k] * ALGEBRA_4LGHS::A_matrix[k][j];
            }
        }
    }
}


uint8_t _build_ata_inv_matrix() {
    // Nie ma sensu tego rozbijać na oddzielne funkcje.
    uint8_t i, j, k;
    float ratio;

    float aug[3][2 * 3];

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            aug[i][j] = ALGEBRA_4LGHS::ATA_matrix[i][j];
            aug[i][j + 3] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (i = 0; i < 3; i++) {
        if (aug[i][i] == 0.0)
            return 1;

        for (j = 0; j < 3; j++) {
            if (i != j) {
                ratio = aug[j][i] / aug[i][i];
                for (k = 0; k < 2 * 3; k++) {
                    aug[j][k] -= ratio * aug[i][k];
                }
            }
        }
    }

    for (i = 0; i < 3; i++) {
        double diag = aug[i][i];
        for (j = 0; j < 2 * 3; j++) {
            aug[i][j] /= diag;
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            ALGEBRA_4LGHS::ATA_inv_matrix[i][j] = aug[i][j + 3];
        }
    }

    return 0;
}


void _build_b_vector_constants(){
    for (uint8_t j=0; j<NUMBER_OF_LIGHTHOUSES; j++){
        ALGEBRA_4LGHS::B_vector_constants[j] = -(lghs_positions[j].x)*(lghs_positions[j].x)
                            - (lghs_positions[j].y)*(lghs_positions[j].y) - (lghs_positions[j].z)*(lghs_positions[j].z);
    }
}


void _build_b_vector(){
    for (uint8_t j=0; j<NUMBER_OF_LIGHTHOUSES-1; j++){
        ALGEBRA_4LGHS::B_vector[j] = (distances_to_lghs[j+1])*(distances_to_lghs[j+1])
         - (distances_to_lghs[0])*(distances_to_lghs[0])
         + ALGEBRA_4LGHS::B_vector_constants[j+1] - ALGEBRA_4LGHS::B_vector_constants[0];

        }
}


void _calculate_atb_vector(){
    uint8_t i, j;
    for (i = 0; i < 3; i++) {
        ALGEBRA_4LGHS::ATB_vector[i] = 0.0;
        for (j = 0; j < NUMBER_OF_LIGHTHOUSES-1; j++) {
            ALGEBRA_4LGHS::ATB_vector[i] += ALGEBRA_4LGHS::AT_matrix[i][j] * ALGEBRA_4LGHS::B_vector[j];
        }
    }
}


void _calculate_solution(){
    uint8_t i, j;
    for (i = 0; i < 3; i++) {
        solution_vector[i] = 0.0;
        for (j = 0; j < 3; j++) {
            solution_vector[i] += ALGEBRA_4LGHS::ATA_inv_matrix[i][j] * ALGEBRA_4LGHS::ATB_vector[j];
        }
    }
    current_position.x = solution_vector[0];
    current_position.y = solution_vector[1];
    current_position.z = solution_vector[2];
}