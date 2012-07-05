//gcc -o gtkwin gtkwin.c `pkg-config --libs --cflags gtk+-2.0`
#include<gtk/gtk.h>

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
#include "carrick-item.h"
#define ICON_LOAD(x, y) \             
        { \                     
                char path[255]; \
                sprintf(path,"/home/user/icon/%s.png",y); \
                x = gdk_pixbuf_new_from_file (path,&err); \
        }

GdkPixbuf* get_strength_icon_from_ap(NMAccessPoint *ap) 
{
	guint8 strength;
        GdkPixbuf *pixbuf = NULL;
	GError *err = NULL;
        strength = nm_access_point_get_strength (ap);	
        strength = CLAMP (strength, 0, 100);
        if (strength > 80)
	{
		ICON_LOAD(pixbuf, "carrick_wireless_100_icon");
	}
        else if (strength > 55)
	{
		ICON_LOAD(pixbuf, "carrick_wireless_75_icon");
	}
        else if (strength > 30)
	{
		ICON_LOAD(pixbuf, "carrick_wireless_50_icon");
	}
        else if (strength > 5)
	{
		ICON_LOAD(pixbuf, "carrick_wireless_25_icon");
	}
        else
	{
		ICON_LOAD(pixbuf, "carrick_wireless_00_icon");
	}
        return pixbuf;
}

GtkWidget*
create_wifi_item(NMAccessPoint* ap)
{
	GtkWidget *item;
	GError *err=NULL;
	GdkPixbuf* wifi_pixbuf;
	GdkPixbuf* strength_pixbuf;
	GdkPixbuf* encrypted_pixbuf;
	gboolean encrypted;
	NMDevice* device;
	char* ssid;
	ssid = get_ssid_from_ap(ap);
	strength_pixbuf = get_strength_icon_from_ap(ap);
	ICON_LOAD(wifi_pixbuf,"nm-type-wireless");
	encrypted = is_encrypted_ap(ap);
	item = carrick_service_item_new();
	g_return_val_if_fail(item != NULL, NULL);
	device = get_wireless_device();
	g_return_val_if_fail(device != NULL, NULL);
	carric_service_item_set_name(item, ssid);
	carric_service_item_set_connect_button_label(item, "Connect");
	carrick_service_item_set_icon(item, wifi_pixbuf);
	carrick_service_item_set_type_icon(item,strength_pixbuf);
	carric_service_item_set_device(item, device);
	carric_service_item_set_ap(item, ap);
	if(encrypted)
	{
		ICON_LOAD(encrypted_pixbuf, "carrick_encrypted");	
		carrick_service_item_set_lock_icon(item, encrypted_pixbuf);
	}
	return item;
}
gboolean  add_all_wireless_connection(GtkWidget* vbox)
{
	GPtrArray *aps;
	GtkWidget *widget;
	int i;
	aps = get_all_wirelss_aps(get_wireless_device());
        for (i = 0; aps && (i < aps->len); i++) {
                NMAccessPoint *ap = g_ptr_array_index (aps, i);
		widget = create_wifi_item(ap);
		g_return_val_if_fail(widget != NULL, FALSE);
	        gtk_box_pack_start (GTK_BOX (vbox),
              		        widget,
                     		FALSE,
                 		FALSE,
                      		20);
        }
	return TRUE;
	
}
gboolean add_all_wired_connection(GtkWidget* vbox)
{

}
static gboolean
add_all_connection_item(GtkWidget* vbox)
{
	add_all_wireless_connection(vbox);	
	return TRUE;
	
}
void destroy_progress( GtkWidget *widget,void *pdata)
{
        gtk_main_quit ();
}

int main(int argc,char * *argv)
{
 GtkWidget *window;/*定义指向控件的指针*/
 GtkWidget *vbox;
 GSettings * launchmanager;
 int i;
 gtk_init(&argc,&argv);/*初始化图形显示环境*/
 window=gtk_window_new(GTK_WINDOW_TOPLEVEL);/*创建窗口*/
 network_init();
 vbox = gtk_vbox_new(TRUE,6);
 gtk_container_add(GTK_CONTAINER(window),vbox);
// gtk_widget_set_size_request (GTK_WIDGET (window), 200, 50);
        gtk_window_set_resizable (GTK_WINDOW (window), TRUE);

        gtk_window_set_title (GTK_WINDOW (window),"Network Setting");
        gtk_container_set_border_width (GTK_CONTAINER(window),0);
        gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
 add_all_connection_item(vbox);
 gtk_widget_show_all(window);
        g_signal_connect (G_OBJECT (window), "destroy",
        G_CALLBACK (destroy_progress),NULL);


        gtk_main();
 return 0;
} 
