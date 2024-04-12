//
// Created by Riley Connors on 3/14/24.
//
#include "KalmanFilter.h"

KalmanFilter::KalmanFilter()
: H(5,7), P(7,7), Q(7, 7), R(5, 5), F(7,7) {
    double velVar = pow(.91, 2);
    double azVar = pow(1, 2);
    double elVar = pow(3, 2);
    m = static_cast<int>(this->H.rows());
    n = static_cast<int>(this->H.cols());

    this->I = Eigen::MatrixXd::Identity(n, n);
    this->F = Eigen::MatrixXd::Zero(n, n);
    this->H << 1, 0, 0, 0, 0, 0, 0,
               0, 1, 0, 0, 0, 0, 0,
               0, 0, 1, 0, 0, 0, 0,
               0, 0, 0, 1, 0, 0, 0,
               0, 0, 0, 0, 1, 0, 0;

    this->R << velVar, 0, 0, 0, 0,
         0, velVar, 0, 0, 0,
         0, 0, velVar, 0, 0,
         0, 0, 0, azVar, 0,
         0, 0, 0, 0, elVar;
}

void KalmanFilter::init(Eigen::VectorXd &x0, double qVal, int currentTrack) {
    this->initialized = true;
    this->currentTrack = currentTrack;
    this->x_hat = Eigen::VectorXd::Zero(n);
    x_hat = x0;
    y = Eigen::VectorXd::Zero(m);
    
    this->P << 1, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 1;

    this->Q << qVal, 0, 0, 0, 0, 0, 0,
        0, qVal, 0, 0, 0, 0, 0,
        0, 0, qVal, 0, 0, 0, 0,
        0, 0, 0, qVal, 0, 0, 0,
        0, 0, 0, 0, qVal, 0, 0,
        0, 0, 0, 0, 0, qVal, 0,
        0, 0, 0, 0, 0, 0, qVal;

    this->F << 1, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0,
        f4_vx(x_hat, dt), 0, f4_vz(x_hat, dt), 1, 0, 0, 0,
        f5_vx(x_hat, dt), f5_vy(x_hat, dt), f5_vz(x_hat, dt), 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 1;
}

void KalmanFilter::predict(double dt) {
    if (!initialized) {
        std::cerr << "KalmanFilter::predict() - Not initialized!" << std::endl;
        return;
    }

    this->dt = dt;

    this->F << 1, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0,
        f4_vx(x_hat, dt), 0, f4_vz(x_hat, dt), 1, 0, 0, 0,
        f5_vx(x_hat, dt), f5_vy(x_hat, dt), f5_vz(x_hat, dt), 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 1;

    x_hat = F * x_hat;
    P = F * P * F.transpose() + Q;
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












