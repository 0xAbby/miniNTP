/**
 * @file miniNTP.cpp
 *
 * @brief A small network time protocol client, that can communicate with NTP servers
 * to request current time.
 *
 *  https://github.com/0xAbby/miniNTP
 *
 * @author Abdullah Ada
 */
#include "NTPclient.h"

/**
 * List of NTP servers:
 *  time.cloudflare.com
 *  ptbtime1.ptb.de
 *  ptbtime2.ptb.de
 *  ptbtime3.ptb.de
 *  nts1.time.nl
 *  nts.netnod.se
 *  nts.ntp.se
 *  nts.sth1.ntp.se
 *  nts.sth2.ntp.se
 */

int main(int argc, char* argv[]) {
  boost::asio::io_service io_service;
  try {
    NTPClient ntp(io_service, "nts1.time.nl", "ntp");

    ntp.setupPacket();
    ntp.sendRequest();
    ntp.printTime();
    
    // checking for ability to change time
    if (geteuid() == 0) {
        ntp.setTime();
    } else {
      std::cout << "Cant change system time without higher privliges.\n";
    }

  } catch(std::exception& e) {
    std::cerr << "Error: \n" << e.what() << '\n';
   }

  return 0;
} 