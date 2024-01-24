
/**
 * @file NTPClient.h
 *
 * @brief 
 *
 *  https://github.com/0xAbby/miniNTP
 *
 * @author Abdullah Ada
 */

#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <time.h>

using boost::asio::ip::udp;

enum class endian
{
  little = __ORDER_LITTLE_ENDIAN__,
  big    = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
};

class NTPClient {
public:
  NTPClient(boost::asio::io_service& io_service, const std::string& host,
      const std::string& port);

  void setupPacket();
  void sendRequest();
  void printTime();
  uint32_t swapBytes(uint32_t value);
  int setTime();

  ~NTPClient();

private:
  struct NtpMessage {
      uint8_t leap_version_mode;
      uint8_t stratum;
      uint8_t poll;
      uint8_t precision;
      uint32_t rootDelay;
      uint32_t rootDispersion;
      uint32_t referenceId;
      uint32_t referenceTimestamp_s;
      uint32_t referenceTimestamp_f;
      uint32_t originTimestamp_s;
      uint32_t originTimestamp_f;
      uint32_t receiveTimestamp_s;
      uint32_t receiveTimestamp_f;
      uint32_t transmitTimestamp_s;
      uint32_t transmitTimestamp_f;

  } NtpMessage;

  boost::asio::io_service& io_service_;
  udp::socket udpSocket;
  udp::endpoint udpEndpoint;


  static constexpr uint64_t NTP_UNIX_OFFSET = 2208988800UL;
  uint64_t unixEpochSeconds;
  double nanoSeconds;
  double seconds;

  struct NtpMessage packet;
};

#endif