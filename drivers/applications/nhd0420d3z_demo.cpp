#include <array>
#include <cstdio>
#include <libhal-util/i2c.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/pointers.hpp>
#include <libhal/timeout.hpp>
#include <libhal/units.hpp>
// #include <nhd0420d3z.hpp>
#include <resource_list.hpp>
#include <string_view>

using namespace std::chrono_literals;

namespace sjsu::drivers {

void application()
{
  auto led = resources::status_led();
  auto clock = resources::clock();

  // auto console = resources::console();

  auto i2c = resources::i2c();
  // auto display = nhd0420d3z(*i2c);


  hal::delay(*clock, 1ms);
  std::array<hal::byte, 1> data_in = { };
  std::span<hal::byte> data_out { 0xFE, 0x41 };

  while (true) {
    try {
      i2c->transaction(0x28, data_out, data_in);
    } catch (hal::no_such_device err) {
      hal::delay(*clock, 10ms);
    }
  }
  bool state = false;
  while (true) {
    led->level(state);
    state = !state;
    hal::delay(*clock, 250ms);
  }

  /*
  hal::delay(*clock, 1ms);
  data_in[0] = 0x41;
  i2c->transaction(0x28, data_out, data_in);
  */

  /*
  display.clear_screen();
  display.set_cursor_position(0, 0);

  display.write_char('a');
  hal::delay(*clock, 100ms);
  display.write_char('7');
  hal::delay(*clock, 100ms);
  display.write_char('b');
  hal::delay(*clock, 100ms);
  display.write_char('8');
  hal::delay(*clock, 100ms);
  display.write_char('c');
  hal::delay(*clock, 100ms);
  */
}
}  // namespace sjsu::drivers
