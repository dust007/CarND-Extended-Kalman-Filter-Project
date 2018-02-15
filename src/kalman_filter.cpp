#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  MatrixXd Ft_;
  Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
  VectorXd y;
  y = z - H_ * x_;
  MatrixXd Ht;
  Ht = H_.transpose();
  MatrixXd S;
  S = H_ * P_ * Ht + R_;
  MatrixXd Si;
  Si = S.inverse();
  MatrixXd K;
  K = P_ * Ht * Si;

  x_ = x_ + K * y;
  int x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
  float rho = sqrt(x_(0) * x_(0) + x_(1) * x_(1));
  float phi = atan2(x_(1), x_(0));
  float rho_prime;
  if (fabs(rho) < 0.0001) {
    rho_prime = 0;
  } else {
    rho_prime = (x_(0) * x_(2) + x_(1) * x_(3)) / rho;
  }

  VectorXd h_x(3);
  h_x << rho, phi, rho_prime;
  VectorXd y;
  y = z - h_x;
  MatrixXd Ht;
  Ht = H_.transpose();
  MatrixXd S;
  S = H_ * P_ * Ht + R_;
  MatrixXd Si;
  Si = S.inverse();
  MatrixXd K;
  K = P_ * Ht * Si;

  x_ = x_ + K * y;
  int x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;

}
