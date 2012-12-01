// Automated Chicken coop door
// jdw - may 2012
// motor driven by Sabertooth 2x12 RC on pin D6
// bottom pot = A5, top pot = A4
// bottom button = D3, top button = 2, switch = 4
// top IR sensor = A0, bottom IR sensor = A1
// light sensor = A2

// Sabertooth 2x12 RC control wire - connect to D6
int door_motor_pin = 6;
// variables for door top Infrared sensor - connect to A0
int ir_top = 0;
int ir_top_val;
int ir_top_sum = 0;
// variables for door bottom Infrared sensor - connect to A1
int ir_bottom = 1;
int ir_bottom_val;
int ir_bottom_sum = 0;
// variables for LDR (light sensor) - connect to A2
int ldr = 2;
int light_val;
int light_sum = 0;
// variables for aux potentiometers - connect to A4 and A5
int pot1 = 5;
int pot2 = 4;
int dawn_dusk = 25;
// variables for red button - connect to D2
int button_red = 2;
int button_red_val;
// variables for black button - connect to D3
int button_black = 3;
int button_black_val
// variables for toggle switch - connect to D4
int toggle_switch = 4;
int toggle_switch_val;
// extra variables
int ir_threshold = 150;
boolean closed = 0;
boolean daytime = 0;
boolean automatic = 0;

void setup(){
  Serial.begin(9600);
  pinMode(door_motor_pin, OUTPUT);
  pinMode(ir_top, INPUT);
  pinMode(ir_bottom, INPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(ldr, INPUT);
  pinMode(button_red, INPUT);
  digitalWrite(button_red, HIGH);
  pinMode(button_black, INPUT);
  digitalWrite(button_black, HIGH);
  pinMode(toggle_switch, INPUT);
  digitalWrite(toggle_switch, HIGH);
  door_stop();
  delay(500);
}

void loop(){

  read_sensors();

  if (automatic){
    if (daytime){
      if (ir_top_val > ir_threshold){
        door_up;
      }
      else {
        door_stop();
      }
    }
    else {
      if (ir_bottom_val > ir_threshold){
        door_down;
      }
      else {
        door_stop();
      }
    }
  }
  else {
    if (button_red_val == HIGH && button_black_val == LOW){
      door_up();
    }
    else if (button_red_val == LOW && button_black_val == HIGH){
      door_down();
    }
    else {
      door_stop();
    }
    Serial.print("LDR: ");
    Serial.print(light_val);
    Serial.print("  Top: ");
    Serial.print(ir_top_val);
    Serial.print("  Bottom: ");
    Serial.print(ir_bottom_val);  

    Serial.print("  DD: ");
    Serial.print(dawn_dusk);
    Serial.print("  speed: ");
    Serial.print(motor_speed);

    Serial.println("");
  }
}

void read_ir_sensors(){
  for (int x = 0; x < 10; x++){
    ir_top_sum += analogRead(ir_top);
    delay(10);
  }
  ir_top_val = ir_top_sum / 10;
  ir_top_sum = 0;

  for (int x = 0; x < 10; x++){
    ir_bottom_sum += analogRead(ir_bottom);
    delay(10);
  }
  ir_bottom_val = ir_bottom_sum / 10;
  ir_bottom_sum = 0;
}

void read_ldr(){
  for (int x = 0; x < 10; x++){
    light_sum += analogRead(ldr);
    delay(10);
  }
  light_val = light_sum / 10;
  light_sum = 0;

  if (light_val > dawn_dusk){
    daytime = true;
  }
  else {
    daytime = false;
  }
}

void read_inputs(){
  automatic = digitalRead(toggle_switch);
  button_red_val = digitalRead(button_red);
  button_black_val = digitalRead(button_black);

  dawn_dusk = analogRead(pot2);
  motor_speed = map(analogRead(pot1), 0, 1023, 0, 500);
}

void read_sensors(){
  read_ir_sensors();
  read_ldr();
  read_inputs();
}

void door_up(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(1500 + motor_speed); 
  digitalWrite(door_motor_pin, LOW);
}

void door_down(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(1500 - motor_speed); 
  digitalWrite(door_motor_pin, LOW); 
}

void door_stop(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(1500); 
  digitalWrite(door_motor_pin, LOW);
}












