# Arduino-RC-car

## Project Overview:
This project was carried out as one of my tasks while working as an Intern Prototyping Engineer for Tricky and Big Ideas, hence I was not able to upload the source code while it was being shown as a prototype. However, now that I am no longer an intern both Tricky and Big Ideas have said I am able to log these projects on my Git.

## What is it?
The code relates to a system I created whereby the following devices/ components were involved:
  - Arduino Nano
  - DfPlayer
  - PN532 reader
  - Mini speaker
  - 16GB MicroSD
  - RC car
  - Portable battery

The objective what to create a simple RFID style reader that would produce a sound when it scanned a tag. When mounted to the front of an RC car it would produce a beep sound when the car drove over a "checkpoint" (tag).

During this project there was a concern of the weight an RC car would be able to manage and still perform with. This was accounted for with the parts list above. 

#### * the whole setup, excluding the RC car, weighed around 180 grams with the portable battery weighing about 120 grams. * 

## Code Explaination:
The code is simple enough, initialising each component and setting parameters like the downtime between successful tag scans.

The common issue was what the wiring would be affected by the vibrations of the RC car, causing the I2C bus of the PN532 to disconnect in which the Arduino would struggle to reinitialise the PN532. Therefore, the code features a number of edge-case handling to ensure that if either the PN532 or the DFPlayer were to disconnect they would be reconnected without the whole system needing rebooting.
