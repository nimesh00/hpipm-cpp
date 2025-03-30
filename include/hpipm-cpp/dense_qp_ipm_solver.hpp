#ifndef HPIPM_CPP_DENSE_QP_IPM_SOLVER_HPP_
#define HPIPM_CPP_DENSE_QP_IPM_SOLVER_HPP_

#include <iostream>
#include <memory>
#include <string>

#include "common.hpp"
#include "eigen3/Eigen/Core"
#include "hpipm-cpp/dense_qp.hpp"
#include "hpipm-cpp/dense_qp_dim.hpp"
#include "hpipm-cpp/dense_qp_ipm_solver_settings.hpp"
#include "hpipm-cpp/dense_qp_ipm_solver_statistics.hpp"
#include "hpipm-cpp/dense_qp_solution.hpp"

namespace hpipm {

std::string to_string(const HpipmStatus& hpipm_status);

std::ostream& operator<<(std::ostream& os, const HpipmStatus& hpipm_status);

///
/// @class DenseQpIpmSolver
/// @brief Ipm solver for dense QP problems.
///
class DenseQpIpmSolver {
   public:
    ///
    /// @brief Constructor.
    /// @param[in] dense_qp Dense-QP problem.
    /// @param[in] solver_settings Solver settings.
    ///
    DenseQpIpmSolver(const DenseQp& dense_qp,
                     const DenseQpIpmSolverSettings& solver_settings =
                         DenseQpIpmSolverSettings());

    ///
    /// @brief Constructor.
    /// @param[in] solver_settings Solver settings.
    ///
    DenseQpIpmSolver(const DenseQpIpmSolverSettings& solver_settings =
                         DenseQpIpmSolverSettings());

    ///
    /// @brief Destructor.
    ///
    ~DenseQpIpmSolver();

    ///
    /// @brief Prohibit copy constructor.
    ///
    DenseQpIpmSolver(const DenseQpIpmSolver&) = delete;

    ///
    /// @brief Prohibit copy assign operator.
    ///
    DenseQpIpmSolver& operator=(const DenseQpIpmSolver&) = delete;

    ///
    /// @brief Default move constructor.
    ///
    DenseQpIpmSolver(DenseQpIpmSolver&&) noexcept = default;

    ///
    /// @brief Default move assign operator.
    ///
    DenseQpIpmSolver& operator=(DenseQpIpmSolver&&) noexcept = default;

    ///
    /// @brief Sets the Ipm solver settings.
    /// @param[in] solver_settings Solver settings.
    ///
    void setSolverSettings(const DenseQpIpmSolverSettings& solver_settings);

    ///
    /// @brief Resizes the solver.
    /// @param[in] dense_qp Dense-QP problem.
    ///
    void resize(const DenseQp& dense_qp);

    ///
    /// @brief Solves the Dense-QP problem.
    /// @param[in] dense_qp Dense-QP problem.
    /// @param[out] qp_sol Solution of the Dense-QP problem.
    /// @return Solver status.
    ///
    HpipmStatus solve(DenseQp& dense_qp, DenseQpSolution& qp_sol);

    ///
    /// @brief Get the Ipm solver settings.
    /// @return const reference to the Ipm solver settings.
    ///
    const DenseQpIpmSolverSettings& getIpmSolverSettings() const;

    ///
    /// @brief Get the solver statistics.
    /// @return const reference to the solver statistics.
    ///
    const DenseQpIpmSolverStatistics& getSolverStatistics() const;

   private:
    DenseQpIpmSolverSettings solver_settings_;
    DenseQpIpmSolverStatistics solver_statistics_;
    DenseQpDim dim_;

    struct WrapperHolder;  // Pimpl
    std::unique_ptr<WrapperHolder> wrapper_holder_;

    // raw pointer storage
    double* H_ptr_ = nullptr;
    double* g_ptr_ = nullptr;
    double* A_ptr_ = nullptr;
    double* b_ptr_ = nullptr;
    int* idxb_ptr_ = nullptr;
    double* lb_ptr_ = nullptr;
    double* ub_ptr_ = nullptr;
    double* lb_mask_ptr_ = nullptr;
    double* ub_mask_ptr_ = nullptr;
    double* C_ptr_ = nullptr;
    double* lg_ptr_ = nullptr;
    double* ug_ptr_ = nullptr;
    double* lg_mask_ptr_ = nullptr;
    double* ug_mask_ptr_ = nullptr;
    double* Zl_ptr_ = nullptr;
    double* Zu_ptr_ = nullptr;
    double* zl_ptr_ = nullptr;
    double* zu_ptr_ = nullptr;
    int* idxs_ptr_ = nullptr;
    double* ls_ptr_ = nullptr;
    double* us_ptr_ = nullptr;
};

}  // namespace hpipm

#endif  // HPIPM_CPP_DENSE_QP_IPM_SOLVER_HPP_