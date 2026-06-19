//CONTROL 13-Jan-2026
#include "defines.h"
#include "OledStuff.h"
extern "C" void app_main(void){
    controll_setup();
    oled_setup();
    while (true) {
        display();
        read_joystick1();
        read_joystick2();
        System_Logs();
        Network_OUT();
        Network_IN();
        vTaskDelay(50/portTICK_PERIOD_MS);
        }   
    }

