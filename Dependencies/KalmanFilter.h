#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <iostream>
#include <string>
#include "Eigen/Dense"
#include <cmath>

class KalmanFilter {

private:

    // State transition matrix.
    Eigen::MatrixXd F;
    // State covariance matrix current and previous.
    Eigen::MatrixXd P;
    // Process noise covariance matrix.
    Eigen::MatrixXd Q;
    // Measurement matrix.
    Eigen::MatrixXd H;
    // Measurement covariance matrix.
    Eigen::MatrixXd R;
    //Kalman gain.
    Eigen::MatrixXd K;
    // Identity matrix.
    Eigen::MatrixXd I;

    //x_hat is the state vector. x_hat_new is the updated state vector. y is the error vector. v is the measurement noise
    Eigen::VectorXd x_hat, y, v;

    //x_hat = [vx, vy, vz, az, el, azdot, eldot]



    int m{}, n{};
    double t0{}, t{}, dt{};

    bool initialized = false;

    double f4_vx(Eigen::VectorXd &x, double &t);
    double f4_vz(Eigen::VectorXd &x, double &t);
    double f5_vx(Eigen::VectorXd &x, double &t);
    double f5_vy(Eigen::VectorXd &x, double &t);
    double f5_vz(Eigen::VectorXd &x, double &t);

public:
    KalmanFilter();
    KalmanFilter(Eigen::MatrixXd &P, Eigen::MatrixXd &Q, Eigen::MatrixXd &H, Eigen::MatrixXd &R, double &dt);
    void init(Eigen::VectorXd& x0, double& t0);
    void predict();
    void update(Eigen::VectorXd& z);

    Eigen::VectorXd get_x_hat() { return x_hat; }
};


#endif //KALMANFILTER_H

