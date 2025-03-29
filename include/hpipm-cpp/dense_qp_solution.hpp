#ifndef HPIPM_CPP_DENSE_QP_SOLUTION_HPP_
#define HPIPM_CPP_DENSE_QP_SOLUTION_HPP_

#include "eigen3/Eigen/Core"

namespace hpipm {

///
/// @class DenseQpSolution
/// @brief Solution of the Dense-QP problem.
///
struct DenseQpSolution {
public:
  ///
  /// @brief Primal variables. 
  ///
  Eigen::VectorXd v;

  ///
  /// @brief Lagrange multipliers for equality constraints. 
  ///
  Eigen::VectorXd pi;

  ///
  /// @brief Lagrange multipliers for lower bounds of inequality constraints.
  ///
  Eigen::VectorXd lam_lb;

  ///
  /// @brief Lagrange multipliers for upper bounds of inequality constraints.
  ///
  Eigen::VectorXd lam_ub;

  ///
  /// @brief Lagrange multipliers for lower bounds of general constraints.
  ///
  Eigen::VectorXd lam_lg;

  ///
  /// @brief Lagrange multipliers for upper bounds of general constraints.
  ///
  Eigen::VectorXd lam_ug;

  ///
  /// @brief Slack variables for lower bounds.
  ///
  Eigen::VectorXd sl;

  ///
  /// @brief Slack variables for upper bounds.
  ///
  Eigen::VectorXd su;

  ///
  /// @brief Objective function value at the solution.
  ///
  double obj_value = 0.0;
};

} // namespace hpipm

#endif // HPIPM_CPP_DENSE_QP_SOLUTION_HPP_