# High Level Requirements for DAQ protocol 

## What is needed 

* A way to tell the device which signal to stream
* A way to tell the device at which rate to send them
* A Data packet to send the signals
* A simple framing to detect start and stop
* A CRC or checksum to detect robust/reliable transmission


## Suggested packet format
Start Byte | Packet Type | Payload Length | Data | CRC | End Byte

## Design on host side (pc)
With respect to UART a simple method to handle the fragmented data is to have 
a short of time of delay between data transmission. So we are loading data to
our buffer until a timeout occured (e.g we have not received uart frames for more than 5 ms)