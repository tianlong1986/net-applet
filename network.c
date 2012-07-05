/*
 * Compile with:
 * gcc -Wall `pkg-config --libs --cflags glib-2.0 dbus-glib-1 libnm-util libnm-glib` main.c -o network
 */
#include "network.h"
enum encryption_type { 
        ET_NULL = -1,   
        ET_NONE,                /* 0 */
        ET_WEP,                 /* 1 */
        ET_WPA_WPA2_PERSONAL,    /* 2 */
        ET_WPA_WPA2_ENTERPRISE  /* 3 */ 
};  

static NMClient* client;

void
fill_wpa_psk_security_setting (NMSettingWirelessSecurity * s_wireless_sec, 
                                NMSettingWireless * s_wireless,const char * key);
const char* 
get_ssid_from_ap(NMAccessPoint *ap)
{
	const GByteArray *ssid; 
	ssid = nm_access_point_get_ssid (ap);
        return nm_utils_ssid_to_utf8(ssid);
}
guint8
get_strength_from_ap(NMAccessPoint *ap)
{
	return nm_access_point_get_strength(ap);
}
NMClient*
get_nm_client()
{
	return client;
}

void 
_connect_button_cb(GtkWidget *widget,gpointer user_data)
{
        //ItemState state; 
        NMDevice *device = NULL;
        NMConnection *connection = NULL;
        NMActiveConnection *active_con = NULL;
        const char *specific_object = NULL;
	NMAccessPoint *ap;
        CarrickServiceItem *item = CARRICK_SERVICE_ITEM(user_data);
        device = carric_service_item_get_device(GTK_WIDGET(item));
	ap = carric_service_item_get_ap(item);
	if (NM_IS_DEVICE_ETHERNET (device)){
        //        wired_info = g_object_get_data(G_OBJECT(item),"connection-info");
        //        applet = wired_info->applet;
        //        connection = wired_info->connection;
        }else if (NM_IS_DEVICE_WIFI (device)){
		connect_wifi(device, get_nm_client(), ap);
	}
}

NMDevice *
get_wireless_device()
{
	int i;
//	NMClient* client;
	const GPtrArray *devices;
/*	client = nm_client_new ();
	if (!client) {
		g_message ("Error: Could not create NMClient.");
		return NULL;
	}
*/
        /* Get all devices managed by NetworkManager */
        devices = nm_client_get_devices (client);

        /* Go through the array and process Wi-Fi devices */
      for(i = 0; devices && (i < devices->len); i++) {
             NMDevice *device = g_ptr_array_index (devices, i);
             if (NM_IS_DEVICE_WIFI (device))
             {
                   return device;
             }
        }
	return NULL;
}

GPtrArray *
get_all_wirelss_aps(NMDevice* device)
{

	const GPtrArray *aps;
	g_return_val_if_fail(device != NULL, NULL);
	aps = nm_device_wifi_get_access_points (NM_DEVICE_WIFI (device));
	return aps;
}
/*
{
        flags = nm_access_point_get_flags (ap);
        wpa_flags = nm_access_point_get_wpa_flags (ap);
        rsn_flags = nm_access_point_get_rsn_flags (ap);
        hwaddr = nm_access_point_get_hw_address (ap);
        freq = nm_access_point_get_frequency (ap);
        mode = nm_access_point_get_mode (ap);
        bitrate = nm_access_point_get_max_bitrate (ap);
        strength = nm_access_point_get_strength (ap);

}*/
enum encryption_type 
get_encrypt_type_from_ap(NMAccessPoint *ap)
{
	guint32 flags, wpa_flags, rsn_flags;
	flags = nm_access_point_get_flags (ap);
        wpa_flags = nm_access_point_get_wpa_flags (ap);
        rsn_flags = nm_access_point_get_rsn_flags (ap);
        if ( !(flags & NM_802_11_AP_FLAGS_PRIVACY)
	    && (wpa_flags == NM_802_11_AP_SEC_NONE)
            && (rsn_flags == NM_802_11_AP_SEC_NONE))
		return ET_NULL;

        if ((flags & NM_802_11_AP_FLAGS_PRIVACY)
	    && (wpa_flags == NM_802_11_AP_SEC_NONE)
            && (rsn_flags == NM_802_11_AP_SEC_NONE))
		return ET_WEP;
        if (wpa_flags != NM_802_11_AP_SEC_NONE)
		return ET_WPA_WPA2_PERSONAL;
        if (rsn_flags != NM_802_11_AP_SEC_NONE)
		return ET_WPA_WPA2_PERSONAL;
        if (   (wpa_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
            || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X))
		return ET_WPA_WPA2_PERSONAL;
	return ET_NULL;
}
/* Convert flags to string */
static char *
ap_wpa_rsn_flags_to_string (guint32 flags)
{
	char *flags_str[16]; /* Enough space for flags and terminating NULL */
	char *ret_str;
	int i = 0;

	if (flags & NM_802_11_AP_SEC_PAIR_WEP40)
		flags_str[i++] = g_strdup ("pair_wpe40");
	if (flags & NM_802_11_AP_SEC_PAIR_WEP104)
		flags_str[i++] = g_strdup ("pair_wpe104");
	if (flags & NM_802_11_AP_SEC_PAIR_TKIP)
		flags_str[i++] = g_strdup ("pair_tkip");
	if (flags & NM_802_11_AP_SEC_PAIR_CCMP)
		flags_str[i++] = g_strdup ("pair_ccmp");
	if (flags & NM_802_11_AP_SEC_GROUP_WEP40)
		flags_str[i++] = g_strdup ("group_wpe40");
	if (flags & NM_802_11_AP_SEC_GROUP_WEP104)
		flags_str[i++] = g_strdup ("group_wpe104");
	if (flags & NM_802_11_AP_SEC_GROUP_TKIP)
		flags_str[i++] = g_strdup ("group_tkip");
	if (flags & NM_802_11_AP_SEC_GROUP_CCMP)
		flags_str[i++] = g_strdup ("group_ccmp");
	if (flags & NM_802_11_AP_SEC_KEY_MGMT_PSK)
		flags_str[i++] = g_strdup ("psk");
	if (flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
		flags_str[i++] = g_strdup ("802.1X");

	if (i == 0)
		flags_str[i++] = g_strdup ("none");

	flags_str[i] = NULL;

	ret_str = g_strjoinv (" ", flags_str);

	i = 0;
	while (flags_str[i])
		 g_free (flags_str[i++]);

	return ret_str;
}

static void
show_access_point_info (NMAccessPoint *ap,int i)
{
	guint32 flags, wpa_flags, rsn_flags, freq, bitrate;
	guint8 strength;
	const GByteArray *ssid; 
	const char *hwaddr;
	NM80211Mode mode;
	char *freq_str, *ssid_str, *bitrate_str, *strength_str, *wpa_flags_str, *rsn_flags_str;
	GString *security_str;

	/* Get AP properties */
	flags = nm_access_point_get_flags (ap);
	wpa_flags = nm_access_point_get_wpa_flags (ap);
	rsn_flags = nm_access_point_get_rsn_flags (ap);
	ssid = nm_access_point_get_ssid (ap);
	hwaddr = nm_access_point_get_hw_address (ap);
	freq = nm_access_point_get_frequency (ap);
	mode = nm_access_point_get_mode (ap);
	bitrate = nm_access_point_get_max_bitrate (ap);
	strength = nm_access_point_get_strength (ap);

	/* Convert to strings */
	ssid_str = nm_utils_ssid_to_utf8 (ssid);
	freq_str = g_strdup_printf ("%u MHz", freq);
	bitrate_str = g_strdup_printf ("%u MB/s", bitrate/1000);
	strength_str = g_strdup_printf ("%u", strength);
	wpa_flags_str = ap_wpa_rsn_flags_to_string (wpa_flags);
	rsn_flags_str = ap_wpa_rsn_flags_to_string (rsn_flags);

	security_str = g_string_new (NULL);
	if (   !(flags & NM_802_11_AP_FLAGS_PRIVACY)
	    &&  (wpa_flags != NM_802_11_AP_SEC_NONE)
	    &&  (rsn_flags != NM_802_11_AP_SEC_NONE))
		g_string_append (security_str, "Encrypted: ");

	if (   (flags & NM_802_11_AP_FLAGS_PRIVACY)
	    && (wpa_flags == NM_802_11_AP_SEC_NONE)
	    && (rsn_flags == NM_802_11_AP_SEC_NONE))
		g_string_append (security_str, "WEP ");
	if (wpa_flags != NM_802_11_AP_SEC_NONE)
		g_string_append (security_str, "WPA ");
	if (rsn_flags != NM_802_11_AP_SEC_NONE)
		g_string_append (security_str, "WPA2 ");
	if (   (wpa_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
	    || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X))
		g_string_append (security_str, "Enterprise ");

	if (security_str->len > 0)
		g_string_truncate (security_str, security_str->len-1);  /* Chop off last space */

	printf ("SSID:       %s\n", ssid_str);
	printf ("tomID:       %d\n", i);
//	printf ("BSSID:      %s\n", hwaddr);
//	printf ("Mode:       %s\n", mode == NM_802_11_MODE_ADHOC ? "Ad-Hoc" : mode == NM_802_11_MODE_INFRA ? "Infrastructure" : "Unknown");
//	printf ("Freq:       %s\n", freq_str);
//	printf ("Bitrate:    %s\n", bitrate_str);
//	printf ("Strength:   %s\n", strength_str);
	printf ("Security:   %s\n", security_str->str);
	printf ("WPA flags:  %s\n", wpa_flags_str);
	printf ("RSN flags:  %s\n", rsn_flags_str);
//	printf ("D-Bus path: %s\n\n", nm_object_get_path (NM_OBJECT (ap)));

	g_free (ssid_str);
	g_free (freq_str);
	g_free (bitrate_str);
	g_free (strength_str);
	g_free (wpa_flags_str);
	g_free (rsn_flags_str);
	g_string_free (security_str, TRUE);
}
const GPtrArray *aps;
static void
show_wifi_device_info (NMDevice *device)
{
	NMAccessPoint *active_ap = NULL;
	const char *iface;
	const char *driver;
	guint32 speed;
	const GByteArray *active_ssid; 
	char *active_ssid_str = NULL;
	int i;

	/* Get active AP */
	if (nm_device_get_state (device) == NM_DEVICE_STATE_ACTIVATED) {
		if ((active_ap = nm_device_wifi_get_active_access_point (NM_DEVICE_WIFI (device)))) {
			active_ssid = nm_access_point_get_ssid (active_ap);
			active_ssid_str = nm_utils_ssid_to_utf8 (active_ssid);
		}
	}

        iface = nm_device_get_iface (device);
        driver = nm_device_get_driver (device);
	speed = nm_device_wifi_get_bitrate (NM_DEVICE_WIFI (device));
	speed /= 1000;

	printf ("Device: %s  ----  Driver: %s  ----  Speed: %d MB/s  ----  Active AP: %s\n",
	         iface, driver, speed, active_ssid_str ? active_ssid_str : "none");
	printf ("=================================================================================\n");
	g_free (active_ssid_str);

	/* Get all APs of the Wi-Fi device */
	aps = nm_device_wifi_get_access_points (NM_DEVICE_WIFI (device));

	/* Print AP details */
	for (i = 0; aps && (i < aps->len); i++) {
		NMAccessPoint *ap = g_ptr_array_index (aps, i);
		show_access_point_info (ap,i);
	}
}

NMConnection* create_wired_auto_connection()
{
	NMConnection *connection;
	NMSetting *setting;
	setting = nm_setting_wired_new ();
	g_return_val_if_fail(setting != NULL, FALSE);
	connection = nm_connection_new();
	g_return_val_if_fail(connection != NULL, FALSE);
	nm_connection_add_setting(connection, setting);
	return connection;
}

NMConnection* create_wifi_auto_connection(char* id)
{
	NMConnection *connection;
	NMSettingConnection * s_con;
        s_con = (NMSettingConnection *)nm_setting_connection_new ();
	g_return_val_if_fail(s_con != NULL, FALSE);
	g_object_set (G_OBJECT (s_con), NM_SETTING_CONNECTION_ID, id, NULL);
        g_object_set (G_OBJECT (s_con), NM_SETTING_CONNECTION_UUID, nm_utils_uuid_generate (), NULL);
	g_object_set (G_OBJECT (s_con), NM_SETTING_CONNECTION_AUTOCONNECT, TRUE, NULL);
  	g_object_set (G_OBJECT (s_con), NM_SETTING_CONNECTION_TYPE, NM_SETTING_WIRELESS_SETTING_NAME, NULL);

	connection = nm_connection_new();
	g_return_val_if_fail(connection != NULL, FALSE);

        nm_connection_add_setting (connection, (NMSetting *)s_con);

	return connection;
}



gboolean 
connect_wired(NMDevice *device, NMClient* client)
{
	NMConnection* connection;
	g_return_val_if_fail(client != NULL, FALSE);
	connection = create_wired_auto_connection();
	g_return_val_if_fail(connection != NULL, FALSE);
	g_message("next is add and activate wired connection");
	nm_client_add_and_activate_connection(client, connection, device, NULL, NULL, NULL);
	return TRUE;
}

static void
wifi_add_and_activate_cb (NMClient *client,
                                NMActiveConnection *active,
                                const char *connection_path,
                                GError *error,
                                gpointer user_data)
{
        if (error)
                g_warning ("Failed to add/activate connection: (%d) %s", error->code, error->message);
	g_message("activate callback");
}

void 
set_wifi_security(NMConnection *connection,enum encryption_type encry_type)
{
        NMSettingWirelessSecurity * s_wireless_sec;
        NMSettingWireless * s_wireless;
	//char key[15];
	char *key;
        s_wireless_sec = (NMSettingWirelessSecurity *) nm_setting_wireless_security_new ();
        s_wireless = (NMSettingWireless *)nm_setting_wireless_new ();
	if(encry_type == ET_WEP)
	{
	        g_object_set (s_wireless_sec,
                      NM_SETTING_WIRELESS_SECURITY_KEY_MGMT, "none",
                      NM_SETTING_WIRELESS_SECURITY_WEP_TX_KEYIDX, 0,
                      NM_SETTING_WIRELESS_SECURITY_AUTH_ALG, 0 ? "shared" : "open",
                      NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE, NM_WEP_KEY_TYPE_KEY,
                      NULL);
		g_print("please the WEP password:\n");
		//scanf("%s", key);
		key = g_strdup_printf("98765");
		g_message("your password is %s",key);
	        nm_setting_wireless_security_set_wep_key (s_wireless_sec, 0, key);
	}else if(encry_type == ET_WPA_WPA2_PERSONAL)
	{
		g_print("please the WPA password:\n");
		//scanf("%s", key);
		key = g_strdup_printf("warrencoles");
		g_message("your password is %s",key);
		g_assert(s_wireless);
	       	g_object_set (s_wireless, NM_SETTING_WIRELESS_MODE, "infrastructure", NULL);
	        g_object_set (s_wireless, NM_SETTING_WIRELESS_SEC, NM_SETTING_WIRELESS_SECURITY_SETTING_NAME, NULL);
                nm_connection_add_setting (connection, (NMSetting *)s_wireless);
        	g_object_set (s_wireless_sec, NM_SETTING_WIRELESS_SECURITY_PSK, key, NULL);
        	g_object_set (s_wireless_sec, NM_SETTING_WIRELESS_SECURITY_KEY_MGMT, "wpa-psk", NULL);
	}
	if (s_wireless_sec)
                nm_connection_add_setting (connection, (NMSetting *)s_wireless_sec);
}
gboolean 
connect_wifi(NMDevice *device, NMClient* client, NMAccessPoint *ap)
{
	NMConnection* connection;
	const GByteArray *ssid; 
	char * ssid_str;
	enum encryption_type encry_type;
	//NMAccessPoint *ap = g_ptr_array_index (aps, id);
	ssid = nm_access_point_get_ssid (ap);
	ssid_str = nm_utils_ssid_to_utf8 (ssid);
	encry_type = get_encrypt_type_from_ap(ap);
	g_message("now you want to connect :%s", ssid_str);	
	g_return_val_if_fail(client != NULL, FALSE);
	connection = create_wifi_auto_connection(ssid_str);
	set_wifi_security(connection,encry_type);
	g_return_val_if_fail(connection != NULL, FALSE);
	g_message("next is add and activate wireless connection");
	nm_client_add_and_activate_connection(client,
					      connection, 
					      device,
					      nm_object_get_path (NM_OBJECT (ap)),
					      wifi_add_and_activate_cb, 
					      NULL);
	return TRUE;
}
//int main (int argc, char *argv[])
int network_init()
{
	DBusGConnection *bus;
//	NMClient *client;
	const GPtrArray *devices;
	int i;

	/* Initialize GType system */
	g_type_init ();

	/* Get system bus */
	bus = dbus_g_bus_get (DBUS_BUS_SYSTEM, NULL);

	/* Get NMClient object */
	client = nm_client_new ();
	if (!client) {
		dbus_g_connection_unref (bus);
		g_message ("Error: Could not create NMClient.");
		return EXIT_FAILURE;
	}

	/* Get all devices managed by NetworkManager */
	devices = nm_client_get_devices (client);

//	NMDevice *wifiDevice = NULL;
	/* Go through the array and process Wi-Fi devices */
/*	for (i = 0; devices && (i < devices->len); i++) {
		NMDevice *device = g_ptr_array_index (devices, i);
		if (NM_IS_DEVICE_WIFI (device))
		{
			show_wifi_device_info (device);
			wifiDevice = device;
		}
	}

	g_print("Now the device num is %d\n", i);
	
	if(NM_IS_DEVICE_WIFI(wifiDevice));
	{
		int Id;
		g_message("is wireless device,please input which wifi to connect:");
		scanf("%d",&Id);	
		connect_wifi(wifiDevice, client,Id);	
		g_usleep(9000000);
	}
	g_message("exit 0");
	g_object_unref (client);
	dbus_g_connection_unref (bus);
*/
	return EXIT_SUCCESS;
}
gboolean
is_encrypted_ap(NMAccessPoint* ap)
{
        guint32 flags, wpa_flags, rsn_flags;
        gboolean encrypted = FALSE;

        flags = nm_access_point_get_flags (ap);
        wpa_flags = nm_access_point_get_wpa_flags (ap);
        rsn_flags = nm_access_point_get_rsn_flags (ap);

        if (   (flags & NM_802_11_AP_FLAGS_PRIVACY)
                || (wpa_flags != NM_802_11_AP_SEC_NONE)
                || (rsn_flags != NM_802_11_AP_SEC_NONE))
                encrypted = TRUE;
	else
		return FALSE;
}

void
fill_wpa_psk_security_setting (NMSettingWirelessSecurity * s_wireless_sec, 
				NMSettingWireless * s_wireless,const char * key)
{
        GtkWidget * widget;
        GtkBuilder * builder;
        g_object_set (s_wireless, NM_SETTING_WIRELESS_SEC, NM_SETTING_WIRELESS_SECURITY_SETTING_NAME, NULL);


        g_object_set (s_wireless_sec, NM_SETTING_WIRELESS_SECURITY_PSK, key, NULL);

        g_object_set (s_wireless_sec, NM_SETTING_WIRELESS_SECURITY_KEY_MGMT, "wpa-psk", NULL);

}
