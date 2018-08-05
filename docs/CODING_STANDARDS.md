# Coding Standards for OpenCorsairLink
Many of these Coding Standards are not being followed currently. That is my (audiohacked's) fault. Eventually, OpenCorsairLink will conform to these Coding Standards. I, Audiohacked, reserve the right to change the standard after much deliberation.

## Basic Formatting
### Tabs
Tabs should be soft-tabs and equivalent to 4 spaces.

### Line Wrap
Line wrap limit should be at 100 columns, its ok to go over but no more than 120 columns total.

## Code Style
### Functions
The return type should be of a type similar to `uint16_t` and be used strictly for error code returning. And variables given or that need to be returned should be put in the parameter list of the function. Function names should follow naming scheme as seen in other files within this project.

### Other formatting
Curly Braces should fall on their own line and line up with their proper indentation. For and If statements must be accompanied by curly braces; no one-liner statements!
