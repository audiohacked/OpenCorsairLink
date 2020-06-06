Use Wireshark with filters:

Packets from Host to Device
```
(usb.src contains "host" && usb.dst contains "1.2.0")
```

Packets from Device to Host
```
(usb.src contains "1.2.1" && usb.dst contains "host")
```

Protocol Packets:
```
(usb.src contains "1.8.1") or (usb.dst contains "1.8.0")
```

Protocol Packet Data Fragments:
```
(usb.data_fragment[0] == 3f && usb.data_fragment[2] == 14)
```

Curve 1
25% = 0x19
30% = 0x1E
35% = 0x23
40% = 0x28
45% = 0x2D
50% = 0x32
55% = 0x37

20C = 0x14
30C = 0x1E
40C = 0x28
50C = 0x32
60C = 0x3C
70C = 0x46
80C = 0x50

Curve 2
35% = 0x23
40% = 0x28
45% = 0x2D
50% = 0x32
55% = 0x37
60% = 0x3C
65% = 0x41

20C = 0x14
30C = 0x1E
40C = 0x28
50C = 0x32
60C = 0x3C
70C = 0x46
80C = 0x50
