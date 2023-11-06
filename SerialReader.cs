using System.IO.Ports;
using UnityEngine;

public class SerialReader : MonoBehaviour {
    SerialPort serialPort;

    void Start() {
        serialPort = new SerialPort("COM3", 9600);
        serialPort.Open();
        serialPort.ReadTimeout = 50; 
    }


//Air Quality: Good  Humidity: 45  Temperature: 22  Microphone Value: 300

    void Update() {
        if (serialPort.IsOpen) {
            try {
                string data = serialPort.ReadLine();
                ParseSerialData(data);
            } catch (System.TimeoutException) {
            }
        }
    }

    void ParseSerialData(string data) {
    string[] parts = data.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

    if (parts.Length >= 8) {
        string airQuality = parts[2]; // "Good"
        int humidity = int.Parse(parts[4]); // 45
        int temperature = int.Parse(parts[6]); // 22
        int microphoneValue = int.Parse(parts[9]); // 300

        Debug.Log("Air Quality: " + airQuality);
        Debug.Log("Humidity: " + humidity);
        Debug.Log("Temperature: " + temperature);
        Debug.Log("Microphone Value: " + microphoneValue);
    } else {
        Debug.LogWarning("Serial data is not in the expected format.");
    }
}

    void OnDestroy() {
        if (serialPort != null && serialPort.IsOpen) {
            serialPort.Close();
        }
    }
}
