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

#include "carrick-item.h"

#include <glib/gi18n.h>

G_DEFINE_TYPE (CarrickServiceItem, carrick_service_item, GTK_TYPE_EVENT_BOX)

#define SERVICE_ITEM_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CARRICK_TYPE_SERVICE_ITEM, CarrickServiceItemPrivate))

typedef struct _CarrickServiceItemPrivate CarrickServiceItemPrivate;

enum
{
  PROP_0,
  PROP_ICON_FACTORY,
};

struct _CarrickServiceItemPrivate
{
 
  NMConnection	     *connection;
  NMDevice	     *device;
  NMAccessPoint	     *ap;
  GtkWidget          *icon;
  GtkWidget          *type_icon;
  GtkWidget          *lock_icon;
  GtkWidget          *name_label;
  GtkWidget          *security_label;
  GtkWidget          *connect_button;
  GtkWidget          *expando;
  GtkWidget          *ebox;
  ItemState    state;
  //CarrickIconFactory *icon_factory;
  gboolean            hover;
  GdkColor            prelight_color;
  GdkColor            active_color;

};

enum
{
  SIGNAL_ITEM_ACTIVATE,
  SIGNAL_LAST
};

static gint service_item_signals[SIGNAL_LAST];

void carrick_service_item_free_data(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);

	if(priv->name_label)
                gtk_widget_destroy(priv->name_label);
	if(priv->icon)
                gtk_widget_destroy(priv->icon);
	if(priv->type_icon)
                gtk_widget_destroy(priv->type_icon);
	if(priv->lock_icon)
                gtk_widget_destroy(priv->lock_icon);
	if(priv->security_label)
                gtk_widget_destroy(priv->security_label);
	if(priv->connect_button)
                gtk_widget_destroy(priv->connect_button);
	if(priv->expando)
                gtk_widget_destroy(priv->expando);
	if(priv->ebox)
                gtk_widget_destroy(priv->ebox);
}

NMDevice *carric_service_item_get_device(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
  	g_return_val_if_fail (widget != NULL,NULL);
  	g_return_val_if_fail (CARRICK_IS_SERVICE_ITEM (widget),NULL);
	priv = SERVICE_ITEM_PRIVATE (widget);
	return priv->device;

}
void carric_service_item_set_device(GtkWidget *widget,NMDevice *device)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	priv->device = device;

}

NMConnection *carric_service_item_get_connection(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
  	g_return_val_if_fail (widget != NULL,NULL);
  	g_return_val_if_fail (CARRICK_IS_SERVICE_ITEM (widget),NULL);
	priv = SERVICE_ITEM_PRIVATE (widget);
	return priv->connection;

}
void carric_service_item_set_connection(GtkWidget *widget,NMConnection *connection)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	priv->connection = connection;

}

NMAccessPoint *carric_service_item_get_ap(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
  	g_return_val_if_fail (widget != NULL,NULL);
  	g_return_val_if_fail (CARRICK_IS_SERVICE_ITEM (widget),NULL);
	priv = SERVICE_ITEM_PRIVATE (widget);
	return priv->ap;

}
void carric_service_item_set_ap(GtkWidget *widget,NMAccessPoint *ap)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	priv->ap = ap;

}
void carric_service_item_set_name(GtkWidget *widget,char *name)
{
	CarrickServiceItemPrivate *priv;

        PangoLayout *layout = NULL;
        GdkScreen * ServerScreen;
        int default_sWidth;
        int Width = 0;

  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));

	priv = SERVICE_ITEM_PRIVATE (widget);

        layout = gtk_widget_create_pango_layout (priv->name_label, name);
        pango_layout_get_pixel_size(layout, &Width, NULL);
        ServerScreen = gdk_screen_get_default();
        default_sWidth  = gdk_screen_get_width (ServerScreen);

        if(Width >= default_sWidth * 5 /22){
                gtk_widget_set_size_request(priv->name_label, default_sWidth * 5/22, -1);
        }else{
                gtk_widget_set_size_request(priv->name_label, -1, -1);
        }
	gtk_label_set_text (GTK_LABEL (priv->name_label),name);
        g_object_unref(layout);

}

void carric_service_item_set_security(GtkWidget *widget,char *name)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_label_set_text (GTK_LABEL (priv->security_label),name);

}

void carric_service_item_set_connect_button_label(GtkWidget *widget,char *name)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_button_set_label(GTK_BUTTON(priv->connect_button),name);

}
ItemState carric_service_item_get_item_state(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
	priv = SERVICE_ITEM_PRIVATE (widget);
	return priv->state;
}
void carric_service_item_set_item_state(GtkWidget *widget,ItemState state)
{
	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	priv->state = state;
}
void carrick_service_item_set_icon(GtkWidget *widget,GdkPixbuf *pixbuf)
{

	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_image_set_from_pixbuf (GTK_IMAGE(priv->icon),pixbuf);
}		
void carrick_service_item_set_type_icon(GtkWidget *widget,GdkPixbuf *pixbuf)
{

	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_image_set_from_pixbuf (GTK_IMAGE(priv->type_icon),pixbuf);
}
void carrick_service_item_set_lock_icon(GtkWidget *widget,GdkPixbuf *pixbuf)
{

	CarrickServiceItemPrivate *priv;
  	g_return_if_fail (widget != NULL);
  	g_return_if_fail (CARRICK_IS_SERVICE_ITEM (widget));
	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_image_set_from_pixbuf (GTK_IMAGE(priv->lock_icon),pixbuf);
}		
void carrick_service_item_set_bg_color(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
    	priv = SERVICE_ITEM_PRIVATE (widget);

        gtk_widget_modify_bg (priv->ebox,
                          GTK_STATE_NORMAL,
                          &priv->active_color);
}
void carrick_service_item_set_button_focus(GtkWidget *widget)
{
	CarrickServiceItemPrivate *priv;
    	priv = SERVICE_ITEM_PRIVATE (widget);
	gtk_widget_grab_focus(priv->connect_button);

}
static void
carrick_service_item_get_property (GObject *object, guint property_id,
                                   GValue *value, GParamSpec *pspec)
{
  CarrickServiceItemPrivate *priv;

  g_return_if_fail (object != NULL);
  g_return_if_fail (CARRICK_IS_SERVICE_ITEM (object));

  priv = SERVICE_ITEM_PRIVATE (object);

  switch (property_id)
    {
      case PROP_ICON_FACTORY:
 //       g_value_set_object (value, priv->icon_factory);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}
static void
carrick_service_item_set_property (GObject *object, guint property_id,
                                   const GValue *value, GParamSpec *pspec)
{
//  CarrickServiceItemPrivate *priv = SERVICE_ITEM_PRIVATE (object);
  g_return_if_fail (object != NULL);
  g_return_if_fail (CARRICK_IS_SERVICE_ITEM (object));

  switch (property_id)
    {
      case PROP_ICON_FACTORY:
       // priv->icon_factory = CARRICK_ICON_FACTORY (g_value_get_object (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
carrick_service_item_dispose (GObject *object)
{
  G_OBJECT_CLASS (carrick_service_item_parent_class)->dispose (object);
}

static void
carrick_service_item_finalize (GObject *object)
{
  G_OBJECT_CLASS (carrick_service_item_parent_class)->finalize (object);
}

static gboolean
carrick_service_item_enter_notify_event (GtkWidget        *widget,
                                         GdkEventCrossing *event)
{
  CarrickServiceItemPrivate *priv;

  priv = SERVICE_ITEM_PRIVATE (widget);

  priv->hover = TRUE;

  gtk_widget_modify_bg (priv->ebox,
                        GTK_STATE_NORMAL,
                        &priv->prelight_color);
  /*gdk_window_set_cursor (widget->window, priv->hand_cursor);*/

  return TRUE;
}

static gboolean
carrick_service_item_leave_notify_event (GtkWidget        *widget,
                                         GdkEventCrossing *event)
{
  /* if pointer moves to a child widget, we want to keep the
   * ServiceItem prelighted, but show the normal cursor */
  GdkColor white;
  gdk_color_parse("#DFDFDF", &white);
  if (event->detail == GDK_NOTIFY_INFERIOR)
  {
    gdk_window_set_cursor (widget->window, NULL);
  }
  else
  {
    CarrickServiceItemPrivate *priv;

    priv = SERVICE_ITEM_PRIVATE (widget);

    priv->hover = FALSE;

    if (priv->state == ACTIVATED)
    {
    	gtk_widget_modify_bg (priv->ebox,
                          GTK_STATE_NORMAL,
                          &priv->active_color);
    }else{
    	gtk_widget_modify_bg (priv->ebox,
                              GTK_STATE_NORMAL,
                              &white);
    }
  }

  return TRUE;
}

static void
carrick_service_item_class_init (CarrickServiceItemClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (CarrickServiceItemPrivate));

  object_class->get_property = carrick_service_item_get_property;
  object_class->set_property = carrick_service_item_set_property;
  object_class->dispose = carrick_service_item_dispose;
  object_class->finalize = carrick_service_item_finalize;

  widget_class->enter_notify_event = carrick_service_item_enter_notify_event;
  widget_class->leave_notify_event = carrick_service_item_leave_notify_event;


/*
  pspec = g_param_spec_object ("icon-factory",
                               "icon-factory",
                               "CarrickIconFactory object",
                               CARRICK_TYPE_ICON_FACTORY,
                               G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
  g_object_class_install_property (object_class,
                                   PROP_ICON_FACTORY,
                                   pspec);
*/
  /* activated == some ui activity in the label part of the item */
  service_item_signals[SIGNAL_ITEM_ACTIVATE] = g_signal_new (
    "activate",
    G_TYPE_FROM_CLASS (object_class),
    G_SIGNAL_RUN_LAST,
    0,
    NULL, NULL,
    g_cclosure_marshal_VOID__VOID,
    G_TYPE_NONE, 0);

}
static void
carrick_service_item_init (CarrickServiceItem *self)
{
  CarrickServiceItemPrivate *priv = SERVICE_ITEM_PRIVATE (self);
  GtkWidget *box,*vbox;
  GdkColor   white;
  gdk_color_parse ("#DFDFDF", &white);
  priv->connection = NULL;
  box = gtk_hbox_new (FALSE,
                      6);
  gtk_widget_show (box);
  vbox = gtk_vbox_new(FALSE,2);
  gtk_widget_show (vbox);
  //priv->expando = nbtk_gtk_expander_new ();
  priv->expando = gtk_frame_new ("");
  gtk_frame_set_label_widget(GTK_FRAME(priv->expando), NULL);
  priv->ebox = gtk_event_box_new();
  gtk_widget_modify_bg(priv->ebox,
                       GTK_STATE_NORMAL,
                       &white);
  gtk_container_add (GTK_CONTAINER (self),
                     priv->ebox);
  gtk_container_add (GTK_CONTAINER (priv->ebox),
                     priv->expando);
  gtk_container_add(GTK_CONTAINER(priv->expando), box);
/*
  nbtk_gtk_expander_set_label_widget (NBTK_GTK_EXPANDER (priv->expando),
                                      box);
  nbtk_gtk_expander_set_has_indicator (NBTK_GTK_EXPANDER (priv->expando),
                                       FALSE);
*/
  gtk_widget_show (priv->ebox);
  gtk_widget_show (priv->expando);

  gdk_color_parse ("#e8e8e8", &priv->active_color);
  gdk_color_parse ("#FFFFFF", &priv->active_color);
  gdk_color_parse ("#cbcbcb", &priv->prelight_color);
//  priv->icon_factory = carrick_icon_factory_new();
  priv->icon = gtk_image_new ();
  gtk_widget_show (priv->icon);
  gtk_widget_set_size_request(priv->icon,40,-1);
  gtk_box_pack_start (GTK_BOX (box),
                      priv->icon,
                      FALSE,
                      FALSE,
                      6);
  priv->type_icon = gtk_image_new ();
  gtk_widget_show (priv->type_icon);
  gtk_box_pack_start (GTK_BOX (box),
                      priv->type_icon,
                      FALSE,
                      FALSE,
                      6);


  priv->name_label = gtk_label_new (" ");
  gtk_label_set_line_wrap (GTK_LABEL(priv->name_label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (priv->name_label),
                          0.0, 0.5);
  gtk_widget_show (priv->name_label);
  gtk_box_pack_start (GTK_BOX (box),
                      priv->name_label,
                      TRUE,
                      TRUE,
                      2);
  gtk_box_pack_end (GTK_BOX (box),
                      vbox,
                      FALSE,
                      FALSE,
                      4);

  priv->connect_button = gtk_button_new ();
  gtk_button_set_label(GTK_BUTTON(priv->connect_button),_("Connect"));
  gtk_widget_set_size_request(priv->connect_button,115,-1);
  gtk_widget_show (priv->connect_button);
  g_signal_connect (priv->connect_button,
                    "clicked",
                    G_CALLBACK (_connect_button_cb),
                    self);
   gtk_box_pack_end (GTK_BOX (vbox),
                      priv->connect_button,
                      TRUE,
                      TRUE,
                      8);
   priv->lock_icon = gtk_image_new ();
   gtk_widget_show (priv->lock_icon);
   gtk_box_pack_end (GTK_BOX (box),
                      priv->lock_icon,
                      FALSE,
                      FALSE,
                      12);

  priv->security_label = gtk_label_new (" ");
  gtk_misc_set_alignment (GTK_MISC (priv->security_label),
                          0.0, 0.5);
  gtk_widget_show (priv->security_label);
  gtk_box_pack_end (GTK_BOX (box),
                      priv->security_label,
                      FALSE,
                      FALSE,
                      12);

   gtk_widget_show (GTK_WIDGET (self));
}

GtkWidget*carrick_service_item_new (void)
{
  return g_object_new (CARRICK_TYPE_SERVICE_ITEM,
                       NULL);
}
