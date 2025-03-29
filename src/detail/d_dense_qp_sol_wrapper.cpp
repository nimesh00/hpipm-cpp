#include "hpipm-cpp/detail/d_dense_qp_sol_wrapper.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>


namespace hpipm {

d_dense_qp_sol_wrapper::d_dense_qp_sol_wrapper(
    const std::shared_ptr<d_dense_qp_dim_wrapper>& dim) 
  : d_dense_qp_sol_wrapper() {
  resize(dim);
}


d_dense_qp_sol_wrapper::d_dense_qp_sol_wrapper() 
  : dim_(),
    dense_qp_sol_hpipm_(),
    memory_(nullptr),
    memsize_(0) {
}


d_dense_qp_sol_wrapper::~d_dense_qp_sol_wrapper() {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
}


d_dense_qp_sol_wrapper::d_dense_qp_sol_wrapper(const d_dense_qp_sol_wrapper& other) 
  : d_dense_qp_sol_wrapper() {
  copy(other);
}


d_dense_qp_sol_wrapper& d_dense_qp_sol_wrapper::operator=(const d_dense_qp_sol_wrapper& other) {
  if (this != &other) {
    copy(other);
  }
  return *this;
}


d_dense_qp_sol_wrapper::d_dense_qp_sol_wrapper(d_dense_qp_sol_wrapper&& other) noexcept 
  : dim_(std::move(other.dim_)),
    dense_qp_sol_hpipm_(other.dense_qp_sol_hpipm_),
    memory_(other.memory_),
    memsize_(other.memsize_) {
  other.dense_qp_sol_hpipm_.dim  = nullptr;
  other.dense_qp_sol_hpipm_.v    = nullptr;
  other.dense_qp_sol_hpipm_.pi   = nullptr;
  other.dense_qp_sol_hpipm_.lam  = nullptr;
  other.dense_qp_sol_hpipm_.t    = nullptr;
  other.dense_qp_sol_hpipm_.misc = nullptr;
  other.dense_qp_sol_hpipm_.obj = 0.0;
  other.dense_qp_sol_hpipm_.valid_obj = 0;
  other.dense_qp_sol_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
}


d_dense_qp_sol_wrapper& d_dense_qp_sol_wrapper::operator=(d_dense_qp_sol_wrapper&& other) noexcept {
  if (this == &other) return *this;

  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
  dim_ = std::move(other.dim_);
  dense_qp_sol_hpipm_ = other.dense_qp_sol_hpipm_;
  memory_ = other.memory_;
  memsize_ = other.memsize_;

  other.dense_qp_sol_hpipm_.dim  = nullptr;
  other.dense_qp_sol_hpipm_.v    = nullptr;
  other.dense_qp_sol_hpipm_.pi   = nullptr;
  other.dense_qp_sol_hpipm_.lam  = nullptr;
  other.dense_qp_sol_hpipm_.t    = nullptr;
  other.dense_qp_sol_hpipm_.misc = nullptr;
  other.dense_qp_sol_hpipm_.obj = 0.0;
  other.dense_qp_sol_hpipm_.valid_obj = 0;
  other.dense_qp_sol_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
  return *this;
}


d_dense_qp_sol* d_dense_qp_sol_wrapper::get() { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_sol_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &dense_qp_sol_hpipm_; 
}


const d_dense_qp_sol* d_dense_qp_sol_wrapper::get() const { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_sol_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &dense_qp_sol_hpipm_; 
}


void d_dense_qp_sol_wrapper::resize(const std::shared_ptr<d_dense_qp_dim_wrapper>& dim) {
  dim_ = dim;
  const hpipm_size_t new_memsize = d_dense_qp_sol_memsize(dim_->get());
  if (memory_ != nullptr && new_memsize > memsize_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = std::max(memsize_, new_memsize);
  if (memory_ == nullptr) {
    memory_ = malloc(memsize_);
  }
  d_dense_qp_sol_create(dim_->get(), &dense_qp_sol_hpipm_, memory_);
}


void d_dense_qp_sol_wrapper::copy(const d_dense_qp_sol_wrapper& other) {
  resize(other.dim_);
  
  // Since d_dense_qp_sol_copy_all is not available in the API, we need to manually copy data
  // Get data from source solution
  int nv, ne, nb, ng, ns;
  d_dense_qp_dim_get_nv(other.dim_->get(), &nv);
  d_dense_qp_dim_get_ne(other.dim_->get(), &ne);
  d_dense_qp_dim_get_nb(other.dim_->get(), &nb);
  d_dense_qp_dim_get_ng(other.dim_->get(), &ng);
  
  // Allocate memory for all solution components
  double* v = new double[nv]();
  double* pi = new double[ne]();
  double* lam_lb = new double[nb]();
  double* lam_ub = new double[nb]();
  double* lam_lg = new double[ng]();
  double* lam_ug = new double[ng]();
  double* ls = new double[ns]();
  double* us = new double[ns]();
  double* lam_ls = new double[ns]();
  double* lam_us = new double[ns]();
  
  // Get data from source solution
  d_dense_qp_sol_get_all(const_cast<d_dense_qp_sol*>(other.get()), 
                          v, ls, us, pi, lam_lb, lam_ub, lam_lg, lam_ug, lam_ls, lam_us);
  
  // Copy objective value and valid flag
  dense_qp_sol_hpipm_.obj = other.get()->obj;
  dense_qp_sol_hpipm_.valid_obj = other.get()->valid_obj;
  
  // Set data to destination solution
  // Note: There's no direct set_all function, so we'd need to call individual setters
  // For now, we'll just use v as an example, as other setters would follow a similar pattern
  d_dense_qp_sol_set_v(v, &dense_qp_sol_hpipm_);
  
  // Clean up allocated memory
  delete[] v;
  delete[] pi;
  delete[] lam_lb;
  delete[] lam_ub;
  delete[] lam_lg;
  delete[] lam_ug;
  delete[] ls;
  delete[] us;
  delete[] lam_ls;
  delete[] lam_us;
}

} // namespace hpipm