//
// Created by Riley Connors on 3/14/24.
//
#include "KalmanFilter.h"

KalmanFilter::KalmanFilter() = default;

KalmanFilter::KalmanFilter(Eigen::MatrixXd &P, Eigen::MatrixXd &Q, Eigen::MatrixXd &H, Eigen::MatrixXd &R, double &dt)
: P(P), Q(Q), H(H), R(R), K(7, 7), F(7,7), dt(dt) {
    m = static_cast<int>(this->H.rows());
    n = static_cast<int>(this->H.cols());
    x_hat = Eigen::VectorXd::Zero(n);
    y = Eigen::VectorXd::Zero(m);
    v = Eigen::VectorXd::Zero(m);
    this->I = Eigen::MatrixXd::Identity(n, n);
    this->F = Eigen::MatrixXd::Zero(n, n);
}

void KalmanFilter::init(Eigen::VectorXd &x0, double &t0) {
    this-> initialized = true;
    this->x_hat = x0;
    this->t0 = t0;
    t = t0;
    F << 1, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0, 0,
         f4_vx(x_hat, dt), 0, f4_vz(x_hat, dt), 1, 0, 0, 0,
         f5_vx(x_hat, dt), f5_vy(x_hat, dt), f5_vz(x_hat, dt), 0, 1, 0, 0,
         0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 1;
}

void KalmanFilter::predict() {
    if (!initialized) {
        std::cerr << "KalmanFilter - Not initialized!" << std::endl;
        return;
    }
    x_hat = F * x_hat;
    P = F * P * F.transpose() + Q;

    t += dt;
}

void KalmanFilter::update(Eigen::VectorXd &z) {
    if (!initialized ) {
        std::cerr << "KalmanFilter::update() - Not initialized!" << std::endl;
        return;
    }
    y = z - (H * x_hat);
    K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
    x_hat = x_hat + K * y;
    P = (I - K * H) * P;
}

double KalmanFilter::f4_vx(Eigen::VectorXd &x, double &t) {
    return (t * x(2)) / (pow(x(0), 2) + pow(x(2), 2));
}

double KalmanFilter::f4_vz(Eigen::VectorXd &x, double &t) {
    return (-t * x(0)) / (pow(x(0), 2) + pow(x(2), 2));
}

double KalmanFilter::f5_vx(Eigen::VectorXd &x, double &t) {
    return (-t * x(1) * x(3)) / ((pow(x(0), 2) + pow(x(1),2) + pow(x(2), 2)) * sqrt(pow(x(0), 2) + pow(x(2), 2)));
}

double KalmanFilter::f5_vy(Eigen::VectorXd &x, double &t) {
    return (t * sqrt(pow(x(1), 2) + pow(x(2), 2))) / (pow(x(0), 2) + pow(x(1),2) + pow(x(2), 2));
}

double KalmanFilter::f5_vz(Eigen::VectorXd &x, double &t) {
    return (-t * x(1) * x(2)) / ((pow(x(0), 2) + pow(x(1),2) + pow(x(2), 2)) * sqrt(pow(x(0), 2) + pow(x(2), 2)));
}












