#include "hpipm-cpp/dense_qp_ipm_solver.hpp"

#include <algorithm>
#include <cstdlib>

#include "hpipm-cpp/detail/d_dense_qp_dim_wrapper.hpp"
#include "hpipm-cpp/detail/d_dense_qp_ipm_arg_wrapper.hpp"
#include "hpipm-cpp/detail/d_dense_qp_ipm_ws_wrapper.hpp"
#include "hpipm-cpp/detail/d_dense_qp_sol_wrapper.hpp"
#include "hpipm-cpp/detail/d_dense_qp_wrapper.hpp"

extern "C" {
#include "hpipm_d_dense_qp_ipm.h"
}

namespace hpipm {

struct DenseQpIpmSolver::WrapperHolder {
    WrapperHolder()
        : dense_qp_dim_wrapper(std::make_shared<d_dense_qp_dim_wrapper>()),
          dense_qp_ipm_arg_wrapper(
              std::make_shared<d_dense_qp_ipm_arg_wrapper>()),
          dense_qp_wrapper(),
          dense_qp_sol_wrapper(),
          dense_qp_ipm_ws_wrapper() {}
    ~WrapperHolder() = default;

    std::shared_ptr<d_dense_qp_dim_wrapper> dense_qp_dim_wrapper;
    std::shared_ptr<d_dense_qp_ipm_arg_wrapper> dense_qp_ipm_arg_wrapper;
    d_dense_qp_wrapper dense_qp_wrapper;
    d_dense_qp_sol_wrapper dense_qp_sol_wrapper;
    d_dense_qp_ipm_ws_wrapper dense_qp_ipm_ws_wrapper;
};

DenseQpIpmSolver::DenseQpIpmSolver(
    const DenseQp& dense_qp, const DenseQpIpmSolverSettings& solver_settings)
    : solver_settings_(),
      solver_statistics_(),
      dim_(),
      wrapper_holder_(new WrapperHolder()) {
    setSolverSettings(solver_settings);
    resize(dense_qp);
}

DenseQpIpmSolver::DenseQpIpmSolver(
    const DenseQpIpmSolverSettings& solver_settings)
    : solver_settings_(),
      solver_statistics_(),
      dim_(),
      wrapper_holder_(new WrapperHolder()) {
    setSolverSettings(solver_settings);
}

DenseQpIpmSolver::~DenseQpIpmSolver() {}

void DenseQpIpmSolver::setSolverSettings(
    const DenseQpIpmSolverSettings& solver_settings) {
    d_dense_qp_ipm_arg* dense_qp_ipm_arg_ptr =
        wrapper_holder_->dense_qp_ipm_arg_wrapper->get();
    solver_settings_ = solver_settings;
    auto convertHpipmMode = [](const HpipmMode mode) {
        switch (mode) {
            case HpipmMode::SpeedAbs:
                return hpipm_mode::SPEED_ABS;
                break;
            case HpipmMode::Speed:
                return hpipm_mode::SPEED;
            case HpipmMode::Balance:
                return hpipm_mode::BALANCE;
            case HpipmMode::Robust:
                return hpipm_mode::ROBUST;
            default:
                return hpipm_mode::SPEED;
                break;
        }
    };
    auto hpipm_mode = convertHpipmMode(solver_settings_.mode);
    d_dense_qp_ipm_arg_set_default(hpipm_mode, dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_mu0(&solver_settings_.mu0, dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_iter_max(&solver_settings_.iter_max,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_alpha_min(&solver_settings_.alpha_min,
                                     dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_mu0(&solver_settings_.mu0, dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_tol_stat(&solver_settings_.tol_stat,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_tol_eq(&solver_settings_.tol_eq,
                                  dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_tol_ineq(&solver_settings_.tol_ineq,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_tol_comp(&solver_settings_.tol_comp,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_tol_dual_gap(&solver_settings_.tol_dual_gap,
                                        dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_reg_prim(&solver_settings_.reg_prim,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_reg_dual(&solver_settings_.reg_dual,
                                    dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_warm_start(&solver_settings_.warm_start,
                                      dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_pred_corr(&solver_settings_.pred_corr,
                                     dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_cond_pred_corr(&solver_settings_.cond_pred_corr,
                                          dense_qp_ipm_arg_ptr);
    int scale = solver_settings_.scale;
    d_dense_qp_ipm_arg_set("scale", &scale, dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_kkt_fact_alg(&solver_settings_.kkt_fact_alg,
                                        dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_remove_lin_dep_eq(
        &solver_settings_.remove_lin_dep_eq, dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_compute_obj(&solver_settings_.compute_obj,
                                       dense_qp_ipm_arg_ptr);
    d_dense_qp_ipm_arg_set_split_step(&solver_settings_.split_step,
                                      dense_qp_ipm_arg_ptr);
}

void DenseQpIpmSolver::resize(const DenseQp& dense_qp) {
    dim_.resize(dense_qp);
    d_dense_qp_dim* dense_qp_dim_ptr =
        wrapper_holder_->dense_qp_dim_wrapper->get();
    d_dense_qp_dim_set_nv(dim_.nv, dense_qp_dim_ptr);
    d_dense_qp_dim_set_ne(dim_.ne, dense_qp_dim_ptr);
    d_dense_qp_dim_set_nb(dim_.nb, dense_qp_dim_ptr);
    d_dense_qp_dim_set_ng(dim_.ng, dense_qp_dim_ptr);
    d_dense_qp_dim_set_nsb(dim_.nsb, dense_qp_dim_ptr);
    d_dense_qp_dim_set_nsg(dim_.nsg, dense_qp_dim_ptr);
    d_dense_qp_dim_set_ns(dim_.ns, dense_qp_dim_ptr);

    // resize hpipm data
    wrapper_holder_->dense_qp_wrapper.resize(
        wrapper_holder_->dense_qp_dim_wrapper);
    wrapper_holder_->dense_qp_sol_wrapper.resize(
        wrapper_holder_->dense_qp_dim_wrapper);
    wrapper_holder_->dense_qp_ipm_ws_wrapper.resize(
        wrapper_holder_->dense_qp_dim_wrapper,
        wrapper_holder_->dense_qp_ipm_arg_wrapper);
}

HpipmStatus DenseQpIpmSolver::solve(DenseQp& dense_qp,
                                    DenseQpSolution& qp_sol) {
    // resize data
    resize(dense_qp);

    // Set up solution vectors
    if (solver_settings_.warm_start) {
        if (qp_sol.v.size() != dim_.nv) {
            throw std::runtime_error("qp_sol.v.size() must be " +
                                     std::to_string(dim_.nv));
        }
    } else {
        qp_sol.v.resize(dim_.nv);
    }

    qp_sol.pi.resize(dim_.ne);
    qp_sol.lam_lb.resize(dim_.nb);
    qp_sol.lam_ub.resize(dim_.nb);
    qp_sol.lam_lg.resize(dim_.ng);
    qp_sol.lam_ug.resize(dim_.ng);
    qp_sol.sl.resize(dim_.ns);
    qp_sol.su.resize(dim_.ns);

    // Set QP data
    H_ptr_ = dense_qp.H.data();
    g_ptr_ = dense_qp.g.data();
    A_ptr_ = dense_qp.A.data();
    b_ptr_ = dense_qp.b.data();
    idxb_ptr_ = dense_qp.idxb.data();
    lb_ptr_ = dense_qp.lb.data();
    ub_ptr_ = dense_qp.ub.data();
    C_ptr_ = dense_qp.C.data();
    lg_ptr_ = dense_qp.lg.data();
    ug_ptr_ = dense_qp.ug.data();
    Zl_ptr_ = dense_qp.Zl.data();
    Zu_ptr_ = dense_qp.Zu.data();
    zl_ptr_ = dense_qp.zl.data();
    zu_ptr_ = dense_qp.zu.data();
    idxs_ptr_ = dense_qp.idxs.data();
    ls_ptr_ = dense_qp.ls.data();
    us_ptr_ = dense_qp.us.data();

    d_dense_qp* dense_qp_ptr = wrapper_holder_->dense_qp_wrapper.get();
    d_dense_qp_set_all(H_ptr_, g_ptr_, A_ptr_, b_ptr_, idxb_ptr_, lb_ptr_,
                       ub_ptr_, C_ptr_, lg_ptr_, ug_ptr_, Zl_ptr_, Zu_ptr_,
                       zl_ptr_, zu_ptr_, idxs_ptr_, ls_ptr_, us_ptr_,
                       dense_qp_ptr);

    // Set constraint masks if provided
    if (dense_qp.lb_mask.size() == dim_.nb) {
        lb_mask_ptr_ = dense_qp.lb_mask.data();
        d_dense_qp_set_lb_mask(lb_mask_ptr_, dense_qp_ptr);
    }

    if (dense_qp.ub_mask.size() == dim_.nb) {
        ub_mask_ptr_ = dense_qp.ub_mask.data();
        d_dense_qp_set_ub_mask(ub_mask_ptr_, dense_qp_ptr);
    }

    if (dense_qp.lg_mask.size() == dim_.ng) {
        lg_mask_ptr_ = dense_qp.lg_mask.data();
        d_dense_qp_set_lg_mask(lg_mask_ptr_, dense_qp_ptr);
    }

    if (dense_qp.ug_mask.size() == dim_.ng) {
        ug_mask_ptr_ = dense_qp.ug_mask.data();
        d_dense_qp_set_ug_mask(ug_mask_ptr_, dense_qp_ptr);
    }

    // Solve QP
    d_dense_qp_dim* dense_qp_dim_ptr =
        wrapper_holder_->dense_qp_dim_wrapper->get();
    d_dense_qp_sol* dense_qp_sol_ptr =
        wrapper_holder_->dense_qp_sol_wrapper.get();
    d_dense_qp_ipm_arg* dense_qp_ipm_arg_ptr =
        wrapper_holder_->dense_qp_ipm_arg_wrapper->get();
    d_dense_qp_ipm_ws* dense_qp_ipm_ws_ptr =
        wrapper_holder_->dense_qp_ipm_ws_wrapper.get();

    // Use warm start if requested
    if (solver_settings_.warm_start) {
        d_dense_qp_sol_set_v(qp_sol.v.data(), dense_qp_sol_ptr);
    }

    // Solve the QP
    d_dense_qp_ipm_solve(dense_qp_ptr, dense_qp_sol_ptr, dense_qp_ipm_arg_ptr,
                         dense_qp_ipm_ws_ptr);

    // Get solution
    d_dense_qp_sol_get_v(dense_qp_sol_ptr, qp_sol.v.data());
    d_dense_qp_sol_get_pi(dense_qp_sol_ptr, qp_sol.pi.data());
    d_dense_qp_sol_get_lam_lb(dense_qp_sol_ptr, qp_sol.lam_lb.data());
    d_dense_qp_sol_get_lam_ub(dense_qp_sol_ptr, qp_sol.lam_ub.data());
    d_dense_qp_sol_get_lam_lg(dense_qp_sol_ptr, qp_sol.lam_lg.data());
    d_dense_qp_sol_get_lam_ug(dense_qp_sol_ptr, qp_sol.lam_ug.data());

    // Get objective value if it was computed
    if (solver_settings_.compute_obj) {
        int valid_obj;
        d_dense_qp_sol_get_valid_obj(dense_qp_sol_ptr, &valid_obj);
        if (valid_obj) {
            d_dense_qp_sol_get_obj(dense_qp_sol_ptr, &qp_sol.obj_value);
        }
    }

    // Get solver statistics
    d_dense_qp_ipm_get_status(dense_qp_ipm_ws_ptr, &solver_statistics_.status);
    d_dense_qp_ipm_get_iter(dense_qp_ipm_ws_ptr, &solver_statistics_.iter);
    d_dense_qp_ipm_get_max_res_stat(dense_qp_ipm_ws_ptr,
                                    &solver_statistics_.max_res_stat);
    d_dense_qp_ipm_get_max_res_eq(dense_qp_ipm_ws_ptr,
                                  &solver_statistics_.max_res_eq);
    d_dense_qp_ipm_get_max_res_ineq(dense_qp_ipm_ws_ptr,
                                    &solver_statistics_.max_res_ineq);
    d_dense_qp_ipm_get_max_res_comp(dense_qp_ipm_ws_ptr,
                                    &solver_statistics_.max_res_comp);
    d_dense_qp_ipm_get_obj(dense_qp_ipm_ws_ptr, &solver_statistics_.obj_value);

    // Get detailed statistics
    const int stat_m = 18;
    solver_statistics_.clear();
    solver_statistics_.reserve(solver_statistics_.iter + 2);
    for (int i = 0; i <= solver_statistics_.iter + 1; ++i) {
        if (i * stat_m < dense_qp_ipm_ws_ptr->stat_max) {
            solver_statistics_.alpha_aff.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 0]);
            solver_statistics_.mu_aff.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 1]);
            solver_statistics_.sigma.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 2]);
            solver_statistics_.alpha_prim.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 3]);
            solver_statistics_.alpha_dual.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 4]);
            solver_statistics_.mu.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 5]);
            solver_statistics_.res_stat.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 6]);
            solver_statistics_.res_eq.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 7]);
            solver_statistics_.res_ineq.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 8]);
            solver_statistics_.res_comp.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 9]);
            solver_statistics_.obj.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 10]);
            solver_statistics_.lq_fact.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 11]);
            solver_statistics_.itref_pred.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 12]);
            solver_statistics_.itref_corr.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 13]);
            solver_statistics_.lin_res_stat.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 14]);
            solver_statistics_.lin_res_eq.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 15]);
            solver_statistics_.lin_res_ineq.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 16]);
            solver_statistics_.lin_res_comp.push_back(
                dense_qp_ipm_ws_ptr->stat[stat_m * i + 17]);
        }
    }

    // Map HPIPM status to our enum
    int hpipm_status = solver_statistics_.status;
    if (0 <= hpipm_status && hpipm_status <= 3) {
        return static_cast<HpipmStatus>(hpipm_status);
    } else {
        return HpipmStatus::UnknownFailure;
    }
}

const DenseQpIpmSolverSettings& DenseQpIpmSolver::getIpmSolverSettings() const {
    return solver_settings_;
}

const DenseQpIpmSolverStatistics& DenseQpIpmSolver::getSolverStatistics()
    const {
    return solver_statistics_;
}

}  // namespace hpipm