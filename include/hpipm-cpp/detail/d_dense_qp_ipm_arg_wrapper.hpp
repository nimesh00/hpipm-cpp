#ifndef HPIPM_CPP_D_DENSE_QP_IPM_ARG_WRAPPER_HPP_
#define HPIPM_CPP_D_DENSE_QP_IPM_ARG_WRAPPER_HPP_

#include <vector>
#include <string>
#include <stdexcept>

#include "eigen3/Eigen/Core"

extern "C" {
#include "hpipm_d_dense_qp_ipm.h"
}

#include "hpipm-cpp/detail/d_dense_qp_dim_wrapper.hpp"


namespace hpipm {

///
/// @class d_dense_qp_ipm_arg_wrapper
/// @brief A wrapper of d_dense_qp_ipm_arg with a memory management.
///
class d_dense_qp_ipm_arg_wrapper {
public:
  ///
  /// @brief Default constructor. Allocates the hpipm resource.
  ///
  d_dense_qp_ipm_arg_wrapper();

  ///
  /// @brief Destructor.
  ///
  ~d_dense_qp_ipm_arg_wrapper();

  ///
  /// @brief Custom copy constructor.
  ///
  d_dense_qp_ipm_arg_wrapper(const d_dense_qp_ipm_arg_wrapper&);

  ///
  /// @brief Custom copy assign operator.
  ///
  d_dense_qp_ipm_arg_wrapper& operator=(const d_dense_qp_ipm_arg_wrapper&);

  ///
  /// @brief Custom move constructor.
  ///
  d_dense_qp_ipm_arg_wrapper(d_dense_qp_ipm_arg_wrapper&&) noexcept;

  ///
  /// @brief Custom move assign operator.
  ///
  d_dense_qp_ipm_arg_wrapper& operator=(d_dense_qp_ipm_arg_wrapper&&) noexcept;

  ///
  /// @brief Gets the pointer to the hpipm resource. 
  /// @return Pointer to the hpipm resource.
  ///
  d_dense_qp_ipm_arg* get();

  ///
  /// @brief Gets the const pointer to the hpipm instance.
  /// @return const pointer to the hpipm resource.
  ///
  const d_dense_qp_ipm_arg* get() const;

private:
  d_dense_qp_ipm_arg dense_qp_ipm_arg_hpipm_;
  void *memory_ = nullptr;
  hpipm_size_t memsize_ = 0;

  void resize();

  void copy(const d_dense_qp_ipm_arg_wrapper& other);
};

} // namespace hpipm

#endif // HPIPM_CPP_D_DENSE_QP_IPM_ARG_WRAPPER_HPP_