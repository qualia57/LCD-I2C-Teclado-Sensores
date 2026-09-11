# Estación de Sensores con LCD I2C y Teclado Matricial (ESP32)

Proyecto físico con ESP32 que integra una pantalla LCD I2C y un teclado matricial 4x4 para leer datos de dos sensores ambientales (BME280 y BMP280) y ejecutar distintas funciones mediante teclas.

## 📋 Descripción del proyecto

Al presionar una tecla del teclado matricial, el sistema ejecuta una función distinta, mostrando el resultado en la LCD y por el puerto serial:

| Tecla | Función |
|---|---|
| `A` | Lee y muestra temperatura, humedad y presión del sensor BME280 |
| `B` | Lee y muestra temperatura y presión del sensor BMP280 |
| `C` | Envía un mensaje de confirmación por el puerto serial |
| `1` | Enciende el LED indicador |
| `2` | Apaga el LED indicador |

## 🔧 Componentes utilizados

- Microcontrolador ESP32
- Pantalla LCD 16x2 con módulo I2C (dirección `0x27`)
- Teclado matricial 4x4
- Sensor BME280 (temperatura, humedad, presión) — dirección I2C `0x76`
- Sensor BMP280 (temperatura, presión) — dirección I2C `0x77`
- LED indicador

## 🔌 Conexiones

**Teclado matricial**

| Señal | GPIO |
|---|---|
| Fila 1–4 | 13, 12, 14, 27 |
| Columna 1–4 | 26, 25, 33, 32 |

**Bus I2C (LCD, BME280, BMP280)**

| Señal | GPIO |
|---|---|
| SDA | 21 (por defecto en ESP32) |
| SCL | 22 (por defecto en ESP32) |

**Otros**

| Elemento | GPIO |
|---|---|
| LED indicador | 23 |
| Alimentación módulo I2C de la LCD | 5 (salida a 5V) |

> Nota: BME280 y BMP280 comparten el mismo bus I2C, por lo que requieren direcciones distintas (`0x76` y `0x77`) configuradas vía el pin SDO/ADDR de cada módulo.

## 📚 Librerías requeridas

- [`Keypad`](https://www.arduino.cc/reference/en/libraries/keypad/)
- [`Wire`](https://www.arduino.cc/reference/en/language/functions/communication/wire/) (incluida en el core de Arduino/ESP32)
- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C)
- [`Adafruit_Sensor`](https://github.com/adafruit/Adafruit_Sensor)
- [`Adafruit_BME280`](https://github.com/adafruit/Adafruit_BME280_Library)
- [`Adafruit_BMP280`](https://github.com/adafruit/Adafruit_BMP280_Library)

Instalables desde el Library Manager de Arduino IDE buscando cada nombre.

## ▶️ Cómo ejecutar

1. Armar el circuito físico según la tabla de conexiones.
2. Instalar las librerías indicadas arriba en Arduino IDE.
3. Cargar `sketch.ino` (o el nombre del archivo fuente) a la ESP32.
4. Abrir el monitor serial a 9600 baudios.
5. Presionar las teclas `A`, `B`, `C`, `1` o `2` para ejecutar cada función.

## 📁 Estructura del repositorio

```
.
├── sketch.ino          # Código fuente principal
└── README.md
```

## 🎯 Objetivo académico

Proyecto de Sistemas Embebidos enfocado en integrar múltiples periféricos (LCD I2C, teclado matricial y sensores I2C) en un mismo microcontrolador, gestionando direcciones de bus compartido y ejecución de funciones por evento de teclado.

## 👤 Autor

Nico
