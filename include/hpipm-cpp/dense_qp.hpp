#ifndef HPIPM_CPP_DENSE_QP_HPP_
#define HPIPM_CPP_DENSE_QP_HPP_

#include <vector>

#include "eigen3/Eigen/Core"


namespace hpipm {

///
/// @class DenseQp
/// @brief The Dense-QP data class.
///
struct DenseQp {
  ///
  /// @brief Hessian matrix in the cost function (1/2) * v^T H v + g^T v.
  ///
  Eigen::MatrixXd H;

  ///
  /// @brief Linear term in the cost function (1/2) * v^T H v + g^T v.
  ///
  Eigen::VectorXd g;

  ///
  /// @brief Equality constraint matrix in A v = b.
  ///
  Eigen::MatrixXd A;

  ///
  /// @brief Equality constraint vector in A v = b.
  ///
  Eigen::VectorXd b;

  ///
  /// @brief Indices of box constrained elements of v.
  ///
  std::vector<int> idxb;

  ///
  /// @brief Lower bounds of box constraints.
  ///
  Eigen::VectorXd lb;

  ///
  /// @brief Upper bounds of box constraints.
  ///
  Eigen::VectorXd ub;

  ///
  /// @brief Masks on the lower bounds of box constraints.
  /// Each element must be composed only by 0 or 1.0.
  ///
  Eigen::VectorXd lb_mask;

  ///
  /// @brief Masks on the upper bounds of box constraints.
  /// Each element must be composed only by 0 or 1.0.
  ///
  Eigen::VectorXd ub_mask;

  ///
  /// @brief Inequality constraint matrix in lg < C v < ug.
  ///
  Eigen::MatrixXd C;

  ///
  /// @brief Lower bounds of inequality constraints.
  ///
  Eigen::VectorXd lg;

  ///
  /// @brief Upper bounds of inequality constraints.
  ///
  Eigen::VectorXd ug;

  ///
  /// @brief Masks on lower bounds of inequality constraints.
  /// Each element must be composed only by 0 or 1.0.
  ///
  Eigen::VectorXd lg_mask;

  ///
  /// @brief Masks on upper bounds of inequality constraints.
  /// Each element must be composed only by 0 or 1.0.
  ///
  Eigen::VectorXd ug_mask;

  ///
  /// @brief Matrix in the slack penalty (1/2) sl^T Zl sl + zl^T sl + (1/2) su^T Zu su + zu^T su.
  ///
  Eigen::MatrixXd Zl;

  ///
  /// @brief Matrix in the slack penalty (1/2) sl^T Zl sl + zl^T sl + (1/2) su^T Zu su + zu^T su.
  ///
  Eigen::MatrixXd Zu;

  ///
  /// @brief Vector in the slack penalty (1/2) sl^T Zl sl + zl^T sl + (1/2) su^T Zu su + zu^T su.
  ///
  Eigen::VectorXd zl;

  ///
  /// @brief Vector in the slack penalty (1/2) sl^T Zl sl + zl^T sl + (1/2) su^T Zu su + zu^T su.
  ///
  Eigen::VectorXd zu;

  ///
  /// @brief Indices of slack variables.
  ///
  std::vector<int> idxs;

  ///
  /// @brief Lower bounds of soft constraints.
  ///
  Eigen::VectorXd ls;

  ///
  /// @brief Upper bounds of soft constraints.
  ///
  Eigen::VectorXd us;
};

} // namespace hpipm

#endif // HPIPM_CPP_DENSE_QP_HPP_