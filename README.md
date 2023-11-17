# Fall2023ArduinoCode

In Fall 2023, we are using a commerical HAB Cutter to implement the cutdown mechanism. This is the final code that went up on the arduino. Remember that the timeout parameter must be changed from 300 (seconds) to 30 seconds in the iridium library, or else we would need to wait for way too long for another message.

This version updates data realtime to https://pac-tracker-5d702d3ce699.herokuapp.com/

This code prioritizes receiving cutdown commands over sending data back to control. This is a beta version.
