This is the github repo created for the submission of my smart-home-and-wearables-project-contest-2025 by circuit digest, sponsered by digikey.

Project Concept:
I propose "Sentio," a project that reimagines the smart home assistant as an interactive "digital pet" rather than a lifeless tool. While most smart speakers are static cylinders, Sentio is an emotive, privacy-focused robot that is interactive and reacts to touch. Crucially, it serves as a central "Guardian" for the home: it integrates a custom Smart Video Doorbell for secure local streaming and supports Emergency Pagers for elderly care. This combines the engagement of a companion robot with the utility of a secure, offline home automation hub.


Hardware Architecture:
The brain of the system is the ESP32-S3-BOX-3, selected for its offline AI capabilities and built-in peripherals.

Kinetic Movement: The device will be mounted on a 3D-printed turntable driven by an MG90/SG90 Micro Servo. This allows the "head" to pan left/right to look at the user or shake its head.
Sensors:
Touch: One TTP223 capacitive sensor embedded in the casing and the touch sensitive home button allows the user to "pet" the device.
Motion: Using an external mpu6050 accelerometer, Sentio will detect when it is physically picked up.
Vision & RF: A separate ESP32-CAM module will act as the wireless doorbell, and a 433MHz RF Receiver connected to the Box-3’s GPIO dock will listen for signals from wireless panic buttons (pagers).


Software Approach:
I will build the firmware using ESPHome for seamless integration with Home Assistant.

UI & Graphics: Since standard OLED libraries don't work on the Box-3's color LCD, I will use LVGL (Light and Versatile Graphics Library) to render procedural eye animations. A complex state machine will govern the emotions:
Interactive: "Happy" (wiggles) when petted; "Surprised" if touched while sleeping; "Dizzy" (spirals) if the IMU detects the device is being carried/shaken.
Passive: "Sleeping" automatically at night or after inactivity; "Hungry/Sad" randomly to demand attention;
Video Integration: The doorbell feed will be an on-demand MJPEG stream over UDP, ensuring low latency on the Box-3’s display without external cloud servers.


Development Methodology:
Phase 1 (The Personality): Develop the LVGL eye animations and state machine. Integrate the TTP223 sensor and the external IMU to perfect the "pet" behavior (sleeping, happy, dizzy states).
Phase 2 (The Mechanics & Safety): Design the 3D-printed servo base and integrate the 433MHz RF receiver for the elderly pagers.
Phase 3 (The Vision): Build and code the external ESP32-CAM doorbell node and optimize the Wi-Fi video streaming to the main unit.


Why This Board?
The ESP32-S3-BOX-3 is the perfect candidate because it consolidates a display, microphones, and a speaker into one package. The expansion dock provides the necessary I/O for the servo and RF receiver, making it possible to build a complex robot without a mess of external wires.
