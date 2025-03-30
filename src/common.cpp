#include "hpipm-cpp/common.hpp"

namespace hpipm {
std::string to_string(const HpipmStatus& hpipm_status) {
    switch (hpipm_status) {
        case HpipmStatus::Success:
            return "HpipmStatus::Success";
        case HpipmStatus::MaxIterReached:
            return "HpipmStatus::MaxIterReached";
        case HpipmStatus::MinStepLengthReached:
            return "HpipmStatus::MinStepLengthReached";
        case HpipmStatus::NaNDetected:
            return "HpipmStatus::NaNDetected";
        default:
            return "HpipmStatus::UnknownFailure";
    }
}

std::ostream& operator<<(std::ostream& os, const HpipmStatus& hpipm_status) {
    os << to_string(hpipm_status);
    return os;
}

}  // namespace hpipm