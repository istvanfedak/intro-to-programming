#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#pragma warning (disable:4267)
#pragma warning (disable:4311)
#pragma warning (disable:4313)
#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4800)
#pragma warning (disable:4018)
using namespace std;

void main();
struct window;
struct fontrec;
extern window * screen;
#define no_console int __dagkjsdgbz = consolestopper()
typedef HMENU menu_reference;

string int_to_string(int i);

struct direction
{ static const char * north, * north_west, * west, * south_west, * south,
                    * south_east, * east, * north_east, * center; };

struct cursor
{ static const int arrow, cross, hand, hbar, vbar, help, eggtimer, noparking; };

const int no_events=0,
          key_typed=1,
          mouse_clicked=2,
          timer_expired=4,
          mouse_but_not_left_click=8,
          all_normal_mouse_events=74,
          window_changed=16,
          file_dragged_in=32,
          mouse_dragged=64,
          mouse_being_dragged=128,
          mouse_movement=256,
          window_close_button=512,
          menu_selection=1024,
          all_normal_events=2047-256-128;

struct event
{ int kind, x, y, x0, y0, code;
  char c;
  string files;
  string description();
  static const int none, mouse_left_press, mouse_left_release, mouse_left_click,
         mouse_right_press, mouse_right_release, mouse_right_click, mouse_middle_press,
         mouse_middle_release, mouse_middle_click, mouse_left_dragging, mouse_right_dragging,
         mouse_middle_dragging, mouse_left_dragged, mouse_right_dragged, mouse_middle_dragged,
         mouse_gone, key_typed, minimized, restored, resized, resizing, file_drop, timer,
         mouse_moved, close_window, menu_selected; };


struct shape
{ double x0, y0;
  POINT * pts;
  BYTE * bts;
  int num, cap;
  shape();
  void clear();
  void enlarge();
  void note_position(double x, double y);
  void move_shape_to(double x, double y);
  void scale_shape(double s);
  ~shape(); };

struct image
{ int width, height;
  int * * pixels;
  image(int w, int h, int * * p);
  ~image(); };

struct windowdata
{ };

struct window
{ HWND wnd;
  HDC dc;
  HBITMAP bitmap;
  HDC bitmapdc;
  static HANDLE semaphore;
  HANDLE creationevent;
  TEXTMETRIC fontdata;
  HPEN pen;
  HMENU menu;
  HBRUSH brush;
  HRGN hregion;
  RECT whole, client, prevclient;
  fontrec * currentfont;
  int color, penw, myindex, xpos, ypos, maxcharheightonline, flags, heading;
  int dpix, dpiy, edgex, edgey, brushcolor, textcolor, wantc, wantw;
  double fxpos, fypos;
  bool haswindow, hasbitmap, usewindow, usebitmap, autoclick, canusewindow;
  bool posint, isprinter, printended, pench;
  windowdata * extradata;

  static const int flag_hadasize=1, flag_minimized=2;
  static vector<window *> windows;
  static int numwindows;

  window(bool isscreen);
  window(int w, int h, int x = -7777, int y = -7777);
  window(char printer);
  window(bool offscreen, int w, int h);
  void nonthreadcreate();
  ~window();
  void destroy();
  void paint();

  void set_window_data(windowdata * e);
  windowdata * get_window_data();
  void floatify();

  static window * window::make(int w, int h, int x = 75, int y = 75);
  menu_reference add_menu(string name);
  menu_reference add_sub_menu(menu_reference parent, string name);
  void add_menu_item(menu_reference parent, string name, int code);
  window * set_window_size(int w, int h);
  int get_window_width();
  int get_window_height();
  window * get_window_size(int & w, int & h);
  window * set_full_window_size(int w, int h);
  window * make_window_active();
  int get_full_window_width();
  int get_full_window_height();
  window * get_full_window_size(int & w, int & h);
  window * set_window_position(int x, int y);
  window * get_window_position(int & x, int & y);
  int get_window_x_position();
  int get_window_y_position();
  int get_pixels_per_inch();
  int get_vertical_pixels_per_inch();
  int get_left_edge();
  int get_top_edge();
  window * set_caption(string s);
  string get_caption();
  window * set_cursor(int n);
  window * enter_fast_mode();
  window * catch_up();
  window * clear();
  window * move_to(int x, int y);
  window * move_to(double x, double y);
  window * draw_to(int x, int y);
  window * draw_to(double x, double y);
  window * move_relative(int x, int y);
  window * move_relative(double x, double y);
  window * draw_relative(int x, int y);
  window * draw_relative(double x, double y);
  window * draw_point(int x, int y);
  window * draw_point(double x, double y);
  window * draw_point();
  window * set_pixel_color(int x, int y, int c);
  window * set_pixel_color(double x, double y, int c);
  window * set_pixel_color(int c);
  int get_pixel_color(int x, int y);
  int get_pixel_color(double x, double y);
  int get_pixel_color();
  window * set_pen_width_color(int w, int rgb);
  window * set_pen_color(int rgb);
  window * set_pen_color_int(int r, int g, int b);
  window * set_pen_color(double r, double g, double b);
  window * set_pen_color_hls(double h, double l, double s);
  window * set_pen_width(int w);
  window * set_pen_width(double w);
  int get_pen_width();
  int get_pen_color();
  window * fill_rectangle(int x, int y, int w, int h, int rgb=-2);
  window * fill_rectangle(double x, double y, double w, double h, int rgb=-2);
  window * set_heading_degrees(double h);
  window * set_heading_radians(double h);
  double get_heading_degrees();
  double get_heading_radians();
  double distance_to(double x, double y);
  double direction_to_in_degrees(double x, double y);
  double direction_to_in_radians(double x, double y);
  double direction_from_to_in_degrees(double x0, double y0, double x1, double y1);
  double direction_from_to_in_radians(double x0, double y0, double x1, double y1);
  window * turn_left_by_degrees(double a);
  window * turn_left_by_radians(double a);
  window * turn_right_by_degrees(double a);
  window * turn_right_by_radians(double a);
  window * move_distance(int d);
  window * move_distance(double d);
  window * draw_distance(int d);
  window * draw_distance(double d);
  window * iwrite_tchar(TCHAR * str, int n, const char * anchor, bool updateposition);
  window * write_string(string s, const char * anchor, bool updateposition=true);
  window * write_string(string s, bool updateposition=true);
  window * write_string(const char * s, const char * anchor, bool updateposition=true);
  window * write_string(const char * s, bool updateposition=true);
  window * write_string(const void * s, const char * anchor, bool updateposition=true);
  window * write_string(const void * s, bool updateposition=true);
  window * write_string(int c, const char * anchor, bool updateposition=true);
  window * write_string(int c, bool updateposition=true);
  window * write_string(char c, const char * anchor, bool updateposition=true);
  window * write_string(char c, bool updateposition=true);
  window * write_string(double c, const char * anchor, bool updateposition=true);
  window * write_string(double c, bool updateposition=true);
  window * write_string(bool c, const char * anchor, bool updateposition=true);
  window * write_string(bool c, bool updateposition=true);
  window * write_char(int c, const char * anchor, bool updateposition=true);
  window * write_char(int c, bool updateposition=true);
  window * write_int(int c, const char * anchor, bool updateposition=true);
  window * write_int(int c, bool updateposition=true);
  window * write_float(double c, const char * anchor, bool updateposition=true);
  window * write_float(double c, bool updateposition=true);
  window * write_double(double c, const char * anchor, bool updateposition=true);
  window * write_double(double c, bool updateposition=true);
  window * write_bool(bool c, const char * anchor, bool updateposition=true);
  window * write_bool(bool c, bool updateposition=true);
  window * write_new_line(int x=0, int extray=0);
  window * get_position(int & x, int & y);
  window * get_position(double & x, double & y);
  window * get_position(float & x, float & y);
  double get_x_position();
  double get_y_position();
  window * get_font_size(int & height, int & ascent, int & descent,
                         int & internalleading, int & externalleading);
  window * measure_string(string s, int & x, int & y);
  window * measure_char(int c, int & x, int & y);
  window * hide_window();
  window * show_window();
  window * set_font(fontrec * f = NULL);
  window * i_set_font(string name, int size, int style, int angle);
  window * set_font(string name="", int size=0, int style=-1, double angle=90);
  window * set_font_size(int size=0);
  window * set_font_style(int style=-1);
  window * set_font_angle_degrees(double angle=90);
  window * set_font_angle_radians(double angle=acos(-1.0)/2.0);
  window * start_shape();
  window * note_position(double x, double y);
  window * note_position();
  window * move_shape_to(double x, double y);
  window * scale_shape(double s);
  window * draw_shape();
  window * fill_shape(int rgb=-2);
  void set_restriction(HRGN h);
  window * restrict_drawing_to_shape();
  window * restrict_drawing_to_rectangle(int x, int y, int w, int h);
  window * unrestricted_drawing();
  window * next_page();
  window * finish_printing();
  window * draw_image(image * im, int x0, int y0);
  window * draw_image(image * im);
  window * draw_image_from_file(string s, int x0, int y0);
  window * draw_image_from_file(string s);
protected:
  void init1();
  void init2();
  void isetp();
  void isett(); };

struct fontrec
{ int size, style, refcount, angle;
  string name;
  HFONT it;
  static vector<fontrec *> fonts;
  static fontrec * defaultfont;
  static fontrec * font(string nm, int sz, int st=FW_NORMAL, int ang=90);
  void use();
  void abandon();
  fontrec(string n, int sz, int st, int ang, HFONT f); };

fontrec * font(string nm, int sz, int st=FW_NORMAL, int ang=90);

struct style
{ static const int normal, italic, underlined, crossed_out, very_light,
                   light, bold, very_bold; };

struct color
{ static const int black, white, grey, light_grey, dark_grey, brown, red, green, blue,
                   yellow, magenta, cyan, pink, orange, dark_green, indigo, violet,
                   mauve, purple, lime_green, dark_blue, sodium_d, light_blue, dark_red,
                   light_green, light_red;
  static string names[];
  static int values[];
  static const int nn, nv;
  static int rgb(double r, double g, double b);
  static int hls(double h, double l, double s); };

struct thread
{ HANDLE windowsidh;
  DWORD windowsidi;
  int state;
  int stack_size;
  virtual void main();
  void start();
  void kill();
  void sleep(double t);
  thread(); };

struct semaphore
{ HANDLE it;
  semaphore();
  ~semaphore();
  void make_free();
  void wait_until_free(); };

window * make_window(int w, int h, int x = -7777, int y = -7777);
window * create_off_screen_area(int w, int h);
void copy_area(int w, int h, window * dst, int xd, int yd, window * src, int xs, int ys);
void copy_area(int w, int h, int xd, int yd, int xs, int ys);
void copy_area(int w, int h, int xd, int yd, window * src);
window * set_window_size(int w, int h);
menu_reference add_menu(string name);
menu_reference add_sub_menu(menu_reference parent, string name);
void add_menu_item(menu_reference parent, string name, int code);
int get_screen_width();
int get_screen_height();
void get_screen_size(int & w, int & h);
int get_window_width();
int get_window_height();
window * get_window_size(int & w, int & h);
window * set_full_window_size(int w, int h);
int get_full_window_width();
int get_full_window_height();
window * get_full_window_size(int & w, int & h);
window * set_window_position(int x, int y);
window * get_window_position(int & x, int & y);
int get_window_x_position();
int get_window_y_position();
int get_pixels_per_inch();
int get_vertical_pixels_per_inch();
int get_left_edge();
int get_top_edge();
window * set_caption(string s);
string get_caption();
int make_cursor(string patt, int x, int y);
window * set_cursor(int n);
window * enter_fast_mode();
window * catch_up();
window * select(window * win);
window * current_window();
window * clear();
window * move_to(int x, int y);
window * move_to(double x, double y);
window * move_relative(int x, int y);
window * move_relative(double x, double y);
window * get_position(int &x, int &y);
window * get_position(double &x, double &y);
window * get_position(float &x, float &y);
double get_x_position();
double get_y_position();
window * draw_to(int x, int y);
window * draw_to(double x, double y);
window * draw_relative(int x, int y);
window * draw_relative(double x, double y);
window * draw_point(int x, int y);
window * draw_point(double x, double y);
window * draw_point();
window * set_pixel_color(int x, int y, int c);
window * set_pixel_color(double x, double y, int c);
window * set_pixel_color(int c);
int get_pixel_color(int x, int y);
int get_pixel_color(double x, double y);
int get_pixel_color();
window * fill_rectangle(int x, int y, int w, int h, int rgb=-2);
window * fill_rectangle(double x, double y, double w, double h, int rgb=-2);
window * set_heading_degrees(double h);
window * set_heading_radians(double h);
double get_heading_degrees();
double get_heading_radians();
double distance_to(double x, double y);
double direction_to_in_degrees(double x, double y);
double direction_to_in_radians(double x, double y);
double direction_from_to_in_degrees(double x0, double y0, double x1, double y1);
double direction_from_to_in_radians(double x0, double y0, double x1, double y1);
window * turn_left_by_degrees(double a);
window * turn_left_by_radians(double a);
window * turn_right_by_degrees(double a);
window * turn_right_by_radians(double a);
window * move_distance(int d);
window * move_distance(double d);
window * draw_distance(int d);
window * draw_distance(double d);
window * write_string(string s, const char * anchor, bool updateposition=true);
window * write_string(string s, bool updateposition=true);
window * write_string(const char * s, const char * anchor, bool updateposition=true);
window * write_string(const char * s, bool updateposition=true);
window * write_string(const void * s, const char * anchor, bool updateposition=true);
window * write_string(const void * s, bool updateposition=true);
window * write_string(int c, const char * anchor, bool updateposition=true);
window * write_string(int c, bool updateposition=true);
window * write_string(char c, const char * anchor, bool updateposition=true);
window * write_string(char c, bool updateposition=true);
window * write_string(double c, const char * anchor, bool updateposition=true);
window * write_string(double c, bool updateposition=true);
window * write_string(bool c, const char * anchor, bool updateposition=true);
window * write_string(bool c, bool updateposition=true);
window * write_char(int c, const char * anchor, bool updateposition=true);
window * write_char(int c, bool updateposition=true);
window * write_int(int c, const char * anchor, bool updateposition=true);
window * write_int(int c, bool updateposition=true);
window * write_float(double c, const char * anchor, bool updateposition=true);
window * write_float(double c, bool updateposition=true);
window * write_double(double c, const char * anchor, bool updateposition=true);
window * write_double(double c, bool updateposition=true);
window * write_bool(bool c, const char * anchor, bool updateposition=true);
window * write_bool(bool c, bool updateposition=true);
window * write_new_line(int x=0, int extray=0);
window * measure_string(string s, int & x, int & y);
window * measure_char(int c, int & x, int & y);
void split_color(int rgb, double & r, double & g, double & b);
void split_color(int rgb, float & r, float & g, float & b);
void split_color_int(int rgb, int & r, int & g, int & b);
int make_color_int(int r, int g, int b);
int make_color(double r, double g, double b);
void hls_to_rgb(double H, double L, double S, double &R, double &G, double &B);
void rgb_to_hls(double R, double G, double B, double &H, double &L, double &S);
int make_color_hls(double h, double l, double s);
window * set_pen_width_color(int w, int rgb);
window * set_pen_color(int rgb);
window * set_pen_color_int(int r, int g, int b);
window * set_pen_color(double r, double g, double b);
window * set_pen_color_hls(double h, double l, double s);
window * set_pen_width(int w);
window * set_pen_width(double w);
int get_pen_color();
int get_pen_width();
void sleep(double t);
void hide_console();
void show_console();
void set_console_position(int x, int y);
void set_console_size(int w, int h);
void make_console_active();
window * make_window_active(window * w);
void set_console_caption(string s);
window * hide_window();
window * show_window();
window * get_font_size(int & height, int & ascent, int & descent,
                       int & internalleading, int & externalleading);
window * set_font(fontrec * f = NULL);
window * set_font(string name="", int size=0, int style=-1, double angle=90);
window * set_font_size(int size=0);
window * set_font_style(int style=-1);
window * set_font_angle_degrees(double angle=90);
window * set_font_angle_radians(double angle=acos(-1.0)/2.0);
int consolestopper();
void start_shape();
void note_position(double x, double y);
void note_position();
void move_shape_to(double x, double y);
void scale_shape(double s);
void draw_shape();
void fill_shape(int rgb=-2);
window * restrict_drawing_to_shape();
window * restrict_drawing_to_rectangle(int x, int y, int w, int h);
window * unrestricted_drawing();
void start_printing();
void next_page();
void finish_printing();
void wait(double t);
void wait_for(int mask, double t=-2.0);
void clear_interesting_events(int mask);
void set_interesting_events(int mask);
void set_interesting_event(int mask);
int get_interesting_events(int mask);
void add_to_interesting_events(int mask);
void remove_from_interesting_events(int mask);
event wait_for_interesting_event(double t=-2.0);
bool any_events();
bool wait_for_mouse_click(double t=-2.0);
char wait_for_key_typed(double t=-2.0);
char wait_for_mouse_click_or_key_typed(double t=-2.0);
char wait_for_key_typed_or_mouse_click(double t=-2.0);
int what_happened();
int get_event_kind();
int get_click_x();
int get_selected_menu();
int get_click_y();
int get_drag_origin_x();
int get_drag_origin_y();
char get_key_typed();
void exit();
void error_pop_up(string message="ERROR", string title="ERROR");
void information_pop_up(string message="", string title="INFORMATION");
bool yes_no_pop_up(string message="Yes or No?", string title="QUESTION");
bool ok_cancel_pop_up(string message="OK?", string title="OK?");
string open_file_pop_up(string suggestedfilename="", string initdirectory="", string title="Select file");
string save_as_pop_up(string suggestedfilename="", string initdirectory="", string defaultextension="", string title="Save as");
string string_parameter(string name);
char char_parameter(string name);
int int_parameter(string name);
double double_parameter(string name);
double float_parameter(string name);
bool bool_parameter(string name);
int color_parameter(string name);
void log_entry(string s);
void log_entry(int n);
void log_entry(char c);
void log_entry(bool b);
void log_entry(double d);
void log_entry(const char *patt, ...);
double what_is_the_time();
void start_timer();
double read_timer();
int get_clock_time();
int get_calendar_date();
void print(string s);
void print(const char * s);
void print(int n);
void print(char c);
void print(double d);
void print(bool b);
void print_string(string s);
void print_int(int n);
void print_char(char c);
void print_double(double d);
void print_float(double d);
void print_hexadecimal(int n);
void print_binary(int n);
void print_bool(bool b);
void new_line();
string read_string();
string read_line();
int read_int();
double read_double();
double read_float();
bool read_bool();
void not_really_random(int n=1289);
int random();
int random_in_range(int a, int b);

image * image_from_gif_file(string s);
image * image_from_bmp_file(string s);
image * image_from_jpeg_file(string s);
image * image_from_file(string s);
window * draw_image(image * im, int x0, int y0);
window * draw_image(image * im);
window * draw_image_from_file(string s, int x0, int y0);
window * draw_image_from_file(string s);
