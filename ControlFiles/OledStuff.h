/*#include "MultipleI2C.h"
#include "SSD1315.h"
#include "MiniGFX.h"
#include "Icons.h"
int battery = 100;
float voltage = 11.9;
float temperature = 23.5;
int meters = 2350;
char buffer1[32];
char buffer2[32];
char buffer3[32];
static MultipleI2C bus;
static SSD1315 oled(&bus);
MiniGFX gfx(&oled);
bool flag = true;
void oled_setup(){
    bus.setup(0x3C);
    oled.init();
}

void display(){   
        oled.clear();
        if(flag){
            for(int i = -40;i<80;i++){
                oled.clear();
                gfx.drawBitmap(i,i,sanvop_company,128,64,true);
                oled.update();
                vTaskDelay(30/portTICK_PERIOD_MS);
            }
            flag = false;
        }
        gfx.drawRect(0, 0, 128, 64, true);
        gfx.drawBitmap(2,2,wifi_16x16,15,15,true);
        gfx.drawBitmap(20,3,battery_3_19x10,19,10,true);
        snprintf(buffer1, sizeof(buffer1), "%d-%.1fV", battery, voltage);
        snprintf(buffer2, sizeof(buffer2), "%d", meters);
        snprintf(buffer3, sizeof(buffer3), "%.1fC", temperature);
        gfx.drawText(42,5,buffer1,true); 
        gfx.drawText(3,22,"Alt:",true);
        gfx.drawText(2,35,buffer2,true);
        gfx.drawText(3,45,"meters",true);
        gfx.drawText(88,22,"Temp:",true);
        gfx.drawText(88,35,buffer3,true);
        gfx.drawLine(0, 16, 127, 16, true); //linea superior
        gfx.drawLine(40, 16, 40, 64, true); //linea izquierda
        gfx.drawLine(86, 16, 86, 64, true); //linea derecha
        gfx.drawBitmap(47,24,sanvop_log_32x32,32,32,true);
        battery --;
        voltage = voltage - 0.1;
        meters --;
        oled.update();
}
*/
#include "MultipleI2C.h"
#include "SSD1315.h"
#include "MiniGFX.h"
#include "Icons.h"

int battery = 50;
float voltage = 11.9;
float temperature = 23.5;
int meters = 2350;

char buffer1[32];
char buffer2[32];
char buffer3[32];

static MultipleI2C bus;
static SSD1315 oled(&bus);
MiniGFX gfx(&oled);

bool flag = true;

/* ===== WiFi Blink (NO BLOQUEANTE) ===== */
bool wifiVisible = true;
uint32_t lastWifiToggle = 0;
const uint32_t WIFI_BLINK_INTERVAL = 1000; // ms

/* ===== Batería iconos ===== */
const uint8_t* batteryIcons[5] = {
    battery_0_19x10,
    battery_1_19x10,
    battery_2_19x10,
    battery_3_19x10,
    battery_4_19x10
};

void oled_setup(){
    bus.setup(0x3C);
    oled.init();
}

void display(){   
    oled.clear();

    /* ===== Animación inicial (solo una vez) ===== */
    if(flag){
        for(int i = -40; i < 90; i++){
            oled.clear();
            gfx.drawBitmap(0, i, sanvop_company, 128, 64, true);
            oled.update();
            vTaskDelay(30 / portTICK_PERIOD_MS);
        }
        flag = false;
    }

    /* ===== WiFi blink sin delay ===== */
    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
    if (now - lastWifiToggle >= WIFI_BLINK_INTERVAL) {
        wifiVisible = !wifiVisible;
        lastWifiToggle = now;
    }

    uint8_t batteryLevel;

    if (battery <= 0) {
        batteryLevel = 0;
    } else {
        batteryLevel = (battery - 1) / 25 + 1;
        if (batteryLevel > 4) batteryLevel = 4;
    }

    /* ===== UI ===== */
    gfx.drawRect(0, 0, 128, 64, true);

    if (wifiVisible) {
        gfx.drawBitmap(2, 2, wifi_16x16, 15, 15, true);
    }

    gfx.drawBitmap(
        20, 3,
        batteryIcons[batteryLevel],
        19, 10,
        true
    );
    snprintf(buffer1, sizeof(buffer1), "%d  %.1fV", valy, pitch);
    snprintf(buffer2, sizeof(buffer2), "%.1f", base_throttle);
    snprintf(buffer3, sizeof(buffer3), "%.1fC", roll);

    gfx.drawText(42, 5, buffer1, true); 
    gfx.drawText(3, 22, "Alt:", true);
    gfx.drawText(2, 35, buffer2, true);
    gfx.drawText(3, 45, "meters", true);
    gfx.drawText(88, 22, "Temp:", true);
    gfx.drawText(88, 35, buffer3, true);

    gfx.drawLine(0, 16, 127, 16, true); // linea superior
    gfx.drawLine(40, 16, 40, 64, true); // linea izquierda
    gfx.drawLine(86, 16, 86, 64, true); // linea derecha
    gfx.drawBitmap(47, 24, sanvop_log_32x32, 32, 32, true);
    oled.update();
}
