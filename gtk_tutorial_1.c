#include <gtk/gtk.h>

int main(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *drawingarea;
    GtkWidget *mainbox, *Hbox;
    GtkWidget *controls;
    GtkWidget *slider;
    GtkWidget *btn_stop, *btn_pause, *btn_play, *btn_open;
    GtkLabel *label_duration, *label_position;
    GtkWidget *Hbox2;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    Hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    drawingarea = gtk_drawing_area_new();
    controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    slider = gtk_hscale_new_with_range(0, 100, 1);
    btn_stop = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);
    btn_pause = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    btn_play = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    btn_open = gtk_button_new_from_stock(GTK_STOCK_OPEN);

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

    gtk_box_pack_start( GTK_BOX(Hbox), drawingarea, 1, 1, 0);
    mainbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), Hbox, 1, 1, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), controls, 1, 1, 0);
    gtk_box_pack_start( GTK_BOX(mainbox), Hbox2, 1, 1, 0);
    gtk_container_add(GTK_CONTAINER (window), mainbox); 

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

