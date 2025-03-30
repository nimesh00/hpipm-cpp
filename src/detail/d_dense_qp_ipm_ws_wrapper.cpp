#include "hpipm-cpp/detail/d_dense_qp_ipm_ws_wrapper.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace hpipm {

d_dense_qp_ipm_ws_wrapper::d_dense_qp_ipm_ws_wrapper(
    const std::shared_ptr<d_dense_qp_dim_wrapper>& dim,
    const std::shared_ptr<d_dense_qp_ipm_arg_wrapper>& ipm_arg)
    : d_dense_qp_ipm_ws_wrapper() {
    resize(dim, ipm_arg);
}

d_dense_qp_ipm_ws_wrapper::d_dense_qp_ipm_ws_wrapper()
    : dim_(),
      ipm_arg_(),
      dense_qp_ipm_ws_hpipm_(),
      memory_(nullptr),
      memsize_(0) {}

d_dense_qp_ipm_ws_wrapper::~d_dense_qp_ipm_ws_wrapper() {
    if (memory_) {
        free(memory_);
        memory_ = nullptr;
        memsize_ = 0;
    }
}

d_dense_qp_ipm_ws_wrapper::d_dense_qp_ipm_ws_wrapper(
    d_dense_qp_ipm_ws_wrapper&& other) noexcept
    : dim_(std::move(other.dim_)),
      ipm_arg_(std::move(other.ipm_arg_)),
      dense_qp_ipm_ws_hpipm_(other.dense_qp_ipm_ws_hpipm_),
      memory_(other.memory_),
      memsize_(other.memsize_) {
    other.dense_qp_ipm_ws_hpipm_.core_workspace = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_ws = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sol_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sol_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.qp_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.qp_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Gamma = nullptr;
    other.dense_qp_ipm_ws_hpipm_.gamma = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Zs_inv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Lv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.AL = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Le = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Ctx = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.se = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nbg = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq0 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_m = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_LQ = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_Q = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Zt = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ZtH = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ZtHZ = nullptr;
    other.dense_qp_ipm_ws_hpipm_.xy = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Yxy = nullptr;
    other.dense_qp_ipm_ws_hpipm_.xz = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_2ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nv2ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_li = nullptr;
    other.dense_qp_ipm_ws_hpipm_.b_li = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_bkp = nullptr;
    other.dense_qp_ipm_ws_hpipm_.b_bkp = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Ab_LU = nullptr;
    other.dense_qp_ipm_ws_hpipm_.stat = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_v = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_e = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_e1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work0 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work_null = nullptr;
    other.dense_qp_ipm_ws_hpipm_.orglq_work_null = nullptr;
    other.dense_qp_ipm_ws_hpipm_.memsize = 0;
    other.memory_ = nullptr;
    other.memsize_ = 0;
}

d_dense_qp_ipm_ws_wrapper& d_dense_qp_ipm_ws_wrapper::operator=(
    d_dense_qp_ipm_ws_wrapper&& other) noexcept {
    if (this == &other) return *this;

    if (memory_) {
        free(memory_);
        memory_ = nullptr;
        memsize_ = 0;
    }
    dim_ = std::move(other.dim_);
    ipm_arg_ = std::move(other.ipm_arg_);
    dense_qp_ipm_ws_hpipm_ = other.dense_qp_ipm_ws_hpipm_;
    memory_ = other.memory_;
    memsize_ = other.memsize_;

    other.dense_qp_ipm_ws_hpipm_.core_workspace = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_ws = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sol_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sol_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.qp_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.qp_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_itref = nullptr;
    other.dense_qp_ipm_ws_hpipm_.res_step = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Gamma = nullptr;
    other.dense_qp_ipm_ws_hpipm_.gamma = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Zs_inv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Lv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.AL = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Le = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Ctx = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.sv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.se = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nbg = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq0 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_m = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_LQ = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_Q = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Zt = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ZtH = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ZtHZ = nullptr;
    other.dense_qp_ipm_ws_hpipm_.xy = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Yxy = nullptr;
    other.dense_qp_ipm_ws_hpipm_.xz = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nv = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_2ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.tmp_nv2ns = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_li = nullptr;
    other.dense_qp_ipm_ws_hpipm_.b_li = nullptr;
    other.dense_qp_ipm_ws_hpipm_.A_bkp = nullptr;
    other.dense_qp_ipm_ws_hpipm_.b_bkp = nullptr;
    other.dense_qp_ipm_ws_hpipm_.Ab_LU = nullptr;
    other.dense_qp_ipm_ws_hpipm_.stat = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_v = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_e = nullptr;
    other.dense_qp_ipm_ws_hpipm_.ipiv_e1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work0 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work1 = nullptr;
    other.dense_qp_ipm_ws_hpipm_.lq_work_null = nullptr;
    other.dense_qp_ipm_ws_hpipm_.orglq_work_null = nullptr;
    other.dense_qp_ipm_ws_hpipm_.memsize = 0;
    other.memory_ = nullptr;
    other.memsize_ = 0;
    return *this;
}

d_dense_qp_ipm_ws* d_dense_qp_ipm_ws_wrapper::get() {
    if (memory_ == nullptr) {
        throw std::runtime_error(
            "[d_dense_qp_ipm_ws_wrapper] hpipm memory is not created. Call "
            "resize() first.");
    }
    return &dense_qp_ipm_ws_hpipm_;
}

const d_dense_qp_ipm_ws* d_dense_qp_ipm_ws_wrapper::get() const {
    if (memory_ == nullptr) {
        throw std::runtime_error(
            "[d_dense_qp_ipm_ws_wrapper] hpipm memory is not created. Call "
            "resize() first.");
    }
    return &dense_qp_ipm_ws_hpipm_;
}

void d_dense_qp_ipm_ws_wrapper::resize(
    const std::shared_ptr<d_dense_qp_dim_wrapper>& dim,
    const std::shared_ptr<d_dense_qp_ipm_arg_wrapper>& ipm_arg) {
    dim_ = dim;
    ipm_arg_ = ipm_arg;
    const hpipm_size_t new_memsize =
        d_dense_qp_ipm_ws_memsize(dim_->get(), ipm_arg_->get());
    if (memory_ != nullptr && new_memsize > memsize_) {
        free(memory_);
        memory_ = nullptr;
    }
    memsize_ = std::max(memsize_, new_memsize);
    if (memory_ == nullptr) {
        memory_ = malloc(memsize_);
    }
    d_dense_qp_ipm_ws_create(dim_->get(), ipm_arg_->get(),
                             &dense_qp_ipm_ws_hpipm_, memory_);
}

}  // namespace hpipm