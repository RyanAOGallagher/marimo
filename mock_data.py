import serial
import time
import random

# Replace 'COM3' with the serial port you want to use
serial_port = 'COM3'
baud_rate = 9600

ser = serial.Serial(serial_port, baud_rate)

def generate_mock_data():
    air_quality = random.choice(['Good', 'Bad', 'Normal', 'Worst'])
    humidity = random.randint(20, 60) 
    temperature = random.randint(15, 30) 
    microphone_value = random.randint(100, 400)  

    data_str = f"Air Quality: {air_quality}  Humidity: {humidity}  Temperature: {temperature}  Microphone Value: {microphone_value}\n"
    
    return data_str

try:
    while True:
        mock_data = generate_mock_data()
        ser.write(mock_data.encode())
        time.sleep(2)

except KeyboardInterrupt:
    print("Stopped by User")

except serial.SerialException as e:
    print(f"Serial error: {e}")

finally:
    ser.close()
