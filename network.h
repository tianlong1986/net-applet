#include <glib.h>
#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>

#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include <nm-device-ethernet.h>
#include <nm-setting-connection.h>
#include <nm-access-point.h>
#include <NetworkManager.h>
#include <nm-utils.h>
#include <gtk/gtk.h>
#include "carrick-item.h"

#define WPA_PMK_LEN 32


const char* get_ssid_from_ap(NMAccessPoint *ap);
guint8 get_strength_from_ap(NMAccessPoint *ap);
NMDevice * get_wireless_device();
GPtrArray * get_all_wirelss_aps(NMDevice* device);
gboolean is_encrypted_ap(NMAccessPoint* ap);
NMClient* get_nm_client();
void _connect_button_cb(GtkWidget *widget,gpointer user_data);

void
fill_wpa_psk_security_setting (NMSettingWirelessSecurity * s_wireless_sec,                    
                                NMSettingWireless * s_wireless,const char * key);
