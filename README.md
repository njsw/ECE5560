# Lab - General Purpose Input / Output - I/O Ports
<br>

## Overview
In this lab you will become familiar with the I/O port features of the AVR.  Throughout this process you will apply C++ to interface with the processor via registers.  Upon completion of this lab, students will be able to:

  * List all the ports of the AVR
  * Describe the dual role of the AVR pins
  * Write code to use the ports for input and output
  * Code I/O bit-manipulation programs for the AVR
  * Explain bit-addressability of AVR ports
  * Become familiar with C++ bitwise operators

To achieve the learning objectives, students are expected to complete the following:

  * attend lecture
  * attend lab
<br>
<br>

## I/O Ports Overview
The Arduino Uno board has 3 ports: *Port C* contains the analog pins A0-A5, *Port D* contains digital pins 0-7, and *Port B* contains digital pins 8-13. For this lab, we will focus on the digital pins in Ports B & D. Digital pins can be configured as either input or output. We have already covered abstractions provided by Arduino such as digitalWrite(), digitalRead(), and pinMode(). This lab will provide the background necessary to write your own version of each of these functions by directly setting bits within specific registers. The figure below shows an overview of the ATmega328P microcontroller.

![io-ports](https://github.com/nlowing/ECE5560/blob/master/io-ports-block-diagram.PNG?raw=true)

The Ports are shown in red in the diagram. Let's now discuss some of the circuitry within the Ports and the registers that will be necessary for controlling input and output.
<br>
<br>

## I/O Registers
There are 3 main registers that we will be concerned with:

  * PORTx: the port register
  * DDRx: the data direction register
  * PINx: the pin register

The details of these registers can be found in the data sheet. The figure below shows the B registers (PORTB, DDRB, & PINB), but the D registers (PORTD, DDRD, & PIND) follow a similar format. Each of the bits within PORTx, DDRx, and PINx correspond to a pin on the Arduino board (i.e. PORTD5, DDD5, & PIND5 sets digital pin 5). Be careful when using Port B registers! The bits start from zero, but the pin numbers do not (i.e. digital pin 10 corresponds to bit 2 in the register). Your generic functions will need to take this mapping issue into account.

<img src="https://github.com/nlowing/ECE5560/blob/master/portb.png?raw=true" width="700px" height="400px">

Let's take a look at the circuitry of a Port and examine how setting bit values within the registers allows for input and output at the digital pins. We'll begin with reading a pin.
<br>
<br>

## Reading a Pin
The figure below shows a schematic for the microcontroller Ports with some comments on reading from a pin.

![pin-read](https://github.com/nlowing/ECE5560/blob/master/reading-pin-5.PNG?raw=true)

Reading or writing to a pin requires a specific state of the internal pull-up resistor. In order to read from a pin the internal pull-up resistor has to be enabled. In the figure you can see that when the PORTx register is set to 1 and the DDRx register is set to 0, the result of the AND-NAND combined logic gate is 1. This 1 is then flipped by the inverter, so the pMOS transistor receives a 0. A pMOS transistor acts like a switch: a value of 0 closes the switch, while a value of 1 opens the switch. When the switch is closed (0), the pull-up resistor is enabled. The figure below shows the pMOS states.

![pmos](https://github.com/nlowing/ECE5560/blob/master/pmos-transistor.PNG?raw=true)

Another feature of the circuitry that is important is the tri-state buffer. The DDRx register controls the tri-buffer. A value of 0 disables the tri-buffer while a value of 1 enables the tri-buffer. Since we are reading from a pin, we need to prevent signal from traveling out to the pin. Thus, we disable the tri-buffer.

**Summary**: we wanted a configuration to read input signal from a pin. We accomplished this by disabling the tri-buffer and by enabling the pull-up resistor through the pMOS transistor (DDRx = 0; PORTx = 1).
<br>
<br>

## Writing a Pin
The figure below shows the same schematic for the microcontroller Ports with some comments on writing to a pin.

![pin-write](https://github.com/nlowing/ECE5560/blob/master/writing-pin-1.PNG?raw=true)

In order to write to a pin the internal pull-up resistor has to be disabled. In the figure you can see that the DDRx register is set to 1, which both enables the tri-buffer and disables the internal pull-up resistor. This allows signal to leave from the PORTx register and travel to the pin. The PORTx register can be set to a value of 0 or 1. This value is what gets transmitted to the pin as an output signal.

**Summary**: we wanted a configuration to write output to a pin. We accomplished this by enabling the tri-buffer and by disabling the pull-up resistor through the pMOS transistor (DDRx = 1; PORTx = 0 or 1).
<br>
<br>

## PINx Register
The only register that we have not yet discussed is PINx, which is used for identifying the state of a pin. A value of 0 within PINx means the pin is in a LOW state and a value of 1 means the pin is in a HIGH state. Determining what each of these states imply requires some cautious examination. Remember that there is an internal pull-up resistor configured. This means that by default when no signal is being read, the logical state of the system is "pulled-up" to HIGH. Therefore, when PINx returns a LOW state this means that a change has been detected in the logical state. On the contrary, a HIGH state in PINx corresponds to no change. This is important when using PINx to assess if an action has been performed at a pin (such as a button press). Hence, you would want to check if the system has entered a LOW state if waiting for a button press.

**Summary**: we wanted to detect the state of a pin. We accomplished this by reading the PINx register, where 0 indicates a LOW logical state and 1 indicates a HIGH logical state.
<br>
<br>

## Some Useful Code
Before starting the lab task, let's look at some simple lines of code for writing and reading from registers using bitwise operations and logical operators. Hopefully these operations are already familiar. Consider the statements below.

```c++
PINB & (1 << 5)
```

This line uses the PINx register for Port B (PINB). The (1 << 5) is the bitwise operation to shift a 1 to the left by 5 bits. The AND operation checks if a 1 is present in the 5th bit of the PINB register. If we evaluate the meaning of this line, this statement checks the state of digital pin 13 (recall that a Port B digital pin value is different than the value of its bit position). This line is useful for establishing the state of a pin.

```c++
PORTD = PORTD & ~(1 << 3)
DDRD = DDRD | (1 << 3)
```

These lines use the PORTx and DDRx registers for Port D (PORTD, DDRD). The first line uses an AND operator to write a 0 to the 3rd register bit while keeping all other register bits the same. The second line uses an OR operator to write a 1 to the 3rd register bit. If we evaluate the meaning of each line, the first line is setting the state of digital pin 3 to LOW. The second line is setting the data direction of digital pin 3 to OUTPUT. Statements like these are useful for writing bits within registers.

```c++
if (pin_number > 7) //if pin is in Port B
{
  pin_number = pin_number - 8; //zero pin_number to get bit position in register
}
else //pin_number <= 7 (pin is in Port D)
{
  //do not need to modify the pin_number
}
```
or an optional, more compressed form with conditional operators
```c++
pin_number = (pin_number > 7) ? (pin_number - 8) : (pin_number)
```

The if-else structure will prove useful in this lab task. Port D digital pin values are the same as their bit positions; however, Port B digital pin values are **not** the same as their bit positions! This if-else statement will be necessary to evaluate the `pin_number` value passed to your generic functions. This mapping is necessary to ensure the proper bit is written to in your registers.
<br>
<br>

## Lab Programming Task
Write your own digitalWriteGeneric(), digitalReadGeneric(), and pinModeGeneric() functions to replace Arduino's digitalWrite(), digitalRead(), and pinMode() functions.

  * `void digitalWriteGeneric(uint8_t pin_number, int pin_setting)`
  * `int digitalReadGeneric(uint8_t pin_number)`
  * `void pinModeGeneric(uint8_t pin_number, int data_direction)`

Then build a system that contains one LED and one push-button within Port D (pins 0-7). Using your generic functions, write some code to turn on/off the LED when the button is pushed. Repeat the lab task using two pins within Port B (pins 8-13). [NOTE: you should not be modifying your generic functions for the two lab tasks. The functions should be able to handle both Ports B & D!] Once you are done, show the system to your TA for credit.
<br>
<br>

## References
Below are some additional references to explore if needed:
  * ATmega328P Data Sheet: http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf
  * Bitwise Operations: http://www.cprogramming.com/tutorial/bitwise_operators.html
  * Conditional Operators: http://www.cplusplus.com/forum/articles/14631/
