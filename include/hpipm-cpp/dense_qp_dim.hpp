#ifndef HPIPM_CPP_DENSE_QP_DIM_HPP_
#define HPIPM_CPP_DENSE_QP_DIM_HPP_

#include "dense_qp.hpp"


namespace hpipm {

///
/// @class DenseQpDim
/// @brief Dimension of a Dense-QP problem.
///
struct DenseQpDim {
public:
  ///
  /// @brief Constructor from DenseQp data.
  /// @param[in] dense_qp Dense-QP data.
  ///
  DenseQpDim(const DenseQp& dense_qp);

  ///
  /// @brief Default constructor.
  ///
  DenseQpDim() = default;

  ///
  /// @brief Destructor.
  ///
  ~DenseQpDim() = default;

  ///
  /// @brief Default copy constructor.
  ///
  DenseQpDim(const DenseQpDim&) = default;

  ///
  /// @brief Default copy assign operator.
  ///
  DenseQpDim& operator=(const DenseQpDim&) = default;

  ///
  /// @brief Default move constructor.
  ///
  DenseQpDim(DenseQpDim&&) noexcept = default;

  ///
  /// @brief Default move assign operator.
  ///
  DenseQpDim& operator=(DenseQpDim&&) noexcept = default;

  ///
  /// @brief Number of variables.
  ///
  int nv = 0;

  ///
  /// @brief Number of equality constraints.
  ///
  int ne = 0;

  ///
  /// @brief Number of box constraints.
  ///
  int nb = 0;

  ///
  /// @brief Number of general inequality constraints.
  ///
  int ng = 0;

  ///
  /// @brief Number of softened box constraints.
  ///
  int nsb = 0;

  ///
  /// @brief Number of softened general constraints.
  ///
  int nsg = 0;

  ///
  /// @brief Number of slack variables. Equal to nsb + nsg.
  ///
  int ns = 0;

  ///
  /// @brief Resizes the dimension based on DenseQp data.
  /// @param[in] dense_qp Dense-QP data.
  ///
  void resize(const DenseQp& dense_qp);

  ///
  /// @brief Check the sizes of Dense-QP data. If something is wrong, throws an exception.
  /// @param[in] dense_qp Dense-QP data.
  ///
  void checkSize(const DenseQp& dense_qp) const;
};

} // namespace hpipm

#endif // HPIPM_CPP_DENSE_QP_DIM_HPP_