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

int ir_right_sensor_val = 0;
int ir_left_sensor_val = 0;
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
static long unsigned int rotation_timer, fetch_timer;

ir_left_sensor_val = analogRead(IR_SENSOR1);
ir_right_sensor_val = analogRead(IR_SENSOR2);
ir_sensor_bl_val = analogRead(IR_SENSOR3);
static uint8_t val;

if(fetch_timer == 0)
{
  fetch_timer = millis();
}
else if(fetch_timer != 0 && is_mstimer_elapsed(&fetch_timer, 500))
{
  val = (((ir_sensor_bl_val)/500) * pow(2,2)) + (((ir_left_sensor_val)/500) * pow(2,1)) + (((ir_right_sensor_val)/500) * pow(2,0));
  fetch_timer = 0;
}

if(val > 0)
{
  rotation_timer =  millis() ;
  switch(val){
    case 1:
      move_right();
      break;
    
    case 3:
    case 2:
      move_left();
      break;
    
    case 7:
    case 5:
    case 6:
    case 4:
      move_fwd();
      break;
  }
}
else
{
  if(is_mstimer_elapsed(&rotation_timer, 9000))
  {
    stop_movement(); 
  }  
  else
  {
    move_left();
  }
}

}
