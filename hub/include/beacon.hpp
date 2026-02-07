#pragma once
#include <iterator>
#include <libhal-util/steady_clock.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/pointers.hpp>
#include <libhal/steady_clock.hpp>
#include <libhal/units.hpp>
#include <sk9822.hpp>

using namespace hal::literals;
using namespace std::chrono_literals;
using namespace sjsu::drivers;

namespace sjsu::hub {

constexpr u64 BEACON_FREQ = 200;

class Beacon
{
public:
  /**
   * @brief Construct a new beacon object using the sk9822 driver.
   *
   * @param p_steady_clock
   * @param p_clock_pin
   * @param p_data_pin
   * @param p_leds refers to the number of LEDs on the light strip used inside
   * the beacon.
   */
  Beacon(hal::optional_ptr<hal::steady_clock> p_steady_clock,
         hal::optional_ptr<hal::output_pin> p_clock_pin,  // green wire
         hal::optional_ptr<hal::output_pin> p_data_pin,   // yellow wire
         std::size_t p_leds);

  /**
   * @brief Sets m_light_strip and m_is_toggle appropriately to reflect one of
   * the following modes: Autonomous (static red), Telooperation (static blue),
   * Successfully Reached Destination (flashing green).
   *
   * @param p_mode The following modes are associted with these byte inputs
   * only: 0x0 = "Autonomous" Mode, 0x1 = "Telooperation" Mode, and 0x2 =
   * "Successfuly Reached Destination" Mode.
   */
  void set_mode(hal::byte p_mode);

  /**
   * @brief Function to run inside a superloop to allow the Beacon to flash
   colors properly according to the desired setting.
   *
   * @param p_now Gets the current uptime to know when how much time has elasped
   to toggle the Beacon at whatever set frequency if in the "Successfully
   Reached Destination" Mode. In any other mode, it doesn't matter.
   */
  void run_led(u64 p_now);

private:
  drivers::sk9822 m_sk9822;
  drivers::light_strip_view m_light_strip;
  size_t m_leds;

  u64 prev_uptime;
  bool m_is_toggle;
};
}  // namespace sjsu::hub