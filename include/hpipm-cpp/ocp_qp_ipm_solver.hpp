#ifndef HPIPM_CPP_OCP_QP_IPM_SOLVER_HPP_
#define HPIPM_CPP_OCP_QP_IPM_SOLVER_HPP_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "common.hpp"
#include "eigen3/Eigen/Core"
#include "hpipm-cpp/ocp_qp.hpp"
#include "hpipm-cpp/ocp_qp_dim.hpp"
#include "hpipm-cpp/ocp_qp_ipm_solver_settings.hpp"
#include "hpipm-cpp/ocp_qp_ipm_solver_statistics.hpp"
#include "hpipm-cpp/ocp_qp_solution.hpp"

namespace hpipm {

///
/// @class OcpQpIpmSolver
/// @brief Ipm solver.
///
class OcpQpIpmSolver {
   public:
    ///
    /// @brief Constructor.
    /// @param[in] ocp_qp OCP-QP problem.
    /// @param[in] solver_settings Solver settings.
    ///
    OcpQpIpmSolver(const std::vector<OcpQp>& ocp_qp,
                   const OcpQpIpmSolverSettings& solver_settings =
                       OcpQpIpmSolverSettings());

    ///
    /// @brief Constructor.
    /// @param[in] solver_settings Solver settings.
    ///
    OcpQpIpmSolver(const OcpQpIpmSolverSettings& solver_settings =
                       OcpQpIpmSolverSettings());

    ///
    /// @brief Destructor.
    ///
    ~OcpQpIpmSolver();

    ///
    /// @brief Prohibit copy constructor.
    ///
    OcpQpIpmSolver(const OcpQpIpmSolver&) = delete;

    ///
    /// @brief Prohibit copy assign operator.
    ///
    OcpQpIpmSolver& operator=(const OcpQpIpmSolver&) = delete;

    ///
    /// @brief Default move constructor.
    ///
    OcpQpIpmSolver(OcpQpIpmSolver&&) noexcept = default;

    ///
    /// @brief Default move assign operator.
    ///
    OcpQpIpmSolver& operator=(OcpQpIpmSolver&&) noexcept = default;

    ///
    /// @brief Sets the Ipm solver settings.
    /// @param[in] solver_settings Solver settings.
    ///
    void setSolverSettings(const OcpQpIpmSolverSettings& solver_settings);

    ///
    /// @brief Resizes the solver.
    /// @param[in] ocp_qp OCP-QP problem.
    ///
    void resize(const std::vector<OcpQp>& ocp_qp);

    ///
    /// @brief Solves the OCP-QP problem.
    /// @param[in] x0 Initial state.
    /// @param[in] ocp_qp OCP-QP problem.
    /// @param[out] qp_sol Solution of the OCP-QP problem.
    /// @return Solver status.
    ///
    HpipmStatus solve(const Eigen::VectorXd& x0, std::vector<OcpQp>& ocp_qp,
                      std::vector<OcpQpSolution>& qp_sol);

    ///
    /// @brief Get the Ipm solver settings.
    /// @return const reference to the Ipm solver settings.
    ///
    const OcpQpIpmSolverSettings& getIpmSolverSettings() const;

    ///
    /// @brief Get the solver statistics.
    /// @return const reference to the solver statistics.
    ///
    const OcpQpIpmSolverStatistics& getSolverStatistics() const;

   private:
    OcpQpIpmSolverSettings solver_settings_;
    OcpQpIpmSolverStatistics solver_statistics_;
    OcpQpDim dim_;

    struct WrapperHolder;  // Pimpl
    std::unique_ptr<WrapperHolder> wrapper_holder_;

    // raw pointer storage
    std::vector<double*> A_ptr_;
    std::vector<double*> B_ptr_;
    std::vector<double*> b_ptr_;
    std::vector<double*> Q_ptr_;
    std::vector<double*> S_ptr_;
    std::vector<double*> R_ptr_;
    std::vector<double*> q_ptr_;
    std::vector<double*> r_ptr_;
    std::vector<int*> idxbx_ptr_;
    std::vector<double*> lbx_ptr_;
    std::vector<double*> ubx_ptr_;
    std::vector<double*> lbx_mask_ptr_;
    std::vector<double*> ubx_mask_ptr_;
    std::vector<int*> idxbu_ptr_;
    std::vector<double*> lbu_ptr_;
    std::vector<double*> ubu_ptr_;
    std::vector<double*> lbu_mask_ptr_;
    std::vector<double*> ubu_mask_ptr_;
    std::vector<double*> C_ptr_;
    std::vector<double*> D_ptr_;
    std::vector<double*> lg_ptr_;
    std::vector<double*> ug_ptr_;
    std::vector<double*> lg_mask_ptr_;
    std::vector<double*> ug_mask_ptr_;
    std::vector<double*> Zl_ptr_;
    std::vector<double*> Zu_ptr_;
    std::vector<double*> zl_ptr_;
    std::vector<double*> zu_ptr_;
    std::vector<int*> idxs_ptr_;
    std::vector<double*> lls_ptr_;
    std::vector<double*> lus_ptr_;

    // initial state embedding
    Eigen::VectorXd b0_, r0_;
    Eigen::MatrixXd Lr0_, Lr0_inv_, G0_inv_, H0_, B0t_P1_, A0t_P1_;
};

}  // namespace hpipm

#endif  // HPIPM_CPP_OCP_QP_IPM_SOLVER_HPP_