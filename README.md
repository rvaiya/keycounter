# What

A tiny evdev keycounter which counts keystrokes and prints usage statistics to stdout upon termination. Works across all the things (ttys, X, wayland, etc) by virtue of being evdev based. By the same token it wont capture changes higher up in the input stack (e.g xcape) since each key maps to its physical correlate. Useful for layout optimization.

# Usage

```
# In a dedicated terminal/screen session:

> make
> sudo ./counter > keys

# Induce RSI
# Terminate with C-c

> cat keys
```
