# CircularBuffer
An implementation of a circular buffer. Test cases through Google unit tests.
Uses table for looking up index to memory indexed locations and stack for available memory locations.
Invalidation would occur during resizing (only done when insert or add is called and there is not enough capacity in buffer)
