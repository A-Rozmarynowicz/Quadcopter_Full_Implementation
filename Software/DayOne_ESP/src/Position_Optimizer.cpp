#include "Position_Optimizer.hpp"

#include <cmath>

namespace
{
    constexpr int MAX_ITERATIONS = 10;
    constexpr int MAX_DAMPING_TRIALS = 6;

    constexpr float POSITION_TOLERANCE = 1e-4f;
    constexpr float PIVOT_EPSILON = 1e-7f;

    constexpr float INITIAL_LAMBDA = 1e-3f;
    constexpr float MIN_LAMBDA = 1e-7f;
    constexpr float MAX_LAMBDA = 1e7f;

    bool Solve_3x3(float A[3][3], float b[3], float x[3])
    {
        for (int col = 0; col < 3; ++col)
        {
            int pivot = col;
            float max_value = std::fabs(A[col][col]);

            for (int row = col + 1; row < 3; ++row)
            {
                float value = std::fabs(A[row][col]);

                if (value > max_value)
                {
                    max_value = value;
                    pivot = row;
                }
            }

            if (!std::isfinite(max_value) ||
                max_value < PIVOT_EPSILON)
                return false;

            if (pivot != col)
            {
                for (int j = 0; j < 3; ++j)
                {
                    float temp = A[col][j];
                    A[col][j] = A[pivot][j];
                    A[pivot][j] = temp;
                }

                float temp = b[col];
                b[col] = b[pivot];
                b[pivot] = temp;
            }

            for (int row = col + 1; row < 3; ++row)
            {
                float factor = A[row][col] / A[col][col];

                for (int j = col; j < 3; ++j)
                    A[row][j] -= factor * A[col][j];

                b[row] -= factor * b[col];
            }
        }

        for (int row = 2; row >= 0; --row)
        {
            float sum = b[row];

            for (int j = row + 1; j < 3; ++j)
                sum -= A[row][j] * x[j];

            if (!std::isfinite(A[row][row]) ||
                std::fabs(A[row][row]) < PIVOT_EPSILON)
                return false;

            x[row] = sum / A[row][row];

            if (!std::isfinite(x[row]))
                return false;
        }

        return true;
    }

    bool Compute_Cost(
        const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES],
        Position (&LGHS_positions)[NUMBER_OF_LIGHTHOUSES],
        float (&weights)[NUMBER_OF_LIGHTHOUSES],
        const Position& p,
        float& cost)
    {
        cost = 0.0f;

        for (int i = 0; i < NUMBER_OF_LIGHTHOUSES; ++i)
        {
            const auto& m = measurements[i];
            const Position& l = LGHS_positions[m.lgh_index];

            float dx = p.x - l.x;
            float dy = p.y - l.y;
            float dz = p.z - l.z;

            float predicted = std::sqrt(
                dx * dx + dy * dy + dz * dz);

            float residual = predicted - m.range;
            cost += weights[i] * residual * residual;
        }

        return std::isfinite(cost);
    }
}

bool Optimize_Position(
    UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES],
    Position (&LGHS_positions)[NUMBER_OF_LIGHTHOUSES],
    float (&weights)[NUMBER_OF_LIGHTHOUSES],
    Position& starting_point,
    Position& estimated_position)
{
    Position p = starting_point;
    estimated_position = p;

    if (!std::isfinite(p.x) ||
        !std::isfinite(p.y) ||
        !std::isfinite(p.z))
        return false;

    for (int i = 0; i < NUMBER_OF_LIGHTHOUSES; ++i)
    {
        const auto& m = measurements[i];

        if (m.lgh_index >= NUMBER_OF_LIGHTHOUSES ||
            !std::isfinite(m.range) ||
            m.range < 0.0f ||
            !std::isfinite(weights[i]) ||
            weights[i] < 0.0f)
            return false;

        const Position& l = LGHS_positions[m.lgh_index];

        if (!std::isfinite(l.x) ||
            !std::isfinite(l.y) ||
            !std::isfinite(l.z))
            return false;
    }

    float cost;

    if (!Compute_Cost(
            measurements, LGHS_positions, weights, p, cost))
        return false;

    float lambda = INITIAL_LAMBDA;

    for (int iteration = 0;
         iteration < MAX_ITERATIONS;
         ++iteration)
    {
        float H[3][3] = {};
        float g[3] = {};

        // Build the weighted normal equations at the current position.
        for (int i = 0; i < NUMBER_OF_LIGHTHOUSES; ++i)
        {
            const auto& m = measurements[i];
            const Position& l = LGHS_positions[m.lgh_index];

            float dx = p.x - l.x;
            float dy = p.y - l.y;
            float dz = p.z - l.z;

            float predicted = std::sqrt(
                dx * dx + dy * dy + dz * dz);

            if (!std::isfinite(predicted) || predicted < 1e-6f)
                return false;

            float residual = predicted - m.range;

            float J[3] = {
                dx / predicted,
                dy / predicted,
                dz / predicted
            };

            float w = weights[i];

            for (int row = 0; row < 3; ++row)
            {
                g[row] += w * J[row] * residual;

                for (int col = 0; col < 3; ++col)
                    H[row][col] += w * J[row] * J[col];
            }
        }

        // A tiny gradient means there is little left to improve.
        float gradient_squared =
            g[0] * g[0] + g[1] * g[1] + g[2] * g[2];

        if (gradient_squared < 1e-12f)
            return true;

        bool accepted = false;

        for (int trial = 0;
             trial < MAX_DAMPING_TRIALS;
             ++trial)
        {
            float A[3][3];
            float rhs[3] = { -g[0], -g[1], -g[2] };
            float delta[3] = {};

            for (int row = 0; row < 3; ++row)
            {
                for (int col = 0; col < 3; ++col)
                    A[row][col] = H[row][col];

                // LM damping, scaled by the diagonal of J^T W J.
                float scale = H[row][row];
                if (scale < 1e-6f)
                    scale = 1e-6f;

                A[row][row] += lambda * scale;
            }

            if (!Solve_3x3(A, rhs, delta))
            {
                lambda *= 10.0f;

                if (lambda > MAX_LAMBDA)
                    return false;

                continue;
            }

            float delta_squared =
                delta[0] * delta[0] +
                delta[1] * delta[1] +
                delta[2] * delta[2];

            if (delta_squared <
                POSITION_TOLERANCE * POSITION_TOLERANCE)
            {
                return true;
            }

            Position candidate = {
                p.x + delta[0],
                p.y + delta[1],
                p.z + delta[2]
            };

            float candidate_cost;

            if (std::isfinite(candidate.x) &&
                std::isfinite(candidate.y) &&
                std::isfinite(candidate.z) &&
                Compute_Cost(
                    measurements,
                    LGHS_positions,
                    weights,
                    candidate,
                    candidate_cost) &&
                candidate_cost < cost)
            {
                p = candidate;
                cost = candidate_cost;
                estimated_position = p;

                lambda *= 0.3f;

                if (lambda < MIN_LAMBDA)
                    lambda = MIN_LAMBDA;

                accepted = true;
                break;
            }

            // Reject the step and try again with more damping.
            lambda *= 10.0f;

            if (lambda > MAX_LAMBDA)
                break;
        }

        if (!accepted)
        {
            // No improving step was found.
            // The current estimate is still valid, but convergence
            // was not established.
            return false;
        }
    }

    // The iteration limit was reached with a finite estimate.
    return true;
}
