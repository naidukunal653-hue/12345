
#include <Wire.h>
#include <HX711.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// =============================
// LYMPHIX ESP32 MULTISENSOR
// =============================

// HX711 pins
#define HX711_DOUT 4
#define HX711_SCK  5

// I2C pins
#define I2C_SDA 21
#define I2C_SCL 22

// Objects
HX711 scale;
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

// Sensor status
bool hx711_ok = false;
bool bmp280_ok = false;
bool mpu6050_ok = false;

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("LYMPHIX MULTISENSOR ESP32");
  Serial.println("================================");

  // -----------------------------
  // I2C
  // -----------------------------

  Wire.begin(I2C_SDA, I2C_SCL);

  // -----------------------------
  // HX711
  // -----------------------------

  scale.begin(HX711_DOUT, HX711_SCK);

  if (scale.is_ready()) {
    hx711_ok = true;
    Serial.println("HX711: OK");
  } else {
    Serial.println("HX711: NOT READY");
  }

  // -----------------------------
  // BMP280
  // -----------------------------

  if (bmp.begin(0x76)) {

    bmp280_ok = true;

  } else if (bmp.begin(0x77)) {

    bmp280_ok = true;

  }

  if (bmp280_ok) {
    Serial.println("BMP280: OK");
  } else {
    Serial.println("BMP280: NOT FOUND");
  }

  // -----------------------------
  // MPU6050
  // -----------------------------

  if (mpu.begin()) {

    mpu6050_ok = true;

    mpu.setAccelerometerRange(
      MPU6050_RANGE_2_G
    );

    mpu.setGyroRange(
      MPU6050_RANGE_250_DEG
    );

    mpu.setFilterBandwidth(
      MPU6050_BAND_21_HZ
    );
  }

  if (mpu6050_ok) {
    Serial.println("MPU6050: OK");
  } else {
    Serial.println("MPU6050: NOT FOUND");
  }

  Serial.println();
  Serial.println("Sensor initialization complete.");
  Serial.println();
}

void loop() {

  // =============================
  // LOAD CELL
  // =============================

  long raw_load = 0;

  if (hx711_ok && scale.is_ready()) {
    raw_load = scale.read();
  }

  // =============================
  // BMP280
  // =============================

  float temperature = 0.0;
  float pressure_hPa = 0.0;

  if (bmp280_ok) {

    temperature = bmp.readTemperature();

    pressure_hPa =
      bmp.readPressure() / 100.0F;
  }

  // =============================
  // MPU6050
  // =============================

  float accel_x = 0.0;
  float accel_y = 0.0;
  float accel_z = 0.0;

  float gyro_x = 0.0;
  float gyro_y = 0.0;
  float gyro_z = 0.0;

  if (mpu6050_ok) {

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    mpu.getEvent(
      &accel,
      &gyro,
      &temp
    );

    accel_x = accel.acceleration.x;
    accel_y = accel.acceleration.y;
    accel_z = accel.acceleration.z;

    gyro_x = gyro.gyro.x;
    gyro_y = gyro.gyro.y;
    gyro_z = gyro.gyro.z;
  }

  // =============================
  // SERIAL OUTPUT
  // =============================

  Serial.println("--------------------------------");

  Serial.print("Load RAW: ");
  Serial.println(raw_load);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(pressure_hPa);
  Serial.println(" hPa");

  Serial.print("Accel X: ");
  Serial.print(accel_x);

  Serial.print(" | Y: ");
  Serial.print(accel_y);

  Serial.print(" | Z: ");
  Serial.println(accel_z);

  Serial.print("Gyro X: ");
  Serial.print(gyro_x);

  Serial.print(" | Y: ");
  Serial.print(gyro_y);

  Serial.print(" | Z: ");
  Serial.println(gyro_z);

  delay(1000);
}