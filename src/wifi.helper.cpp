
#include "wifi.helper.h"
#include <WiFi.h> 
#include <esp_wifi.h> 
// #include <esp_event_legacy.h>
// #include <WiFitype.h>

void wifiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            delay(1000);
            ESP.restart();
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            wifi_sta_list_t wifi_sta_list;
            tcpip_adapter_sta_list_t adapter_sta_list;
            
            memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
            memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
 
            esp_wifi_ap_get_sta_list(&wifi_sta_list);
            tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
 
            
            Serial.print("wifi_sta_list.num: ");            
            Serial.println(wifi_sta_list.num);  

            for (int i = 0; i < adapter_sta_list.num; i++) {
                tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

                Serial.print("station nr ");
                Serial.println(i);

                Serial.print("MAC: ");

                for(int i = 0; i< 6; i++){

                    Serial.printf("%02X", station.mac[i]);  
                    if(i<5)Serial.print(":");
                }

                Serial.print("\nIP: ");  
                Serial.println(ip4addr_ntoa(&(station.ip)));
            
                q.push({ millis() + 2000, [&station] {
                    Serial.println("Say hello in 1 second"); 
                    sendHello(IPAddress(station.ip.addr));    
                } });
                //send `hello` to IP over UDP 

            }


            // for (int i = 0; i < wifi_sta_list.num; i++) {
            //     String mac = "";
            //     for (int j = 0; j < 6; j++) {
            //         String part = String(wifi_sta_list.sta[i].mac[j],HEX);
            //         if (part.length() == 1) {
            //             part = "0"+part;
            //         }
            //         mac += part;
            //     }

            //     // for(int i = 0; i< 6; i++){
                    
            //     // Serial.printf("%02X", station.mac[i]);  
            //     // if(i<5)Serial.print(":");
            //     // }


            //     Serial.print("MAC: ");
            //     Serial.println(mac);

            //     // Serial.print("\nIP: ");  
            //     // Serial.println(ip4addr_ntoa(&(wifi_sta_list.sta[i].ip)));

            //     // wifi_sta_list.sta[i].mac
            // }          


//   wifi_sta_info_t sta[ESP_WIFI_MAX_CONN_NUM]; /**< station list */
    // int       num; 


            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default: break;
    }
  
  }
  
