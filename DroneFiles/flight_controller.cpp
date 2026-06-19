#include "define.h"
using namespace std;
float maxmin(float value){
    if(value>=100)value=100;
    if(value<=0)value=0;
    if(base_thrust<=0)value=0;
    return value;
}

void master_setup(){
    nvs_flash_init();
    vector<array<uint8_t, 6>> peers = {peer_mac};
    espnow.setup(peers);
    bmi.init_BMI160(3,3,0x68,false);
    bmi.set_or_reset_offsets(0.517803,0.275906,0.049337,-0.406107,-0.201527,-0.039695);
    hmc.init_HMC5883L(false);
    hmc.set_offset(0.046, -0.115, -0.189); //offset probado  0.065,-0.275,-0.272
    kalman_pitch.setup(Q_angle_pitch,Q_bias,R_measure_1);
    kalman_roll.setup(Q_angle_roll,Q_bias,R_measure_2);
    kalman_yaw_Wtilt.setup(Q_angle_yaw,Q_bias,R_measure_3,true);
    roll_pid.setup(roll_gains,dt_250hz);
    pitch_pid.setup(pitch_gains,dt_250hz); 
    yaw_pid.setup(yaw_gains,dt_250hz,100.0F,true);
    roll_rate_pid.setup(roll_rate_gains,dt_1Khz);
    pitch_rate_pid.setup(pitch_rate_gains,dt_1Khz);
    yaw_rate_pid.setup(yaw_rate_gains,dt_1Khz);
    motorUL.setup(channel_0,MUL_pin);
    motorUR.setup(channel_1,MUR_pin);
    motorDL.setup(channel_2,MDL_pin);
    motorDR.setup(channel_3,MDR_pin);
    esp_task_wdt_deinit();
    motorUL.setDuty(0);
    motorUR.setDuty(0);
    motorDL.setDuty(0);
    motorDR.setDuty(0);
    vTaskDelay(5000/portTICK_PERIOD_MS);    
}

void read_gyro(){
    bmi.readGyro(gyro);
}

void read_acc(){
    bmi.readAcc(Acc);
}   

void read_mag(){
    hmc.read_hmc5883l(hmc_data);
    B_xyz[0] = hmc_data[0];
    B_xyz[1] = hmc_data[1];
    B_xyz[2] = hmc_data[2];
}

void Kalman_roll_pitch(){
    float mea_roll = atan2(Acc[1],Acc[2]) * 180.0 / M_PI;
    float mea_pitch = atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI;
    pitch = kalman_pitch.calculate(pitch,gyro[0],mea_pitch,dt_250hz);
    roll = kalman_roll.calculate(roll,gyro[1],mea_roll,dt_250hz);
    //printf(">roll:%.2f,pitch%.2f\r\n",roll,pitch);
}

void Kalman_yaw(){
    Rot_xy[0] = roll;
    Rot_xy[1] = pitch;
    float mea_Tyaw = kalman_yaw_Wtilt.tilt_compensation(B_xyz, Rot_xy);
    Tyaw = kalman_yaw_Wtilt.calculate(Tyaw,gyro[2],mea_Tyaw,dt_50hz);
    
}

void EMA_Filter(){
    for(int i=0;i<3;i++) gyro_filtered[i] = (alpha * gyro[i]) + ((1.0f - alpha) * gyro_filtered[i]);
}

void Network_IN(){
    if (espnow.is_data_received()) {
        espnow_data_t dataIn = espnow.get_received_data();
        //ESP_LOGI(TAG, "Datos recibidos por ESPNOW: %s", dataIn.message);
        int items_read = sscanf(dataIn.message, "%f,%f,%f", &base_thrust,&roll_target,&pitch_target);
    }
}

void PID_ANGLE(){
    //Primary Loop
    roll_rate_output = roll_pid.calculate(roll_target,roll);
    pitch_rate_output = pitch_pid.calculate(pitch_target,pitch);
    //Limitar la velocidad angular de salida de acuerdo a lo que puede ver tu sensor en °/s
    if(roll_rate_output>300)roll_rate_output = 300;          
    if(roll_rate_output<-300)roll_rate_output = -300;
    if(pitch_rate_output>300)pitch_rate_output = 300;
    if(pitch_rate_output<-300)pitch_rate_output = -300;
}
void PID_RATE(){

    //Secondary Loop
    float roll_output = roll_rate_pid.calculate(roll_rate_output,gyro_filtered[1]);
    float pitch_output = pitch_rate_pid.calculate(pitch_rate_output,gyro_filtered[0]);
    float yaw_output = yaw_rate_pid.calculate(yaw_target_rate, gyro_filtered[2]);

    float duty_UL = base_thrust - roll_output*1.15 - pitch_output + yaw_output; // M1 (Adelante, Izquierda)
    float duty_UR = base_thrust - roll_output*1.15 + pitch_output - yaw_output; // M2 (Adelante, Derecha)
    float duty_DL = base_thrust + roll_output*0.92 - pitch_output - yaw_output; // M3 (Atrás, Izquierda)
    float duty_DR = base_thrust + roll_output*0.92 + pitch_output + yaw_output; // M4 (Atrás, Derecha)

    //printf("UL: %.2f UR: %.2f DL: %.2f DR: %.2f PITCH: %.2f ROLL: %.2f\n",duty_UL,duty_UR,duty_DL,duty_DR,pitch,roll);
    //printf("pitch_t: %.2f roll_t: %.2f\n",pitch_target,roll_target);
    //printf("YAW: %f\n",Tyaw);
    
    duty_UL = maxmin(duty_UL);
    duty_UR = maxmin(duty_UR);
    duty_DL = maxmin(duty_DL);
    duty_DR = maxmin(duty_DR);
    
    motorUL.setDuty(duty_UL);
    motorUR.setDuty(duty_UR);
    motorDL.setDuty(duty_DL);
    motorDR.setDuty(duty_DR);

    if(base_thrust<5.0){
        roll_pid.resetIntegral();
        pitch_pid.resetIntegral();
        yaw_pid.resetIntegral();
        
        roll_rate_pid.resetIntegral();
        pitch_rate_pid.resetIntegral();
        yaw_rate_pid.resetIntegral();
    }
}