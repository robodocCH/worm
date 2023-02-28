#ifndef ORG_FOURIERSIGNALSOURCE_HPP_
#define ORG_FOURIERSIGNALSOURCE_HPP_

#include <eeros/control/Blockio.hpp>
#include <cmath>
#include <eeros/core/System.hpp>

using namespace eeros::control;

/**
 * This block provides a signal source for a Fourier signal. Of course, you can 
 * also use the source only for generating a sine signal. The parameters are the 
 * basic frequency f and the coefficients a0, a1, a2, a3, etc.
 * 
 * out = a0 + a1*cos(omega*t) + a2*cos(2*omega*t) + a3*sin(3*omega*t) + a4*sin(4*omega*t);
 * 
 * @tparam omega: Fourier base frequency 
 * @tparam a0 .. aN: Fourier coefficients 
 * 
 * @since v0.0
 */

template <typename T = double>
class FourierSignalSource : public Blockio<0,1,T> 
{
 public:
  FourierSignalSource(double omega, double a0, double a1=0, double a2=0, double a3=0, double a4=0) {
    this->omega = omega;
    this->a0 = a0;
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
    this->a4 = a4;
    // this.n = 4
  }

  virtual void run() {
    timestamp_t time = eeros::System::getTimeNs();
    double t = time / 1000000000.0;
    double value = fourierAlgo(t);
    // std::lock_guard<std::mutex> lock(mtx);
    this->out.getSignal().setValue(value);
    this->out.getSignal().setTimestamp(time);
  }

  virtual void enable() {
    // enabled = true;
  }

  virtual void disable() {
    // enabled = false;
  }
  
  virtual void set(double transTime, double omega, double a0, double a1=0, double a2=0, double a3=0, double a4=0) {
   // 
 }
  
 private:
  double omega = 0;
  double a0 = 0;
  double a1 = 0;
  double a2 = 0;
  double a3 = 0;
  double a4 = 0;

  // int n = 0;

/*  double fourierAlgo() {
    Signal<T> sig = this->in.getSignal();
    double t = sig.getTimestamp() / 1000000000.0;
    double out = a0 + a1*cos(omega*t);
    if n >1 
      out = out + a2*cos(2*omega*t)
    else return out;
    if n >2 
      out = out + a3*cos(3*omega*t)
    else return out;
    if n >3 {
      out = out + a4*cos(4*omega*t)
      return out
    }
  }

  double fourierAlgo (double omega, double t, double a0, double a1, double a2) {
      double out = a0 + a1*cos(omega*t) + a2*cos(2*omega*t);
      return out;
  }

  double fourierAlgo (double omega, double t, double a0, double a1, double a2, double a3) {
      double out = a0 + a1*cos(omega*t) + a2*cos(2*omega*t) + a3*sin(3*omega*t);
      return out;
  }
  */

  double fourierAlgo (double t) {
      double out = a0 + a1*cos(omega*t) + a2*cos(2*omega*t) + a3*sin(3*omega*t) + a4*sin(4*omega*t);
      return out;
  }
};

#endif /* ORG_FOURIERSIGNALSOURCE_HPP_ */
