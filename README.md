# Avaons
To use the example script try something like

    avaons ExampleScript tcp://*:5024 tcp://*:5025

This will listen at localhost 5024 and broadcast to 5025.

## Writing scripts
A script must have a function called 'RejectEvent', that takes a single argument and returns True or False (bool). 
