#define IR_SENSOR1 A0
#define IR_SENSOR2 A1
#define IR_SENSOR3 A2

#define MOTOR1_LEFT_PIN PD2
#define MOTOR2_RIGHT_PIN PD4
#define MOTOR1_LEFT_L_PIN PD7
#define MOTOR2_RIGHT_L_PIN PD3

#define MOTOR1_EN_PIN PD5
#define MOTOR2_EN_PIN PD6

#define OFFSET 5
#define TURN_SPEED 200
#define FWD_SPEED 100

int ir_sensor1_val = 0;
int ir_sensor2_val = 0;
int ir_sensor_bl_val = 0;

bool is_mstimer_elapsed(long unsigned int * timer, uint16_t duration)
{
  long unsigned int diff = 0;
  diff = millis() - *timer;
  if(diff >= duration)
  return true;
  else
  return false;
}

void setup() {
  Serial.begin(9600);  
  pinMode(MOTOR1_EN_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);
  pinMode(MOTOR1_LEFT_PIN, OUTPUT);
  pinMode(MOTOR2_RIGHT_PIN, OUTPUT);
  pinMode(MOTOR1_LEFT_L_PIN, OUTPUT);
  pinMode(MOTOR2_RIGHT_L_PIN, OUTPUT);
  
  analogWrite(MOTOR1_EN_PIN, 0);
  analogWrite(MOTOR2_EN_PIN, 0);

  digitalWrite(MOTOR1_LEFT_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_PIN, 0);  
  digitalWrite(MOTOR1_LEFT_L_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_L_PIN, 0);  
}

void move_right()
{ 
  analogWrite(MOTOR1_EN_PIN, TURN_SPEED);
  analogWrite(MOTOR2_EN_PIN, TURN_SPEED + OFFSET);
  
  digitalWrite(MOTOR1_LEFT_PIN, 1);
  digitalWrite(MOTOR2_RIGHT_PIN, 0);

  digitalWrite(MOTOR1_LEFT_L_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_L_PIN, 1);  
  
}

void move_left()
{
  analogWrite(MOTOR1_EN_PIN, TURN_SPEED);
  analogWrite(MOTOR2_EN_PIN, TURN_SPEED + OFFSET);
  
  digitalWrite(MOTOR1_LEFT_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_PIN, 1);

  digitalWrite(MOTOR1_LEFT_L_PIN, 1);
  digitalWrite(MOTOR2_RIGHT_L_PIN, 0);  
}

void move_fwd()
{
  analogWrite(MOTOR1_EN_PIN, FWD_SPEED);
  analogWrite(MOTOR2_EN_PIN, FWD_SPEED + OFFSET);
  
  digitalWrite(MOTOR1_LEFT_PIN, 1);
  digitalWrite(MOTOR2_RIGHT_PIN, 1);

  digitalWrite(MOTOR1_LEFT_L_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_L_PIN, 0);  
    
}

void stop_movement()
{
  analogWrite(MOTOR1_EN_PIN, 0);
  analogWrite(MOTOR2_EN_PIN, 0);
  
  digitalWrite(MOTOR1_LEFT_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_PIN, 0);

  digitalWrite(MOTOR1_LEFT_L_PIN, 0);
  digitalWrite(MOTOR2_RIGHT_L_PIN, 0);  
  
}

void loop() {
static long unsigned int rotation_timer;

ir_sensor1_val = analogRead(IR_SENSOR1);
ir_sensor2_val = analogRead(IR_SENSOR2);
ir_sensor_bl_val = analogRead(IR_SENSOR3);

if(ir_sensor_bl_val > 500 || ir_sensor2_val > 500 || ir_sensor1_val > 500)
{
  rotation_timer = millis();  
  if(ir_sensor_bl_val > 500)
  {
    move_fwd();
  }
  else
  {
    if(ir_sensor2_val > 500)
    {
      // Serial.println("MOVE RIGHT");
      move_right();
    }

    else if(ir_sensor1_val > 500)
    {
      // Serial.println("MOVE LEFT");
      move_left();
    }
  }
}
else
{
  if(is_mstimer_elapsed(&rotation_timer, 3000))
  {
    stop_movement(); 
  }  
  else
  {
    move_left();
  }
}

}
