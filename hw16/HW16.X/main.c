#include "nu32dip.h"   // constants, functions for startup and UART
#include <stdio.h>
#include "utilities.h" // Helper functions for motor control
#include "uart2.h" // include the library

int main(void) {
    NU32DIP_Startup();             // cache on, interrupts on, LED/button init, UART init
    setup_motor_timers_pins();     // Setup timers and interrupts for motor control
    UART2_Startup();

    while (1) {
        // PIC UART2 code
        int com = 0;
        // uart2_flag() is 1 when uart2 has rx a message and sprintf'd it into a value
        if(get_uart2_flag()){
                set_uart2_flag(0); // set the flag to 0 to be ready for the next message
                com = get_uart2_value();
                char line_center[100];
                sprintf(line_center,"%d\n\n\n\r",com);
                NU32DIP_WriteUART1(line_center);
                
//        // P Controller
          // Somewhat follows the line, but stalls and is very slow.
//        int set_point = 30;
//        int Kp = 1.0;
//        int error = 0;
//        int max_pwm = 70;
//        int min_pwm = 20;
//        int base_pwm = 30;
//        
//        error = com - set_point;
//        
//        MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//        MOTOR_2_DIRECTION = 0; // Forward -> Right wheel
//        
//        int left_pwm = error*Kp + base_pwm;
//        int right_pwm = -error*Kp + base_pwm;
//        
//        if (left_pwm >= max_pwm){
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
//        }
//        else if (left_pwm <= min_pwm){
//            MOTOR_1_DIRECTION = 0;
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
//        }
//        else {
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)left_pwm/100.0);
//        }
//        
//        if (right_pwm >= max_pwm){
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
//        }
//        else if (right_pwm <= min_pwm){
//            MOTOR_2_DIRECTION = 1;
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
//        }
//        else {
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)right_pwm/100.0);
//        }
                
                
                
                
                
                
                
                
                
                
                
        // P Controller -> WORKS GREAT JUST HOLD A LIGHT
        int set_point = 30;
        int Kp = 1.0;
        int error = 0;
        int max_pwm = 70;
        int min_pwm = 20;
        int base_pwm = 30;
        
        error = com - set_point;
        
        MOTOR_RIGHT_DIRECTION = 1; // Forward -> Right wheel
        MOTOR_LEFT_DIRECTION = 0; // Forward -> Left wheel
        
        int left_pwm = error*Kp + base_pwm;
        int right_pwm = -error*Kp + base_pwm;
        
        if (left_pwm >= max_pwm){
            MOTOR_RIGHT_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
        }
        else if (left_pwm <= min_pwm){
            MOTOR_RIGHT_DIRECTION = 0;
            MOTOR_RIGHT_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
        }
        else {
            MOTOR_RIGHT_SPEED = (int)(PR3_PERIOD*(float)left_pwm/100.0);
        }
        
        if (right_pwm >= max_pwm){
            MOTOR_LEFT_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
        }
        else if (right_pwm <= min_pwm){
            MOTOR_LEFT_DIRECTION = 1;
            MOTOR_LEFT_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
        }
        else {
            MOTOR_LEFT_SPEED = (int)(PR3_PERIOD*(float)right_pwm/100.0);
        }
                
                
                
                
//        // P Controller -> 5 sections ->>>> Pretty okay
//        int set_point = 30;
//        int Kp = 1.0; // 3           // DO NOT INCREASE ABOVE THIS WITHOUT ADDING PWM LIMITS
//        int error = 0;
//        int max_pwm = 50;
//        int min_pwm = 20;
//        int base_pwm = 30;
//        
//        // Quarters
//        int Q1 = 0;
//        int Q2 = 15;
//        int Q3 = 25;
//        int Q4 = 35;
//        int Q5 = 45;
//        int Q6 = 60;
//        
//        error = com - set_point;
//        
//        int left_pwm = 0;
//        int right_pwm = 0;
//        
//        // Q1 Max right wheel forward, Left wheel reverse with PID controller
//        if (Q1 <= com && com <= Q2){
//            MOTOR_RIGHT_DIRECTION = 0; // Forward -> Right wheel
//            MOTOR_LEFT_DIRECTION = 0; // Reverse -> Left wheel
//            left_pwm = max_pwm;
//            right_pwm = -error*Kp; // error is negative
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",1);
//            NU32DIP_WriteUART1(line_center);
//            
//        }
//        else if (Q2 < com && com <= Q3){
//            MOTOR_RIGHT_DIRECTION = 1; // Forward -> Right wheel
//            MOTOR_LEFT_DIRECTION = 0; // Forward -> Left wheel
//            left_pwm = max_pwm;
//            right_pwm = max_pwm + error*Kp; // error is negative
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",2);
//            NU32DIP_WriteUART1(line_center);
//        }
//        else if (Q3 < com && com <= Q4){ // Line in center
//            MOTOR_RIGHT_DIRECTION = 1; // Forward -> Right wheel
//            MOTOR_LEFT_DIRECTION = 0; // Forward -> Left wheel
//            left_pwm = max_pwm;
//            right_pwm = max_pwm;
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",3);
//            NU32DIP_WriteUART1(line_center);
//        }
//        else if (Q4 < com && com <= Q5){
//            MOTOR_RIGHT_DIRECTION = 1; // Forward -> Right wheel
//            MOTOR_LEFT_DIRECTION = 0; // Forward -> Left wheel
//            right_pwm = max_pwm;
//            left_pwm = max_pwm - error*Kp; // error is positive 
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",4);
//            NU32DIP_WriteUART1(line_center);
//        }
//        else if (Q5 < com && com <= Q6){
//            MOTOR_RIGHT_DIRECTION = 1; // Reverse -> Right wheel
//            MOTOR_LEFT_DIRECTION = 1; // Forward -> Left wheel
//            left_pwm = error*Kp; // error is positive
//            right_pwm = max_pwm;
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",5);
//            NU32DIP_WriteUART1(line_center);
//        }
//        else { // Center value not in image or broken or FLAG
//            left_pwm = 0;
//            right_pwm = 0;
//            
//            char line_center[100];
//            sprintf(line_center,"%d\n\n\n\r",6);
//            NU32DIP_WriteUART1(line_center);
//        }
//        
//        
//        // Limit PWM
//        if (left_pwm > 100.0){
//            left_pwm = 100.0;
//        }
//        else if (left_pwm < 0.0){
//            left_pwm = 0.0;
//        }
//        
//        if (right_pwm > 100.0){
//            right_pwm = 100.0;
//        }
//        else if (right_pwm < 0.0){
//            right_pwm = 0.0;
//        }
//        
//        MOTOR_RIGHT_SPEED = (int)(PR3_PERIOD*(float)right_pwm/100.0);
//        MOTOR_LEFT_SPEED = (int)(PR3_PERIOD*(float)left_pwm/100.0);
        
        
        
        
        
        
        
        
        
        
        
        
        
        
//        
//        MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//        MOTOR_2_DIRECTION = 0; // Forward -> Right wheel
//        
//        int left_pwm = error*Kp + base_pwm;
//        int right_pwm = -error*Kp + base_pwm;
//        
//        if (left_pwm >= max_pwm){
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
//        }
//        else if (left_pwm <= min_pwm){
//            MOTOR_1_DIRECTION = 0;
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
//        }
//        else {
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)left_pwm/100.0);
//        }
//        
//        if (right_pwm >= max_pwm){
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)max_pwm/100.0);
//        }
//        else if (right_pwm <= min_pwm){
//            MOTOR_2_DIRECTION = 1;
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)base_pwm/100.0);
//        }
//        else {
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)right_pwm/100.0);
//        }
        
        
        
        
        
//        if (com == 1000) { // No line
//            MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
//            MOTOR_2_DIRECTION = 1; // Backward -> Right wheel
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)50.0/100.0);
//        }       
//        else if (com <= 32 && com >= 28) { 
//            MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)80.0/100.0);
//            MOTOR_2_DIRECTION = 0; // Forward -> Right wheel
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)80.0/100.0);
//        }
//        else if (com < 28) {
//            MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)40.0/100.0);
//            MOTOR_2_DIRECTION = 0; // Forward -> Right wheel
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)70.0/100.0);
//        }
//        else if (com > 32) {
//            MOTOR_1_DIRECTION = 1; // Forward -> Left wheel
//            MOTOR_1_SPEED = (int)(PR3_PERIOD*(float)70.0/100.0);
//            MOTOR_2_DIRECTION = 0; // Forward -> Right wheel
//            MOTOR_2_SPEED = (int)(PR3_PERIOD*(float)40.0/100.0);
//        }
        
    }
    }
}