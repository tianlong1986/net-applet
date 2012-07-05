/*
 * Carrick - a connection panel for the Moblin Netbook
 * Copyright (C) 2009 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Written by - Joshua Lock <josh@linux.intel.com>
 *
 */

#ifndef _CARRICK_SERVICE_ITEM_H
#define _CARRICK_SERVICE_ITEM_H

#include <gtk/gtk.h>
#include <nm-connection.h>
#include <nm-device.h>
#include <nm-access-point.h>
#include "network.h"


G_BEGIN_DECLS

#define CARRICK_TYPE_SERVICE_ITEM carrick_service_item_get_type()

#define CARRICK_SERVICE_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CARRICK_TYPE_SERVICE_ITEM, CarrickServiceItem))

#define CARRICK_SERVICE_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CARRICK_TYPE_SERVICE_ITEM, CarrickServiceItemClass))

#define CARRICK_IS_SERVICE_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CARRICK_TYPE_SERVICE_ITEM))

#define CARRICK_IS_SERVICE_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CARRICK_TYPE_SERVICE_ITEM))

#define CARRICK_SERVICE_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CARRICK_TYPE_SERVICE_ITEM, CarrickServiceItemClass))

typedef struct {
  GtkEventBox parent;
} CarrickServiceItem;

typedef struct {
  GtkEventBoxClass parent_class;
} CarrickServiceItemClass;

typedef enum
{
        UN_KNOWN,
        CONFIGURE,
        ACTIVATED,
}ItemState;

GType carrick_service_item_get_type (void);
/*
gint carrick_service_item_get_order (CarrickServiceItem *item);
void carrick_service_item_set_service (CarrickServiceItem *item,
                                       CmService *service);
void carrick_service_item_set_active (CarrickServiceItem *item,
                                      gboolean active);
*/
//GtkWidget* carrick_service_item_new (CarrickIconFactory *icon_factory);
GtkWidget* carrick_service_item_new (void);
void carric_service_item_set_name(GtkWidget *widget,char *name);
void carrick_service_item_set_icon(GtkWidget *widget,GdkPixbuf *pixbuf);
void carrick_service_item_set_type_icon(GtkWidget *widget,GdkPixbuf *pixbuf);
void carric_service_item_set_connect_button_label(GtkWidget *widget,char *name);
ItemState carric_service_item_get_item_state(GtkWidget *widget);
void carric_service_item_set_item_state(GtkWidget *widget,ItemState state);
NMConnection *carric_service_item_get_connection(GtkWidget *widget);
void carric_service_item_set_connection(GtkWidget *widget,NMConnection *connection);
NMAccessPoint *carric_service_item_get_ap(GtkWidget *widget);
void carric_service_item_set_ap(GtkWidget *widget,NMAccessPoint *ap);
NMDevice *carric_service_item_get_device(GtkWidget *widget);
void carric_service_item_set_device(GtkWidget *widget,NMDevice *device);
void carrick_service_item_set_lock_icon(GtkWidget *widget,GdkPixbuf *pixbuf);
void carric_service_item_set_security(GtkWidget *widget,char *name);
void carrick_service_item_set_bg_color(GtkWidget *widget);
void carrick_service_item_set_button_focus(GtkWidget *widget);
void carrick_service_item_free_data(GtkWidget *widget);
G_END_DECLS

#endif /* _CARRICK_SERVICE_ITEM_H */
