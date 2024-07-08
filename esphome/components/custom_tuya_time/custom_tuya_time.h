#include "esphome.h"
#include "esphome/components/tuya/tuya.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace tuya {

class CustomTuyaTime : public Component, public Tuya {
 public:
  void setup() override {
    Tuya::setup();
  }

  void loop() override {
    Tuya::loop();
  }

  void on_datapoint(uint8_t datapoint_id, uint32_t value) override {
    if (datapoint_id == 0x1C) {  // LOCAL_TIME_QUERY
      auto time = App.get_time("sntp_time")->now();
      this->set_local_time_(time);
    } else {
      Tuya::on_datapoint(datapoint_id, value);
    }
  }

 protected:
  void set_local_time_(const time::ESPTime &time) {
    uint8_t data[8];
    data[0] = time.year - 2000;
    data[1] = time.month;
    data[2] = time.day_of_month;
    data[3] = time.hour;
    data[4] = time.minute;
    data[5] = time.second;
    data[6] = 0;  // Weekday is not used
    data[7] = 0;  // Reserved
    this->send_command_(0x1C, data, sizeof(data));
  }
};

}  // namespace tuya
}  // namespace esphome
