#include <gtk/gtk.h>
#include <gst/gst.h>
typedef struct _CustomData {
    GstElement* m_pPipeline;
    GstElement* m_pAudioSink;
    GstElement*	m_pVideoSink;

    GtkWidget*	slider;
    gulong 	slider_update_signal_id;

    GstState*	state;
    guint64	duration;
    guint64	position;
} CustomData;

static bool handle_message(GstBus* bus, GstMessage* msg, CustomData* data);

static void (GtkWidget *widget, CustomData *data)
{
    GdkWindow *window = gtk_widget_get_window(wigdet);
    guintptr window_handle;

    if (!gdk_window_ensure_native(window))
}
static void pause_cb(GtkButton *button, CustomData* data)
{
    GstState state = GST_STATE_NULL, statePending = GST_STATE_NULL;
    gst_element_get_state(data->m_pPipeline, &state, &statePending, 0);
    if (state >= GST_STATE_READY )
	gst_element_set_state(data->m_pPipeline, GST_STATE_PAUSED);
}
static void play_cb(GtkButton *button, CustomData* data)
{
    gst_element_set_state (data->m_pPipeline, GST_STATE_PLAYING);
}

static void stop_cb(GtkButton *button, CustomData* data)
{
    gst_element_set_state (data->m_pPipeline, GST_STATE_NULL);
}
static void slider_cb(GtkRange *range, CustomData *data)
{
    gdouble value = gtk_range_get_value(GTK_RANGE(data->slider));
    gst_element_seek_simple (data->m_pPipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, (gint64)(value * GST_SECOND));
}
static bool refresh_ui(CustomData* data)
{
    GstFormat fmt = GST_FORMAT_TIME;
    guint64 position;
    guint64 duration;
    if (data->state < GST_STATE_PLAYING)
	return true;

    if (!GST_CLOCK_TIME_IS_VALID(data->duration))
    {
	if (!gst_element_query_duration (data->m_pPipeline, &fmt, &data->duration)){
	    g_printerr ("Could not query current duration.\n");
    	}else{
	gtk_range_set_range(GTK_RANGE(data->slider), 0, (gdouble)data->duration / GST_SECOND);
	}
    }

    if (gst_element_query_positon (data->m_pPipeline, &fmt, &position))
    {
	g_signal_handler_block (data->slider, data->slider_update_signal_id);
	gtk_range_set_value(GTK_RANGE(data->slider), (gdouble) position/GST_SECOND);
	g_signal_handler_unblock (data->slider, data->slider_update_signal_id);
    }
}
static bool handle_message(GstBus *bus, GstMessage* message, CustomData* data)
{
    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_STATE_CHANGED:
       	if (GST_MESSAGE_SRC(message) == GST_OBJECT_CAST(data->m_pPipeline))
	{
	    GstState oldState, newState;
	    gst_message_parse_state_changed(message, &oldState, &newState, NULL);
	    if (oldState == GST_STATE_READY && newState == GST_STATE_PAUSED)
		refresh_ui(data);
	}
    case GST_MESSAGE_EOS:
	g_print("End-of-Stream reached.\n");
	gst_element_seek_simple( data->m_pPipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, 0);
    }

}
static bool BuildPipeline(char *uri, CustomData *data)
{
    GstBus* bus;
    data->m_pPipeline = gst_element_factory_make("playbin", "pipeline");
    if (m_pPipeline == NULL)
    {
	printf("create pipeline failed!\n");
	return FALSE;
    }
    g_object_set(m_pPipeline, "uri", uri, NULL);

    data->m_pAudioSink = gst_element_factory_make("waveformsink", "audiosink");
    g_object_set(m_pPipeline, "audio-sink", m_pAudioSink, NULL);

    data->m_pVideoSink = gst_element_factory_make("cluttersink", "videosink");
    g_object_set(m_pPipeline, "video-sink", m_pVideoSink, NULL);

    bus = gst_pipeline_get_bus(data->m_pPipeline);
    gst_bus_add_watch (bus, (GstBusFunc)handle_message, &data);
    gst_object_unref(bus);

    return TRUE;
}

int main(int argc, char **argv)
{
    CustomData data;
    GstStateChangeReturn ret;

    GtkWidget *window;
    GtkWidget *video_window;
    GtkWidget *mainbox, *Hbox;
    GtkWidget *controls;
    GtkWidget *slider;
    GtkWidget *btn_stop, *btn_pause, *btn_play, *btn_open;
    GtkLabel *label_duration, *label_position;
    GtkWidget *Hbox2;

    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);
    memset(&data, 0, sizeof(data));


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    Hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    video_window = gtk_drawing_area_new();
    g_signal_connect (video_window, "realize", G_CALLBACK(realize_cb), data);
//    g_signal_connect (video_window, "expose_event", G_CALLBACK(expose_cb), data);
    controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    data.slider = gtk_hscale_new_with_range(0, 100, 1);
    gtk_scale_set_draw_value(GTK_SCALE(data.slider), 0);
    data.slider_update_signal_id = g_signal_connect(G_OBJECT(data.slider), "value-changed", G_CALLBACK(slider_cb), &data);


    btn_stop = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);
    g_signal_connect (G_OBJECT(btn_stop), "clicked", G_CALLBACK(stop_cb), NULL);
    btn_pause = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    g_signal_connect (G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_cb), NULL);
    btn_play = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    g_signal_connect (G_OBJECT(btn_play), "clicked", G_CALLBACK(play_cb), NULL);
    btn_open = gtk_button_new_from_stock(GTK_STOCK_OPEN);
//    g_signal_connect (G_OBJECT(btn_stop), "clicked", G_CALLBACK(on_open_clicked), NULL);

    Hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    label_position = gtk_label_new_with_mnemonic("position: --:--");
    label_duration = gtk_label_new_with_mnemonic("duration: --:--");
    gtk_box_pack_start( GTK_BOX(Hbox2), label_position, 0, 0, 0);
    gtk_box_pack_end( GTK_BOX(Hbox2), label_duration, 0, 0, 20);
    
    gtk_box_pack_start( GTK_BOX (controls), btn_stop, 0, 0, 0);
    gtk_box_pack_start( GTK_BOX (controls), btn_play, 0, 0, 0);
    gtk_box_pack_start( GTK_BOX (controls), btn_pause, 0, 0, 0);
    gtk_box_pack_start( GTK_BOX (controls), btn_open, 0, 0, 0);
    gtk_box_pack_start( GTK_BOX (controls), slider, 1, 1, 2);

    gtk_box_pack_start( GTK_BOX(Hbox), video_window, 1, 1, 0);
    mainbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), Hbox, 1, 1, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), controls, 1, 1, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), Hbox2, 1, 1, 0);
    gtk_container_add(GTK_CONTAINER (window), mainbox); 

    gtk_widget_show_all(window);
    BuildPipeline("/usr/yw07/Home/Video/samples/720x384.mp4", data);
    ret = gst_element_set_state(data.m_pPipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
	g_printerr("Unable to set the pipeline to the playing state.\n");
	gst_object_unref(data.m_pPipeline);
	return -1;
    }
    g_timeout_add_seconds(1, (GSourceFunc)refresh_ui, &data);
    gtk_main();
    return 0;
}

