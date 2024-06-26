#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);  // the lcd display is initialized.0x27 indicates the I2C module's address.
//16 means the number of characters in each row and 2 means the number of lines

//moisture sensor pins
int moisture_sensor_1 = A0;
int moisture_sensor_2 = A1;

int pump_pin = 8;  // the pin which is connected to the relay switch that is responsible for turning the pump on or off. that's why it is named as pump pin.
int waterlevel_sensor = A3;
int buzzer = 12;

int moisture_1_percentage = 0;
int moisture_2_percentage = 0;

int soilMoistureValue1 = 0;
int soilMoistureValue2 = 0;
int waterlevel = 0;

int percentage = 0;

//these values are set to zero cause there can be some garbage value in these pin at first

void setup() {
  lcd.init();       //initializes the lcd display
  lcd.backlight();  // the backlight of the lcd is turned on

  pinMode(pump_pin, OUTPUT);  //pump_pin is set to output that means the arduino will treat the pin as output pin. i.e arduino uno will use this pin to give output to the relay switch
  pinMode(buzzer, OUTPUT);    //buzzer pin will be used to turn on the buzzer

  digitalWrite(pump_pin, HIGH);  // the realy switch is negative triggered, so when the input is high it will be turned off and turned on when the input gets low.
  lcd.setCursor(1, 1);
  lcd.print("pump off");

  Serial.begin(4800);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("                ");

  soilMoistureValue1 = analogRead(moisture_sensor_1);
  soilMoistureValue2 = analogRead(moisture_sensor_2);

  moisture_1_percentage = map(soilMoistureValue1, 0, 1023, 100, 0);
  moisture_2_percentage = map(soilMoistureValue2, 0, 1023, 100, 0);

  lcd.setCursor(0, 0);

  lcd.print("M1: ");
  lcd.print(moisture_1_percentage);
  lcd.print("%");

  lcd.print(" M2: ");
  lcd.print(moisture_2_percentage);
  lcd.print("%");

  delay(700);

  lcd.setCursor(0, 0);
  lcd.print("               ");

  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print("%");

  Serial.print("soilMoistureValue1 : ");
  Serial.println(moisture_1_percentage);
  Serial.print("soilMoistureValue2 : ");
  Serial.println(moisture_2_percentage);

  lcd.setCursor(1, 1);
  waterlevel = analogRead(waterlevel_sensor);

  if ((moisture_1_percentage < 40 || moisture_2_percentage < 40)) {
    if (waterlevel >= 200) {
      lcd.setCursor(1, 1);
      lcd.print("                    ");
      lcd.setCursor(1, 1);
      lcd.print("pump on");
      digitalWrite(pump_pin, LOW);
    } else {
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print("pump off");

      digitalWrite(pump_pin, HIGH);
    }
  } else if (moisture_1_percentage > 75 && moisture_2_percentage > 75) {
    lcd.setCursor(1, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("pump off");

    digitalWrite(pump_pin, HIGH);
  }

  Serial.print("waterlevel value : ");
  Serial.println(waterlevel);

  if (waterlevel <= 200) {
    tone(buzzer, 130.81);
    delay(500);
    noTone(buzzer);
    lcd.setCursor(1, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("pump off");
    digitalWrite(pump_pin, HIGH);
  }
  delay(700);
}
