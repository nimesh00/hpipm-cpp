#ifndef _HPIPM_CPP_COMMON_UTILS_HPP_
#define _HPIPM_CPP_COMMON_UTILS_HPP_

///
/// @class HpipmMode
/// @brief Solver mode.
///
enum class HpipmMode { SpeedAbs, Speed, Balance, Robust };

///
/// @class HpipmStatus
/// @brief Solver status.
///
enum class HpipmStatus {
    Success = 0,
    MaxIterReached = 1,
    MinStepLengthReached = 2,
    NaNDetected = 3,
    UnknownFailure = 4,
};

#endif