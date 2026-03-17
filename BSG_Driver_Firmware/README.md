# Starter Generator Controller Firmware

> [!WARNING]
> This firmware is still work-in-progress and meant to test the features / limits of the starter generator.
>
> It does not provide any simple or reliable control features as of right now (like e.g. with some potentiometers or CAN/UART API).

The current work-in-progress control firmware runs on an STM32F072C8 with a CAN transceiver connected to PB8/PB9 (RX/TX).

It can be controlled via the built-in USB connection of the STM32 using a serial terminal.

Currently, the firmware has the following features:
- [x] Full control over the starter generator via CAN
  - [x] RPM / Torque control mode switching
  - [x] Setting of limits (RPM, torque, min/max current, min/max voltage)
  - [x] Display and decoding of the real-time data / status messages sent by the starter generator
- [x] Automatic FBS4 bypass via XCP
- [x] Rudimentary serial control using keyboard keys (see src/main.cpp, around line 90 for the commands)
  
But be warned, the firmware contains some hacky C++ code, that simplifies the definition and usage of the dozens of CAN messages / signals that need to be sent periodically.  
(I've tried to optimize it, so that as little duplicate code as possible needs to be written in multiple places, while still keeping the runtime memory overhead low.)

## Usage
### Hardware Setup Example
```
                      CAN                        Starter           
 STM32F072        Transceiver                   Generator          
                               +12V         ┌────────────────┐ +48V
┌─────────┐      ┌───────────┐   │          │                │   │ 
│         │      │           │   ├──────────┼3 +12V         +├───┘ 
│      3V3├──────┤3V3        │   └─[E-Stop]─┤6 Crash         │     
│         │      │           │              │                │     
│      PB8├──────┤RXD    CANH├──────────────┤5 CANH          │     
│      PB9├──────┤TXD    CANL├──────────────┤4 CANL          │     
│         │      │           │              │                │     
│   GND   │      │    GND    │          ┌───┤1 SGND         -├───┐  
└────┬────┘      └─────┬─────┘          │   │                │   │  
     │                 │                │   └────────────────┘   │  
     └─────────────────┴────────────────┴────────────────────────┘  
```
Best to connect an Emergency-Stop switch between 12V and Pin 6 (crash signal)

### Building & Flashing
This project is built using PlatformIO + Arduino Framework for ease of initial development.

To compile & flash it, open this directory (BSG_Driver_Firmware) in Visual Studio Code with the PlatformIO extension installed.  
It should automatically handle installing the necessary toolchain (might take a few minutes).

Once everything is set up, you can chose the upload method by switching the PIO environment in the toolbar at the bottom and start the upload.

### Test Controls
When the STM32 is flashed and connected to CAN and USB, connect to its serial port, where you should see the debug output and can enter the following commands: 
- `r`: RPM Mode
  - Switches to RPM mode
    - You might hear the BSG inverter starting to make a high pitched noise
  - `q`/`a`: Set a RPM target
  - `e`/`d`: Set a maximum positive torque target
    - The BSG should start to spin if no significant load is connected
  - `s`/`w`: Optional: Set a minimum negative torque target
    - This sets the recuperation torque, when driven externally or braking (to the target RPM)
    - When the minimum torque gets set to a positive value, the BSG will accelerate past the RPM target (iirc)
- `t`: Torque Mode
  - Switches to torque mode
  - `e`/`d`: Set a torque target
    - You only need to change "Max Torque Request" in torque mode. It can go both negative and positive.
- `i`: Idle Mode
  - Default mode when the BSG starts, the power electronics inside the inverter are disabled
  - (I think I've noticed it applying negative torque sometimes when switching to idle while it's still running. I haven't investigated that further.)
- `␣`: Stop power immediately
  - If you want to switch back to idle quickly, simply press the space bar.
  - This will switch back to idle mode and sets all requests to 0.

### Fault State
If the BSG enters the fault state (`[190] State: 6`), simply disconnect both +12V and +48V for a few seconds. It will be operational again on the next power up.

Possible causes for a "Fault State":
- When the input voltage drops suddenly, while the BSG is pulling some power (and it's unable to react to the falling voltage fast enough).
- When the input voltage rises suddenly, while the BSG is recuperating with quite some power. 
  - It will stop recuperating if the bus voltage would exceed ~56V, but it'll put itself in the fault state afterwards.
  - If the voltage rises more slowly (e.g. lower power operation on a lab bench PSU or when caused by cable losses), it won't fault and will regulate itself to not exceed the bus voltage according to the set "Max. Charge Voltage" by reducing the recuperation torque accordingly.

