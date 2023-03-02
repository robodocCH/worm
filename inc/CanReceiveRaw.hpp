#ifndef ORG_EEROS_CONTROL_CANRECEIVERAW_
#define ORG_EEROS_CONTROL_CANRECEIVERAW_

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
 * This block serves to receive CAN messages from a MYACTUATOR RMD-X series drive.
 *
 * @tparam N - number of CAN nodes (2 - default)
 *
 * @since v3.2
 */
template < uint8_t N = 2 >
class CanReceiveRaw : public Blockio<0,1,Matrix<N,1,double>> {

 public:
  /**
   * Constructs a CAN receive block instance for a given set of nodes.
   * Sets the scale of position to 1.
   *
   * @param socket - socket of number of associated CAN bus
   * @param node - vector with node id's of all connected CAN nodes
   */
  CanReceiveRaw(int socket, std::initializer_list<uint8_t> node)
      : socket(socket), nodes(node), log(Logger::getLogger('Y')) {
    for (size_t i = 0; i < node.size(); i++) {
      scale[i] = 1;
    }
    log.info() << "CAN receive block constructed, " << node.size() << " nodes";
  }

  /**
   * Receives on the CAN bus. If enabled a CAN frame with data load of 8 bytes is received
   * from each CAN node.
   *
   * @see enable()
   * @see disable()
   */
  virtual void run() {
    Matrix<N,1,double> angle{0, 0};
    if (enabled) {
      for (std::size_t i = 0; i < nodes.size(); i++) {
        can_frame f;
        int err = read(socket, &f, sizeof(struct can_frame));
        canid_t node = f.can_id-0x240;
//        log.warn() << "node " << node;
        if (node > 0 && node < 3) {
          int16_t val = f.data[7] << 8 | f.data[6];
          angle[node-1] = val * scale[node-1];
        }
      }
      this->getOut().getSignal().setValue(angle);
      this->out.getSignal().setTimestamp(System::getTimeNs());
    }
  }

  /**
   * Enables the block.
   *
   * If enabled, run() will receive from the CAN bus.
   *
   * @see run()
   */
  virtual void enable() {
    enabled = true;
  }

  /**
   * Disables the block.
   *
   * If disabled, nothing will be received.
   *
   * @see run()
   */
  virtual void disable() {
    enabled = false;
  }

  /**
   * Sets the scaling for the position information.
   *
   * The drive gets its position information as a 2 bytes value.
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
  std::vector<uint8_t> nodes;
  Logger log;
};

}
}

#endif // ORG_EEROS_CONTROL_CANRECEIVERAW_


