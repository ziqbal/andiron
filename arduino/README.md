Andiron
=======

Arduino

Listens on serial port

3 byte + 1 byte checksum protocol

Currently checksum is simple summation of first 3 bytes

When checksum is detected it triggers actions

Current actions

1 1 X -> DigitalOutput HIGH PinX ( Set Pin X as output and set to HIGH )

1 0 X -> DigitalOutput HIGH PinX ( Set Pin X as output and set to LOW )
