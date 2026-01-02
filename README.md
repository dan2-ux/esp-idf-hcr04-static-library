This is a static library for the HC-SR04 ultrasonic sensor in the ESP-IDF environment.

You only need to declare the GPIO pins to which the TRIG and ECHO are connected, and the library will handle the rest.

To use this library, place the folder inside the components directory of your ESP-IDF project.
Then, make sure the folder name (in this case, hcr04) is included in the CMakeLists.txt file inside your main folder.
