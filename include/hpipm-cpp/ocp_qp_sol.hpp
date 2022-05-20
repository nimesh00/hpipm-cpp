#ifndef HPIPM_CPP_OCP_QP_SOL_HPP_
#define HPIPM_CPP_OCP_QP_SOL_HPP_

#include <vector>
#include <string>

#include "Eigen/Core"

extern "C" {
#include "hpipm_d_ocp_qp_sol.h"
}

#include "hpipm-cpp/ocp_qp_dim.hpp"


namespace hpipm {

struct ocp_qp_sol {
public:
  ocp_qp_sol() = default;

  ~ocp_qp_sol();

  void create_hpipm(ocp_qp_dim& dim);

  d_ocp_qp_sol* to_hpipm() { return &ocp_qp_sol_hpipm_; }

  const d_ocp_qp_sol* to_hpipm() const { return &ocp_qp_sol_hpipm_; }

  void from_hpipm(const ocp_qp_dim& dim);

  std::vector<Eigen::VectorXd> x;
  std::vector<Eigen::VectorXd> u;
  std::vector<Eigen::VectorXd> pi; // the Lagrange multiplier w.r.t. the state equation

  // TODO: treat the following variables
  // std::vector<Eigen::VectorXd> sl; // slack lower 
  // std::vector<Eigen::VectorXd> su; // slack upper 
  // std::vector<Eigen::VectorXd> lam_lb; // the Lagrange multiplier w.r.t. the lower box constraint 
  // std::vector<Eigen::VectorXd> lam_ub; // the Lagrange multiplier w.r.t. the upper box constraint
  // std::vector<Eigen::VectorXd> lam_lg; // the Lagrange multiplier w.r.t. the lower constraint
  // std::vector<Eigen::VectorXd> lam_ug; // the Lagrange multiplier w.r.t. the upper constraint
  // std::vector<Eigen::VectorXd> lam_ls; // the Lagrange multiplier w.r.t. the lower soft constraint
  // std::vector<Eigen::VectorXd> lam_us; // the Lagrange multiplier w.r.t. the upper soft constraint

  void s_qp_sol_get_all(struct s_ocp_qp_sol *qp_sol, float **u, float **x, float **ls, float **us, float **pi, float **lam_lb, float **lam_ub, float **lam_lg, float **lam_ug, float **lam_ls, float **lam_us);

private:
  d_ocp_qp_sol ocp_qp_sol_hpipm_;
  void *memory_ = nullptr;
  hpipm_size_t memsize_ = 0;
};

} // namespace hpipm

#endif // HPIPM_CPP_OCP_QP_SOL_HPP_