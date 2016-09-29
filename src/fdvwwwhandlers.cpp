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

namespace fdv {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPHelperConfiguration

void MTD_FLASHMEM HTTPHelperConfiguration::getWiFiMode(HTTPTemplateResponse *response) {
  switch (ConfigurationManager::getWiFiMode()) {
  case WiFi::Client:
    response->addParamStr(STR_clientmode, STR_checked);
    break;
  case WiFi::AccessPoint:
    response->addParamStr(STR_apmode, STR_checked);
    break;
  case WiFi::ClientAndAccessPoint:
    response->addParamStr(STR_bothsmode, STR_checked);
    break;
  }
}

void MTD_FLASHMEM HTTPHelperConfiguration::setWiFiMode(HTTPTemplateResponse *response) {
  char const *Mode = response->getRequest().form[STR_Mode];
  if (f_strcmp(Mode, STR_bothsmode) == 0)
    ConfigurationManager::setWiFiMode(WiFi::ClientAndAccessPoint);
  else if (f_strcmp(Mode, STR_clientmode) == 0)
    ConfigurationManager::setWiFiMode(WiFi::Client);
  else if (f_strcmp(Mode, STR_apmode) == 0)
    ConfigurationManager::setWiFiMode(WiFi::AccessPoint);
}

void MTD_FLASHMEM HTTPHelperConfiguration::getClientModeWiFiParams(HTTPTemplateResponse *response) {
  char const *SSID;
  char const *securityKey;
  ConfigurationManager::getClientParams(&SSID, &securityKey);
  response->addParamStr(FSTR("CLPSW"), securityKey);
  response->addParamStr(FSTR("CLSSID"), SSID);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setClientModeWiFiParams(HTTPTemplateResponse *response) {
  ConfigurationManager::setClientParams(response->getRequest().form[FSTR("CLSSID")],
                                        response->getRequest().form[FSTR("CLPSW")]);
}

void MTD_FLASHMEM HTTPHelperConfiguration::getClientModeIP(HTTPTemplateResponse *response) {
  bool staticIP;
  char const *IP;
  char const *netmask;
  char const *gateway;
  ConfigurationManager::getClientIPParams(&staticIP, &IP, &netmask, &gateway);
  if (staticIP)
    response->addParamStr(STR_stcIP, STR_checked);
  else
    response->addParamStr(STR_dynIP, STR_checked);
  response->addParamStr(STR_CLIP, IP);
  response->addParamStr(STR_CLMSK, netmask);
  response->addParamStr(STR_CLGTW, gateway);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setClientModeIP(HTTPTemplateResponse *response) {
  ConfigurationManager::setClientIPParams(f_strcmp(response->getRequest().form[STR_CLIPMOD], STR_stcIP) == 0,
                                          response->getRequest().form[STR_CLIP], response->getRequest().form[STR_CLMSK],
                                          response->getRequest().form[STR_CLGTW]);
}

// DNS1str and DNS2str cannot be allocated into the stack here because they are used in response object
void MTD_FLASHMEM HTTPHelperConfiguration::getDNSServer(HTTPTemplateResponse *response,
                                                        IPAddress::IPAddressStr &DNS1str,
                                                        IPAddress::IPAddressStr &DNS2str) {
  IPAddress DNS1, DNS2;
  ConfigurationManager::getDNSParams(&DNS1, &DNS2);
  DNS1str = DNS1.get_str();
  DNS2str = DNS2.get_str();
  response->addParamStr(STR_DNS1, DNS1str);
  response->addParamStr(STR_DNS2, DNS2str);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setDNSServer(HTTPTemplateResponse *response) {
  ConfigurationManager::setDNSParams(IPAddress(response->getRequest().form[STR_DNS1]),
                                     IPAddress(response->getRequest().form[STR_DNS2]));
}

// must be "char dateStr[11]" and "char timeStr[9]"
void MTD_FLASHMEM HTTPHelperConfiguration::getDateTime(HTTPTemplateResponse *response, char *dateStr, char *timeStr) {
  // get current date
  DateTime::now().format(dateStr, FSTR("%d/%m/%Y"));
  response->addParamStr(STR_date, dateStr);

  // get current time
  DateTime::now().format(timeStr, FSTR("%H:%M:%S"));
  response->addParamStr(STR_time, timeStr);

  // get timezone and NTP server
  int8_t timezoneHours;
  uint8_t timezoneMinutes;
  char const *defaultNTPServer;
  ConfigurationManager::getDateTimeParams(&timezoneHours, &timezoneMinutes, &defaultNTPServer);
  response->addParamInt(STR_tzh, timezoneHours);
  response->addParamInt(STR_tzm, timezoneMinutes);
  response->addParamStr(STR_ntpsrv, defaultNTPServer);

  if (f_strlen(defaultNTPServer) == 0)
    response->addParamStr(STR_CHTIMEMAN, STR_checked);
  else
    response->addParamStr(STR_CHTIMEAUT, STR_checked);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setDateTime(HTTPTemplateResponse *response) {
  // set current date and time
  char const *dateStr = response->getRequest().form[STR_date];
  char const *timeStr = response->getRequest().form[STR_time];
  if (dateStr && timeStr) {
    DateTime dt;
    dt.decode(dateStr, FSTR("%d/%m/%Y"));
    dt.decode(timeStr, FSTR("%H:%M:%S"));
    DateTime::setCurrentDateTime(dt);
  }

  // set timezone and NTP server
  char const *tzh = response->getRequest().form[STR_tzh];
  char const *tzm = response->getRequest().form[STR_tzm];
  char const *ntpsrv = response->getRequest().form[STR_ntpsrv];
  if (tzh && tzm) {
    ConfigurationManager::setDateTimeParams(strtol(tzh, NULL, 10), strtol(tzm, NULL, 10), ntpsrv ? ntpsrv : STR_);
    ConfigurationManager::applyDateTime();
  }
}

void MTD_FLASHMEM HTTPHelperConfiguration::getUART(HTTPTemplateResponse *response) {
  uint32_t baudRate;
  bool enableSystemOutput;
  SerialService serialService;
  ConfigurationManager::getUARTParams(&baudRate, &enableSystemOutput, &serialService);
  response->addParamInt(STR_baud, baudRate);
  if (enableSystemOutput)
    response->addParamStr(STR_debugout, STR_checked);
  switch (serialService) {
  case SerialService_None:
    response->addParamStr(STR_srv0, STR_checked);
    break;
  case SerialService_Console:
    response->addParamStr(STR_srv1, STR_checked);
    break;
  case SerialService_BinaryProtocol:
    response->addParamStr(STR_srv2, STR_checked);
    break;
  }
}

void MTD_FLASHMEM HTTPHelperConfiguration::setUART(HTTPTemplateResponse *response) {
  char const *baud = response->getRequest().form[STR_baud];
  char const *serv = response->getRequest().form[FSTR("srv")];
  if (baud && serv) {
    ConfigurationManager::setUARTParams(strtol(baud, NULL, 10), response->getRequest().form[STR_debugout] != NULL,
                                        (SerialService)strtol(serv, NULL, 10));
    ConfigurationManager::applyUARTServices();
  }
}

void MTD_FLASHMEM HTTPHelperConfiguration::getAPWiFiParams(HTTPTemplateResponse *response, APtr<char> &APCHStr,
                                                           APtr<char> &APSECStr) {
  uint8_t channel;
  WiFi::SecurityProtocol securityProtocol;
  bool hiddenSSID;
  char const *SSID;
  char const *securityKey;
  ConfigurationManager::getAccessPointParams(&SSID, &securityKey, &channel, &securityProtocol, &hiddenSSID);
  response->addParamStr(FSTR("APSSID"), SSID);
  response->addParamStr(FSTR("APPSW"), securityKey);
  APCHStr.reset(f_printf(FSTR("APCH%d"), channel));
  response->addParamStr(APCHStr.get(), STR_selected);
  APSECStr.reset(f_printf(FSTR("APSEC%d"), (int32_t)securityProtocol));
  response->addParamStr(APSECStr.get(), STR_selected);
  if (hiddenSSID)
    response->addParamStr(FSTR("APHSSID"), STR_checked);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setAPWiFiParams(HTTPTemplateResponse *response) {
  char const *APCH = response->getRequest().form[FSTR("APCH")];
  char const *APSEC = response->getRequest().form[FSTR("APSEC")];
  if (APCH && APSEC)
    ConfigurationManager::setAccessPointParams(
        response->getRequest().form[FSTR("APSSID")], response->getRequest().form[FSTR("APPSW")], strtol(APCH, NULL, 10),
        (WiFi::SecurityProtocol)strtol(APSEC, NULL, 10), response->getRequest().form[FSTR("APHSSID")] != NULL);
}

void MTD_FLASHMEM HTTPHelperConfiguration::getAPModeIP(HTTPTemplateResponse *response) {
  char const *IP;
  char const *netmask;
  char const *gateway;
  ConfigurationManager::getAccessPointIPParams(&IP, &netmask, &gateway);
  response->addParamStr(STR_APIP, IP);
  response->addParamStr(STR_APMSK, netmask);
  response->addParamStr(STR_APGTW, gateway);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setAPModeIP(HTTPTemplateResponse *response) {
  ConfigurationManager::setAccessPointIPParams(response->getRequest().form[STR_APIP],
                                               response->getRequest().form[STR_APMSK],
                                               response->getRequest().form[STR_APGTW]);
}

void MTD_FLASHMEM HTTPHelperConfiguration::getDHCPServer(HTTPTemplateResponse *response) {
  bool DHCPDEnabled;
  char const *startIP;
  char const *endIP;
  ConfigurationManager::getDHCPServerParams(&DHCPDEnabled, &startIP, &endIP);
  if (DHCPDEnabled)
    response->addParamStr(STR_DHCPD, STR_checked);
  response->addParamStr(STR_startIP, startIP);
  response->addParamStr(STR_endIP, endIP);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setDHCPServer(HTTPTemplateResponse *response) {
  if (response->getRequest().form[STR_DHCPD] != NULL)
    ConfigurationManager::setDHCPServerParams(true, response->getRequest().form[STR_startIP],
                                              response->getRequest().form[STR_endIP]);
  else
    ConfigurationManager::setDHCPServerParams(false);
}

void MTD_FLASHMEM HTTPHelperConfiguration::getRouting(HTTPTemplateResponse *response) {
  bool routing;
  ConfigurationManager::getRouting(&routing);
  if (routing)
    response->addParamStr(STR_ROUTING, STR_checked);
}

void MTD_FLASHMEM HTTPHelperConfiguration::setRouting(HTTPTemplateResponse *response) {
  ConfigurationManager::setRouting(response->getRequest().form[STR_ROUTING] != NULL);
}

// looks for "gpio" (0..16), "val" (0..1) and "store" (0..1) parameters in the http query
// if "store=1" then the gpio value is stored in flash
// "store" is optional
void MTD_FLASHMEM HTTPHelperConfiguration::GPIOSetValue(HTTPResponse *response) {
  char const *gpio = response->getRequest().query[STR_gpio];
  char const *val = response->getRequest().query[FSTR("val")];
  char const *storestr = response->getRequest().query[STR_store];
  bool store = storestr && f_strcmp(storestr, STR_1) == 0;
  if (gpio && val) {
    uint8_t gpion = strtol(gpio, NULL, 10);
    bool newvalue = *val - '0';
    if (store) {
      bool configured, isOutput, pullUp, oldvalue;
      ConfigurationManager::getGPIOParams(gpion, &configured, &isOutput, &pullUp, &oldvalue);
      ConfigurationManager::setGPIOParams(gpion, configured, isOutput, pullUp, newvalue);
    }
    GPIO(gpion).write(newvalue);
  }
}

// looks for "gpio" (0..16), "mode" (in..out), "pullup" (0..1), "store" (0..1) parameters in the http query
// if "store=1" then the gpio configuration is stored in flash
// "store" and "pullup" are optionals
void MTD_FLASHMEM HTTPHelperConfiguration::GPIOConf(HTTPResponse *response) {
  char const *gpiostr = response->getRequest().query[STR_gpio];
  char const *modestr = response->getRequest().query[STR_mode];
  char const *pullupstr = response->getRequest().query[STR_pullup];
  bool pullup = pullupstr && f_strcmp(pullupstr, STR_1) == 0;
  char const *storestr = response->getRequest().query[STR_store];
  bool store = storestr && f_strcmp(storestr, STR_1) == 0;
  if (gpiostr && modestr) {
    uint8_t gpio = strtol(gpiostr, NULL, 10);
    bool mode = f_strcmp(modestr, STR_out) == 0;
    if (store) {
      ConfigurationManager::setGPIOParams(gpio, true, mode, pullup, false);
    }
    GPIO g(gpio);
    if (mode)
      g.modeOutput();
    else
      g.modeInput();
    g.enablePullUp(pullup);
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPWifiConfigurationResponse

MTD_FLASHMEM HTTPWifiConfigurationResponse::HTTPWifiConfigurationResponse(HTTPHandler *httpHandler,
                                                                          char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPWifiConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    // set WiFi mode
    HTTPHelperConfiguration::setWiFiMode(this);

    // set client mode WiFi parameters
    HTTPHelperConfiguration::setClientModeWiFiParams(this);

    // set access point parameters
    HTTPHelperConfiguration::setAPWiFiParams(this);
  }

  // get WiFi mode
  HTTPHelperConfiguration::getWiFiMode(this);

  // get client mode WiFi parameters
  HTTPHelperConfiguration::getClientModeWiFiParams(this);

  // get access point parameters
  APtr<char> APCHStr, APSECStr;
  HTTPHelperConfiguration::getAPWiFiParams(this, APCHStr, APSECStr);

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPNetworkConfigurationResponse

MTD_FLASHMEM HTTPNetworkConfigurationResponse::HTTPNetworkConfigurationResponse(HTTPHandler *httpHandler,
                                                                                char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPNetworkConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    // set client mode IP configuration
    HTTPHelperConfiguration::setClientModeIP(this);

    // set access point IP configuration
    HTTPHelperConfiguration::setAPModeIP(this);

    // set DHCP server configuration
    HTTPHelperConfiguration::setDHCPServer(this);

    // set DNS
    HTTPHelperConfiguration::setDNSServer(this);

    // set Routing
    HTTPHelperConfiguration::setRouting(this);
    ConfigurationManager::applyRouting();
  }

  // get client mode IP configuration
  HTTPHelperConfiguration::getClientModeIP(this);
  WiFi::Mode mode = ConfigurationManager::getWiFiMode();
  addParamStr(STR_DISP_CLIPCONF,
              mode == WiFi::Client || mode == WiFi::ClientAndAccessPoint ? STR_ : STR_style_display_none);

  // get access point IP configuration
  HTTPHelperConfiguration::getAPModeIP(this);
  addParamStr(STR_DISP_APIPCONF,
              mode == WiFi::AccessPoint || mode == WiFi::ClientAndAccessPoint ? STR_ : STR_style_display_none);

  // get DHCP server configuration
  HTTPHelperConfiguration::getDHCPServer(this);
  addParamStr(FSTR("DISP_DHCPD"),
              mode == WiFi::AccessPoint || mode == WiFi::ClientAndAccessPoint ? STR_ : STR_style_display_none);

  // get DNS server configuration
  IPAddress::IPAddressStr DNS1str, DNS2str;
  HTTPHelperConfiguration::getDNSServer(this, DNS1str, DNS2str);

  // get routing configuration
  HTTPHelperConfiguration::getRouting(this);

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPServicesConfigurationResponse

MTD_FLASHMEM HTTPServicesConfigurationResponse::HTTPServicesConfigurationResponse(HTTPHandler *httpHandler,
                                                                                  char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPServicesConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    // set Web server configuration
    char const *httpport = getRequest().form[STR_httpport];
    if (httpport)
      ConfigurationManager::setWebServerParams(strtol(httpport, NULL, 10));

    // set UART configuration
    HTTPHelperConfiguration::setUART(this);
  }

  // get Web server configuration
  uint16_t webPort;
  ConfigurationManager::getWebServerParams(&webPort);
  addParamInt(STR_httpport, webPort);

  // get UART configuration
  HTTPHelperConfiguration::getUART(this);

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPWiFiScanResponseHTMLRows
// Helper for HTTPWizardConfigurationResponse

MTD_FLASHMEM HTTPWiFiScanResponseHTMLRows::HTTPWiFiScanResponseHTMLRows(HTTPHandler *httpHandler)
    : HTTPResponse(httpHandler, NULL) {}

void MTD_FLASHMEM HTTPWiFiScanResponseHTMLRows::flush() {
  setStatus(STR_200_OK);
  addHeader(STR_Content_Type, STR_TEXTHTML);

  uint32_t count = 0;
  WiFi::APInfo *infos = WiFi::getAPList(&count, true);

  addContent(FSTR("<tr> <th>SSID</th> <th>Address</th> <th>Channel</th> <th>RSSI</th> <th>Security</th> </tr>"));
  for (uint32_t i = 0; i != count; ++i) {
    addContent(f_printf(FSTR("<tr> <td><input type='radio' name='selssid' value='%s' onclick='SelSSID(this)'>%s</td> "
                             "<td>%02X:%02X:%02X:%02X:%02X:%02X</td> <td>%d</td> <td>%d</td> <td>%s</td> </tr>"),
                        infos[i].SSID, infos[i].SSID, infos[i].BSSID[0], infos[i].BSSID[1], infos[i].BSSID[2],
                        infos[i].BSSID[3], infos[i].BSSID[4], infos[i].BSSID[5], infos[i].Channel, infos[i].RSSI,
                        WiFi::convSecurityProtocolToString(infos[i].AuthMode)),
               true);
  }

  HTTPResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPGPIOConfigurationResponse

MTD_FLASHMEM HTTPGPIOConfigurationResponse::HTTPGPIOConfigurationResponse(HTTPHandler *httpHandler,
                                                                          char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPGPIOConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    char const *gpio = getRequest().form[FSTR("GPIO")];
    if (getRequest().form[FSTR("configured")]) {
      // gpio enabled
      char const *mode = getRequest().form[STR_mode];
      char const *pullUp = getRequest().form[STR_pullup];
      ConfigurationManager::setGPIOParams(strtol(gpio, NULL, 10), true, f_strcmp(mode, STR_out) == 0, pullUp != NULL,
                                          false);
      ConfigurationManager::applyGPIO();
    } else if (gpio) {
      // gpio disabled (not configured)
      ConfigurationManager::setGPIOParams(strtol(gpio, NULL, 10), false, false, false, false);
    }
  }

  HTTPHelperConfiguration::GPIOSetValue(this);

  LinkedCharChunks *linkedChunks = addParamCharChunks(FSTR("GPIOS"));
  for (uint32_t i = 0; i != 16; ++i) {
    if (i != 1 && i != 3 && (i < 6 || i > 11)) {
      bool configured, isOutput, pullUp, value;
      ConfigurationManager::getGPIOParams(i, &configured, &isOutput, &pullUp, &value);

      linkedChunks->addChunk(f_printf(FSTR("<tr> <td>%d</td> <td><form method='POST'>"), i), true);
      linkedChunks->addChunk(f_printf(FSTR("Enabled <input type='checkbox' name='configured' value='1' "
                                           "onclick=\"document.getElementById('GPIO%d').disabled=!this.checked\" %s>"),
                                      i, configured ? STR_checked : STR_),
                             true);
      linkedChunks->addChunk(
          f_printf(FSTR("<fieldset class='inline' id='GPIO%d' %s>"), i, configured ? STR_ : STR_disabled), true);
      linkedChunks->addChunk(
          f_printf(
              FSTR("<select name='mode'><option value='in' %s>IN</option><option value='out' %s>OUT</option></select>"),
              isOutput ? STR_ : STR_selected, isOutput ? STR_selected : STR_),
          true);
      linkedChunks->addChunk(
          f_printf(FSTR("     PullUp <input type='checkbox' name='pullup' value='1' %s> </fieldset>"),
                   pullUp ? STR_checked : STR_),
          true);
      linkedChunks->addChunk(f_printf(FSTR("<input type='hidden' name='GPIO' value='%d'>"), i), true);
      linkedChunks->addChunk(FSTR("<input type='submit' value='Save'></form></td>"));
      if (configured) {
        if (isOutput) {
          linkedChunks->addChunk(
              f_printf(FSTR("<td><a href='confgpio?gpio=%d&val=%d&store=1' class='link_button2'>%s</a></td> </tr>"), i,
                       !value, value ? STR_HI : STR_LO),
              true);
        } else {
          linkedChunks->addChunk(f_printf(FSTR("<td>%s</td> </tr>"), GPIO(i).read() ? STR_HI : STR_LO), true);
        }
      } else {
        linkedChunks->addChunk(FSTR("<td></td></tr>"));
      }
    }
  }

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPGPIOResponseHTML

MTD_FLASHMEM HTTPGPIOResponseHTML::HTTPGPIOResponseHTML(HTTPHandler *httpHandler) : HTTPResponse(httpHandler, NULL) {}

void MTD_FLASHMEM HTTPGPIOResponseHTML::flush() {
  setStatus(STR_200_OK);
  addHeader(STR_Content_Type, STR_TEXTHTML);

  char const *cmd = getRequest().query[FSTR("cmd")];
  if (cmd && f_strcmp(cmd, FSTR("set")) == 0) {
    // set gpio
    HTTPHelperConfiguration::GPIOSetValue(this);
  } else if (cmd && f_strcmp(cmd, FSTR("conf")) == 0) {
    // conf gpio
    HTTPHelperConfiguration::GPIOConf(this);
  }

  char const *gpio = getRequest().query[STR_gpio];
  if (gpio) {
    uint8_t gpion = strtol(gpio, NULL, 10);
    addContent(GPIO(gpion).read() ? STR_1 : STR_0);
  }

  HTTPResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPTimeConfigurationResponse

MTD_FLASHMEM HTTPTimeConfigurationResponse::HTTPTimeConfigurationResponse(HTTPHandler *httpHandler,
                                                                          char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPTimeConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    HTTPHelperConfiguration::setDateTime(this);
  }

  char dateStr[11], timeStr[9];
  HTTPHelperConfiguration::getDateTime(this, dateStr, timeStr);

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPWizardConfigurationResponse

MTD_FLASHMEM HTTPWizardConfigurationResponse::HTTPWizardConfigurationResponse(HTTPHandler *httpHandler,
                                                                              char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void HTTPWizardConfigurationResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    // set WiFi mode
    HTTPHelperConfiguration::setWiFiMode(this);

    // set client mode WiFi parameters
    HTTPHelperConfiguration::setClientModeWiFiParams(this);

    // set client mode IP configuration
    HTTPHelperConfiguration::setClientModeIP(this);

    // set DNS
    HTTPHelperConfiguration::setDNSServer(this);

    // date time, ntp, ...
    HTTPHelperConfiguration::setDateTime(this);

    // set UART configuration
    HTTPHelperConfiguration::setUART(this);

    // set access point parameters
    HTTPHelperConfiguration::setAPWiFiParams(this);

    // set access point IP configuration
    HTTPHelperConfiguration::setAPModeIP(this);

    // set DHCP server configuration
    HTTPHelperConfiguration::setDHCPServer(this);

    // set Routing
    HTTPHelperConfiguration::setRouting(this);

    // reboot
    HTTPTemplateResponse response(getHttpHandler(), FSTR("reboot.html"));
    reboot(3000); // reboot in 3s
    response.flush();
    return;
  }

  // get WiFi mode
  HTTPHelperConfiguration::getWiFiMode(this);

  // get client mode WiFi parameters
  HTTPHelperConfiguration::getClientModeWiFiParams(this);

  // get client mode IP configuration
  HTTPHelperConfiguration::getClientModeIP(this);

  // get DNS server configuration
  IPAddress::IPAddressStr DNS1str, DNS2str;
  HTTPHelperConfiguration::getDNSServer(this, DNS1str, DNS2str);

  // datetime, ntp, ...
  char dateStr[11], timeStr[9];
  HTTPHelperConfiguration::getDateTime(this, dateStr, timeStr);

  // get UART configuration
  HTTPHelperConfiguration::getUART(this);

  // get access point parameters
  APtr<char> APCHStr, APSECStr;
  HTTPHelperConfiguration::getAPWiFiParams(this, APCHStr, APSECStr);

  // get access point IP configuration
  HTTPHelperConfiguration::getAPModeIP(this);

  // get DHCP server configuration
  HTTPHelperConfiguration::getDHCPServer(this);

  // get routing configuration
  HTTPHelperConfiguration::getRouting(this);

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// HTTPFileSystemBrowserResponse

MTD_FLASHMEM HTTPFileSystemBrowserResponse::HTTPFileSystemBrowserResponse(HTTPHandler *httpHandler,
                                                                          char const *filename)
    : HTTPTemplateResponse(httpHandler, filename) {}

void MTD_FLASHMEM HTTPFileSystemBrowserResponse::flush() {
  if (getRequest().method == HTTPHandler::Post) {
    char const *CMD = getRequest().form[FSTR("CMD")];
    char const *filename = getRequest().form[FSTR("fname")];
    if (filename && CMD && f_strcmp(CMD, FSTR("Delete")) == 0) {
      FlashFileSystem::remove(filename);
    }
  }

  addParamInt(FSTR("FREESPC"), FlashFileSystem::getFreeSpace());
  addParamInt(FSTR("TOTSPC"), FlashFileSystem::getTotalSpace());

  LinkedCharChunks *linkedChunks = addParamCharChunks(FSTR("FILES"));
  FlashFileSystem::Item item;
  for (int32_t i = 0; FlashFileSystem::getNext(&item); ++i) {
    linkedChunks->addChunk(f_printf(FSTR("<tr> <td>%s</td> <td>%d</td> <td>%s</td> "
                                         "<td> "
                                         "<button type='button' onclick='xdelfile(\"%s\")'>Delete</button>"
                                         "<button type='button' onclick='openfile(\"%s\")'>Open</button>"
                                         "</td></tr>"),
                                    item.filename, item.datalength, item.mimetype, item.filename, item.filename),
                           true);
  }

  HTTPTemplateResponse::flush();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// DefaultHTTPHandler

MTD_FLASHMEM DefaultHTTPHandler::DefaultHTTPHandler() {
  static const Route routes[] = {
      {FSTR("/"), (PageHandler)&DefaultHTTPHandler::get_home},
      {FSTR("/confwizard"), (PageHandler)&DefaultHTTPHandler::get_confwizard},
      {FSTR("/fsbrowser"), (PageHandler)&DefaultHTTPHandler::get_fsbrowser},
      {FSTR("/confwifi"), (PageHandler)&DefaultHTTPHandler::get_confwifi},
      {FSTR("/wifiscan"), (PageHandler)&DefaultHTTPHandler::get_wifiscan},
      {FSTR("/confnet"), (PageHandler)&DefaultHTTPHandler::get_confnet},
      {FSTR("/confserv"), (PageHandler)&DefaultHTTPHandler::get_confserv},
      {FSTR("/confgpio"), (PageHandler)&DefaultHTTPHandler::get_confgpio},
      {FSTR("/gpio"), (PageHandler)&DefaultHTTPHandler::get_gpio},
      {FSTR("/conftime"), (PageHandler)&DefaultHTTPHandler::get_conftime},
      {FSTR("/reboot"), (PageHandler)&DefaultHTTPHandler::get_reboot},
      {FSTR("/restore"), (PageHandler)&DefaultHTTPHandler::get_restore},
      {FSTR("*"), (PageHandler)&DefaultHTTPHandler::get_all},
  };
  setRoutes(routes, sizeof(routes) / sizeof(Route));
}

void MTD_FLASHMEM DefaultHTTPHandler::dispatch() {
#if (FDV_INCLUDE_SERIALBINARY == 1)
  if (ConfigurationManager::getSerialBinary() && ConfigurationManager::getSerialBinary()->isReady()) {
    StringList *routes = ConfigurationManager::getSerialBinary()->getHTTPRoutes();
    if (routes) {
      for (uint32_t i = 0; i != routes->size(); ++i) {
        if (f_strcmp(FSTR("*"), routes->getItem(i)) == 0 ||
            t_strcmp(getRequest().requestedPage, CharIterator(routes->getItem(i))) == 0) {
          ConfigurationManager::getSerialBinary()->send_CMD_HTTPREQUEST(i, this);
          return;
        }
      }
    }
  }
#endif
  HTTPHandler::dispatch();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_home() {
  if (!FlashDictionary::isContentValid()) {
    // no setting stored, execute setup wizard instead of the home page
    get_confwizard();
    return;
  }
  HTTPTemplateResponse response(this, FSTR("home.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_confwifi() {
  HTTPWifiConfigurationResponse response(this, FSTR("configwifi.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_wifiscan() {
  HTTPWiFiScanResponseHTMLRows response(this);
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_confnet() {
  HTTPNetworkConfigurationResponse response(this, FSTR("confignet.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_confserv() {
  HTTPServicesConfigurationResponse response(this, FSTR("confserv.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_confgpio() {
  HTTPGPIOConfigurationResponse response(this, FSTR("confgpio.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_gpio() {
  HTTPGPIOResponseHTML response(this);
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_conftime() {
  HTTPTimeConfigurationResponse response(this, FSTR("conftime.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_reboot() {
  HTTPTemplateResponse response(this, FSTR("reboot.html"));
  reboot(3000); // reboot in 3s
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_restore() {
  if (getRequest().method == HTTPHandler::Get) {
    HTTPTemplateResponse response(this, FSTR("restore.html"));
    response.flush();
  } else {
    ConfigurationManager::restore();
    get_reboot();
  }
}

void MTD_FLASHMEM DefaultHTTPHandler::get_confwizard() {
  HTTPWizardConfigurationResponse response(this, FSTR("confwizard.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_fsbrowser() {
  HTTPFileSystemBrowserResponse response(this, FSTR("fsbrowser.html"));
  response.flush();
}

void MTD_FLASHMEM DefaultHTTPHandler::get_all() {
  HTTPStaticFileResponse response(this, getRequest().requestedPage);
  response.flush();
}
}
