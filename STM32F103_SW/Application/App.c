
#include "App.h"
// -----------------------------------------------
//              Global Variables
// -----------------------------------------------
Ultrasonic_Init_t ultrasonic_init;

volatile uint8_t is_open = 0; 			 // 0 = closed, 1 = open
volatile uint16_t distance = 0;

// -----------------------------------------------
//              Initialization
// -----------------------------------------------
void App_Init()
{
    ultrasonic_init.echo_pin  = ULTRASONIC_ECHO_PIN;
    ultrasonic_init.echo_port = ULTRASONIC_ECHO_PORT;
    ultrasonic_init.trig_pin  = ULTRASONIC_TRIG_PIN;
    ultrasonic_init.trig_port = ULTRASONIC_TRIG_PORT;

    Servo_Attach(SERVO_PIN);       // Servo init
    Ultrasonic_Init(&ultrasonic_init); // Ultrasonic init

    LCD_Init();
    LCD_Write_Command(LCD_Clear);
    LCD_Write_String((uint8_t*)"System Ready");
    TIMx_delay_ms(500);
}
// -----------------------------------------------
//            Helper — Smooth door movement
// -----------------------------------------------
static void Door_MoveSmooth(uint8_t start, uint8_t end)
{
    if(start < end)  // open
    {
        for(int8_t a = start; a <= end; a += 5)
        {
            Servo_Write((uint8_t)a);
            TIMx_delay_ms(DOOR_SPEED_DELAY);
        }
    }
    else             // close
    {
        for(int8_t a = start; a >= end; a -= 5)
        {
            Servo_Write((uint8_t)a);
            TIMx_delay_ms(DOOR_SPEED_DELAY);
        }
    }
}

// -----------------------------------------------
//              Main Logic
// -----------------------------------------------
void App_Start()
{
    distance = Ultrasonic_GetDistance(&ultrasonic_init);
    // ---- LCD Update ----
    LCD_Write_Command(LCD_Clear);
    LCD_Write_String((uint8_t*) "Distance: ");
    LCD_Write_Number(distance);
    LCD_Write_String((uint8_t*)"cm");

    LCD_Write_Command(LINE_TWO);

    // ---------------------------------------
    //              Door Logic
    // ---------------------------------------

    if(distance < OPEN_TH && is_open == 0)  // Someone present + door closed  → open
    {
        Door_MoveSmooth(DOOR_CLOSE_ANGLE, DOOR_OPEN_ANGLE);
        is_open = 1;

        LCD_Write_String((uint8_t*)"Door Opened");
    }
    else if(distance < OPEN_TH && is_open == 1)   // Someone present + door open → keep open
    {
        Servo_Write(DOOR_OPEN_ANGLE);
        LCD_Write_String((uint8_t*)"Door Open");
    }
    else if(distance > CLOSE_TH && is_open == 1)  // Someone present + door open → keep open
    {
        Door_MoveSmooth(DOOR_OPEN_ANGLE, DOOR_CLOSE_ANGLE);
        is_open = 0;

        LCD_Write_String((uint8_t*)"Door Closed");
    }
    else if(distance > CLOSE_TH && is_open == 0)  // No one + door closed → keep closed
    {
        Servo_Write(DOOR_CLOSE_ANGLE);
        LCD_Write_String((uint8_t*)"Door Closed");
    }
    TIMx_delay_ms(REFRESH_RATE);
}

