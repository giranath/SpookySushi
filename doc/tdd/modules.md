# Modules
The engine is divised in multiple modules.

A module is defined by a public interface and a private implementation.
External modules can only access the public interface but a module can access it's internal private implementation.

## Implementation details
Each module is contained in a single folder.
A module folder must contain 3 elements:
* A public folder
* A private folder
* A CMakeLists.txt file describing it

For the first iteration, a module is statically linked to the engine.

## Ideas
It would be interesting to allow modules to be dynamicaly linked to engine.
This feature could allow hot reloading and could favorise the developement of multiple "plugins".