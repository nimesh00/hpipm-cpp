#include "hpipm-cpp/detail/d_dense_qp_ipm_arg_wrapper.hpp"


namespace hpipm {

d_dense_qp_ipm_arg_wrapper::d_dense_qp_ipm_arg_wrapper() 
  : dense_qp_ipm_arg_hpipm_(),
    memory_(nullptr),
    memsize_(0) {
  resize();
}


d_dense_qp_ipm_arg_wrapper::~d_dense_qp_ipm_arg_wrapper() {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
}


d_dense_qp_ipm_arg_wrapper::d_dense_qp_ipm_arg_wrapper(const d_dense_qp_ipm_arg_wrapper& other)
  : d_dense_qp_ipm_arg_wrapper() {
  copy(other);
}


d_dense_qp_ipm_arg_wrapper& d_dense_qp_ipm_arg_wrapper::operator=(const d_dense_qp_ipm_arg_wrapper& other) {
  if (this != &other) {
    copy(other);
  }
  return *this;
}


d_dense_qp_ipm_arg_wrapper::d_dense_qp_ipm_arg_wrapper(d_dense_qp_ipm_arg_wrapper&& other) noexcept 
  : dense_qp_ipm_arg_hpipm_(other.dense_qp_ipm_arg_hpipm_),
    memory_(other.memory_),
    memsize_(other.memsize_) {
  other.memory_ = nullptr;
  other.memsize_ = 0;
}


d_dense_qp_ipm_arg_wrapper& d_dense_qp_ipm_arg_wrapper::operator=(d_dense_qp_ipm_arg_wrapper&& other) noexcept {
  if (this == &other) return *this;

  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
  dense_qp_ipm_arg_hpipm_ = other.dense_qp_ipm_arg_hpipm_;
  memory_ = other.memory_;
  memsize_ = other.memsize_;

  other.memory_ = nullptr;
  other.memsize_ = 0;
  return *this;
}


d_dense_qp_ipm_arg* d_dense_qp_ipm_arg_wrapper::get() { 
  return &dense_qp_ipm_arg_hpipm_; 
}


const d_dense_qp_ipm_arg* d_dense_qp_ipm_arg_wrapper::get() const { 
  return &dense_qp_ipm_arg_hpipm_; 
}


void d_dense_qp_ipm_arg_wrapper::resize() {
  d_dense_qp_dim dense_qp_dim_hpipm; // this does nothing in the below
  const hpipm_size_t new_memsize = d_dense_qp_ipm_arg_memsize(&dense_qp_dim_hpipm);
  if (memory_ && new_memsize > memsize_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = std::max(memsize_, new_memsize);
  if (!memory_) {
    memory_ = malloc(memsize_);
    d_dense_qp_ipm_arg_create(&dense_qp_dim_hpipm, &dense_qp_ipm_arg_hpipm_, memory_);
  }
}


void d_dense_qp_ipm_arg_wrapper::copy(const d_dense_qp_ipm_arg_wrapper& other) {
  const d_dense_qp_ipm_arg* other_ptr = other.get();
  dense_qp_ipm_arg_hpipm_.mu0 = other_ptr->mu0;
  dense_qp_ipm_arg_hpipm_.alpha_min = other_ptr->alpha_min;
  dense_qp_ipm_arg_hpipm_.res_g_max = other_ptr->res_g_max;
  dense_qp_ipm_arg_hpipm_.res_b_max = other_ptr->res_b_max;
  dense_qp_ipm_arg_hpipm_.res_d_max = other_ptr->res_d_max;
  dense_qp_ipm_arg_hpipm_.res_m_max = other_ptr->res_m_max;
  dense_qp_ipm_arg_hpipm_.dual_gap_max = other_ptr->dual_gap_max;
  dense_qp_ipm_arg_hpipm_.reg_prim = other_ptr->reg_prim;
  dense_qp_ipm_arg_hpipm_.reg_dual = other_ptr->reg_dual;
  dense_qp_ipm_arg_hpipm_.lam_min = other_ptr->lam_min;
  dense_qp_ipm_arg_hpipm_.t_min = other_ptr->t_min;
  dense_qp_ipm_arg_hpipm_.tau_min = other_ptr->tau_min;
  dense_qp_ipm_arg_hpipm_.iter_max = other_ptr->iter_max;
  dense_qp_ipm_arg_hpipm_.stat_max = other_ptr->stat_max;
  dense_qp_ipm_arg_hpipm_.pred_corr = other_ptr->pred_corr;
  dense_qp_ipm_arg_hpipm_.cond_pred_corr = other_ptr->cond_pred_corr;
  dense_qp_ipm_arg_hpipm_.scale = other_ptr->scale;
  dense_qp_ipm_arg_hpipm_.itref_pred_max = other_ptr->itref_pred_max;
  dense_qp_ipm_arg_hpipm_.itref_corr_max = other_ptr->itref_corr_max;
  dense_qp_ipm_arg_hpipm_.warm_start = other_ptr->warm_start;
  dense_qp_ipm_arg_hpipm_.lq_fact = other_ptr->lq_fact;
  dense_qp_ipm_arg_hpipm_.abs_form = other_ptr->abs_form;
  dense_qp_ipm_arg_hpipm_.comp_res_exit = other_ptr->comp_res_exit;
  dense_qp_ipm_arg_hpipm_.comp_res_pred = other_ptr->comp_res_pred;
  dense_qp_ipm_arg_hpipm_.kkt_fact_alg = other_ptr->kkt_fact_alg;
  dense_qp_ipm_arg_hpipm_.remove_lin_dep_eq = other_ptr->remove_lin_dep_eq;
  dense_qp_ipm_arg_hpipm_.compute_obj = other_ptr->compute_obj;
  dense_qp_ipm_arg_hpipm_.split_step = other_ptr->split_step;
  dense_qp_ipm_arg_hpipm_.t_lam_min = other_ptr->t_lam_min;
  dense_qp_ipm_arg_hpipm_.mode = other_ptr->mode;
}

} // namespace hpipm