#include <iostream>
#include <limits>

#include "Eigen/Core"
#include "hpipm-cpp/hpipm-cpp.hpp"

void runQpTest(const std::string& test_name, const Eigen::VectorXd& lb_mask,
               const Eigen::VectorXd& ub_mask, const Eigen::VectorXd& lg_mask,
               const Eigen::VectorXd& ug_mask,
               bool use_infinite_bounds = false) {
    // Size of the problem
    const int nv = 4;  // number of variables
    const int ne = 1;  // number of equality constraints
    const int nb = 4;  // number of box constraints
    const int ng = 2;  // number of general constraints

    // Risk matrix (positive definite)
    Eigen::MatrixXd H(nv, nv);
    H << 0.04, 0.01, 0.02, 0.01, 0.01, 0.09, 0.02, 0.03, 0.02, 0.02, 0.16, 0.02,
        0.01, 0.03, 0.02, 0.25;

    // Negative expected returns (we minimize, so negate the returns)
    Eigen::VectorXd g(nv);
    g << -0.12, -0.10, -0.15, -0.08;

    // Equality constraint: A * v = b (sum of allocations = 1)
    Eigen::MatrixXd A(ne, nv);
    A << 1.0, 1.0, 1.0, 1.0;
    Eigen::VectorXd b(ne);
    b << 1.0;

    // Box constraints: lb <= v <= ub
    std::vector<int> idxb{0, 1, 2, 3};
    Eigen::VectorXd lb(nb);
    Eigen::VectorXd ub(nb);
    if (use_infinite_bounds) {
        lb << 0.0, -std::numeric_limits<double>::infinity(), 0.0,
            -std::numeric_limits<double>::infinity();
        ub << 0.5, std::numeric_limits<double>::infinity(), 0.5,
            std::numeric_limits<double>::infinity();
    } else {
        lb << 0.0, 0.0, 0.0, 0.0;
        ub << 0.5, 0.5, 0.5, 0.5;
    }

    // General constraints: lg <= C * v <= ug
    Eigen::MatrixXd C(ng, nv);
    C << 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0;
    Eigen::VectorXd lg(ng);
    Eigen::VectorXd ug(ng);
    if (use_infinite_bounds) {
        lg << 0.3, -std::numeric_limits<double>::infinity();
        ug << 0.7, std::numeric_limits<double>::infinity();
    } else {
        lg << 0.3, 0.3;
        ug << 0.7, 0.7;
    }

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

    // Assign masks
    qp.lb_mask = lb_mask;
    qp.ub_mask = ub_mask;
    qp.lg_mask = lg_mask;
    qp.ug_mask = ug_mask;

    // Solver settings
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

    // Solve the QP
    hpipm::DenseQpSolution solution;
    hpipm::DenseQpIpmSolver solver(qp, solver_settings);
    const auto res = solver.solve(qp, solution);

    // Print results
    std::cout << "\n=== " << test_name << " ===\n";
    std::cout << "QP result: " << res << "\n";
    if (res == hpipm::HpipmStatus::Success) {
        std::cout << "Optimal allocation: " << solution.v.transpose() << "\n";
        std::cout << "Objective value: " << solution.obj_value << "\n";
    } else {
        std::cout << "Solver failed\n";
    }
}

int main() {
    const int nb = 4;  // number of box constraints
    const int ng = 2;  // number of general constraints

    // Test Case 1: All constraints active (original problem)
    Eigen::VectorXd lb_mask_all(nb);
    lb_mask_all.setOnes();
    Eigen::VectorXd ub_mask_all(nb);
    ub_mask_all.setOnes();
    Eigen::VectorXd lg_mask_all(ng);
    lg_mask_all.setOnes();
    Eigen::VectorXd ug_mask_all(ng);
    ug_mask_all.setOnes();
    runQpTest("All Constraints Active", lb_mask_all, ub_mask_all, lg_mask_all,
              ug_mask_all, false);

    // Test Case 2: No box upper bounds
    Eigen::VectorXd ub_mask_none(nb);
    ub_mask_none.setZero();
    runQpTest("No Upper Box Constraints", lb_mask_all, ub_mask_none,
              lg_mask_all, ug_mask_all, true);

    // Test Case 3: No general upper bounds
    Eigen::VectorXd ug_mask_none(ng);
    ug_mask_none.setZero();
    runQpTest("No Upper General Constraints", lb_mask_all, ub_mask_all,
              lg_mask_all, ug_mask_none, true);

    // Test Case 4: No box lower bounds
    Eigen::VectorXd lb_mask_none(nb);
    lb_mask_none.setZero();
    runQpTest("No Lower Box Constraints", lb_mask_none, ub_mask_all,
              lg_mask_all, ug_mask_all, true);

    // Test Case 5: No general lower bounds
    Eigen::VectorXd lg_mask_none(ng);
    lg_mask_none.setZero();
    runQpTest("No Lower General Constraints", lb_mask_all, ub_mask_all,
              lg_mask_none, ug_mask_all, true);

    // Test Case 6: Mixed constraints (some active, some inactive)
    Eigen::VectorXd lb_mask_mixed(nb);
    lb_mask_mixed << 1.0, 0.0, 1.0,
        0.0;  // Active for v0, v2; inactive for v1, v3
    Eigen::VectorXd ub_mask_mixed(nb);
    ub_mask_mixed << 0.0, 1.0, 0.0,
        1.0;  // Inactive for v0, v2; active for v1, v3
    Eigen::VectorXd lg_mask_mixed(ng);
    lg_mask_mixed << 1.0, 0.0;  // Active for first group, inactive for second
    Eigen::VectorXd ug_mask_mixed(ng);
    ug_mask_mixed << 0.0, 1.0;  // Inactive for first group, active for second
    runQpTest("Mixed Constraints", lb_mask_mixed, ub_mask_mixed, lg_mask_mixed,
              ug_mask_mixed, true);

    return 0;
}