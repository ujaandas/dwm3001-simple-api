# Simplified DWM3001 API

## Overview
The existing API for the DWM3001(C/CDK) did not meet our demands in terms of system agnosticity, portability, and was mind-bogglingly obfuscated. This is a ground-up rewrite of the API in C with much clearer parallels to Qorvo's documentation and UCI spec. Any information not directly available from the dev documentation were parsed and translated from the Python API. 

For our use case, we only required a single controller and controlee, so the API in its current state is written to accomodate that and nothing else. In the future, I may consider adding the extensibility, as well as other helper functions beyond those pertaining directly to the ranging mechanism and flow. 

I've tried to keep everything as clear as possible (in terms of data sent/received).

