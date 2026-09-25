# Lab 5 – Adjustable-Rate Counter Using the ARM Private Timer

## Goal

In this laboratory, you will implement a **4-bit binary counter** displayed on the Zybo LEDs.

The counting speed is controlled by the four switches.

The main purpose of the laboratory is to learn how to use the **ARM Cortex-A9 private timer** instead of a software delay.

---

## System Behavior

The application works as follows:

```text
Switches
   |
   v
Select time interval
   |
   v
ARM private timer
   |
timer expires
   |
   v
counter = counter + 1
   |
   v
LEDs
```

The switch value determines how often the counter is incremented.

| Switch value | Counter update interval |
|-------------:|------------------------:|
| 1 | approximately 0.1 s |
| 2 | approximately 0.2 s |
| 5 | approximately 0.5 s |
| 10 | approximately 1.0 s |
| 15 | approximately 1.5 s |

Therefore:

- a **smaller switch value** produces faster counting;
- a **larger switch value** produces slower counting.

Use switch values from **1 to 15**.

---

## Using the Private Timer

The ARM Cortex-A9 contains a hardware timer called the **private timer**.  
The application controls it through the `XScuTimer` driver.

The basic sequence is:

```text
Initialize timer
      |
      v
Load timer value
      |
      v
Enable auto-reload
      |
      v
Start timer
      |
      v
Check expiration
      |
      v
Update LEDs
```

The timer is used in **polling mode**. No interrupt is required in this exercise.

The program repeatedly checks whether the timer has expired:

```c
XScuTimer_IsExpired(&Timer)
```

When the timer expires, the program clears the expiration status, writes the current counter value to the LED peripheral, and increments the counter.

---

## Setting the Timer Period

With the prescaler set to zero, the private timer runs at half of the CPU clock frequency.

A time interval of approximately 0.1 seconds can be defined as:

```c
#define ONE_TENTH \
    (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 20U)
```

The current switch value multiplies this basic interval:

```c
XScuTimer_LoadTimer(&Timer, ONE_TENTH * switch_value);
```

For example:

```text
switch_value = 1  -> approximately 0.1 s
switch_value = 5  -> approximately 0.5 s
switch_value = 10 -> approximately 1.0 s
```

When the switch setting changes, reload the timer with the new value and restart the visible count from zero.

---

## Main Program

The main loop can be understood as:

```c
while (1) {

    read switches;

    if (switch value changed) {
        update timer period;
        counter = 0;
    }

    if (timer expired) {
        clear timer status;
        display counter on LEDs;
        counter++;
    }

    if (button pressed) {
        stop timer;
        exit program;
    }
}
```

The four LEDs display the four least significant bits of the counter:

```text
0000
0001
0010
0011
0100
...
1111
0000
...
```

After `1111`, the visible 4-bit pattern starts again from `0000`.

---

## Experiment

1. Start the application with the switches set to `0001`.

   Observe the binary counter on the LEDs.

2. Change the switches to `0101` (`5`).

   The counter should now advance approximately once every 0.5 seconds.

3. Try several switch values and observe how the counting speed changes.

4. Press any push button.

   The timer is stopped and the application terminates.

---

## Key Idea

The important concept in this laboratory is not the binary counter itself, but **how the timing is generated**.

A software delay makes the processor wait before continuing.  
The private timer is a dedicated hardware component that counts independently while the processor can execute program code.

Conceptually:

```text
Software delay                    Hardware timer

CPU waits                         Timer counts
   |                                  |
   v                                  v
delay finished                    expiration flag
   |                                  |
   v                                  v
update LEDs                       program detects expiration
                                      |
                                      v
                                  update LEDs
```

The complete idea of the laboratory can be summarized in one line:

```text
switches -> timer period -> timer expiration -> counter -> LEDs
```

The application therefore demonstrates how a hardware timer can provide controlled and repeatable timing for an embedded-software task without changing the FPGA hardware design.
