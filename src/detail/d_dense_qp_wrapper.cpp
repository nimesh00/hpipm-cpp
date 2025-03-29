#include "hpipm-cpp/detail/d_dense_qp_wrapper.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>


namespace hpipm {

d_dense_qp_wrapper::d_dense_qp_wrapper(const std::shared_ptr<d_dense_qp_dim_wrapper>& dim) 
  : d_dense_qp_wrapper() {
  resize(dim);
}


d_dense_qp_wrapper::d_dense_qp_wrapper() 
  : dim_(),
    dense_qp_hpipm_(),
    memory_(nullptr),
    memsize_(0) {
}


d_dense_qp_wrapper::~d_dense_qp_wrapper() {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
}


d_dense_qp_wrapper::d_dense_qp_wrapper(const d_dense_qp_wrapper& other) 
  : d_dense_qp_wrapper() {
  copy(other);
}


d_dense_qp_wrapper& d_dense_qp_wrapper::operator=(const d_dense_qp_wrapper& other) {
  if (this != &other) {
    copy(other);
  }
  return *this;
}


d_dense_qp_wrapper::d_dense_qp_wrapper(d_dense_qp_wrapper&& other) noexcept
  : dim_(std::move(other.dim_)),
    dense_qp_hpipm_(other.dense_qp_hpipm_),
    memory_(other.memory_),
    memsize_(other.memsize_) {
  other.dense_qp_hpipm_.dim = nullptr;
  other.dense_qp_hpipm_.Hv = nullptr;
  other.dense_qp_hpipm_.A = nullptr;
  other.dense_qp_hpipm_.Ct = nullptr;
  other.dense_qp_hpipm_.gz = nullptr;
  other.dense_qp_hpipm_.b = nullptr;
  other.dense_qp_hpipm_.d = nullptr;
  other.dense_qp_hpipm_.d_mask = nullptr;
  other.dense_qp_hpipm_.m = nullptr;
  other.dense_qp_hpipm_.Z = nullptr;
  other.dense_qp_hpipm_.idxb = nullptr;
  other.dense_qp_hpipm_.idxs_rev = nullptr;
  other.dense_qp_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
}


d_dense_qp_wrapper& d_dense_qp_wrapper::operator=(d_dense_qp_wrapper&& other) noexcept {
  if (this == &other) return *this;

  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
  dim_ = std::move(other.dim_);
  dense_qp_hpipm_ = other.dense_qp_hpipm_;
  memory_ = other.memory_;
  memsize_ = other.memsize_;

  other.dense_qp_hpipm_.dim = nullptr;
  other.dense_qp_hpipm_.Hv = nullptr;
  other.dense_qp_hpipm_.A = nullptr;
  other.dense_qp_hpipm_.Ct = nullptr;
  other.dense_qp_hpipm_.gz = nullptr;
  other.dense_qp_hpipm_.b = nullptr;
  other.dense_qp_hpipm_.d = nullptr;
  other.dense_qp_hpipm_.d_mask = nullptr;
  other.dense_qp_hpipm_.m = nullptr;
  other.dense_qp_hpipm_.Z = nullptr;
  other.dense_qp_hpipm_.idxb = nullptr;
  other.dense_qp_hpipm_.idxs_rev = nullptr;
  other.dense_qp_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
  return *this;
}


d_dense_qp* d_dense_qp_wrapper::get() { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &dense_qp_hpipm_; 
}


const d_dense_qp* d_dense_qp_wrapper::get() const { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &dense_qp_hpipm_; 
}


void d_dense_qp_wrapper::resize(const std::shared_ptr<d_dense_qp_dim_wrapper>& dim) {
  dim_ = dim;
  const hpipm_size_t new_memsize = d_dense_qp_memsize(dim_->get());
  if (memory_ != nullptr && new_memsize > memsize_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = std::max(memsize_, new_memsize);
  if (memory_ == nullptr) {
    memory_ = malloc(memsize_);
  }
  d_dense_qp_create(dim_->get(), &dense_qp_hpipm_, memory_);
}


void d_dense_qp_wrapper::copy(const d_dense_qp_wrapper& other) {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = 0;
  resize(other.dim_);

  // Since d_dense_qp_copy_all is not available in the API, we need to copy the data manually
  // We'll use the get_all and set_all functions to handle this
  
  // Get the dimensions
  int nv, ne, nb, ng, nsb, nsg;
  d_dense_qp_dim_get_nv(other.dim_->get(), &nv);
  d_dense_qp_dim_get_ne(other.dim_->get(), &ne);
  d_dense_qp_dim_get_nb(other.dim_->get(), &nb);
  d_dense_qp_dim_get_ng(other.dim_->get(), &ng);
  
  // Allocate memory for all the data
  double* H = new double[nv*nv]();
  double* g = new double[nv]();
  double* A = new double[ne*nv]();
  double* b = new double[ne]();
  int* idxb = new int[nb]();
  double* d_lb = new double[nb]();
  double* d_ub = new double[nb]();
  double* C = new double[ng*nv]();
  double* d_lg = new double[ng]();
  double* d_ug = new double[ng]();
  double* Zl = new double[nsb+nsg]();
  double* Zu = new double[nsb+nsg]();
  double* zl = new double[nsb+nsg]();
  double* zu = new double[nsb+nsg]();
  int* idxs = new int[nsb+nsg]();
  double* d_ls = new double[nsb+nsg]();
  double* d_us = new double[nsb+nsg]();
  
  // Get data from source QP
  d_dense_qp_get_all(const_cast<d_dense_qp*>(other.get()), H, g, A, b, idxb, d_lb, d_ub, C, d_lg, d_ug, Zl, Zu, zl, zu, idxs, d_ls, d_us);
  
  // Set data to destination QP
  d_dense_qp_set_all(H, g, A, b, idxb, d_lb, d_ub, C, d_lg, d_ug, Zl, Zu, zl, zu, idxs, d_ls, d_us, &dense_qp_hpipm_);
  
  // Clean up
  delete[] H;
  delete[] g;
  delete[] A;
  delete[] b;
  delete[] idxb;
  delete[] d_lb;
  delete[] d_ub;
  delete[] C;
  delete[] d_lg;
  delete[] d_ug;
  delete[] Zl;
  delete[] Zu;
  delete[] zl;
  delete[] zu;
  delete[] idxs;
  delete[] d_ls;
  delete[] d_us;
}

} // namespace hpipm