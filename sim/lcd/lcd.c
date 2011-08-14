/******************************************************************************/
/* Lcd Simulator v2.2                                                         */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-06-14 */
/******************************************************************************/

/*
 * Lcd Simulator: this program simulates MieruLCD.
 * Copyright (C) 2010, Arch Lab. Tokyo Institute of Technology.
 * All rights reserved.
 *
 * This program is dual licensed under the New BSD or GNU GPL Version 2
 * licenses. You may redistribute and/or modify it under either license.
 * You don't have to notify anyone which license you are using.
 *
 * See COPYING file for the terms of the licenses.
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

#define WIDTH     480
#define HEIGHT    272

#define PAGE_SIZE 4

#define DEFAULT_WIDTH  WIDTH
#define DEFAULT_HEIGHT HEIGHT

typedef struct _point{
    int x;
    int y;
}point;

typedef struct _vram_rect{
    int width;
    int height;
    point pos_src;
    point pos_dst;
    int page_src;
    int page_dst;
}vram_rect;

//static const char *ASCII_FONT_NAME = "-b&h-lucidatypewriter-medium-r-normal-sans-12-120-75-75-m-70-iso8859-1"; // w7,h9
static const char *ASCII_FONT_NAME = "7x13";

// initial value
int screen_width  = 0;
int screen_height = 0;
point screen_pos = {0,0};

// internal variables
int scrolly[PAGE_SIZE], scrollx[PAGE_SIZE];
int foreground_color;
int background_color;
char display_page;
char display_page_by_menu;
char draw_page;
char overwrite_mode;
char cursor_mode;
point pos_ascii;
point pos_graphic;

// simulation variables
GdkFont *font_ascii = NULL;
GdkColormap *colormap;
GdkColor colors[256];
GdkPixmap *vram[PAGE_SIZE];
GdkGC *gc_draw;
GdkGC *gc_back;
unsigned int timer_count = 0;
GdkPixbuf *cursor_tmp;


// simulation flags
int update_flag;
int comm_pipe;
char *comm_pipe_name;
GMainLoop *interface_loop;

//mode variables
int debug_mode = 0;
int server_mode = 0;

GtkWidget *hoge;


void set_defaultcolormap(void);
void set_foreground(int index);
void set_background(int index);
void set_color(int red, int green, int blue, int index);
void set_display_page(int n);
void set_draw_page(int n);
void clear_point(void);
void set_rotate(void);
void set_overwrite(int n);
void set_cursor(int n);

void putchar_ascii(char c);
void print_ascii(char *str);
void locate_ascii(int x,int y);
void locatex_ascii(int x);
void locatey_ascii(int y);
void next_ascii();

point adjust_graphic_pos(int x, int y);
void locate_graphic(int x, int y);
void locatex_graphic(int x);
void locatey_graphic(int y);
void locatex_graphic_relative(int x);
void locatey_graphic_relative(int y);
void draw_dot(void);
void draw_point(point p);

void bitblt_vram(
    int src_page, int src_x, int src_y,
    int dst_page, int dst_x, int dst_y,
    int width,    int height);
void copy_page2page_vram(int src, int dst);
void copy_88dot_vram(
    int src_page, int src_x, int src_y,
    int dst_page, int dst_x, int dst_y, int mode);

void copy_1616dot_vram(
    int src_page, int src_x, int src_y,
    int dst_page, int dst_x, int dst_y, int mode);


gint do_expose_event(GtkWidget *widget, GdkEventExpose *event,
                     GdkPixmap *pixmap );
gint do_delete_event(GtkWidget *widget, gpointer data);
void do_destroy_event(GtkWidget *widget, gpointer data);

int init(void);
gint open_pipe(gpointer p);
void free_all(void);

int chartoint(char c);
int parse(char *str, int len);

static gboolean interface_callback(GIOChannel *io, GIOCondition cond, gpointer user_data) ;
gboolean cursor_callback(gpointer data);


void set_defaultcolormap(void){
    int i, ret;
    gboolean success[256];
    for(i=0;i<256;i++){
        colors[i].pixel = 0;
        colors[i].red   = ((i >> 5) & 0x7) * 0x2492;
        colors[i].green = ((i >> 2) & 0x7) * 0x2492;
        colors[i].blue  = (i & 0x3)        * 0x5555;
    }
    
    ret = gdk_colormap_alloc_colors(colormap, colors, 256, TRUE, TRUE, success);
    if(ret != 0)
        g_print("color allocation error(set_defaultcolormap)\n");
}

void set_foreground(int index){
    if(0 <= index && index < 256){
        foreground_color = index;
        gdk_gc_set_foreground(gc_draw, &colors[index]);
    }
}

void set_background(int index){
    if(0 <= index && index < 256){
        background_color = index;
        gdk_gc_set_foreground(gc_back, &colors[index]);
    }
}

// red 4bit, green 4bit, blue 4bit
void set_color(int red, int green, int blue, int index){
    int r,g,b;

    if(index < 0 || index >= 256) return;

    // adjust to 16bit colors?
    r = ((red  ) & 0xf) * 0x1111;
    g = ((green) & 0xf) * 0x1111;
    b = ((blue ) & 0xf) * 0x1111;

    colors[index].red = r;
    colors[index].green = g;
    colors[index].blue = b;

    gdk_colormap_alloc_color(colormap,
                             &colors[index],
                             TRUE, TRUE);

    set_foreground(foreground_color);
    set_background(background_color);
}

void set_display_page(int n){
    if( 0 <= n && n < PAGE_SIZE ){
        display_page = n;
    }
}


void set_draw_page(int n){
    if( 0 <= n && n < PAGE_SIZE ){
        draw_page = n;
    }
}

void clear_page(int n){
    if( 0 <= n && n < PAGE_SIZE ){
        gdk_draw_rectangle(vram[(int)n],
                           gc_back,
                           TRUE,
                           0, 0,
                           screen_width, screen_height);

        scrollx[n] = scrolly[n] = 0;
    }
}

void clear_point(void){
    pos_ascii.x = pos_ascii.y = 0;
    pos_graphic.x = pos_graphic.y = 0;
}

void set_overwrite(int n){
    if(n == 0)
        overwrite_mode = 0;
    else
        overwrite_mode = 1;
}

void set_cursor(int n){
    if(0 <= n && n < 3)
        cursor_mode = n;
}

/***************************************************************************/
/***** ASCII Commands ******************************************************/
void putchar_ascii(char c){
    if(!overwrite_mode){
        gdk_draw_rectangle(vram[(int)draw_page],
                           gc_back,
                           TRUE,
                           pos_ascii.x * 8,
                           pos_ascii.y * 16,
                           8, 16);
        
    }

    gdk_draw_text(vram[(int)draw_page],
                  font_ascii,
                  gc_draw,
                  pos_ascii.x * 8,
                  pos_ascii.y * 16 + 14,
                  &c,1);
    
    next_ascii();
}

void print_ascii(char *str){
    for(; *str != 0; str++)
        putchar_ascii(*str);
}

void locate_ascii(int x,int y){
    locatex_ascii(x);
    locatey_ascii(y);
}

void locatex_ascii(int x){
    if( 0 <= x && x < screen_width/8 )
        pos_ascii.x = x;
}

void locatey_ascii(int y){
    if( 0 <= y && y < screen_height/16 )
        pos_ascii.y = y;
}

void move_ascii(int x, int y){
    pos_ascii.x += x;
    if(pos_ascii.x >= screen_width/8){
        pos_ascii.x -= screen_width/8;
        pos_ascii.y++;
    }else if(pos_ascii.x < 0){
        pos_ascii.x += screen_width/8;
        pos_ascii.y--;
    }

    pos_ascii.y += y;
    if(pos_ascii.y >= screen_height/16){
        pos_ascii.y -= screen_height/16;
    }else if(pos_ascii.y < 0){
        pos_ascii.y += screen_height/16;
    }
}

void next_ascii(){
    pos_ascii.x++;
    if(pos_ascii.x >= screen_width/8){
        pos_ascii.x = 0;
        pos_ascii.y++;

        if(pos_ascii.y >= screen_height/16){
            pos_ascii.y = 0;
        }
    }
}


/***** End ASCII Commands **************************************************/
/***************************************************************************/

/****************************************************************************/
/***** BEGIN GRAPHIC Commands ***********************************************/

void locate_graphic(int x, int y){
    if( 0 <= x && x < screen_width &&
        0 <= y && y < screen_height ){
        pos_graphic.x = x;
        pos_graphic.y = y;
    }
}

void locatex_graphic(int x){
    locate_graphic(x, pos_graphic.y);
}

void locatey_graphic(int y){
    locate_graphic(pos_graphic.x, y);
}

void draw_dot(void){
    draw_point(pos_graphic);
}

void draw_point(point p){
    gdk_draw_point(vram[(int)draw_page],
                   gc_draw,
                   p.x, p.y); 
}

void move_graphic(int x, int y){
    pos_graphic.x += x;
    if(pos_graphic.x >= screen_width){
        pos_graphic.x -= screen_width;
    }else if(pos_graphic.x < 0){
        pos_graphic.x += screen_width;
    }
    
    pos_graphic.y += y;
    if(pos_graphic.y >= screen_height){
        pos_graphic.y -= screen_height;
    }else if(pos_graphic.y < 0){
        pos_graphic.y += screen_height;
    }
}

/***** END GRAPHIC Commands *************************************************/
/****************************************************************************/

/****************************************************************************/
/***** BEGIN VRAM Commands **************************************************/

// this function checks NO errors!
// Be Carefull to use.
void bitblt_vram(
    int src_page, int src_x, int src_y,
    int dst_page, int dst_x, int dst_y,
    int width,    int height){

    // checking only vram page
    if( 0 <= src_page && src_page < PAGE_SIZE &&
        0 <= dst_page && dst_page < PAGE_SIZE ){
    
        gdk_draw_pixmap(vram[dst_page],
                        gc_draw,
                        vram[src_page],
                        src_x,
                        src_y,
                        dst_x,
                        dst_y,
                        width,
                        height);
    }
}

void bitblt_vram_with_alpha(
    int src_page, int src_x, int src_y,
    int dst_page, int dst_x, int dst_y,
    int width,    int height,
    int red, int green, int blue){

    // checking only vram page
    if( 0 <= src_page && src_page < PAGE_SIZE &&
        0 <= dst_page && dst_page < PAGE_SIZE ){

        GdkPixbuf *pixbuf, *alpha;
        pixbuf = gdk_pixbuf_get_from_drawable(
            NULL,
            vram[src_page],
            NULL,
            src_x, src_y,
            0, 0,
            width, height);

        alpha = gdk_pixbuf_add_alpha (
            pixbuf,
            TRUE,
            red   & 0xff,
            green & 0xff,
            blue  & 0xff);

        gdk_draw_pixbuf(
            vram[dst_page],
            gc_draw,
            alpha, 
            0, 0,
            dst_x, dst_y, 
            width, height, 
            0, 0, 0);
    

        g_object_unref(pixbuf);
        g_object_unref(alpha);
    }
}

void copy_page2page_vram(int src, int dst){
    if( 0 <= src && src < PAGE_SIZE &&
        0 <= dst && dst < PAGE_SIZE ){
        bitblt_vram(
            src, 0, 0,
            dst, 0, 0,
            screen_width, screen_height);
    }
}

// 8*16 dot in block 
void copy_block_vram(
    int sp, int sx, int sy,
    int dp, int dx, int dy, int mode){

    if( 0 <= sp && sp < PAGE_SIZE &&
        0 <= dp && dp < PAGE_SIZE &&
        0 <= sx && sx < screen_width/8  &&
        0 <= dx && dx < screen_width/8  &&
        0 <= sy && sy < screen_height/16  &&
        0 <= dy && dy < screen_height/16 )
    {
        if(mode)
            bitblt_vram_with_alpha(
                sp, sx * 8, sy * 16, // source pos
                dp, dx * 8, dy * 16, // destination pos
                8, 16,  // block size
                colors[background_color].red   >> 8,
                colors[background_color].green >> 8,
                colors[background_color].blue  >> 8);
        else
            bitblt_vram(
                sp, sx * 8, sy * 16, // source pos
                dp, dx * 8, dy * 16, // destination pos
                8, 16); // block size
    }
}

void vram_vscroll(int scroll){    
    scroll = scroll % (screen_height/16);
    scrolly[(int)draw_page] += scroll;
    if(scrolly[(int)draw_page] >= (screen_height/16))
        scrolly[(int)draw_page] -= (screen_height/16);

    move_ascii(0, scroll);
    move_graphic(0, scroll * 16);
}

void vram_hscroll(int scroll){
    scroll = scroll % (screen_width/8);
    scrollx[(int)draw_page] += scroll;
    if(scrollx[(int)draw_page] >= (screen_width/8))
        scrollx[(int)draw_page] -= (screen_width/8);

    move_ascii(scroll, 0);
    move_graphic(scroll * 8, 0);
}


/***** END VRAM Commands ****************************************************/
/****************************************************************************/

void initialize(void){
    int i;
    set_defaultcolormap();
    clear_point();
    set_foreground(0xff); // White
    set_background(0x00); // Black
    set_overwrite(0);
    set_cursor(0);
    for(i=0;i<PAGE_SIZE;i++)
        clear_page(i);
    

    set_draw_page(0);
    set_display_page(0);
}

void blink_cursor(GtkWidget *widget, int count){
    static int prev_count = 0;
    GdkPixbuf *pixbuf;
    guchar *pixels, *p;
    int rowstride, n_channels;
    int x,y;

    int disp;
    if(display_page_by_menu >= 0)
        disp = display_page_by_menu;
    else
        disp = display_page;

    int scx, scy;
    scx = (pos_ascii.x - scrollx[disp]) * 8;
    scy = (pos_ascii.y - scrolly[disp]) * 16;
    if(scx < 0)
        scx += screen_width;
    if(scy < 0)
        scy += screen_height;

    if(count % 2) return;
    
    if(count == prev_count){
        gdk_draw_pixbuf(
        widget->window,
        gc_draw,
        cursor_tmp, 
        0, 0,
        //pos_ascii.x * 8, pos_ascii.y * 16,
        scx, scy,
        8, 16, 
        0, 0, 0);
        return;
    }

    gdk_pixbuf_get_from_drawable(
        cursor_tmp,
        vram[disp],
        NULL,
        pos_ascii.x * 8, pos_ascii.y * 16,
        0, 0,
        8, 16); // block size

    pixbuf = cursor_tmp;

    pixels = gdk_pixbuf_get_pixels(pixbuf);
    rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    n_channels = gdk_pixbuf_get_n_channels(pixbuf);
     
    if(cursor_mode == 1){
        for(y=14;y<16;y++){
            for(x=0;x<8;x++){
                p = pixels + y * rowstride + x * n_channels;
                p[0] ^= 0xff;
                p[1] ^= 0xff;
                p[2] ^= 0xff;
            }
        }
    }else{
        for(y=0;y<16;y++){
            for(x=0;x<8;x++){
                p = pixels + y * rowstride + x * n_channels;
                p[0] ^= 0xff;
                p[1] ^= 0xff;
                p[2] ^= 0xff;
            }
        }
    }


    gdk_draw_pixbuf(
        widget->window,
        gc_draw,
        pixbuf, 
        0, 0,
        scx, scy,
        8, 16, 
        0, 0, 0);

    prev_count = count;
}

gint do_expose_event( GtkWidget *widget, GdkEventExpose *event, GdkPixmap *pixmap )
{
    int disp;
    if(display_page_by_menu >= 0)
        disp = display_page_by_menu;
    else
        disp = display_page;

    if(scrollx[disp] == 0 && scrolly[disp] == 0){
        gdk_draw_drawable( widget->window,
                       widget->style->fg_gc[GTK_WIDGET_STATE( widget )],
                       vram[disp],
                       event->area.x, event->area.y,
                       event->area.x, event->area.y,
                       event->area.width, event->area.height );
    }else{
        int scx, scy;
        scx = scrollx[disp] * 8;
        scy = scrolly[disp] * 16;

        // lower right TO upper left
        gdk_draw_drawable( widget->window,
                           widget->style->fg_gc[GTK_WIDGET_STATE( widget )],
                           vram[disp],
                           scx, scy,
                           0, 0,
                           screen_width - scx, screen_height - scy);

        // upper right to lower left
        gdk_draw_drawable( widget->window,
                           widget->style->fg_gc[GTK_WIDGET_STATE( widget )],
                           vram[disp],
                           scx, 0,
                           0, screen_height - scy,
                           screen_width - scx, scy);

        // lower left to upper right
        gdk_draw_drawable( widget->window,
                           widget->style->fg_gc[GTK_WIDGET_STATE( widget )],
                           vram[disp],
                           0, scy,
                           screen_width - scx, 0,
                           scx, screen_height - scy);

        // upper left to lower right
        gdk_draw_drawable( widget->window,
                           widget->style->fg_gc[GTK_WIDGET_STATE( widget )],
                           vram[disp],
                           0, 0,
                           screen_width - scx, screen_height - scy,
                           scx, scy);
    }

    if(cursor_mode > 0){
        blink_cursor(widget, timer_count);
    }

    return FALSE;
}


gint do_delete_event( GtkWidget *widget, gpointer data){
    free_all();
    g_main_loop_quit(interface_loop);
    return FALSE;
}

void do_destroy_event( GtkWidget *widget, gpointer data){
    close(comm_pipe);
    g_main_loop_quit(interface_loop);
}

static void cb_quit(GtkAction *action, gpointer user_data){
    free_all();
    g_main_loop_quit(interface_loop);
}

static void cb_disp(GtkRadioAction *action, gpointer user_data){
    int now = gtk_radio_action_get_current_value( action );
    if(now == 100)
        display_page_by_menu = -1;
    else
        display_page_by_menu = now;

    gtk_widget_draw(hoge, NULL);
}

static const gchar *menu_info =
    "<ui>"
    "  <menubar name='Menubar'>"
    "    <menu name='File' action='File'>"
    "      <menuitem name='Quit' action='Quit'/>"
    "    </menu>"
    "    <menu name='Display' action='Display'>"
    "      <menuitem name='Realtime' action='Realtime'/>"
    "      <menuitem name='Disp0' action='Disp0'/>"
    "      <menuitem name='Disp1' action='Disp1'/>"
    "      <menuitem name='Disp2' action='Disp2'/>"
    "      <menuitem name='Disp3' action='Disp3'/>"
    "    </menu>"
    "  </menubar>"
    "</ui>";



static GtkActionEntry entries[] = {
    {"File", NULL, "_File"},
    {"Quit", GTK_STOCK_QUIT, "_Quit", "<control>Q", "Quit this program",
     G_CALLBACK(cb_quit)},
    {"Display", NULL, "_Display"},
};

static GtkRadioActionEntry entries2[] = {
    {"Realtime", NULL, "_Realtime", NULL, "realtime", 100},
    {"Disp0", NULL, "_Disp0", NULL, "Disp0", 0},
    {"Disp1", NULL, "_Disp1", NULL, "Disp1", 1},
    {"Disp2", NULL, "_Disp2", NULL, "Disp2", 2},
    {"Disp3", NULL, "_Disp3", NULL, "Disp3", 3},
};

static GtkUIManager *create_menu(GtkWidget *parent){
    GtkUIManager *ui;
    GtkActionGroup *actions;

    ui = gtk_ui_manager_new();
    actions = gtk_action_group_new("menu");
    gtk_action_group_add_actions(actions, entries, sizeof(entries) / sizeof(entries[0]), parent);
    gtk_action_group_add_radio_actions(actions, entries2, sizeof(entries2) / sizeof(entries2[0]),
                                       100, G_CALLBACK(cb_disp), G_OBJECT(parent));
    gtk_ui_manager_insert_action_group(ui, actions, 0);
    gtk_ui_manager_add_ui_from_string(ui, menu_info, -1, NULL);
    gtk_window_add_accel_group(GTK_WINDOW(parent),
                               gtk_ui_manager_get_accel_group(ui));

    return ui;
}

int init(){
    
    GtkWidget *window;
    GtkWidget *drawing_area;
    
    int i;


    // decide windows size (rotate mode)
    if(screen_width == 0)
        screen_width = DEFAULT_WIDTH;
    if(screen_height == 0)
        screen_height = DEFAULT_HEIGHT;
    
    
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_widget_set_uposition( window, screen_pos.x, screen_pos.y);
    gtk_window_set_resizable( (GtkWindow *)window, FALSE);    

    if(server_mode)
        gtk_window_set_title((GtkWindow *)window, "LCD server mode");
    else
        gtk_window_set_title((GtkWindow *)window, "LCD pipe mode");
    
    
    drawing_area = gtk_drawing_area_new();  
    gtk_widget_set_size_request( drawing_area, screen_width, screen_height);
    hoge = drawing_area;


    GtkWidget *vbox;
    GtkUIManager *ui;
    GtkWidget *menubar;
    vbox = gtk_vbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    ui = create_menu(window);
    g_object_set_data(G_OBJECT(window), "ui", (gpointer)ui);
    menubar = gtk_ui_manager_get_widget(ui, "/Menubar");
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_set_events( drawing_area, GDK_EXPOSURE_MASK);
    //gtk_container_add( GTK_CONTAINER( window ), drawing_area );
    //gtk_widget_show( drawing_area );
    
    
    // signal
    g_signal_connect( G_OBJECT( window ), "delete_event",
                      G_CALLBACK( do_delete_event ), NULL );
    g_signal_connect( G_OBJECT( window ), "destroy",
                      //G_CALLBACK( gtk_main_quit ), NULL );
                      G_CALLBACK( do_destroy_event ), NULL );
    g_signal_connect( G_OBJECT( drawing_area ), "expose_event",
                      G_CALLBACK( do_expose_event ), NULL );
    

    gtk_widget_show_all(window);
    /* gtk_widget_show(drawing_area); */
    /* gtk_widget_show(menubar); */
    

    
    gc_draw = gdk_gc_new(drawing_area->window);
    gc_back = gdk_gc_new(drawing_area->window);
    

    // set default colors (256 colors)
    colormap = gdk_colormap_get_system(); 
    set_defaultcolormap();

    // vram
    for(i=0;i<PAGE_SIZE;i++){
        vram[i] = gdk_pixmap_new(window->window,
                                 screen_width, screen_height, -1);
    }
    cursor_tmp = gdk_pixbuf_get_from_drawable(
        NULL,
        window->window,
        NULL,
        0, 0,
        0, 0,
        8, 16); // block size


    // font
    font_ascii = gdk_font_load(ASCII_FONT_NAME);
    if(font_ascii == NULL){
        g_print("ERROR: ascii font was not found\n");
        return -1;
    }

    
    /* // test */
    /* int w,h; */
    /* w = gdk_string_width(font_ascii, "a"); */
    /* h = gdk_string_height(font_ascii, "a"); */
    /* g_print("font a w:%d h:%d\n",w,h); */
    /* w = gdk_string_width(font_ascii, "M"); */
    /* h = gdk_string_height(font_ascii, "M"); */
    /* g_print("font M w:%d h:%d\n",w,h); */
    /* exit(1); */

    /* initialize internal values */
    clear_point();
    set_foreground(0xff); // White
    set_background(0x00); // Black
    set_overwrite(0);
    set_cursor(0);
    for(i=0;i<PAGE_SIZE;i++)
        clear_page(i);
    

    set_draw_page(0);
    set_display_page(0);
    display_page_by_menu = -1;
    g_timeout_add( 1000, cursor_callback, G_OBJECT( window ) );

    interface_loop = g_main_loop_new(NULL, FALSE);
    GIOChannel *io = g_io_channel_unix_new(comm_pipe);
    g_io_add_watch(io, G_IO_IN, interface_callback, G_OBJECT(window));
    g_io_channel_set_flags(io, G_IO_FLAG_NONBLOCK, NULL);
    g_io_channel_set_close_on_unref(io, TRUE);
    g_io_channel_set_encoding(io, NULL, NULL);
    g_main_loop_run(interface_loop);
    g_io_channel_unref(io);
    g_main_loop_unref(interface_loop);



    return 0;
}

gint open_pipe(gpointer p){
    if(!access(comm_pipe_name, 0x00)){
        remove(comm_pipe_name);
    }
    
    mkfifo(comm_pipe_name,S_IRWXU|S_IRWXG|S_IRWXO);
    
    g_print("waiting pipe to be opened\n");
    comm_pipe = open(comm_pipe_name, O_RDWR);
    if(comm_pipe < 0){
        return -1;
    }
    g_print("done!\n");

    return 0;
}

void free_all(void){
    int i;

    gdk_gc_unref(gc_draw);
    gdk_gc_unref(gc_back);
    if(font_ascii) gdk_font_unref(font_ascii);
    for(i=0;i<PAGE_SIZE;i++){
        gdk_pixmap_unref(vram[i]);
    }
}

int chartoint(char c){
    if('0' <= c && c <= '9')
        return c - '0';
    if('a' <= c && c <= 'f')
        return c - 'a' + 10;
    if('A' <= c && c <= 'F')
        return c - 'A' + 10;
    return -1;
}


int parse(char *str, int len){

// this macro checks if cmd is enough to read in 'str'
#define length_check(n) {\
if((end - (p + (n)) + 1) < 0){\
    out = 1;\
    break;\
  }\
}

    char *p = str;
    char *end = &str[len - 1];
    int out = 0;
    int a0,a1,a2,a3,a4;
    for(;;){
        if( end - p < 0){
            out = 1;
            break;
        }
        
        switch((unsigned char)(*p)){
        case 0:
            p++;
            break;

        case 'I':
            length_check(1);
            p++;
            initialize();
            // no effect
            break;

        case 'C':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            if(0 <= a0 && a0 < PAGE_SIZE){
                clear_page(a0);
            }
            break;

        case 'W':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            if(0 <= a0 && a0 < PAGE_SIZE){
                set_draw_page(a0);
            }
            break;
      
        case 'P':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            if(0 <= a0 && a0 < PAGE_SIZE){
                set_display_page(a0);
            }
            break;

            /*
        case 'R':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            // no effect
            break;
            */

        case 'T':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            if( a0 == 0 || a0 == 1)
                set_overwrite(a0);
            break;

        case 'c':
            length_check(2);
            p++;
            a0 = chartoint(*p++);
            if( a0 == 0 || a0 == 1 || a0 == 2)
                set_cursor(a0);
            break;

        case 'F':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            set_foreground(a0 * 16 + a1);
            break;

        case 'B':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            set_background(a0 * 16 + a1);
            break;

        case 'D':
            length_check(2);
            p++;
            putchar_ascii(*p++);
            break;
        
        case 'X':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            locatex_ascii(3);
            if( 0 <= a0 && a0 <= 9 &&
                0 <= a1 && a1 <= 9 ){
                locatex_ascii(a0*10 + a1);
            }
            break;
      
        case 'Y':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            if( 0 <= a0 && a0 <= 9 &&
                0 <= a1 && a1 <= 9 ){
                locatey_ascii(a0*10 + a1);
            }
            break;

        case 'H':
            length_check(1);
            p++;
            move_ascii(-1, 0); // move left
            break;

        case 'J':
            length_check(1);
            p++;
            move_ascii(0, 1); // move down
            break;

        case 'K':
            length_check(1);
            p++;
            move_ascii(0, -1); // move up
            break;

        case 'L':
            length_check(1);
            p++;
            move_ascii(1, 0); // move right
            break;

        case 'd':
            length_check(1);
            p++;
            draw_dot();
            move_graphic(1, 0);
            break;

        case 'x':
            length_check(4);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            a2 = chartoint(*p++);
            if( 0 <= a0 && a0 <= 9 &&
                0 <= a1 && a1 <= 9 &&
                0 <= a2 && a2 <= 9 ){

                locatex_graphic(a0 * 100 + a1 * 10 + a2);
            }
            break;

        case 'y':
            length_check(4);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            a2 = chartoint(*p++);
            if( 0 <= a0 && a0 <= 9 &&
                0 <= a1 && a1 <= 9 &&
                0 <= a2 && a2 <= 9 ){

                locatey_graphic(a0 * 100 + a1 * 10 + a2);
            }
            break;

        case 'h':
            length_check(1);
            p++;
            move_graphic(-1, 0); // move left
            break;

        case 'j':
            length_check(1);
            p++;
            move_graphic(0, 1); // move down
            break;

        case 'k':
            length_check(1);
            p++;
            move_graphic(0, -1); // move up
            break;

        case 'l':
            length_check(1);
            p++;
            move_graphic(1, 0); // move right
            break;


        case 'V':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            if( 0 <= a0 && a0 < PAGE_SIZE &&
                0 <= a1 && a1 < PAGE_SIZE )
                copy_page2page_vram(a0, a1);
            break;

        case 'v':
            length_check(7);
            p++;
            {
                int sp, sx, sy, mode;
                sp  = chartoint(*p++);
                sx  = chartoint(*p++) * 10;
                sx += chartoint(*p++);
                sy  = chartoint(*p++) * 10;
                sy += chartoint(*p++);
                mode = chartoint(*p++);
                
                copy_block_vram(sp, sx, sy, draw_page, pos_ascii.x, pos_ascii.y, mode);
                move_ascii(1, 0);
            }
            break;

        case 'S':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            vram_hscroll(a0 * 10 + a1);
            break;

        case 's':
            length_check(3);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            vram_vscroll(a0 * 10 + a1);
            break;

        case 'p':
            length_check(6);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            a2 = chartoint(*p++);
            a3 = chartoint(*p++);
            a4 = chartoint(*p++);
            set_color(a2, a3, a4, a0 * 16 + a1);
            break;

        case 'M':
            length_check(5);
            p++;
            a0 = chartoint(*p++);
            a1 = chartoint(*p++);
            a2 = chartoint(*p++);
            a3 = chartoint(*p++);
            // no effect
            break;

        case '\n':
            p++;
            break;
      
        default:
            g_print("command was not found (%c:%02x)\n",*p,*p);
            p++;
        }

        if(out)
            break;
    }

    return end - p + 1;

}

void usage(){
    char usagemessage[] = "\
lcdsim version 2.2\n\
  -v: show this message\n\
  -d: debug mode\n\
  -s [named_pipe]: server mode using named pipe\n\
  -x[pos]: set default x-coordinate\n\
  -y[pos]: set default y-coordinate\n\
  -w[size]: set default window width size\n\
  -y[size]: set default window height size\n\
\n\
   Commands are received from standard input by default. \n\
   You can send commands via 'named pipe' by using server mode. \n\
\n";

    printf("%s\n", usagemessage);
}

int checkarg(int argc, char **argv){
    int i;
    char *opt;
    int n;

    for(i=1; (opt = argv[i]) != NULL; i++){
        if(opt[0] == '-'){
            switch(opt[1]){
            case 'd':
                debug_mode = 1;
                break;

            case 's':
                server_mode = 1;
                if( (comm_pipe_name = argv[++i]) == NULL){
                    fprintf(stderr, "## -s option needs pipe path\n");
                    return -1;
                }
                break;

            case 'w':
                n = atoi(&opt[2]);
                if(n > 0)
                    screen_width = n;
                break;
            case 'h':
                n = atoi(&opt[2]);
                if(n > 0)
                    screen_height = n;
                break;

            case 'x':
                n = atoi(&opt[2]);
                if(n > 0)
                    screen_pos.x = n;
                break;

            case 'y':
                n = atoi(&opt[2]);
                if(n > 0)
                    screen_pos.y = n;
                break;

            case 'v':
            default:
                usage();
                return -1;
            }
        }else{
            usage();
            return -1;
        }
    }
    
    return 0;
}

static gboolean interface_callback(GIOChannel *io, GIOCondition cond, gpointer user_data) {
    static gsize rest = 0;
    static char buf[2048];
    int pos;
    gsize length;
    gboolean continue_to_watch = FALSE;
    GError *e = NULL;

    if (cond & G_IO_IN) {
        
        switch (g_io_channel_read_chars(io, &buf[rest], 2040 - rest, &length, &e)) {

        case G_IO_STATUS_NORMAL:
        case G_IO_STATUS_AGAIN:
            length += rest;
            buf[length] = 0;
        
            if(debug_mode){
                buf[length]= 0;
                if(strlen(buf) != 0)
                    g_print(">%s(%d:%d)\n\r",buf,(int)strlen(buf), (int)length);
            }

            continue_to_watch = TRUE;
            rest = parse(buf, length);
            pos = length - rest;
            memmove(buf, &buf[pos], rest);

            gtk_widget_draw((GtkWidget *)user_data, NULL);
            break;

        case G_IO_STATUS_ERROR:
            g_message("%s: error: %s", __func__, e->message);
            g_error_free(e);
            break;
        case G_IO_STATUS_EOF:
            g_message("%s: EOF", __func__);
            break;
        default:
            break;
        }
    }
    if (! continue_to_watch) {
        g_main_loop_quit(interface_loop);
        g_main_loop_unref(interface_loop);
    }
    return continue_to_watch;

}

gint main( int argc, char **argv ){
    int ret;

    if(checkarg(argc, argv) < 0)
        return 1;
    
    gtk_init( &argc, &argv );

    if(server_mode){
        if(!access(comm_pipe_name, 0x00)){
            remove(comm_pipe_name);
        }
    
        mkfifo(comm_pipe_name,S_IRWXU|S_IRWXG|S_IRWXO);
    
        g_print("waiting pipe to be opened\n");
        comm_pipe = open(comm_pipe_name, O_RDWR);
        if(comm_pipe < 0){
            return -1;
        }
        g_print("done!\n");
    }else{
        comm_pipe = 0;
    }
    
    ret = init();
    if(ret < 0)
        return 1;


    return 0;
}

gboolean cursor_callback(gpointer data){
    timer_count++;
    gtk_widget_draw((GtkWidget *)data, NULL);
    return TRUE;
}
