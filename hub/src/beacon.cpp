#include <beacon.hpp>

using namespace hal::literals;
using namespace std::chrono_literals;
using namespace sjsu::drivers;

namespace sjsu::hub {
Beacon::Beacon(hal::optional_ptr<hal::steady_clock> p_steady_clock,
               hal::optional_ptr<hal::output_pin> p_clock_pin,
               hal::optional_ptr<hal::output_pin> p_data_pin,
               std::size_t p_leds)
  : m_leds(p_leds)
{
  m_sk9822 = sk9822(p_clock_pin, p_data_pin, p_steady_clock);
  m_light_strip = drivers::light_strip<m_leds>;
}

Beacon::set_mode(hal::byte p_mode)
{
  rgb_brightness new_color;
  {
    case 0x0: {
      new_color.set(0xff, 0x00, 0x00, 0b11111);
      m_is_toggle = false;
      break;
    }
    case 0x1: {
      new_color.set(0x00, 0x00, 0xff, 0b11111);
      m_is_toggle = false;
      break;
    }
    case 0x2: {
      new_color.set(0x00, 0xff, 0x00, 0b11111);
      m_is_toggle = true;
      break;
    }
    default: {  // Shines static RED; change is requested
      new_color.set(0xff, 0x00, 0x00, 0b11111);
      m_is_toggle = false;
      break;
    }
  }

  for (auto i = m_light_strip.begin(); i != m_light_strip.end(); ++i) {
    *i = new_color;
  }
}

Beacon::run_led(u64 p_now)
{
  if (!m_is_toggle) {
    m_sk9822.update(m_light_strip);
    //   } else if (m_is_toggle && (p_now - prev_uptime) >=)
  }
}  // namespace sjsu::hub