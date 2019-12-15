# BMW e28 - custom seat control

Problem: Front seats have 5 motos each, currently without control panel.
Goal: Add control module with user-friendly UI and commutation block.
On the front panel add OLED-display and encoder for user interactions.
Also, a good point to use ESP-32 as MCU, because it has Bluetooth and wifi on board. It allows adding some functionality in the future. 
MCU connects to every seat over UART, or I2C (todo decision).
On every seat will be relay block, fuses, and UART/I2C receiver like Arduino mini/micro, or something else. 

## UI
### Navigation with encoder
### The menu on display.

1. Select seat
    1. Left
    2. Right
    3. Options

2. Select motor
    0. Back
    1. Motor 1
    2. Motor 2
    3. Motor 3
    4. Motor 4
    5. Motor 5

3. Tune position
    1. On every rotation-step add 0.5s to motor execution. The direction depends on the step direction.
    2. On single counter-step or button - stop motor execution
    3. On button go to the previous menu 

4. Options
    1. Connectivity
        0. Off
        1. Wifi
        2. Bluetooth
    2. Idle-screen
        0. Off
        1. Seat-selection
        2. Time
        3. Time + voltage
        4. Other...