
# miniNTP


The Network Time Protocol (NTP) is a widely-used protocol that operates on port 123 via the User Datagram Protocol (UDP). Its purpose is to provide an accurate and reliable time synchronization service to connected clients. The protocol enables clients to send a request for the current time to an NTP server, which, in turn, responds with a packet containing time-related data.

![NTP](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c9/Network_Time_Protocol_servers_and_clients.svg/859px-Network_Time_Protocol_servers_and_clients.svg.png)

miniNTP is a minimal implementation, written in C++, miniNTP sends a crafted NTP message packet to an NTP server on port 123. When the server responds with an NTP packet miniNTP uses it to print the current time. The networking operations of miniNTP are handled via libasio, a widely-used C++ library for asynchronous I/O operations (http://think-async.com/Asio/).

# Compiling

With the Libboost installed and g++ compiler, you can compile the program and run it:
```
$ git clone https://github.com/0xAbby/miniNTP
$ cd miniNTP
$ make
...

$ ./miniNTP
Unix Epoch seconds: 1704991065
Nanoseconds: 7.89241e+08 (0.789241 seconds)

Time is: Thu Jan 11 15:38:40 2024
Cant change system time without higher privliges.

```

The program won't set the time if its ran as a normal user.

Example of the networking traffic sent and received

The sent NTP message

![ntp-send](https://github.com/0xAbby/miniNTP/assets/147592615/2f383d8f-ceb2-424f-ae64-9a16c4bd3449)



The response NTP message

![ntp-receive](https://github.com/0xAbby/miniNTP/assets/147592615/fa03662b-1f93-4f17-aec3-eb606c049c6e)


# NOTES

The NTP packet contains specific fields that help communicate between the client and NTP server how to best figure out the most accurate time

![image](https://github.com/0xAbby/miniNTP/assets/147592615/16cd595f-1b71-4559-a714-8a206f5fb5e2)

(minimal fields for NTPv3, there are more fields for NTPv4 mentioned in RFC5905)

These fields are:

- LI: Leap indicator (2 bits), defines whether the current day would have a leap second added.

- VN: NTP version mode (3 bits).

- Mode: (3 bits), could indicates one of the following modes.

  - 0: Reserved

  - 1: Symmetric active

  - 2: Symmetric passive

  - 3: Client

  - 4: Server

  - 5: Broadcast

  - 6: NTP control message

  - 7: Reserved for private use

- Stratum: (8 bits) level of the time source

  - 0: Unspecified or invalid

  - 1: Primary server

  - 2–15: Secondary server

  - 16: Unsynchronized

- Poll: (8 bits) A signed integer representing the maximum interval between successive messages.

- Precision: (8 bits) A signed integer representing precision of the clock.

- Root delay: (32 bits) Sum of roundtrip delay to the reference clock.

- Reference ID: (32 bits) code identifying the source clock, 4-character ascii string refereed to as 'kiss code', could be either one of:

  - GOES: Geosynchronous Orbit Environment Satellite

  - GPS: Global Position System

  - GAL: Galileo Positioning System

  - PPS: Generic pulse-per-second

  - IRIG: Inter-Range Instrumentation Group

  - More are listed in RFC5905 [see references at the bottom]

- Reference Timestamp: Time when the system clock was last set or corrected, in NTP timestamp format.

- Origin Timestamp: Time at the client when the request departed for the server, in NTP timestamp format.

- Receive Timestamp: Time at the server when the request arrived from the client, in NTP timestamp format.

- Transmit Timestamp: Time at the server when the response left for the client, in NTP timestamp format.

- The NTP protocol utilizes four timestamps in its packet message  (shown in the screenshots) to determine the accurate time to be configured on a computer. 


The timestamps include the **Reference Timestamp**, which denotes the time when the system clock was last set or corrected in NTP timestamp format, the **Origin Timestamp**, 
which specifies the time at the client when the request left for the server in NTP timestamp format.

The **Receive Timestamp**, which identifies the time at the server when the request arrived from the client in NTP timestamp format, and the **Transmit Timestamp**, 
which represents the time at the server when the response left for the client in NTP timestamp format. 


# Sources and References

- RFC5905 Internet Engineering Task Forc - Figure 8 UDP packet  https://datatracker.ietf.org/doc/html/rfc5905#page-19
- Protocol Basics – The Network Time Protocol, Geoff Huston APNIC lab (2014 march 10), https://labs.apnic.net/index.php/2014/03/10/protocol-basics-the-network-time-protocol/



