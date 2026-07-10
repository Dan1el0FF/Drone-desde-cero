#include "define.h"
using namespace std;
// ================= OBJETOS =================
ESPNow espnow;

MultipleI2C bus_bmi;
MultipleI2C bus_hmc;
BMI160 bmi(&bus_bmi);
HMC5883L hmc(&bus_hmc);

KalmanFilter kalman_pitch;
KalmanFilter kalman_roll;
KalmanFilter kalman_yaw_Wtilt;

ESCDriver motorUL;
ESCDriver motorUR;
ESCDriver motorDL;
ESCDriver motorDR;

PID roll_pid;
PID pitch_pid;
PID yaw_pid;

PID roll_rate_pid;
PID pitch_rate_pid;
PID yaw_rate_pid;

// ================= VARIABLES =================
array<uint8_t, 6> peer_mac = {0xf0, 0x24, 0xf9, 0xf7, 0x62, 0xfc};

float hmc_data[5], B_xyz[3], Rot_xy[2], Acc[3], gyro[3], gyro_filtered[3];
float yaw = 0, Tyaw = 0, pitch = 0, roll = 0;

float pitch_target = 0.0;
float roll_target = 0.0;
float yaw_target = 0.0;

float roll_rate_output = 0;
float pitch_rate_output = 0;

float pitch_target_rate = 0.0;
float roll_target_rate = 0.0;
float yaw_target_rate = 0.0;

float roll_gains[3]={2.5f,0.0f,0.0f};
float pitch_gains[3]={2.0f,0.0f,0.0f};
float yaw_gains[3]={0.0f,0.0f,0.0f};

float roll_rate_gains[3]={0.045f,0.0001f,0.0f};
float pitch_rate_gains[3]={0.04f,0.0f,0.0f};
float yaw_rate_gains[3]={0.08f,0.0f,0.0f};
float base_thrust = 0.0; //dejame en cero por seguridad la ultima vez me dejaste en 25 borrame si ya lo cambiaste!

uint64_t last_packet_time = 0;
