#ifndef ORG_EEROS_CONTROL_CANSENDRAW_
#define ORG_EEROS_CONTROL_CANSENDRAW_

#include <eeros/logger/Logger.hpp>
#include <eeros/control/Blockio.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/math/Matrix.hpp>
#include <eeros/core/Fault.hpp>
#include <linux/can.h>

using namespace eeros::control;
using namespace eeros::logger;
using namespace eeros::math;

namespace eeros {
namespace control {

/**
 * This block serves to send CAN messages to a MYACTUATOR RMD-X series drive.
 *
 * @tparam N - number of CAN nodes (2 - default)
 *
 * @since v3.2
 */
template < uint8_t N = 2 >
class CanSendRaw : public Blockio<1,0,Matrix<N,1,double>> {

 public:
  /**
   * Constructs a CAN send block instance for a given set of nodes.
   * Sets the scale of position to 1.
   *
   * @param socket - socket of number of associated CAN bus
   * @param node - vector with node id's of all connected CAN nodes
   */
  CanSendRaw(int socket, std::initializer_list<uint8_t> node)
      : socket(socket), nodes(node), log(Logger::getLogger('Y')) {
    for (size_t i = 0; i < node.size(); i++) {
      scale[i] = 1;
    }
    log.info() << "CAN send block constructed, " << node.size() << " nodes";
  }

  /**
   * Transmits on the CAN bus. If enabled a CAN frame with data load of 8 bytes is sent
   * to each CAN node.
   *
   * @see enable()
   * @see disable()
   */
  virtual void run() {
    if (enabled) {
  //       for (std::size_t i = 0; i < nodes.size(); i++) {
      for (std::size_t i = 0; i < 2; i++) {
        uint32_t pos = this->getIn().getSignal().getValue()[i] * scale(i);
        uint16_t sp = speed[i];
        can_frame f;
        f.can_id = 0x140 + nodes[i];
        f.can_dlc = 8;
        f.data[0] = 0xa5;
        f.data[2] = (uint8_t)sp;
        f.data[3] = (uint8_t)(sp >> 8);
        f.data[4] = (uint8_t)pos;
        f.data[5] = (uint8_t)(pos >> 8);
        f.data[6] = (uint8_t)(pos >> 16);
        f.data[7] = (uint8_t)(pos >> 24);
        int err = write(socket, &f, sizeof(struct can_frame));
        if (err < 0) throw eeros::Fault("sending motion setpoint over CAN failed");
      }
    }
  }

  /**
   * Motor shutdown
   *
   */
  virtual void motorShutdown() {
  //       for (std::size_t i = 0; i < nodes.size(); i++) {
    for (std::size_t i = 0; i < 2; i++) {
      can_frame f;
      f.can_id = 0x140 + nodes[i];
      f.can_dlc = 8;
      f.data[0] = 0x80;
      f.data[2] = 0;
      f.data[3] = 0;
      f.data[4] = 0;
      f.data[5] = 0;
      f.data[6] = 0;
      f.data[7] = 0;
      int err = write(socket, &f, sizeof(struct can_frame));
      if (err < 0) throw eeros::Fault("sending motor shutdown command over CAN failed");
    }
  }

  /**
   * Enables the block.
   *
   * If enabled, run() will send onto the CAN bus.
   *
   * @see run()
   */
  virtual void enable() {
    enabled = true;
  }

  /**
   * Disables the block.
   *
   * If disabled, nothing will be sent.
   *
   * @see run()
   */
  virtual void disable() {
    enabled = false;
  }

  /**
   * Sets the maximum speed.
   *
   * The speed is a 2 bytes value.
   *
   * @param scale The scaling factor for the position for all drives
   */
  virtual void setSpeed(Matrix<N,1,double>& speed) {
    this->speed = speed;
  }

  /**
   * Sets the scaling for the position information.
   *
   * The drive needs its position information as a 4 bytes value.
   * The scaling allows to transform this value into meaningful
   * position information in rad/s or m/s.
   *
   * @param scale The scaling factor for the position for all drives
   */
  virtual void setScale(Matrix<N,1,double>& scale) {
    this->scale = scale;
  }

 private:
  int socket;
  bool enabled = false;
  Matrix<N,1,double> scale;
  Matrix<N,1,double> speed;
  std::vector<uint8_t> nodes;
  Logger log;
};

}
}

#endif // ORG_EEROS_CONTROL_CANSENDRAW_

