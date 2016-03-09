Energy Meter
============

This project aims to measure the AC in kWh (kilowatt-hour) on a single, two or three-phase power supply for home use (less than 100A), using the Arduino Uno platform and some cheap components.

The voltage is measured using a 200mA transformer and a resistive voltage divider.

The current is measured using a [non-invasive sensor](https://store.arduino.cc/product/E000019).

The power factor is measured in software.

The output can be shown in a LCD display or sent to a server on internet. The possibilities are endless.

How it works
------------

The input signal is being constantly read by Arduino using the Free Running Mode. It Is necessary two readings for each phase (voltage and current).
Although of to be possible a higher sample rate using analogRead, the interrupt-driven is preferable because the measurements are constant. The output information is secondary and can be delayed without problems.
The ATMega328P's Free Running Mode can be faster than analogRead for a single port measurement, but it is definitively slower for multiple alternating measures, because is necessary to throw away the first measure after each the port changing (there is only one ADC, multiplexed among six ports).

Links
------------

https://en.wikipedia.org/wiki/Electricity_meter

https://en.wikipedia.org/wiki/Kilowatt_hour

https://en.wikipedia.org/wiki/Power_factor

![latest photo](https://raw.githubusercontent.com/oxesoft/energymeter/master/2016-03-08-measuring-voltage.jpg)

![plotter serial](https://raw.githubusercontent.com/oxesoft/energymeter/master/2016-03-08-plotter-serial.png)
