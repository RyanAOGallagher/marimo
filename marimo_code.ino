#include <DHT.h>
#include <pm2008_i2c.h>

#define PWM_PIN 3
#define DHTPIN 6
#define DHTTYPE DHT11
#define MIC_PIN A0

DHT dht(DHTPIN, DHTTYPE);
PM2008_I2C pm2008_i2c;

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 5000;        // interval at which to blink (milliseconds)
int pwmValue = 127;                // starting PWM value
bool pwmState = true;              // true means PWM is set to 127, false means 0

void setup() {
    Serial.begin(9600);
    pinMode(PWM_PIN, OUTPUT);
    dht.begin();
#ifdef PM2008N
    delay(10000);
#endif
    pm2008_i2c.begin();
    pm2008_i2c.command();
    delay(1000);
}

void loop() {
    delay(2000);
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (pwmState) {
            pwmValue = 0;
            pwmState = false;
        } else {
            pwmValue = 127;
            pwmState = true;
        }
        analogWrite(PWM_PIN, pwmValue);

        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        int micValue = analogRead(MIC_PIN);
        uint8_t ret = pm2008_i2c.read();

        uint8_t pm1p0_grade = 0;
        uint8_t pm2p5_grade = 0;
        uint8_t pm10p_grade = 0;
        uint8_t total_grade = 0;

        if (ret == 0) {
            // PM 1.0
            if (pm2008_i2c.pm1p0_grimm < 16) {
                pm1p0_grade = 1;
            } else if (pm2008_i2c.pm1p0_grimm < 51) {
                pm1p0_grade = 2;
            } else if (pm2008_i2c.pm1p0_grimm < 101) {
                pm1p0_grade = 3;
            } else {
                pm1p0_grade = 4;
            }

            // PM 2.5
            if (pm2008_i2c.pm2p5_grimm < 16) {
                pm2p5_grade = 1;
            } else if (pm2008_i2c.pm2p5_grimm < 51) {
                pm2p5_grade = 2;
            } else if (pm2008_i2c.pm2p5_grimm < 101) {
                pm2p5_grade = 3;
            } else {
                pm2p5_grade = 4;
            }

            // PM 10
            if (pm2008_i2c.pm10_grimm < 31) {
                pm10p_grade = 1;
            } else if (pm2008_i2c.pm10_grimm < 81) {
                pm10p_grade = 2;
            } else if (pm2008_i2c.pm10_grimm < 151) {
                pm10p_grade = 3;
            } else {
                pm10p_grade = 4;
            }

            // Get worst grade
            total_grade = max(pm1p0_grade, pm2p5_grade);
            total_grade = max(total_grade, pm10p_grade);

            // Print the results to the Serial Monitor
            switch (total_grade) {
                case 1: {
                    Serial.print("Air Quality: Good");
                    break;
                }
                case 2: {
                    Serial.print("Air Quality: Normal");
                    break;
                }
                case 3: {
                    Serial.print("Air Quality: Bad");
                    break;
                }
                case 4: {
                    Serial.print("Air Quality: Worst");
                    break;
                }
                default:
                    break;
            }
        }

        Serial.print(F("  Humidity: "));
        Serial.print(h);
        Serial.print(F("  Temperature: "));
        Serial.print(t);
        Serial.print(F("  Microphone Value: "));
        Serial.println(micValue);
    }
}
