# Welcome to HC-SR04 repository
This is a ready-to-use component with a clean API that only requires developers to define the pins. It is designed for the ESP32 to work with the HC-SR04 sensor, allowing developers to focus on integrating the features provided by the HC-SR04 into their own projects.

## HC-SR04 Ultrasonic Distance Sensor

This repository demonstrates how to interface the HC-SR04 ultrasonic distance sensor to measure distance using sound wave reflection.

The HC-SR04 is commonly used in obstacle detection, robotics, and distance measurement projects.

<img width="500" height="500" alt="image" src="https://github.com/user-attachments/assets/afafd816-f0e5-442a-909f-d349b9198721" />

## Sensor Overview
- Measurement Range:	2 cm – 400 cm
- Accuracy:	±3 mm
- Operating Voltage:	5 V
- Interface:	Trigger / Echo
- Working Frequency:	40 kHz
## How It Works

The HC-SR04 measures distance using ultrasonic sound waves:

1. The microcontroller sends a 10 µs HIGH pulse to the TRIG pin

2. The sensor emits an ultrasonic burst (40 kHz)

3. The sound wave reflects off an object

4. The ECHO pin stays HIGH for the duration of the round trip

5. The pulse width is measured to calculate distance

## GPIO Configuration

- TRIG pin → Output

- ECHO pin → Input
## Distance Calculation
Distance is calculated using the pulse duration:
<pre>
  Distance (cm) = (Echo time in µs × Speed of Sound) / 2
  Distance (cm) ≈ Echo time / 58
</pre>
The division by 2 accounts for the sound traveling to the object and back.
