#include "hpipm-cpp/dense_qp_dim.hpp"

#include <stdexcept>
#include <string>


namespace hpipm {

DenseQpDim::DenseQpDim(const DenseQp& dense_qp) {
  resize(dense_qp);
}


void DenseQpDim::resize(const DenseQp& dense_qp) {
  nv = dense_qp.g.size();
  ne = dense_qp.b.size();
  nb = dense_qp.idxb.size();
  ng = dense_qp.lg.size();
  nsb = 0; // Initialize to zero - will be set based on slack variables
  nsg = 0; // Initialize to zero - will be set based on slack variables
  ns = dense_qp.idxs.size();
  
  // In HPIPM, slack variables can be associated with either box constraints (nsb)
  // or general constraints (nsg). For simplicity, we currently assign all to nsg.
  // This could be refined based on the indices in idxs.
  nsg = ns;
  
  checkSize(dense_qp);
}


void DenseQpDim::checkSize(const DenseQp& dense_qp) const {
  // Check cost function dimensions
  if (dense_qp.H.rows() != nv || dense_qp.H.cols() != nv) {
    throw std::runtime_error("dense_qp.H must be of size " + std::to_string(nv) + "x" + std::to_string(nv));
  }
  if (dense_qp.g.size() != nv) {
    throw std::runtime_error("dense_qp.g must be of size " + std::to_string(nv));
  }

  // Check equality constraints dimensions
  if (dense_qp.A.rows() != ne || (ne > 0 && dense_qp.A.cols() != nv)) {
    throw std::runtime_error("dense_qp.A must be of size " + std::to_string(ne) + "x" + std::to_string(nv));
  }
  if (dense_qp.b.size() != ne) {
    throw std::runtime_error("dense_qp.b must be of size " + std::to_string(ne));
  }

  // Check box constraints dimensions
  if (dense_qp.idxb.size() != nb) {
    throw std::runtime_error("dense_qp.idxb must be of size " + std::to_string(nb));
  }
  if (dense_qp.lb.size() != nb) {
    throw std::runtime_error("dense_qp.lb must be of size " + std::to_string(nb));
  }
  if (dense_qp.ub.size() != nb) {
    throw std::runtime_error("dense_qp.ub must be of size " + std::to_string(nb));
  }
  if (dense_qp.lb_mask.size() != 0 && dense_qp.lb_mask.size() != nb) {
    throw std::runtime_error("dense_qp.lb_mask must be of size 0 or " + std::to_string(nb));
  }
  if (dense_qp.ub_mask.size() != 0 && dense_qp.ub_mask.size() != nb) {
    throw std::runtime_error("dense_qp.ub_mask must be of size 0 or " + std::to_string(nb));
  }

  // Check inequality constraints dimensions
  if (dense_qp.C.rows() != ng || (ng > 0 && dense_qp.C.cols() != nv)) {
    throw std::runtime_error("dense_qp.C must be of size " + std::to_string(ng) + "x" + std::to_string(nv));
  }
  if (dense_qp.lg.size() != ng) {
    throw std::runtime_error("dense_qp.lg must be of size " + std::to_string(ng));
  }
  if (dense_qp.ug.size() != ng) {
    throw std::runtime_error("dense_qp.ug must be of size " + std::to_string(ng));
  }
  if (dense_qp.lg_mask.size() != 0 && dense_qp.lg_mask.size() != ng) {
    throw std::runtime_error("dense_qp.lg_mask must be of size 0 or " + std::to_string(ng));
  }
  if (dense_qp.ug_mask.size() != 0 && dense_qp.ug_mask.size() != ng) {
    throw std::runtime_error("dense_qp.ug_mask must be of size 0 or " + std::to_string(ng));
  }

  // Check soft constraints dimensions
  if (dense_qp.Zl.rows() != ns || dense_qp.Zl.cols() != ns) {
    throw std::runtime_error("dense_qp.Zl must be of size " + std::to_string(ns) + "x" + std::to_string(ns));
  }
  if (dense_qp.Zu.rows() != ns || dense_qp.Zu.cols() != ns) {
    throw std::runtime_error("dense_qp.Zu must be of size " + std::to_string(ns) + "x" + std::to_string(ns));
  }
  if (dense_qp.zl.size() != ns) {
    throw std::runtime_error("dense_qp.zl must be of size " + std::to_string(ns));
  }
  if (dense_qp.zu.size() != ns) {
    throw std::runtime_error("dense_qp.zu must be of size " + std::to_string(ns));
  }
  if (dense_qp.idxs.size() != ns) {
    throw std::runtime_error("dense_qp.idxs must be of size " + std::to_string(ns));
  }
  if (dense_qp.ls.size() != ns) {
    throw std::runtime_error("dense_qp.ls must be of size " + std::to_string(ns));
  }
  if (dense_qp.us.size() != ns) {
    throw std::runtime_error("dense_qp.us must be of size " + std::to_string(ns));
  }
}

} // namespace hpipm