#ifndef HPIPM_CPP_DENSE_QP_IPM_SOLVER_SETTINGS_HPP_
#define HPIPM_CPP_DENSE_QP_IPM_SOLVER_SETTINGS_HPP_

namespace hpipm {

// ///
// /// @class HpipmMode
// /// @brief Solver mode.
// ///
// enum class HpipmMode {
//   SpeedAbs,
//   Speed,
//   Balance,
//   Robust
// };

///
/// @class DenseQpIpmSolverSettings
/// @brief Ipm solver settings for dense QP.
///
struct DenseQpIpmSolverSettings {
   public:
    ///
    /// @brief Solver mode. Default is HpipmMode::Speed.
    ///
    HpipmMode mode = HpipmMode::Speed;

    ///
    /// @brief Maximum number of iterations. Must be non-negative. Default
    /// is 15.
    ///
    int iter_max = 15;

    ///
    /// @brief Minimum step size. Must be positive and less than 1.0. Default
    /// is 1.0e-08.
    ///
    double alpha_min = 1.0e-08;

    ///
    /// @brief Initial barrier parameter. Must be positive. Default is 1.0e+02.
    ///
    double mu0 = 1.0e+02;

    ///
    /// @brief Convergence criteria for stationarity. Must be positive. Default
    /// is 1.0e-08.
    ///
    double tol_stat = 1.0e-08;

    ///
    /// @brief Convergence criteria for equality constraints. Must be positive.
    /// Default is 1.0e-08.
    ///
    double tol_eq = 1.0e-08;

    ///
    /// @brief Convergence criteria for inequality constraints. Must be
    /// positive. Default is 1.0e-08.
    ///
    double tol_ineq = 1.0e-08;

    ///
    /// @brief Convergence criteria for complementarity. Must be positive.
    /// Default is 1.0e-08.
    ///
    double tol_comp = 1.0e-08;

    ///
    /// @brief Convergence criteria for dual gap. Must be positive. Default
    /// is 1.0e-08.
    ///
    double tol_dual_gap = 1.0e-08;

    ///
    /// @brief Regularization term for primal Hessian. Must be non-negative.
    /// Default is 1.0e-12.
    ///
    double reg_prim = 1.0e-12;

    ///
    /// @brief Regularization term for dual Hessian. Must be non-negative.
    /// Default is 1.0e-12.
    ///
    double reg_dual = 1.0e-12;

    ///
    /// @brief Warm start flag (0: disable, 1: enable primal, 2: enable primal
    /// and dual). Default is 0.
    ///
    int warm_start = 0;

    ///
    /// @brief Prediction-correction flag (0: disable, 1: enable). Default is 1.
    ///
    int pred_corr = 1;

    ///
    /// @brief Conditional prediction-correction flag (0: disable, 1: enable).
    /// Default is 1.
    ///
    int cond_pred_corr = 1;

    ///
    /// @brief Scale Hessian flag (0: disable, 1: enable). Default is 0.
    ///
    int scale = 0;

    ///
    /// @brief KKT factorization algorithm (0: null-space, 1: schur-complement).
    /// Default is 1.
    ///
    int kkt_fact_alg = 1;

    ///
    /// @brief Remove linearly dependent equality constraints flag (0: disable,
    /// 1: enable). Default is 0.
    ///
    int remove_lin_dep_eq = 0;

    ///
    /// @brief Compute objective value flag (0: disable, 1: enable). Default
    /// is 1.
    ///
    int compute_obj = 1;

    ///
    /// @brief Use different step for primal and dual variables (0: disable, 1:
    /// enable). Default is 0.
    ///
    int split_step = 0;

    ///
    /// @brief Check the settings. If something is wrong, throws an exception.
    ///
    void checkSettings() const;
};

}  // namespace hpipm

#endif  // HPIPM_CPP_DENSE_QP_IPM_SOLVER_SETTINGS_HPP_