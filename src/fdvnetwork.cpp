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

#include "fdv.h"

extern "C" {
#include "lwip/dns.h"
#include "lwip/netif/etharp.h"
#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
}

namespace fdv {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// IPAddress

void MTD_FLASHMEM IPAddress::operator=(IPAddress const &c) {
  address[0] = c.address[0];
  address[1] = c.address[1];
  address[2] = c.address[2];
  address[3] = c.address[3];
}

void MTD_FLASHMEM IPAddress::operator=(in_addr inaddr) {
  address[0] = ((uint8_t *)&inaddr.s_addr)[0];
  address[1] = ((uint8_t *)&inaddr.s_addr)[1];
  address[2] = ((uint8_t *)&inaddr.s_addr)[2];
  address[3] = ((uint8_t *)&inaddr.s_addr)[3];
}

void MTD_FLASHMEM IPAddress::operator=(in_addr_t inaddr) {
  address[0] = ((uint8_t *)&inaddr)[0];
  address[1] = ((uint8_t *)&inaddr)[1];
  address[2] = ((uint8_t *)&inaddr)[2];
  address[3] = ((uint8_t *)&inaddr)[3];
}

void MTD_FLASHMEM IPAddress::operator=(ip_addr_t ipaddr) {
  address[0] = ((uint8_t *)&ipaddr)[0];
  address[1] = ((uint8_t *)&ipaddr)[1];
  address[2] = ((uint8_t *)&ipaddr)[2];
  address[3] = ((uint8_t *)&ipaddr)[3];
}

void MTD_FLASHMEM IPAddress::operator=(char const *str) {
  if (!str || f_strlen(str) == 0)
    *this = IPAddress(0, 0, 0, 0);
  else
    *this = IPAddress(ipaddr_addr(APtr<char>(f_strdup(str)).get()));
}

in_addr_t MTD_FLASHMEM IPAddress::get_in_addr_t() const {
  in_addr_t a;
  ((uint8_t *)&a)[0] = address[0];
  ((uint8_t *)&a)[1] = address[1];
  ((uint8_t *)&a)[2] = address[2];
  ((uint8_t *)&a)[3] = address[3];
  return a;
}

ip_addr_t MTD_FLASHMEM IPAddress::get_ip_addr_t() const {
  ip_addr_t a;
  ((uint8_t *)&a.addr)[0] = address[0];
  ((uint8_t *)&a.addr)[1] = address[1];
  ((uint8_t *)&a.addr)[2] = address[2];
  ((uint8_t *)&a.addr)[3] = address[3];
  return a;
}

uint32_t MTD_FLASHMEM IPAddress::get_uint32() const {
  uint32_t a;
  ((uint8_t *)&a)[0] = address[0];
  ((uint8_t *)&a)[1] = address[1];
  ((uint8_t *)&a)[2] = address[2];
  ((uint8_t *)&a)[3] = address[3];
  return a;
}

IPAddress::IPAddressStr MTD_FLASHMEM IPAddress::get_str() const {
  IPAddressStr str;
  ip_addr_t a = get_ip_addr_t();
  ipaddr_ntoa_r(&a, (char *)str, 16);
  return str;
}

bool MTD_FLASHMEM IPAddress::operator==(IPAddress const &rhs) const {
  return address[0] == rhs.address[0] && address[1] == rhs.address[1] && address[2] == rhs.address[2] &&
         address[3] == rhs.address[3];
}

bool MTD_FLASHMEM IPAddress::operator!=(IPAddress const &rhs) const { return !(*this == rhs); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// WiFi

WiFi::Mode STC_FLASHMEM WiFi::setMode(Mode mode) {
  Critical critical;
  wifi_set_opmode(mode);
}

WiFi::Mode STC_FLASHMEM WiFi::getMode() {
  Mode mode = (Mode)wifi_get_opmode();
  return mode;
}

char const *STC_FLASHMEM WiFi::convSecurityProtocolToString(SecurityProtocol securityProtocol) {
  char const *authMode = STR_;
  switch (securityProtocol) {
  case WiFi::Open:
    authMode = FSTR("Open");
    break;
  case WiFi::WEP:
    authMode = FSTR("WEP");
    break;
  case WiFi::WPA_PSK:
    authMode = FSTR("WPA-PSK");
    break;
  case WiFi::WPA2_PSK:
    authMode = FSTR("WPA2-PSK");
    break;
  case WiFi::WPA_WPA2_PSK:
    authMode = FSTR("WPA-WPA2-PSK");
    break;
  }
  return authMode;
}

// setMode must be called with AccessPoint or ClientAndAccessPoint
// note: make sure there is enough stack space free otherwise mail cause reset (fatal exception)!
// channel: 1..13
void STC_FLASHMEM WiFi::configureAccessPoint(char const *SSID, char const *securityKey, uint8_t channel,
                                             SecurityProtocol securityProtocol, bool hiddenSSID) {
  softap_config config = {0};
  wifi_softap_get_config(&config);
  f_strcpy((char *)config.ssid, SSID);
  config.ssid_len = f_strlen(SSID);
  f_strcpy((char *)config.password, securityKey);
  config.channel = channel;
  config.authmode = (AUTH_MODE)securityProtocol;
  config.ssid_hidden = (uint8)hiddenSSID;
  Critical critical;
  wifi_softap_set_config(&config);
}

// setMode must be called with Client or ClientAndAccessPoint
void STC_FLASHMEM WiFi::configureClient(char const *SSID, char const *securityKey) {
  station_config config = {0};
  f_strcpy((char *)config.ssid, SSID);
  f_strcpy((char *)config.password, securityKey);
  Critical critical;
  wifi_station_disconnect();
  wifi_station_set_config(&config);
  wifi_station_connect();
}

// fills MAC with MAC address of the specified network
// MAC must be a pointer to 6 bytes buffer
void STC_FLASHMEM WiFi::getMACAddress(WiFi::Network network, uint8_t *MAC) { wifi_get_macaddr((uint8_t)network, MAC); }

WiFi::ClientConnectionStatus STC_FLASHMEM WiFi::getClientConnectionStatus() {
  return (ClientConnectionStatus)wifi_station_get_connect_status();
}

// returns access point list
WiFi::APInfo *STC_FLASHMEM WiFi::getAPList(uint32_t *count, bool rescan, bool canRetry) {
  if (rescan) {
    if (getMode() == AccessPoint)
      setMode(ClientAndAccessPoint);
    wifi_station_scan(NULL, scanDoneCB);
    getAPInfo()->receive(); // wait for completion
  }
  APInfo *infos;
  getAPInfo(&infos, count);
  if (*count == 0 && canRetry) {
    // retry
    return getAPList(count, true, false); // canRetry = false, no more retry to avoid infinite recursive calls
  }
  return infos;
}

void STC_FLASHMEM WiFi::scanDoneCB(void *arg, STATUS status) {
  if (status == OK) {
    // count items
    uint32_t count = 0;
    for (bss_info *bss_link = ((bss_info *)arg)->next.stqe_next; bss_link; bss_link = bss_link->next.stqe_next)
      ++count;
    // fill items
    APInfo *infos;
    getAPInfo(&infos, &count, count);
    for (bss_info *bss_link = ((bss_info *)arg)->next.stqe_next; bss_link;
         bss_link = bss_link->next.stqe_next, ++infos) {
      memcpy(infos->BSSID, bss_link->bssid, 6);
      memset(infos->SSID, 0, 33);
      memcpy(infos->SSID, bss_link->ssid, 32);
      infos->Channel = bss_link->channel;
      infos->RSSI = bss_link->rssi;
      infos->AuthMode = (SecurityProtocol)bss_link->authmode;
      infos->isHidden = (bool)bss_link->is_hidden;
    }
    getAPInfo()->signal();
  }
}

// allocateCount >= 0 -> allocate (or reallocate or free) AP info
// allocateCount < 0  -> get infos
Queue<bool> *STC_FLASHMEM WiFi::getAPInfo(APInfo **infos, uint32_t *count, int32_t allocateCount) {
  static bool s_init = false;
  static APInfo *s_infos = NULL;
  static uint32_t s_count = 0;
  static Queue<bool> *s_queue;
  if (!s_init) {
    s_queue = new Queue<bool>(1); // never deleted
    s_init = true;
  }
  if (allocateCount >= 0) {
    if (s_infos != NULL)
      delete[] s_infos;
    s_infos = NULL;
    s_count = 0;
    if (allocateCount > 0) {
      s_infos = new APInfo[allocateCount];
      s_count = allocateCount;
    }
  }
  if (infos && count) {
    *infos = s_infos;
    *count = s_count;
  }
  return s_queue;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// IP

void STC_FLASHMEM IP::configureStatic(WiFi::Network network, char const *IP, char const *netmask, char const *gateway) {
  ip_info info;
  info.ip.addr = ipaddr_addr(APtr<char>(f_strdup(IP)).get());
  info.netmask.addr = ipaddr_addr(APtr<char>(f_strdup(netmask)).get());
  info.gw.addr = ipaddr_addr(APtr<char>(f_strdup(gateway)).get());

  Critical critical;
  wifi_station_dhcpc_stop();
  wifi_softap_dhcps_stop();
  wifi_set_ip_info(network, &info);
}

// applies only to ClientNetwork
void STC_FLASHMEM IP::configureDHCP(WiFi::Network network) {
  if (network == WiFi::ClientNetwork) {
    Critical critical;
    wifi_station_dhcpc_start();
  }
}

// fills IP with IP address of the specified network
// IP, netmask, gateway must be a pointer to 4 bytes buffer
void STC_FLASHMEM IP::getIPInfo(WiFi::Network network, uint8_t *IP, uint8_t *netmask, uint8_t *gateway) {
  ip_info info;
  wifi_get_ip_info(network, &info);
  for (uint32_t i = 0; i != 4; ++i) {
    IP[i] = ((uint8_t *)&info.ip.addr)[i];
    netmask[i] = ((uint8_t *)&info.netmask.addr)[i];
    gateway[i] = ((uint8_t *)&info.gw.addr)[i];
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// DHCPServer

// warn: each IP in the range requires memory!
void STC_FLASHMEM DHCPServer::configure(char const *startIP, char const *endIP) {
  wifi_softap_dhcps_stop();

  dhcps_lease lease;
  lease.start_ip = IPAddress(startIP).get_ip_addr_t();
  lease.end_ip = IPAddress(endIP).get_ip_addr_t();
  wifi_softap_set_dhcps_lease(&lease);

  uint8_t mode = 1;
  wifi_softap_set_dhcps_offer_option(OFFER_ROUTER, &mode);

  wifi_softap_dhcps_start();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// NSLookup (DNS client)

MTD_FLASHMEM NSLookup::NSLookup(char const *hostname) { m_ipaddr = lookup(hostname); }

// returns IPAddress(0, 0, 0, 0) on fail
IPAddress MTD_FLASHMEM NSLookup::lookup(char const *hostname) {
  APtr<char> memHostName(f_strdup(hostname));
  addrinfo *addrinfo;
  if (lwip_getaddrinfo(memHostName.get(), NULL, NULL, &addrinfo) == 0 && addrinfo) {
    // according to lwip documentation uses only first item of "addrinfo"
    sockaddr_in *sa = (sockaddr_in *)(addrinfo->ai_addr);
    in_addr_t addr = sa->sin_addr.s_addr;
    lwip_freeaddrinfo(addrinfo);
    return IPAddress(addr);
  }
  return IPAddress(0, 0, 0, 0); // fail!
}

IPAddress MTD_FLASHMEM NSLookup::get() {
  return m_ipaddr; 
}

void MTD_FLASHMEM NSLookup::setDNSServer(uint32_t num, IPAddress server) {
  ip_addr_t a = server.get_ip_addr_t();
  dns_setserver(num, &a);
}

IPAddress MTD_FLASHMEM NSLookup::getDNSServer(uint32_t num) {
  return IPAddress(dns_getserver(num)); 
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Socket

// ret -1 = error, ret 0 = disconnected
int32_t MTD_FLASHMEM Socket::read(void *buffer, uint32_t maxLength) {
  int32_t bytesRecv = lwip_recv(m_socket, buffer, maxLength, 0);
  if (maxLength > 0)
    m_connected = (bytesRecv > 0);
  return bytesRecv;
}

// ret 1 = error
int32_t MTD_FLASHMEM Socket::read(void *buffer, uint32_t maxLength, IPAddress *sourceAddress, uint16_t *sourcePort) {
  sockaddr_in from;
  int fromlen;
  int32_t bytesRecv = lwip_recvfrom(m_socket, buffer, maxLength, 0, (sockaddr *)&from, (socklen_t *)&fromlen);
  if (bytesRecv > 0) {
    *sourceAddress = IPAddress(from.sin_addr.s_addr);
    *sourcePort = ntohs(from.sin_port);
  }
  return bytesRecv;
}

// ret -1 = error, ret 0 = disconnected
int32_t MTD_FLASHMEM Socket::peek(void *buffer, uint32_t maxLength, bool nowait) {
  int32_t flags = MSG_PEEK | (nowait ? MSG_DONTWAIT : 0);
  int32_t bytesRecv = lwip_recv(m_socket, buffer, maxLength, flags);
  if (maxLength > 0 && !nowait)
    m_connected = (bytesRecv > 0);
  return bytesRecv;
}

bool MTD_FLASHMEM Socket::checkConnection() {
  uint8_t b;
  peek(&b, 1, true);
  int32_t lr = getLastError();
  return (lr == 0 || lr == EAGAIN) && isConnected();
}

// buffer can stay in RAM of Flash
// ret -1 = error, ret 0 = disconnected
int32_t MTD_FLASHMEM Socket::write(void const *buffer, uint32_t length) {
  static uint32_t const MAXCHUNKSIZE = 128;

  if (!checkConnection())
    return -1;

  int32_t bytesSent = 0;
  // send in chunks of up to MAXCHUNKSIZE bytes
  uint8_t rambuf[min(length, MAXCHUNKSIZE)];
  uint8_t const *src = (uint8_t const *)buffer;
  while (bytesSent < length) {
    uint32_t bytesToSend = min(MAXCHUNKSIZE, length - bytesSent);
    f_memcpy(rambuf, src, bytesToSend);
    int32_t chunkBytesSent =
        m_remoteAddress.sin_len == 0
            ? lwip_send(m_socket, rambuf, bytesToSend, MSG_DONTWAIT)
            : lwip_sendto(m_socket, rambuf, bytesToSend, 0, (sockaddr *)&m_remoteAddress, sizeof(m_remoteAddress));
    int32_t lasterr = getLastError();
    if (lasterr == EAGAIN || lasterr == EWOULDBLOCK) {
      chunkBytesSent = 0;
    } else if (chunkBytesSent <= 0 || lasterr != 0) {
      // error
      bytesSent = -1;
      break;
    }
    bytesSent += chunkBytesSent;
    src += chunkBytesSent;
  }
  if (length > 0)
    m_connected = (bytesSent > 0);
  return bytesSent;
}

// str can stay in RAM of Flash
// ret -1 = error, ret 0 = disconnected
int32_t MTD_FLASHMEM Socket::write(char const *str) { return write((uint8_t const *)str, f_strlen(str)); }

// like printf
// buf can stay in RAM or Flash
// "strings" of args can stay in RAM or Flash
uint16_t MTD_FLASHMEM Socket::writeFmt(char const *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  uint16_t len = f_vsprintf(NULL, fmt, args);
  va_end(args);

  char buf[len + 1];

  va_start(args, fmt);
  f_vsprintf(buf, fmt, args);
  va_end(args);

  write(buf, len);

  return len;
}

void MTD_FLASHMEM Socket::close() {
  if (m_socket > 0) {
    lwip_shutdown(m_socket, SHUT_RDWR);
    lwip_close(m_socket);
    m_socket = 0;
  }
  m_connected = false;
}

void MTD_FLASHMEM Socket::setNoDelay(bool value) {
  int32_t one = (int32_t)value;
  lwip_setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
}

// from now Socket will use "sendto" instead of "send"
void MTD_FLASHMEM Socket::setRemoteAddress(IPAddress remoteAddress, uint16_t remotePort) {
  memset(&m_remoteAddress, 0, sizeof(sockaddr_in));
  m_remoteAddress.sin_family = AF_INET;
  m_remoteAddress.sin_len = sizeof(sockaddr_in);
  m_remoteAddress.sin_addr.s_addr = remoteAddress.get_in_addr_t();
  m_remoteAddress.sin_port = htons(remotePort);
}

// timeOut in ms (0 = no timeout)
void MTD_FLASHMEM Socket::setTimeOut(uint32_t timeOut) {
  lwip_setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeOut, sizeof(timeOut));
}

int32_t MTD_FLASHMEM Socket::getLastError() {
  int32_t r = 0;
  socklen_t l = sizeof(int32_t);
  lwip_getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (void *)&r, &l);
  return r;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// UDPClient

MTD_FLASHMEM UDPClient::UDPClient(IPAddress remoteAddress, uint16_t remotePort) {
  init(remoteAddress, remotePort); 
}

MTD_FLASHMEM UDPClient::~UDPClient() {
  m_socket.close(); 
}

void MTD_FLASHMEM UDPClient::init(IPAddress remoteAddress, uint16_t remotePort) {
  m_socket = lwip_socket(AF_INET, SOCK_DGRAM, 0);

  sockaddr_in localAddress = {0};
  localAddress.sin_family = AF_INET;
  localAddress.sin_len = sizeof(sockaddr_in);
  localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddress.sin_port = htons(getUDPPort());
  lwip_bind(m_socket.getSocket(), (sockaddr *)&localAddress, sizeof(sockaddr_in));

  m_socket.setRemoteAddress(remoteAddress, remotePort);
}

uint16_t MTD_FLASHMEM UDPClient::getUDPPort() {
  static uint16_t s_port = 59999;
  s_port = (++s_port == 0 ? 60000 : s_port);
  return s_port;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// SNTPClient

// serverIP is a uint8_t[4] IP address or NULL
MTD_FLASHMEM SNTPClient::SNTPClient(IPAddress serverIP, uint16_t port) : m_server(serverIP), m_port(port) {
}

bool MTD_FLASHMEM SNTPClient::query(uint64_t *outValue) const {
  // send req (mode 3), unicast, version 4
  uint8_t const MODE_CLIENT = 3;
  uint8_t const VERSION = 4;
  uint8_t const BUFLEN = 48;
  uint32_t const REPLYTIMEOUT = 3000;
  uint8_t buf[BUFLEN];
  memset(&buf[0], 0, BUFLEN);
  buf[0] = MODE_CLIENT | (VERSION << 3);

  UDPClient UDP(m_server, m_port);
  Socket *socket = UDP.getSocket();
  socket->setTimeOut(REPLYTIMEOUT);

  if (socket->write(&buf[0], BUFLEN)) {
    // get reply
    if (socket->read(&buf[0], BUFLEN) == BUFLEN) {
      memcpy(outValue, &buf[40], sizeof(uint64_t));
      return true; // ok
    }
  }

  return false; // error
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// ICMP

MTD_FLASHMEM ICMP::ICMP() : m_waitingID(rand() & 0xFFFF), m_waitingSeq(0), m_queue(1) {
}

int32_t MTD_FLASHMEM ICMP::ping(IPAddress const &dest) {
  static uint32_t const TIMEOUT = 4000;
  static int32_t const TIMEOUT_RESULT = -1;

  uint32_t result = TIMEOUT_RESULT;

  // generate seq
  m_waitingSeq++;

  // prepare packet to send
  pbuf *hdrbuf = pbuf_alloc(PBUF_IP, sizeof(icmp_echo_hdr), PBUF_RAM);
  icmp_echo_hdr *hdr = (icmp_echo_hdr *)hdrbuf->payload;
  hdr->type = ICMP_ECHO;
  hdr->code = 0;
  hdr->chksum = 0;
  hdr->id = htons(m_waitingID);
  hdr->seqno = htons(m_waitingSeq);
  hdr->chksum = inet_chksum((uint16_t *)hdr, sizeof(icmp_echo_hdr));

  // send Echo request
  raw_pcb *pcb = raw_new(IP_PROTO_ICMP);
  raw_recv(pcb, ICMP::raw_recv_fn, this);
  raw_bind(pcb, IP_ADDR_ANY);

  ip_addr_t addr = dest.get_ip_addr_t();
  raw_sendto(pcb, hdrbuf, &addr);
  pbuf_free(hdrbuf);

  uint32_t t1 = micros();
  if (m_queue.receive(TIMEOUT))
    result = (micros() - t1);

  raw_remove(pcb);

  return result;
}

uint8_t STC_FLASHMEM ICMP::raw_recv_fn(void *arg, raw_pcb *pcb, pbuf *p, ip_addr_t *addr) {
  ICMP *this_ = (ICMP *)arg;

  ip_hdr *iphdr = (ip_hdr *)p->payload;

  uint16_t ttl = IPH_TTL(iphdr);

  if (p->tot_len >= PBUF_IP_HLEN + sizeof(icmp_echo_hdr) && pbuf_header(p, -PBUF_IP_HLEN) == 0) {
    icmp_echo_hdr *hdr = (icmp_echo_hdr *)p->payload;
    if (ntohs(hdr->id) == this_->m_waitingID && ntohs(hdr->seqno) == this_->m_waitingSeq) {
      this_->m_receivedBytes = p->tot_len;
      this_->m_receivedTTL = ttl;
      this_->m_queue.signal();
      pbuf_free(p);
      return 1;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// NetInterface

netif *NetInterface::get(uint32_t index) {
  char name[4] = "enX";
  name[2] = '0' + index;
  return netif_find(name);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Router

netif_input_fn Router::s_prevInput[INTFCOUNT];

bool Router::s_enabled = false;

void MTD_FLASHMEM Router::enable() {
  if (!s_enabled) {
    Critical critical;
    for (uint32_t i = 0; i != INTFCOUNT; ++i) {
      netif *en = NetInterface::get(i);
      if (en) {
        s_prevInput[i] = en->input;
        en->input = Router::netif_input;
      } else
        s_prevInput[i] = NULL;
    }
    s_enabled = true;
  }
}

void MTD_FLASHMEM Router::disable() {
  if (s_enabled) {
    Critical critical;
    for (uint32_t i = 0; i != INTFCOUNT; ++i) {
      netif *en = NetInterface::get(i);
      if (en && s_prevInput[i])
        en->input = s_prevInput[i];
    }
    s_enabled = false;
  }
}

// remove MTD_FLASHMEM to speedup routing
err_t MTD_FLASHMEM Router::netif_input(pbuf *p, netif *inp) {
  eth_hdr *ethdr = (eth_hdr *)p->payload;

  if (ntohs(ethdr->type) == ETHTYPE_IP) {

    // move buffer pointer to start of IP header
    pbuf_header(p, -sizeof(eth_hdr));
    ip_hdr *iphdr = (ip_hdr *)(p->payload);

    // needs to route?
    // 1. check match of source interface IP/netmask and destination IP
    bool route = ((iphdr->dest.addr & inp->netmask.addr) != (inp->ip_addr.addr & inp->netmask.addr));
    // 2. check if not multicast or broadcast (>=224.0.0.0 up to 255.255.255.255)
    route = route && ((iphdr->dest.addr & 0xE0) != 0xE0);

    if (route) {
      /*
      debug("netif_input intf=%d len=%d id=%d prot=%d src=%s dst=%s route?=%c\r\n",
            inp->num, p->tot_len, IPH_ID(iphdr), IPH_PROTO(iphdr),
            (char const*)IPAddress(iphdr->src.addr).get_str(),
            (char const*)IPAddress(iphdr->dest.addr).get_str(),
            route?'Y':'N');
      */

      // find destination interface
      ip_addr_t ipdest;
      ipdest.addr = iphdr->dest.addr;
      netif *destIntf = ip_route(&ipdest);

      // decrement TTL
      IPH_TTL_SET(iphdr, IPH_TTL(iphdr) - 1);

      if (IPH_TTL(iphdr) > 0) {
        // update IP checksum
        if (IPH_CHKSUM(iphdr) >= PP_HTONS(0xffffU - 0x100))
          IPH_CHKSUM_SET(iphdr, IPH_CHKSUM(iphdr) + PP_HTONS(0x100) + 1);
        else
          IPH_CHKSUM_SET(iphdr, IPH_CHKSUM(iphdr) + PP_HTONS(0x100));

        // send the packet
        ip_output_if(p, NULL, IP_HDRINCL, 0, 0, 0, destIntf);
      }

      pbuf_free(p);
      return ERR_OK;
    }

    // restore buffer pointer to start of Ethernet header
    pbuf_header(p, +sizeof(eth_hdr));
  }

  return (Router::s_prevInput[inp->num])(p, inp);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// TCPConnectionHandler

void MTD_FLASHMEM TCPConnectionHandler::exec() {
  while (true) {
    if (m_socketQueue->receive(&m_socket)) {
      connectionHandler();
      m_socket.close();
    }
  }
}


} // fdv namespace
