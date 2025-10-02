# Coal-Shuttle

This is an Arduino based train shuttle controller for a model railway coal-hopper.

The direcrtion of the train is controlled by the on-off-on switch connected to the resistor divider on the Analog3 pin. The trimmers connected to the other Analog pins are used to control the Delay and Dwell for when the train is stopped. A Reed Switch is used to determine when a Coal Truck is in position under the shute. In forward mode, this will stop the train and turn on the converyor belt. It wil then wait for the bucket tip mercury switch to trigger, showing the coal has been delivered. After a period of time, the track power is enabled to move to the next truck.

The direction switch also controls the direction of two points that will send the train either through the Coal Loading chute path, or through the Coal Unloading path.

The pin assignment is:

  D2  - Track Power  
  D3  - Track Direction  
  D4  - Reed Switch (Train Detect)  
  D5  - Tilt Switch (Coal Tipper)  
  D6  - Conveyor Power  
  D7  - Spare  
  D8  - Points 1 Normal (Relay 1)  
  D9  - Points 1 Divert (Relay 2)  
  D10 - Points 2 Normal (Relay 3)  
  D11 - Points 2 Divert (Relay 4)  
  D12 - Red LED  
  D13 - Green LED  

  A0 - Train Detect Delay  
  A1 - Coal Tipper Dwell  
  A2 - Spare  
  A3 - Direction (0v - 2.5v - 5v)  
  
_It should be noted that there is a small design flaw on V1 of the board. The Relay module interface uses 5v from the external regulator. This should be from the Arduino 5v so it is common with the output pins. This is so it can drive the opto-coupler LED correctly (common 5v not ground). The external 5v <ins>must</ins> be used to drive the relays, also knows as JD Power. Ideally, the 5v lines should be through a jumper so the correct one can be selected for the interface type. This will be fixed on the next board. In the short term, the 5v from the I2C connector can be used for the relay 'IN' connections and the 5v on the module interface can be used for JD power._


## Arduino Code
The code is held under the Arduino folder. 

## Kicad Files
A Custom PCB was made for this and can be found under the Kicad folder.

![Image of the PCB Front](Images/pcb-front.png)
![Image of the PCB Rear](Images/pcb-back.png)

## Completed Boards
The image below shows a completed board. 

![Image of PCB Built](Images/pcb-built.png)
