/*
# Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com)
# Copyright (c) 2015/2016 Fabrizio Di Vittorio.
# All rights reserved.

# GNU GPL LICENSE
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; latest version thereof,
# available at: <http://www.gnu.org/licenses/gpl.txt>.
#
# This module is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this module; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*/

#ifndef _FDVNETWORK_H_
#define _FDVNETWORK_H_

#include "fdv.h"

extern "C" {
#include "lwip/netdb.h"
#include "lwip/raw.h"
}

namespace fdv {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// IPAddress

struct IPAddress {

  uint8_t address[4];

  // a wrapper for IPAddress.get_str()
  struct IPAddressStr {
    char address[16];

    operator char *() { return address; }
  };

  IPAddress() {
    address[0] = 0;
    address[1] = 0;
    address[2] = 0;
    address[3] = 0;
  }

  IPAddress(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4) {
    address[0] = v1;
    address[1] = v2;
    address[2] = v3;
    address[3] = v4;
  }

  IPAddress(IPAddress const &c) { *this = c; }

  // empty string generates "0.0.0.0"
  IPAddress(char const *str) { *this = str; }

  IPAddress(in_addr inaddr) { *this = inaddr; }

  IPAddress(in_addr_t inaddr) { *this = inaddr; }

  IPAddress(ip_addr_t ipaddr) { *this = ipaddr; }

  void operator=(IPAddress const &c);

  void operator=(in_addr inaddr);

  void operator=(in_addr_t inaddr);

  void operator=(ip_addr_t ipaddr);

  void operator=(char const *str);

  in_addr_t get_in_addr_t() const;

  ip_addr_t get_ip_addr_t() const;

  uint32_t get_uint32() const;

  IPAddressStr get_str() const;

  bool operator==(IPAddress const &rhs) const;

  bool operator!=(IPAddress const &rhs) const;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// WiFi

struct WiFi {

  enum Network { ClientNetwork = 0, AccessPointNetwork = 1 };

  enum Mode { Client = STATION_MODE, AccessPoint = SOFTAP_MODE, ClientAndAccessPoint = STATIONAP_MODE };

  enum SecurityProtocol {
    Open = AUTH_OPEN,
    WEP = AUTH_WEP,
    WPA_PSK = AUTH_WPA_PSK,
    WPA2_PSK = AUTH_WPA2_PSK,
    WPA_WPA2_PSK = AUTH_WPA_WPA2_PSK
  };

  enum ClientConnectionStatus {
    ClientConnectionStatus_Idle = STATION_IDLE,
    ClientConnectionStatus_Connecting = STATION_CONNECTING,
    ClientConnectionStatus_WrongPassword = STATION_WRONG_PASSWORD,
    ClientConnectionStatus_NoAPFound = STATION_NO_AP_FOUND,
    ClientConnectionStatus_Fail = STATION_CONNECT_FAIL,
    ClientConnectionStatus_GotIP = STATION_GOT_IP
  };

  struct APInfo {
    uint8_t BSSID[6];
    char SSID[33]; // includes ending zero
    uint8_t Channel;
    uint8_t RSSI;
    SecurityProtocol AuthMode;
    bool isHidden;
  };

  static Mode setMode(Mode mode);

  static Mode getMode();

  static char const *convSecurityProtocolToString(SecurityProtocol securityProtocol);

  // setMode must be called with AccessPoint or ClientAndAccessPoint
  // note: make sure there is enough stack space free otherwise mail cause reset (fatal exception)!
  // channel: 1..13
  static void configureAccessPoint(char const *SSID, char const *securityKey, uint8_t channel,
                                   SecurityProtocol securityProtocol = WPA2_PSK, bool hiddenSSID = false);

  // setMode must be called with Client or ClientAndAccessPoint
  static void configureClient(char const *SSID, char const *securityKey);

  // fills MAC with MAC address of the specified network
  // MAC must be a pointer to 6 bytes buffer
  static void getMACAddress(WiFi::Network network, uint8_t *MAC);

  static ClientConnectionStatus getClientConnectionStatus();

  // returns access point list
  static APInfo *getAPList(uint32_t *count, bool rescan, bool canRetry = true);

  static void scanDoneCB(void *arg, STATUS status);

  // allocateCount >= 0 -> allocate (or reallocate or free) AP info
  // allocateCount < 0  -> get infos
  static Queue<bool> *getAPInfo(APInfo **infos = NULL, uint32_t *count = NULL, int32_t allocateCount = -1);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// IP

struct IP {

  static void configureStatic(WiFi::Network network, char const *IP, char const *netmask, char const *gateway);

  // applies only to ClientNetwork
  static void configureDHCP(WiFi::Network network);

  // fills IP with IP address of the specified network
  // IP, netmask, gateway must be a pointer to 4 bytes buffer
  static void getIPInfo(WiFi::Network network, uint8_t *IP, uint8_t *netmask, uint8_t *gateway);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// DHCPServer

struct DHCPServer {

  // warn: each IP in the range requires memory!
  static void configure(char const *startIP, char const *endIP);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// NSLookup (DNS client)
//
// Two ways to use NSLookup. First:
//    NSLookup lookup("www.google.com");
//    char const* ipaddr = strdup(lookup.get().get_str());   // ipaddr will contain a string like "149.3.176.24" (zero
//    terminated). Resulting string has the same lifetime as NSLookup object.
// Second:
//    IPAddress ipaddr = NSLookup::lookup("www.google.com");

struct NSLookup {
  NSLookup(char const *hostname);
  IPAddress get();

  // returns IPAddress(0, 0, 0, 0) on fail
  static IPAddress lookup(char const *hostname);

  // configuration
  // num = 0 or 1 (which dns server to set or get)
  static void setDNSServer(uint32_t num, IPAddress server);
  static IPAddress getDNSServer(uint32_t num);

private:
  IPAddress m_ipaddr;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Socket
// The lwip socket wrapper

class Socket {
public:
  Socket() : m_socket(0), m_connected(false), m_remoteAddress() {}

  Socket(int socket) : m_socket(socket), m_connected(socket != 0), m_remoteAddress() {}

  Socket(Socket const &c) : m_socket(c.m_socket), m_connected(c.m_connected), m_remoteAddress() {}

  // ret -1 = error, ret 0 = disconnected
  int32_t read(void *buffer, uint32_t maxLength);

  // ret -1 = error
  int32_t read(void *buffer, uint32_t maxLength, IPAddress *sourceAddress, uint16_t *sourcePort);

  // ret -1 = error, ret 0 = disconnected
  int32_t peek(void *buffer, uint32_t maxLength, bool nowait = false);

  // buffer can stay in RAM of Flash
  // ret -1 = error, ret 0 = disconnected
  int32_t write(void const *buffer, uint32_t length);

  // str can stay in RAM of Flash
  // ret -1 = error, ret 0 = disconnected
  int32_t write(char const *str);

  // like printf
  // buf can stay in RAM or Flash
  // "strings" of args can stay in RAM or Flash
  uint16_t writeFmt(char const *fmt, ...);

  void close();

  bool isConnected() { return m_connected; }

  void setNoDelay(bool value);

  int getSocket() { return m_socket; }

  // from now Socket will use "sendto" instead of "send"
  void setRemoteAddress(IPAddress remoteAddress, uint16_t remotePort);

  // timeOut in ms (0 = no timeout)
  void setTimeOut(uint32_t timeOut);

  int32_t getLastError();

  bool checkConnection();

private:
  int m_socket;
  bool m_connected;
  sockaddr_in m_remoteAddress; // used by sendTo
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// UDPClient

class UDPClient {
public:
  UDPClient(IPAddress remoteAddress, uint16_t remotePort);

  ~UDPClient();

  Socket *getSocket() { return &m_socket; }

private:
  void init(IPAddress remoteAddress, uint16_t remotePort);
  static uint16_t getUDPPort();

private:
  Socket m_socket;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// TCPServer

template <typename ConnectionHandler_T, uint16_t MaxThreads_V, uint16_t ThreadsStackDepth_V> class TCPServer {

  static uint32_t const ACCEPTWAITTIMEOUTMS = 20000;
  static uint32_t const SOCKETQUEUESIZE = 1; // can queue only one socket (nothing to do with working threads)

  void MTD_FLASHMEM init(uint16_t port) {
    m_socket = lwip_socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in sLocalAddr = {0};
    sLocalAddr.sin_family = AF_INET;
    sLocalAddr.sin_len = sizeof(sockaddr_in);
    sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY); // todo: allow other choices other than IP_ADDR_ANY
    sLocalAddr.sin_port = htons(port);
    lwip_bind(m_socket, (sockaddr *)&sLocalAddr, sizeof(sockaddr_in));
    lwip_listen(m_socket, MaxThreads_V);

    // prepare listener task
    m_listenerTask.setStackDepth(200);
    m_listenerTask.setObject(this);
    m_listenerTask.resume();

    // prepare handler tasks
    for (uint16_t i = 0; i != MaxThreads_V; ++i) {
      m_threads[i].setStackDepth(ThreadsStackDepth_V);
      m_threads[i].setSocketQueue(&m_socketQueue);
      m_threads[i].resume();
    }
  }

public:
  TCPServer(uint16_t port) : m_socketQueue(SOCKETQUEUESIZE) { init(port); }

  virtual ~TCPServer() { lwip_close(m_socket); }

  void MTD_FLASHMEM listenerTask() {
    while (true) {
      sockaddr_in clientAddr;
      socklen_t addrLen = sizeof(sockaddr_in);
      Socket clientSocket = lwip_accept(m_socket, (sockaddr *)&clientAddr, &addrLen);
      if (clientSocket.isConnected()) {
        if (!m_socketQueue.send(clientSocket, ACCEPTWAITTIMEOUTMS)) {
          // Timeout, no thread available, disconnecting
          clientSocket.close();
        }
      }
    }
  }

private:
  int m_socket;
  MethodTask<TCPServer, &TCPServer::listenerTask> m_listenerTask;
  ConnectionHandler_T m_threads[MaxThreads_V];
  Queue<Socket> m_socketQueue;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// TCPConnectionHandler

struct TCPConnectionHandler : public Task {

  TCPConnectionHandler() {}

  void setSocketQueue(Queue<Socket> *socketQueue) { m_socketQueue = socketQueue; }

  Socket *getSocket() { return &m_socket; }

  void exec();

  // applications override this
  virtual void connectionHandler() = 0;

private:
  Socket m_socket;
  Queue<Socket> *m_socketQueue;
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// SNTPClient
// Gets current date/time from a NTP (SNTP) server (default is ntp1.inrim.it).

class SNTPClient {

public:
  // default is 193.204.114.232 (ntp1.inrim.it)
  explicit SNTPClient(IPAddress serverIP = IPAddress(193, 204, 114, 232), uint16_t port = 123);

  bool query(uint64_t *outValue) const;

private:
  IPAddress m_server;
  uint16_t m_port;
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// ICMP

class ICMP {

public:
  ICMP();

  // send Echo Request and wait for Echo Reply
  // return "measured" echo time in microseconds. ret -1 on timeout or error
  int32_t ping(IPAddress const &dest);

  uint16_t receivedBytes() { return m_receivedBytes; }

  uint16_t receivedTTL() { return m_receivedTTL; }

  uint16_t receivedSeq() { return m_waitingSeq; }

private:
  static uint8_t raw_recv_fn(void *arg, raw_pcb *pcb, pbuf *p, ip_addr_t *addr);

  uint16_t m_waitingID;
  uint16_t m_waitingSeq;
  Queue<uint32_t> m_queue;
  uint16_t m_receivedBytes;
  uint16_t m_receivedTTL;
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// NetInterface
// Returns interfaces info

struct NetInterface {
  static netif *get(uint32_t index);
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Router

class Router {

  static uint32_t const INTFCOUNT = 2;

public:
  static void enable();
  static void disable();

private:
  static err_t netif_input(pbuf *p, netif *inp);

  static bool s_enabled;
  static netif_input_fn s_prevInput[INTFCOUNT];
};

} // fdv namespace

#endif	// _FDVNETWORK_H_
