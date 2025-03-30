#include <iostream>

#include "Eigen/Core"
#include "hpipm-cpp/hpipm-cpp.hpp"

int main() {
    // Size of the problem
    const int nv = 4;  // number of variables
    const int ne = 1;  // number of equality constraints
    const int nb = 4;  // number of box constraints
    const int ng = 2;  // number of general constraints

    // Problem data (portfolio optimization example)
    // Objective: min 0.5 * v^T * H * v + g^T * v
    // Where H represents risk (covariance matrix) and g represents negative
    // expected returns

    // Risk matrix (positive definite)
    Eigen::MatrixXd H(nv, nv);
    H << 0.04, 0.01, 0.02, 0.01, 0.01, 0.09, 0.02, 0.03, 0.02, 0.02, 0.16, 0.02,
        0.01, 0.03, 0.02, 0.25;

    // Negative expected returns (we minimize, so negate the returns)
    Eigen::VectorXd g(nv);
    g << -0.12, -0.10, -0.15, -0.08;

    // Equality constraint: A * v = b (sum of allocations = 1)
    Eigen::MatrixXd A(ne, nv);
    A << 1.0, 1.0, 1.0, 1.0;  // sum of allocations

    Eigen::VectorXd b(ne);
    b << 1.0;  // total allocation = 1 (100%)

    // Box constraints: lb <= v <= ub (bounds on each asset allocation)
    std::vector<int> idxb{0, 1, 2, 3};  // indices of box-constrained variables

    Eigen::VectorXd lb(nb);
    lb << 0.0, 0.0, 0.0, 0.0;  // non-negative allocations

    Eigen::VectorXd ub(nb);
    ub << 0.5, 0.5, 0.5, 0.5;  // max 50% in any single asset

    // General constraints: lg <= C * v <= ug
    Eigen::MatrixXd C(ng, nv);
    C << 1.0, 0.0, 1.0, 0.0,  // sum of assets 0 and 2
        0.0, 1.0, 0.0, 1.0;   // sum of assets 1 and 3

    Eigen::VectorXd lg(ng);
    lg << 0.3, 0.3;  // min 30% in each group

    Eigen::VectorXd ug(ng);
    ug << 0.7, 0.7;  // max 70% in each group

    // Set up the DenseQp struct
    hpipm::DenseQp qp;
    qp.H = H;
    qp.g = g;
    qp.A = A;
    qp.b = b;
    qp.idxb = idxb;
    qp.lb = lb;
    qp.ub = ub;
    qp.C = C;
    qp.lg = lg;
    qp.ug = ug;

    // Setup solver settings
    hpipm::DenseQpIpmSolverSettings solver_settings;
    solver_settings.mode = hpipm::HpipmMode::Balance;
    solver_settings.iter_max = 30;
    solver_settings.alpha_min = 1e-8;
    solver_settings.mu0 = 1e2;
    solver_settings.tol_stat = 1e-4;
    solver_settings.tol_eq = 1e-4;
    solver_settings.tol_ineq = 1e-4;
    solver_settings.tol_comp = 1e-4;
    solver_settings.reg_prim = 1e-12;
    solver_settings.reg_dual = 1e-12;
    solver_settings.warm_start = 0;
    solver_settings.pred_corr = 1;
    solver_settings.compute_obj = 1;
    solver_settings.split_step = 1;

    // Create solution object
    hpipm::DenseQpSolution solution;

    // Create and initialize solver
    hpipm::DenseQpIpmSolver solver(qp, solver_settings);

    // Solve the QP
    const auto res = solver.solve(qp, solution);
    std::cout << "QP result: " << res << std::endl;

    // Print solution
    std::cout << "Dense QP solution:" << std::endl;
    std::cout << "Optimal allocation: " << solution.v.transpose() << std::endl;
    std::cout << "Objective value: " << solution.obj_value << std::endl;

    // Print Lagrange multipliers
    std::cout << "Dual solution (Lagrange multipliers):" << std::endl;
    std::cout << "Equality constraints (pi): " << solution.pi.transpose()
              << std::endl;
    std::cout << "Lower box constraints: " << solution.lam_lb.transpose()
              << std::endl;
    std::cout << "Upper box constraints: " << solution.lam_ub.transpose()
              << std::endl;
    std::cout << "Lower general constraints: " << solution.lam_lg.transpose()
              << std::endl;
    std::cout << "Upper general constraints: " << solution.lam_ug.transpose()
              << std::endl;

    // Print solver statistics
    const auto& stat = solver.getSolverStatistics();
    std::cout << stat << std::endl;

    return 0;
}