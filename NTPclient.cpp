

/**
 * @file NTPClient.h
 *
 * @brief 
 *
 *  https://github.com/0xAbby/miniNTP
 *
 * @author Abdullah Ada
 */

#include "NTPclient.h"


NTPClient::NTPClient(boost::asio::io_service& io_service,
      const std::string& host,
      const std::string& port) : io_service_(io_service)
      , udpSocket(io_service, udp::endpoint(udp::v4(), 0)) {

    udp::resolver resolver(io_service_);
    udp::resolver::query query(udp::v4(), host, port);
    udp::resolver::iterator iter = resolver.resolve(query);
    udpEndpoint = *iter;
  }


/**
 * @brief Sets up the NTP message packet, setting minimal required fields.
*/
void NTPClient::setupPacket() {
  // packet is zeroed out in NTPClient constructor.
  memset(&packet, 0, sizeof packet);

  // setting basic values
  packet.leap_version_mode = 0b00'011'011;  // 0-1: Leap Indicator
                                            // 2-4: Version number
                                            // 5-7: mode
  packet.stratum = 1; 
  
  // 47 4f 45 53 == GEOS -- Reference ID: Geostationary Orbit Environment Satellite
  packet.referenceId = 0x53454f47; 
}


/**
 * @brief Sends an NTP message packet and listens for response.
*/
void NTPClient::sendRequest() {
  // sending the NTP message packet.
  udpSocket.send_to(boost::asio::buffer(&packet, sizeof packet), udpEndpoint);

  // filling the packet with zeros again, to use it for receiving a response.
  memset(&packet, 0, sizeof packet);
  size_t reply_length = udpSocket.receive_from(boost::asio::buffer(&packet, sizeof packet), udpEndpoint);

  if ( !(reply_length > 0) ) {
    throw std::runtime_error {"No NTP message received."};
  }
}

/**
 * @brief After sendRequest() function is called, this functions prints time, 
 * taking into consideration local machine's byte order.
*/
void NTPClient::printTime() {

  if (endian::native == endian::big) {
      unixEpochSeconds = (packet.transmitTimestamp_s) - NTP_UNIX_OFFSET;
      nanoSeconds = (packet.transmitTimestamp_f) * 1000000000ULL / 0xFFFFFFFF;
  }
  else if (endian::native == endian::little) {
      unixEpochSeconds = swapBytes(packet.transmitTimestamp_s) - NTP_UNIX_OFFSET;
      nanoSeconds = swapBytes(packet.transmitTimestamp_f) * 1000000000ULL / 0xFFFFFFFF;
  } else {
      throw std::runtime_error {"Unknown machine byte order. exiting"};
  }

  seconds = nanoSeconds / 1000000000UL;

  time_t epch = (time_t)unixEpochSeconds;

  std::cout << "Unix Epoch seconds: " << unixEpochSeconds << std::endl;
  std::cout << "Nanoseconds: " << nanoSeconds;
  std::cout << " (" << seconds << " seconds)" << std::endl;

  std::cout << "\nTime is: " << ctime((const time_t*)&epch);
}


/**
 * @brief Converts a 32bit unsigned integer's byte order.
 * @param value An unsigned 32bit integer.
 * @return returns a 32 bit number after reversing byte order
*/
uint32_t NTPClient::swapBytes(uint32_t value) {
  uint32_t result = 0;
  while (value != 0) {
      result <<= 8;
      result |= (value & 0xff);
      value >>= 8;
  }
  return result;
}

/**
 * @brief NTPClient destructor.
*/
NTPClient::~NTPClient() {
    udpSocket.close();
}

/**
 * @brief Updates the system time, this function is 
 * only called if we have higher privileges (root / superuser).
*/
int NTPClient::setTime() {
  struct timespec now;

  /* Set clock to zero. */
  now.tv_sec = unixEpochSeconds;
  now.tv_nsec = nanoSeconds;

  if (clock_settime(CLOCK_REALTIME, &now) < 0 ) {
      std::cout << strerror(errno) << std::endl;
      throw std::runtime_error {"exiting"};
  }

  return 0;
}