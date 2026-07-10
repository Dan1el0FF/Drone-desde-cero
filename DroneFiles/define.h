#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "HMC5883L.h"
#include "KalmanFilter.h"
#include "BMI160.h"
#include <esp_timer.h>
#include <esp_task_wdt.h>
#include "ESCDriver.h"
#include "PID.h"
#include "ESPNow.h"
#include "esp_log.h"
#include <array>
#include <vector>
#include <math.h>



// ================== CONSTANTES ==================
#define Q_angle_roll 0.015
#define Q_angle_pitch 0.015
#define Q_angle_yaw 0.07
#define Q_bias 0.003
#define R_measure_1 0.0042
#define R_measure_2 0.0035
#define R_measure_3 0.0245
#define alpha 0.38

#define channel_0 0
#define channel_1 1
#define channel_2 2
#define channel_3 3

#define MUL_pin GPIO_NUM_26
#define MUR_pin GPIO_NUM_19
#define MDL_pin GPIO_NUM_25
#define MDR_pin GPIO_NUM_18

#define dt_1Khz 0.001
#define dt_250hz 0.004
#define dt_50hz 0.020

#define FAILSAFE_TIMEOUT_US 300000ULL       // 300 ms en microsegundos
// ================== OBJETOS GLOBALES ==================
extern ESPNow espnow;

extern MultipleI2C bus_bmi;
extern MultipleI2C bus_hmc;
extern BMI160 bmi;
extern HMC5883L hmc;

extern KalmanFilter kalman_pitch;
extern KalmanFilter kalman_roll;
extern KalmanFilter kalman_yaw_Wtilt;

extern ESCDriver motorUL;
extern ESCDriver motorUR;
extern ESCDriver motorDL;
extern ESCDriver motorDR;

extern PID roll_pid;
extern PID pitch_pid;
extern PID yaw_pid;

extern PID roll_rate_pid;
extern PID pitch_rate_pid;
extern PID yaw_rate_pid;

// ================== VARIABLES ==================
extern std::array<uint8_t, 6> peer_mac;

extern float hmc_data[5], B_xyz[3], Rot_xy[2], Acc[3], gyro[3], gyro_filtered[3];
extern float yaw, Tyaw, pitch, roll;

extern float pitch_target, roll_target, yaw_target;
extern float roll_rate_output, pitch_rate_output;

extern float pitch_target_rate, roll_target_rate, yaw_target_rate;

extern float roll_gains[3];
extern float pitch_gains[3];
extern float yaw_gains[3];

extern float roll_rate_gains[3];
extern float pitch_rate_gains[3];
extern float yaw_rate_gains[3];

extern float base_thrust;

extern uint64_t last_packet_time;          // marca de tiempo del último paquete válido (us)

void check_failsafe();
float maxmin(float value);
void master_setup();
void read_gyro();
void read_acc();
void read_mag();
void Kalman_roll_pitch();
void Kalman_yaw();
void EMA_Filter();
void Network_IN();
void PID_ANGLE();
void PID_RATE();
void wifi_init_softap();
void start_webserver();
#endif // __DEFINE_H__
