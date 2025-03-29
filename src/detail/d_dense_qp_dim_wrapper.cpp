#include "hpipm-cpp/detail/d_dense_qp_dim_wrapper.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>


namespace hpipm {

d_dense_qp_dim_wrapper::d_dense_qp_dim_wrapper() 
  : dense_qp_dim_hpipm_(),
    memory_(nullptr),
    memsize_(0) {
  memsize_ = d_dense_qp_dim_memsize();
  memory_ = malloc(memsize_);
  d_dense_qp_dim_create(&dense_qp_dim_hpipm_, memory_);
}


d_dense_qp_dim_wrapper::~d_dense_qp_dim_wrapper() {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
}


d_dense_qp_dim_wrapper::d_dense_qp_dim_wrapper(const d_dense_qp_dim_wrapper& other) 
  : d_dense_qp_dim_wrapper() {
  copy(other);
}


d_dense_qp_dim_wrapper& d_dense_qp_dim_wrapper::operator=(const d_dense_qp_dim_wrapper& other) {
  if (this != &other) {
    copy(other);
  }
  return *this;
}


d_dense_qp_dim_wrapper::d_dense_qp_dim_wrapper(d_dense_qp_dim_wrapper&& other) noexcept
  : dense_qp_dim_hpipm_(other.dense_qp_dim_hpipm_),
    memory_(other.memory_),
    memsize_(other.memsize_) {
  other.dense_qp_dim_hpipm_.nv = 0;
  other.dense_qp_dim_hpipm_.ne = 0;
  other.dense_qp_dim_hpipm_.nb = 0;
  other.dense_qp_dim_hpipm_.ng = 0;
  other.dense_qp_dim_hpipm_.nsb = 0;
  other.dense_qp_dim_hpipm_.nsg = 0;
  other.dense_qp_dim_hpipm_.ns = 0;
  other.dense_qp_dim_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
}


d_dense_qp_dim_wrapper& d_dense_qp_dim_wrapper::operator=(d_dense_qp_dim_wrapper&& other) noexcept {
  if (this == &other) return *this;

  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
  dense_qp_dim_hpipm_ = other.dense_qp_dim_hpipm_;
  memory_ = other.memory_;
  memsize_ = other.memsize_;

  other.dense_qp_dim_hpipm_.nv = 0;
  other.dense_qp_dim_hpipm_.ne = 0;
  other.dense_qp_dim_hpipm_.nb = 0;
  other.dense_qp_dim_hpipm_.ng = 0;
  other.dense_qp_dim_hpipm_.nsb = 0;
  other.dense_qp_dim_hpipm_.nsg = 0;
  other.dense_qp_dim_hpipm_.ns = 0;
  other.dense_qp_dim_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
  return *this;
}


d_dense_qp_dim* d_dense_qp_dim_wrapper::get() { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_dim_wrapper] hpipm memory is not created.");
  }
  return &dense_qp_dim_hpipm_; 
}


const d_dense_qp_dim* d_dense_qp_dim_wrapper::get() const { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_dense_qp_dim_wrapper] hpipm memory is not created.");
  }
  return &dense_qp_dim_hpipm_; 
}


void d_dense_qp_dim_wrapper::copy(const d_dense_qp_dim_wrapper& other) {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = 0;
  
  memsize_ = d_dense_qp_dim_memsize();
  memory_ = malloc(memsize_);
  d_dense_qp_dim_create(&dense_qp_dim_hpipm_, memory_);

  // Copy all dimension values from other
  dense_qp_dim_hpipm_.nv = other.get()->nv;
  dense_qp_dim_hpipm_.ne = other.get()->ne;
  dense_qp_dim_hpipm_.nb = other.get()->nb;
  dense_qp_dim_hpipm_.ng = other.get()->ng;
  dense_qp_dim_hpipm_.nsb = other.get()->nsb;
  dense_qp_dim_hpipm_.nsg = other.get()->nsg;
  dense_qp_dim_hpipm_.ns = other.get()->ns;
}

} // namespace hpipm