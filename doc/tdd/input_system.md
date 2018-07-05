#Low level Input System

##Requirements
1. Handles 3 basic input methods
    1. Action input
    2. State input [true, false]
    3. Axis input [-1.0, 1.0]
2. Input mapping can be modified at runtime
3. Abstract platform

##Layers
The system will be divided in 3 layers
1. System layer: raw os events are converted to sushi events
2. Dispatching layer: map sushi events to high level handler
3. High level handlers layer: reacts to event

###System layer
Polls SDL to recuperate each events. Then, the event is converted to
a sushi InputEvent of the good type

### Dispatching layer
Manages a sequence of InputEvent to call the good input handler.
This is where InputEvents are converted to ActionInputs, StateInputs
and AxisInputs

### High level handlers layer
Code that reacts to Inputs from the dispatching layer.
This is a domain specific layer.