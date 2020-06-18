# Home-Automation-System
### **Arduino-based Smart home**

**Arduino 1**
<p align="center">
  <img src="Block diagrams/Arduino1.jpg">
</p>

1. Gate Access only using Smart Card (RFID)
2. Automatic closing of Gate using Ultrasonic sensor
3. Gardening System

Gate access: Gate movement is controlled via DC motor and rack-and-pinion gear system. For To enable clockwise and anticlockwise movement, we used an H-bridge. The RFID reader has stored the authorized access ID. When it reads the card/tag presented, it compares the ID read with the authorized access ID. If the ID matches, the gate opens. If it does not match, access is denied .i.e. the gate remains closed. After the gate has been opened, the ultrasonic sensor continuously monitors movement in the system. If there is an obstacle car/consumer), the gate remains open. If no obstacle is in the path of the gate, the gate is closed.

Gardening system: The soil moisture sensor measures moisture content present in the soil. The pump supplies water to the garden when the moisture content in the soil is low. Pump working is controlled by H-bridge.

**Arduino 2**
<p align="center">
  <img src="Block diagrams/Arduino2.jpg">
</p>

1. Main Door access using Key entered in a Keypad
2. Burglar Alarm
3. Smoke Detector

Door access: When the door is closed, there is a closed connection made. When the door is opened, the connection is broken and the user has ten seconds to enter the correct code into the keypad in the house. If an incorrect code is entered, or no code is entered, the buzzer is rung and an alert (a call, and a text message) is sent to the consumer using the GSM module.

Smoke alarm: The smoke detector MQ7 continuously monitors the kitchen for presence of carbon monoxide (CO), which is an indication of smoke. If smoke is detected, the buzzer is rung and an alert (a call, and a text message) is sent to the consumer using the GSM module.

**Arduino 3 and 4**
<p align="center">
  <img src="Block diagrams/Arduino3&4.jpg">
</p>

1. Bluetooth-controlled Lights and Fans enabling Manual as well as Automatic Control
2. Temperature-Controlled Fans
3. Light intensity-Controlled Lights

In the kitchen and bedroom, there are lights and fans. In the hall, there are lights. Using the Bluetooth module, we can control these appliances- individually, room-wise, appliance-wise. The lights used are bar LEDs – they have levels that can be controlled too. There is also a mains option to control all appliances at once. Apart from manual control, there is an option to let it be controlled automatically- lights by a light dependent resistor (LDR) and fans by Digital humidity temperature sensor (DHT). The main Arduino UNO is connected to the Bluetooth module and controls the bedroom. It sends Bluetooth commands about control of kitchen and halls to secondary Arduino UNO. The secondary Arduino UNO controls the kitchen and the hall.

