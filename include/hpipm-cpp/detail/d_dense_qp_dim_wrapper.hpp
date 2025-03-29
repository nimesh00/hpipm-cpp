#ifndef HPIPM_CPP_D_DENSE_QP_DIM_WRAPPER_HPP_
#define HPIPM_CPP_D_DENSE_QP_DIM_WRAPPER_HPP_

extern "C" {
#include "hpipm_d_dense_qp_dim.h"
}


namespace hpipm {

///
/// @class d_dense_qp_dim_wrapper
/// @brief A wrapper of d_dense_qp_dim with a memory management.
///
class d_dense_qp_dim_wrapper {
public:
  ///
  /// @brief Constructor. Allocates the hpipm resource.
  ///
  d_dense_qp_dim_wrapper();

  ///
  /// @brief Destructor.
  ///
  ~d_dense_qp_dim_wrapper();

  ///
  /// @brief Custom copy constructor.
  ///
  d_dense_qp_dim_wrapper(const d_dense_qp_dim_wrapper&);

  ///
  /// @brief Custom copy assign operator.
  ///
  d_dense_qp_dim_wrapper& operator=(const d_dense_qp_dim_wrapper&);

  ///
  /// @brief Custom move constructor.
  ///
  d_dense_qp_dim_wrapper(d_dense_qp_dim_wrapper&&) noexcept;

  ///
  /// @brief Custom move assign operator.
  ///
  d_dense_qp_dim_wrapper& operator=(d_dense_qp_dim_wrapper&&) noexcept;

  ///
  /// @brief Gets the pointer to the hpipm resource. Throw an exception if the 
  /// memory for the instance is not allocated.
  /// @return Pointer to the hpipm resource.
  ///
  d_dense_qp_dim* get();

  ///
  /// @brief Gets the const pointer to the hpipm instance.
  /// @return const pointer to the hpipm resource.
  ///
  const d_dense_qp_dim* get() const;

private:
  d_dense_qp_dim dense_qp_dim_hpipm_;
  void *memory_ = nullptr;
  hpipm_size_t memsize_ = 0;

  void copy(const d_dense_qp_dim_wrapper& other);
};

} // namespace hpipm

#endif // HPIPM_CPP_D_DENSE_QP_DIM_WRAPPER_HPP_