
#include "Positioning_Algebra_5LGHS.hpp"

float ALGEBRA_5LGHS::B_vector_constants[5] = {0};
float ALGEBRA_5LGHS::A_matrix[4][3] = {0};
float ALGEBRA_5LGHS::AT_matrix[3][4] = {0};
float ALGEBRA_5LGHS::ATA_matrix[3][3] = {0};
float ALGEBRA_5LGHS::ATA_inv_matrix[3][3] = {0};
float ALGEBRA_5LGHS::B_vector[4] = {0};
float ALGEBRA_5LGHS::ATB_vector[3] = {0};


bool ALGEBRA_5LGHS::Build_Constant_Matrices(
    Position (&lghs_positions)[5])
{
    _build_a_matrix(lghs_positions);
    _build_at_matrix();
    _build_ata_matrix();

    if (_build_ata_inv_matrix() != 0) {
        return false;
    }

    _build_b_vector_constants(lghs_positions);

    return true;
}


void ALGEBRA_5LGHS::Estimate_Position(
    UWB_Measurement (&distances_to_lghs)[5],
    Position &position)
{
    _build_b_vector(distances_to_lghs);
    _calculate_atb_vector();
    _calculate_solution(position);
}


#pragma region Calculations

void ALGEBRA_5LGHS::_build_a_matrix(
    Position (&lghs_positions)[5])
{
    float x1 = lghs_positions[0].x;
    float y1 = lghs_positions[0].y;
    float z1 = lghs_positions[0].z;

    for (uint8_t row = 0; row < 4; row++) {
        float xj = lghs_positions[row + 1].x;
        float yj = lghs_positions[row + 1].y;
        float zj = lghs_positions[row + 1].z;

        A_matrix[row][0] = 2.0f * (x1 - xj);
        A_matrix[row][1] = 2.0f * (y1 - yj);
        A_matrix[row][2] = 2.0f * (z1 - zj);
    }
}


void ALGEBRA_5LGHS::_build_at_matrix()
{
    for (uint8_t row = 0; row < 3; row++) {
        for (uint8_t column = 0; column < 4; column++) {
            AT_matrix[row][column] =
                A_matrix[column][row];
        }
    }
}


void ALGEBRA_5LGHS::_build_ata_matrix()
{
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            ATA_matrix[i][j] = 0.0f;

            for (uint8_t k = 0; k < 4; k++) {
                ATA_matrix[i][j] +=
                    AT_matrix[i][k] * A_matrix[k][j];
            }
        }
    }
}


// Returns 0 on success, 1 if the matrix is singular
uint8_t ALGEBRA_5LGHS::_build_ata_inv_matrix()
{
    float aug[3][6];

    // Build the augmented matrix [ATA | I].
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            aug[i][j] = ATA_matrix[i][j];
            aug[i][j + 3] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Gauss-Jordan elimination with partial pivoting.
    for (uint8_t i = 0; i < 3; i++) {
        uint8_t pivot_row = i;
        float max_value = (aug[i][i] < 0.0f)
            ? -aug[i][i] : aug[i][i];

        for (uint8_t row = i + 1; row < 3; row++) {
            float value = (aug[row][i] < 0.0f)
                ? -aug[row][i] : aug[row][i];

            if (value > max_value) {
                max_value = value;
                pivot_row = row;
            }
        }

        // Detect a singular or numerically degenerate matrix.
        if (max_value < 1e-6f) {
            return 1;
        }

        // Swap rows if needed.
        if (pivot_row != i) {
            for (uint8_t column = 0; column < 6; column++) {
                float temp = aug[i][column];
                aug[i][column] = aug[pivot_row][column];
                aug[pivot_row][column] = temp;
            }
        }

        // Normalize the pivot row.
        float pivot = aug[i][i];

        for (uint8_t column = 0; column < 6; column++) {
            aug[i][column] /= pivot;
        }

        // Eliminate the other entries in this column.
        for (uint8_t row = 0; row < 3; row++) {
            if (row == i) {
                continue;
            }

            float ratio = aug[row][i];

            for (uint8_t column = 0; column < 6; column++) {
                aug[row][column] -=
                    ratio * aug[i][column];
            }
        }
    }

    // Extract the inverse from the augmented matrix.
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            ATA_inv_matrix[i][j] = aug[i][j + 3];
        }
    }

    return 0;
}


void ALGEBRA_5LGHS::_build_b_vector_constants(
    Position (&lghs_positions)[5])
{
    for (uint8_t j = 0; j < 5; j++) {
        float x = lghs_positions[j].x;
        float y = lghs_positions[j].y;
        float z = lghs_positions[j].z;

        B_vector_constants[j] =
            -(x * x) - (y * y) - (z * z);
    }
}


void ALGEBRA_5LGHS::_build_b_vector(
    UWB_Measurement (&distances_to_lghs)[5])
{
    float r1 = distances_to_lghs[0].range;

    for (uint8_t j = 0; j < 4; j++) {
        float rj = distances_to_lghs[j + 1].range;

        B_vector[j] =
            (rj * rj)
            - (r1 * r1)
            + B_vector_constants[j + 1]
            - B_vector_constants[0];
    }
}


void ALGEBRA_5LGHS::_calculate_atb_vector()
{
    for (uint8_t i = 0; i < 3; i++) {
        ATB_vector[i] = 0.0f;

        for (uint8_t j = 0; j < 4; j++) {
            ATB_vector[i] +=
                AT_matrix[i][j] * B_vector[j];
        }
    }
}


void ALGEBRA_5LGHS::_calculate_solution(Position &position)
{
    float solution_vector[3] = {0.0f, 0.0f, 0.0f};

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            solution_vector[i] +=
                ATA_inv_matrix[i][j] * ATB_vector[j];
        }
    }

    position.x = solution_vector[0];
    position.y = solution_vector[1];
    position.z = solution_vector[2];
}

#pragma endregion
