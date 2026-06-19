#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "driver/adc.h"
#include "esp_log.h"
#include <Freertos/FreeRTOS.h>
#include "cmath"
#include "ESPNow.h"
#include "nvs_flash.h"
#include <string.h>
#include <driver/gpio.h>
#include "stdio.h"
#include <esp_task_wdt.h>
#include "Joystick.h"

using namespace std;
array<uint8_t, 6> peer_mac = {0xd4, 0xe9, 0xf4, 0xf9, 0x63, 0xe4};
#define ADC_CHANNEL6 ADC1_CHANNEL_6   // GPIO34 JOYSTICK 1 X
#define ADC_CHANNEL7 ADC1_CHANNEL_7   // GPIO35 JOYSTICK 1 Y 
#define ADC_CHANNEL4 ADC1_CHANNEL_4   // GPIO32 JOYSTICK 2 X  
#define ADC_CHANNEL5 ADC1_CHANNEL_5   // GPIO33 JOYSTICK 2 Y
#define SW_BOTTON GPIO_NUM_33         // boton del JOYSTICK 1
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define alpha 0.25f //tasa de aceleracion
#define beta 0.05f //tasa de desaceleracion
#define alpha_sticks 0.3
static const char* TAG = "CONTROLLER";
ESPNow espnow;
Joystick left;
Joystick right;
float base_throttle = 0;
float roll = 0;
float pitch = 0;

int val,valx,valy;
static char messageT[64];
static char last_sent_messageT[64] = "";

void controll_setup(){
    nvs_flash_init();
    vector<array<uint8_t, 6>> peers = {peer_mac};
    espnow.setup(peers);
    left.setupVRX(adc1_channel_t(6),false,0);
    left.set_paramsX(1881,36);
    right.setupVRX(adc1_channel_t(4),false,0);
    right.setupVRY(adc1_channel_t(5),true,0); 
    right.set_paramsX(1814,84);
    right.set_paramsY(1821,88);
    esp_task_wdt_deinit();
}

void read_joystick1(){
    val = left.readVRX();
    if (val > base_throttle) base_throttle += alpha * (val - base_throttle);//aceleracion
    else base_throttle += beta * (val - base_throttle);//desaceleracion
    if (base_throttle > 100.0f) base_throttle = 100.0f; //limites
    if (base_throttle < 0.0f)   base_throttle = 0.0f; //limites
}

void read_joystick2(){
    valx = right.readVRX();
    valy = right.readVRY();
    float target_pitch = valy * 0.1f; 
    float target_roll = valx * 0.1f;
    pitch += alpha_sticks * (target_pitch - pitch);
    roll += alpha_sticks * (target_roll - roll);
    //printf("valx: %d  valy: %d\n",valx,valy);
}

void Network_OUT(){
    int len = snprintf(messageT, sizeof(messageT), "%f,%f,%f", base_throttle,roll,pitch);
        if (len > 0 && len < sizeof(messageT) && strcmp(messageT, last_sent_messageT) != 0) {
            espnow.send_data(messageT, peer_mac.data()); // Send the data
            ESP_LOGI(TAG,"Valor enviado: %s",messageT);
            strcpy(last_sent_messageT, messageT);
        }
}

void Network_IN(){
    
}

void System_Logs(){
}

#endif // __DEFINES_H__