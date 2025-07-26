# PFD_FSDIY_RSG
Code for the Arduino Mega to make it a work-alike of an old RSG PFD display, but using the FSDIY hardware and pin assignments. 

Here' a link to the hardware: https://flightsimdiy.com/product/fsd-g1000-nxi/

You'll want to install the RSG binary from here: https://help.realsimgear.com/en/articles/7971505-downloads-for-microsoft-flight-simulator-2020-and-2024
(Or go to the realsimgear site and search for the current downloads).

Modify the CommandMapping.ini to change the commands associated with the buttons if you'd like. Should work out-of-box.

You can test your hardware with the RSG interface, or just open up a serial terminal (PuTTY or the VS Code serial monitor both work well) and run through all the controls.

The current code supports repeating move on the pan/range joystick. It does not support long-press of the CLR button. I'll add that.
