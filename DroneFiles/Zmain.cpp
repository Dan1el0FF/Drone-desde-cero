#include "define.h"

const uint64_t Loop_1KHZ  = 1000;    // 1 ms
const uint64_t Loop_250HZ = 4000;    // 4 ms
const uint64_t Loop_50HZ  = 20000;   // 20 ms


extern "C" void app_main() {

    master_setup();

    // 🔥 Inicializar red privada del drone
    wifi_init_softap();
    start_webserver();

    uint64_t t1khz  = esp_timer_get_time();
    uint64_t t250hz = t1khz;
    uint64_t t50hz  = t1khz;

    while (true) {
        uint64_t now = esp_timer_get_time();
        
        //Loop 1 kHz
        if (now - t1khz >= Loop_1KHZ) {
            t1khz += Loop_1KHZ;
            read_gyro();
            EMA_Filter();
            PID_RATE();
        }

        //Loop 250 Hz
        if (now - t250hz >= Loop_250HZ) {
            t250hz += Loop_250HZ;
            read_acc();
            Kalman_roll_pitch();
            PID_ANGLE();
        }

        //Loop 50 Hz
        if (now - t50hz >= Loop_50HZ) {
            t50hz += Loop_50HZ;
            Network_IN();
            read_mag();
            Kalman_yaw();
        }

        taskYIELD();
    }
}
