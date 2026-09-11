#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>

// -------------------------
// LCD
// -------------------------

LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------------------------
// SENSORES I2C
// -------------------------
// IMPORTANTE: el BME280 y el BMP280 son sensores I2C, NO analógicos.
// Por defecto ambos suelen usar la dirección 0x76 o 0x77.
// Como tienes dos módulos en el mismo bus, deben tener direcciones distintas.
// Revisa el pin SDO/ADDR de cada módulo (o los puentes/resistencias en la
// placa) para saber cuál dirección le corresponde a cada uno. Si tu módulo
// no trae ese pin, prueba 0x76 y 0x77 hasta que begin() responda true.

#define BME280_ADDR 0x76
#define BMP280_ADDR 0x77

Adafruit_BME280 bme; // Temperatura, Humedad, Presión
Adafruit_BMP280 bmp; // Temperatura, Presión

bool bmeOK = false;
bool bmpOK = false;

// -------------------------
// ALIMENTACION LCD
// -------------------------
// Este pin no alimenta a los sensores (esos van por I2C), es la
// fuente de 5V para el modulo I2C de la LCD.

#define Potenciado 5

// -------------------------
// LED
// -------------------------

#define LED 23

// -------------------------
// TECLADO
// -------------------------

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = {13, 12, 14, 27}; /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// -------------------------
// SETUP
// -------------------------

void setup() {

  Serial.begin(9600);

  // LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Alimentar el modulo I2C de la LCD (5V a traves de GPIO5)
  pinMode(Potenciado, OUTPUT);
  digitalWrite(Potenciado, HIGH);
  delay(100); // dar tiempo a que la LCD arranque antes de hablarle por I2C

  // I2C (usa los pines I2C por defecto del ESP32: SDA=21, SCL=22,
  // a menos que ya los hayas cambiado con Wire.begin(SDA, SCL))
  Wire.begin();

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("UdaVinci");

  // Inicializar BME280
  bmeOK = bme.begin(BME280_ADDR, &Wire);
  if (!bmeOK) {
    Serial.println("No se encontro el BME280. Revisa direccion I2C/cableado.");
  }

  // Inicializar BMP280
  bmpOK = bmp.begin(BMP280_ADDR);
  if (!bmpOK) {
    Serial.println("No se encontro el BMP280. Revisa direccion I2C/cableado.");
  }

  delay(1000);

  lcd.clear();
  lcd.print("Presiona tecla");
}

// -------------------------
// LOOP
// -------------------------

void loop() {

  char key = keypad.getKey();

  if (key) {

    Serial.print("Tecla presionada: ");
    Serial.println(key);

    // -------------------------
    // A = SENSOR BME280 (Temp / Humedad / Presion)
    // -------------------------

    if (key == 'A') {

      lcd.clear();

      if (!bmeOK) {
        lcd.setCursor(0, 0);
        lcd.print("BME280 no");
        lcd.setCursor(0, 1);
        lcd.print("detectado");
        Serial.println("BME280 no disponible.");
      } else {
        float temperatura = bme.readTemperature();       // °C
        float humedad      = bme.readHumidity();          // %
        float presion       = bme.readPressure() / 100.0F; // hPa

        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperatura, 1);
        lcd.print((char)223); // simbolo de grado
        lcd.print("C H:");
        lcd.print(humedad, 0);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("P:");
        lcd.print(presion, 1);
        lcd.print("hPa");

        Serial.print("BME280 -> Temp: ");
        Serial.print(temperatura);
        Serial.print(" C, Humedad: ");
        Serial.print(humedad);
        Serial.print(" %, Presion: ");
        Serial.print(presion);
        Serial.println(" hPa");
      }
    }

    // -------------------------
    // B = SENSOR BMP280 (Temp / Presion)
    // -------------------------

    else if (key == 'B') {

      lcd.clear();

      if (!bmpOK) {
        lcd.setCursor(0, 0);
        lcd.print("BMP280 no");
        lcd.setCursor(0, 1);
        lcd.print("detectado");
        Serial.println("BMP280 no disponible.");
      } else {
        float temperatura = bmp.readTemperature();       // °C
        float presion       = bmp.readPressure() / 100.0F; // hPa

        lcd.setCursor(0, 0);
        lcd.print("Sensor BMP280");

        lcd.setCursor(0, 1);
        lcd.print(temperatura, 1);
        lcd.print((char)223);
        lcd.print("C ");
        lcd.print(presion, 0);
        lcd.print("hPa");

        Serial.print("BMP280 -> Temp: ");
        Serial.print(temperatura);
        Serial.print(" C, Presion: ");
        Serial.print(presion);
        Serial.println(" hPa");
      }
    }

    // -------------------------
    // C = MENSAJE SERIAL
    // -------------------------

    else if (key == 'C') {

      Serial.println("Tecla C presionada.");
      Serial.println("Mensaje enviado por el puerto serie.");

      lcd.clear();
      lcd.print("Mensaje enviado");
    }

    // -------------------------
    // 1 = ENCENDER LED
    // -------------------------

    else if (key == '1') {

      digitalWrite(LED, HIGH);

      Serial.println("LED encendido");

      lcd.clear();
      lcd.print("LED ENCENDIDO");
    }

    // -------------------------
    // 2 = APAGAR LED
    // -------------------------

    else if (key == '2') {

      digitalWrite(LED, LOW);

      Serial.println("LED apagado");

      lcd.clear();
      lcd.print("LED APAGADO");
    }

    delay(500);
  }
}