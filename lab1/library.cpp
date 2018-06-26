#include <io.h>
#include <fcntl.h>
#include "library.h"
#include <setjmp.h>
#include <direct.h>
#include <time.h>
#include <sys/timeb.h>

#if _MSC_VER < 1500
#endif
#ifndef CLEARTYPE_QUALITY
  #define CLEARTYPE_QUALITY ANTIALIASED_QUALITY
#endif
#ifndef SPI_SETFONTSMOOTHINGTYPE
  #define SPI_SETFONTSMOOTHINGTYPE 0x200B
#endif
#ifndef FE_FONTSMOOTHINGCLEARTYPE
  #define FE_FONTSMOOTHINGCLEARTYPE 0x0002
#endif

LRESULT CALLBACK windowproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
DWORD __stdcall threadfunction(LPVOID);
DWORD __stdcall windowfunction(LPVOID);
#ifdef UNICODE
  TCHAR * classname = L"BFMICROSOFTSUCKSQKSHXNZKAKDISUW";
  TCHAR * windowname = L"A Window";
  TCHAR * contitle = L"Starting...";
#else
  TCHAR * classname = "BFMICROSOFTSUCKSQKSHXNZKAKDISUW";
  TCHAR * windowname = "A Window";
  TCHAR * contitle = "Starting...";
#endif

HINSTANCE instance;
static const int MESSAGE_CREATE = WM_APP+1,
                 MESSAGE_KILLIT = WM_APP+2,
                 MESSAGE_CURSOR = WM_APP+3;
static jmp_buf jb;
static string homedir="";
static FILE * logfile = NULL;
static bool logisdead=false;
static int countdown=0;
static HWND conwin = NULL;
static bool unrandom=false;
static clock_t timerstart;
static vector<HCURSOR> thecursors;
static HCURSOR curcurs=NULL;

const int color::      black=0x000000,
          color::      white=0xFFFFFF,
          color::       grey=0xC0C0C0,
          color:: light_grey=0xE0E0E0,
          color::  dark_grey=0x909090,
          color::      brown=0x13458B,
          color::        red=0x0000FF,
          color::      green=0x00FF00,
          color::       blue=0xFF0000,
          color:: light_blue=0xFFCE90,
          color::     yellow=0x00FFFF,
          color::    magenta=0xFF00FF,
          color::       cyan=0xFFFF00,
          color::       pink=0xC1B6FF,
          color::     orange=0x0094FF,
          color::   dark_red=0x0000C0,
          color::light_green=0x80FF80,
          color::  light_red=0x8080FF,
          color:: dark_green=0x008000,
          color::  dark_blue=0x900000,
          color::     indigo=0x8B3D48,
          color::     violet=0x900070,
          color::      mauve=0xE22B8A,
          color::     purple=0x8515B7,
          color:: lime_green=0x32CD9A,
          color::   sodium_d=0x00D7FF;

const int cursor::arrow      = 0,
          cursor::cross      = 1,
          cursor::hand       = 2,
          cursor::hbar       = 3,
          cursor::vbar       = 4,
          cursor::help       = 5,
          cursor::eggtimer   = 6,
          cursor::noparking  = 7;

int color::rgb(double r, double g, double b)
{ return make_color(r, g, b); }

int color::hls(double h, double l, double s)
{ return make_color_hls(h, l, s); }

string color::names[] = { "black", "white", "grey", "lightgrey", "darkgrey", "brown", "red", "green", "blue",
                    "yellow", "magenta", "cyan", "pink", "orange", "darkgreen", "indigo", "violet",
                    "mauve", "purple", "limegreen", "darkblue", "sodiumd", "lightblue", "darkred", "lightgreen",
                    "lightred" };

int color::values[] = { color::black, color::white, color::grey, color::light_grey, color::dark_grey,
                        color::brown, color::red, color::green, color::blue, color::yellow,
                        color::magenta, color::cyan, color::pink, color::orange, color::dark_green,
                        color::indigo, color::violet, color::mauve, color::purple, color::lime_green,
                        color::dark_blue, color::sodium_d, color::light_blue, color::dark_red,
                        color::light_green, color::light_red };

const int color::nn=sizeof(color::names)/sizeof(string),
          color::nv=sizeof(color::values)/sizeof(int);

const char * direction::     north="north",
           * direction::north_west="north_west",
           * direction::      west="west",
           * direction::south_west="south_west",
           * direction::     south="south",
           * direction::south_east="south_east",
           * direction::      east="east",
           * direction::north_east="north-east",
           * direction::    center="centre";

struct ievent
{ static const int none, mouse_left_press, mouse_left_release, mouse_left_click,
         mouse_right_press, mouse_right_release, mouse_right_click, mouse_middle_press,
         mouse_middle_release, mouse_middle_click, mouse_left_dragging, mouse_right_dragging,
         mouse_middle_dragging, mouse_left_dragged, mouse_right_dragged, mouse_middle_dragged,
         mouse_gone, key_typed, minimized, restored, resized, resizing, file_drop, timer,
         mouse_moved, close_window, menu_selected;
  static string name(int k);
  static const int qsize=1000;
  static ievent queue[qsize];
  static int qnum, qbegin, qend;
  static window * mousewin;
  static int mousex, mousey, mousecountdown, mousebutt, mouseisdown, currentfiledrop,
             eventmask, dragoriginx, dragoriginy;
  static string currentfiles;
  static bool draggling;
  ievent();
  ievent(int k);
  window * which;
  int kind, x_pos, y_pos, char_code;

  static bool add(window * w, int k, int x, int y, int c);
  static ievent next(bool waitforit=false);
  static bool any();
  static void mousedown(window * w, int x, int y, int b);
  static void mouseup(window * w, int x, int y, int b);
  static void mousedragged(window * w, int x, int y);
  static void mousedragging(window * w, int x, int y);
  static void mouseexit(window * w);
  static void dropfiles(window * w, int x, int y, string files);
  static void tick(); };

static ievent the_event(ievent::none);
bool ievent::draggling=false;

const int ievent::                  none=0,
          ievent::      mouse_left_press=1,
          ievent::    mouse_left_release=2,
          ievent::      mouse_left_click=3,
          ievent::     mouse_right_press=4,
          ievent::   mouse_right_release=5,
          ievent::     mouse_right_click=6,
          ievent::    mouse_middle_press=7,
          ievent::  mouse_middle_release=8,
          ievent::    mouse_middle_click=9,
          ievent::   mouse_left_dragged=10,
          ievent:: mouse_middle_dragged=11,
          ievent::  mouse_right_dragged=12,
          ievent::           mouse_gone=13,
          ievent::            key_typed=14,
          ievent::            minimized=15,
          ievent::             restored=16,
          ievent::              resized=17,
          ievent::             resizing=18,
          ievent::            file_drop=19,
          ievent::                timer=20,
          ievent::  mouse_left_dragging=21,
          ievent::mouse_middle_dragging=22,
          ievent:: mouse_right_dragging=23,
          ievent::          mouse_moved=24,
          ievent::         close_window=25,
          ievent::        menu_selected=26;

const int event::                  none=0,
          event::      mouse_left_press=1,
          event::    mouse_left_release=2,
          event::      mouse_left_click=3,
          event::     mouse_right_press=4,
          event::   mouse_right_release=5,
          event::     mouse_right_click=6,
          event::    mouse_middle_press=7,
          event::  mouse_middle_release=8,
          event::    mouse_middle_click=9,
          event::   mouse_left_dragged=10,
          event:: mouse_middle_dragged=11,
          event::  mouse_right_dragged=12,
          event::           mouse_gone=13,
          event::            key_typed=14,
          event::            minimized=15,
          event::             restored=16,
          event::              resized=17,
          event::             resizing=18,
          event::            file_drop=19,
          event::                timer=20,
          event::  mouse_left_dragging=21,
          event::mouse_middle_dragging=22,
          event:: mouse_right_dragging=23,
          event::          mouse_moved=24,
          event::         close_window=25,
          event::        menu_selected=26;

const int style::     italic=1,
          style:: underlined=2,
          style::crossed_out=4,
          style:: very_light=8,
          style::     light=16,
          style::    normal=32,
          style::      bold=64,
          style::very_bold=128;

struct DROPFILES
{ DWORD pFiles;
  POINT pt;
  BOOL fNC;
  BOOL fWide; };

static HANDLE programthread, windowsthread, threadsync, deadsync, waitsync, eventsync;
static DWORD programthreadid, windowsthreadid;
static window * babywindow, * cw;
window * screen = NULL;
static bool running=false, consolewanted=true;
static vector<string> fontlist;
static shape theshape;
static bool quittened = false, suspended = false;

string properstring(const TCHAR * s)
{ string r="";
  for (int i=0; 1; i+=1)
  { if (s[i]==0) break;
    r+=(char)(s[i]); }
  return r; }

TCHAR * widestring(const char * s)
{ int n=strlen(s);
  TCHAR * r = new TCHAR [n+1];
  for (int i=0; i<n; i+=1)
    r[i]=s[i];
  r[n]=0;
  return r; }

TCHAR * widestring(string s)
{ return widestring(s.c_str()); }

void widestrcpy(TCHAR * dest, const char * src, int destlen)
{ for (int i=0; i<destlen; i+=1)
  { dest[i]=src[i];
    if (src[i]==0) break; } }

void widestrcpy(TCHAR * dest, string src, int destlen)
{ widestrcpy(dest, src.c_str(), destlen); }

string int_to_string(int i)
{ char s[50];
  sprintf(s, "%d", i);
  return s; }

int ievent::qnum=0, ievent::qbegin=0, ievent::qend=0;
int ievent::mousex=0, ievent::mousey=0, ievent::mousecountdown=0, ievent::mousebutt=0,
    ievent::mouseisdown=0, ievent::currentfiledrop=0, ievent::eventmask=0,
    ievent::dragoriginx=0, ievent::dragoriginy=0;
string ievent::currentfiles="";
window * ievent::mousewin = NULL;
ievent ievent::queue[ievent::qsize];
HANDLE window::semaphore=NULL;

ievent::ievent()
{ }

ievent::ievent(int k)
{ kind=k;
  which=NULL; }

string ievent::name(int e)
{ switch (e)
  { case none: return "None";
    case mouse_left_press: return "LeftPress";
    case mouse_left_release: return "LeftRelease";
    case mouse_left_click: return "LeftClick";
    case mouse_right_press: return "RightPress";
    case mouse_right_release: return "RightRelease";
    case mouse_right_click: return "RightClick";
    case mouse_middle_press: return "MiddlePress";
    case mouse_middle_release: return "MiddleRelease";
    case mouse_middle_click: return "MiddleClick";
    case mouse_left_dragged: return "LeftDragged";
    case mouse_middle_dragged: return "MiddleDragged";
    case mouse_right_dragged: return "RightDragged";
    case mouse_left_dragging: return "LeftDragging";
    case mouse_middle_dragging: return "MiddleDragging";
    case mouse_right_dragging: return "RightDragging";
    case mouse_moved: return "MouseMoved";
    case mouse_gone: return "MouseGone";
    case key_typed: return "KeyTyped";
    case minimized: return "Minimized";
    case restored: return "Restored";
    case resized: return "Resized";
    case resizing: return "Resizing";
    case file_drop: return "FileDrop";
    case timer: return "Timer";
    case close_window: return "CloseWindow";
    case menu_selected: return "MenuSelected";
    default: return "UnknownEvent"; } }

static const int presses[] =
    { ievent::mouse_left_press, ievent::mouse_middle_press, ievent::mouse_right_press };
static const int releases[] =
    { ievent::mouse_left_release, ievent::mouse_middle_release, ievent::mouse_right_release };
static const int clicks[] =
    { ievent::mouse_left_click, ievent::mouse_middle_click, ievent::mouse_right_click };
static const int draggeds[] =
    { ievent::mouse_left_dragged, ievent::mouse_middle_dragged, ievent::mouse_right_dragged };
static const int draggings[] =
    { ievent::mouse_left_dragging, ievent::mouse_middle_dragging, ievent::mouse_right_dragging };

void ievent::mousedown(window * w, int x, int y, int b)
{ mousebutt=b;
  mouseisdown=1;
  if (w->autoclick)
  { mousewin=w;
    mousex=x;
    mousey=y;
    mousecountdown=20; }
  else if (eventmask & mouse_but_not_left_click)
    add(w, presses[b], x, y, 0); }

void ievent::mouseup(window * w, int x, int y, int b)
{ if (!mouseisdown)
    return;
  mouseisdown=0;
  if (w->autoclick)
  { if (mousewin==NULL)
    { if (eventmask & mouse_but_not_left_click)
        add(w, releases[mousebutt], x, y, 0); }
    else if (mousewin==w && mousex==x && mousey==y && mousebutt==b)
    { if ((eventmask & mouse_but_not_left_click)!=0 ||
          b==0 && (eventmask & mouse_clicked)!=0)
      add(w, clicks[mousebutt], x, y, 0); }
    mousewin=NULL; }
  else if (eventmask & mouse_but_not_left_click)
    add(w, releases[b], x, y, 0); }

void ievent::mouseexit(window * w)
{ if (mouseisdown)
  { if (eventmask & mouse_but_not_left_click)
      add(w, mouse_gone, 0, 0, 0); }
  mouseisdown=0;
  if (w->autoclick)
  { mousewin=NULL; } }

void ievent::dropfiles(window * w, int x, int y, string files)
{ currentfiledrop+=1;
  currentfiles=files;
  if (eventmask & file_dragged_in)
    add(w, file_drop, x, y, currentfiledrop); }

void ievent::tick()
{ if (mousewin!=NULL && mousewin->autoclick)
  { mousecountdown-=1;
    if (mousecountdown>0)
      return;
    if (eventmask & mouse_but_not_left_click)
      add(mousewin, presses[mousebutt], mousex, mousey, 0);
    mousewin=NULL; }
  if (countdown>0)
  { WaitForSingleObject(eventsync, INFINITE);
    if (countdown>0)
    { countdown-=1;
      if (countdown==0)
      { add(NULL, timer, 0, 0, 0); } }
    ReleaseMutex(eventsync); } }

void ievent::mousedragged(window * w, int x, int y)
{ if (mousewin!=NULL)
  { dragoriginx=mousex;
    dragoriginy=mousey;
    if (eventmask & mouse_but_not_left_click)
      add(mousewin, presses[mousebutt], mousex, mousey, 0); }
  mousewin=NULL;
  if (mouseisdown)
  { ievent::draggling=true;
    if ((eventmask & mouse_dragged)!=0)
      add(w, draggeds[mousebutt], x, y, 0); } }

void ievent::mousedragging(window * w, int x, int y)
{ if (mousewin!=NULL)
  { dragoriginx=mousex;
    dragoriginy=mousey;
    if (eventmask & mouse_but_not_left_click)
      add(mousewin, presses[mousebutt], mousex, mousey, 0); }
  mousewin=NULL;
  if (mouseisdown)
  { ievent::draggling=true;
    if ((eventmask & mouse_being_dragged)!=0)
      add(w, draggings[mousebutt], x, y, 0); } }

bool ievent::add(window * w, int k, int x, int y, int c)
{ if (qnum>=qsize)
    return false;
  WaitForSingleObject(eventsync, INFINITE);
  ievent & e = queue[qend];
  qend+=1;
  if (qend>=qsize)
    qend=0;
  qnum+=1;
  e.which=w;
  e.kind=k;
  e.x_pos=x;
  e.y_pos=y;
  e.char_code=c;
  SetEvent(waitsync);
  ReleaseMutex(eventsync);
  return true; }

ievent ievent::next(bool wait)
{ ievent r;
  while (1)
  { WaitForSingleObject(eventsync, INFINITE);
    if (qnum<=0)
    { if (!wait)
      { ReleaseMutex(eventsync);
        return ievent(none); }
      ResetEvent(waitsync);
      ReleaseMutex(eventsync);
      WaitForSingleObject(waitsync, 567);
      continue; }
    r = queue[qbegin];
    qbegin+=1;
    if (qbegin>=qsize)
      qbegin=0;
    qnum-=1;
    ReleaseMutex(eventsync);
    if (r.kind!=file_drop)
      break;
    if (r.char_code==currentfiledrop)
      break; }
  return r; }

bool ievent::any()
{ while (1)
  { WaitForSingleObject(eventsync, INFINITE);
    if (qnum<=0)
    { ReleaseMutex(eventsync);
      return false; }
    ievent r = queue[qbegin];
    if (r.kind!=file_drop || r.char_code==currentfiledrop)
    { ReleaseMutex(eventsync);
      return true; }
    qbegin+=1;
    if (qbegin>=qsize)
      qbegin=0;
    qnum-=1;
    ReleaseMutex(eventsync); } }

vector<window *> window::windows;
int window::numwindows = 0;

vector<fontrec *> fontrec:: fonts;
fontrec * fontrec::defaultfont = NULL;

fontrec::fontrec(string n, int sz, int st, int ang, HFONT f):
  name(n), size(sz), style(st), angle(ang), it(f), refcount(0)
{ }

fontrec * fontrec::font(string nm, int sz, int st, int ang)
{ for (int i=0; i<fonts.size(); i+=1)
  { fontrec * x = fonts[i];
    if (_stricmp(x->name.c_str(), nm.c_str())==0 && x->size==sz && x->style==st && x->angle==ang)
      return x; }
  int ital=0, und=0, cross=0, wt=400;
  if (st & style::italic) ital=1;
  if (st & style::underlined) und=1;
  if (st & style::crossed_out) cross=1;
  if (st & style::very_light) wt=FW_THIN;
  if (st & style::light) wt=FW_LIGHT;
  if (st & style::normal) wt=FW_NORMAL;
  if (st & style::bold) wt=FW_BOLD;
  if (st & style::very_bold) wt=FW_HEAVY;
  int angle=900-ang;
  if (angle<0) angle+=3600;
  if (angle>=3600) angle-=3600;
  HFONT x = CreateFont(sz, /* -MulDiv(sz, GetDeviceCaps(screen->dc, LOGPIXELSY), 72), */
                       0, angle, angle, wt, ital, und, cross, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH,
                       widestring(nm.c_str()));
  fontrec * f = new fontrec(nm, sz, st, ang, x);
  fonts.push_back(f);
  return f; }

fontrec * font(string nm, int sz, int st, int ang)
{ return fontrec::font(nm, sz, st, ang); }

void fontrec::use()
{ refcount+=1; }

void fontrec::abandon()
{ refcount-=1;
  if (refcount<=0 && this!=defaultfont)
  { size=0;
    style=0;
    name="";
    for (int i=0; i<fonts.size(); i+=1)
    { if (fonts[i]==this)
      { fonts[i]=fonts[fonts.size()-1];
        fonts.pop_back();
        break; } }
    DeleteObject(it);
    delete this; } }

HCURSOR imake_cursor(string patt, int x, int y)
{ int ands[32], xors[32];
  int row=0, col=0;
  unsigned int mask, aval, xval;
  for (int i=0; i<1024; i+=1)
  { char c = ' ';
    if (i<patt.length())
      c = toupper(patt[i]);
    if (col==0)
    { mask = 0x00000080;
      aval = 0;
      xval = 0; }
    else if (col==8)
      mask = 0x00008000;
    else if (col==16)
      mask = 0x00800000;
    else if (col==24)
      mask = 0x80000000;
    if (c=='W')
      xval |= mask;
    else if (c==' ')
      aval |= mask;
    else if (c=='R')
    { xval |= mask;
      aval |= mask; }
    if (col==31)
    { ands[row] = aval;
      xors[row] = xval;
      col = 0;
      row += 1; }
    else
      col += 1;
    mask >>= 1; }
  return CreateCursor(instance, x, y, 32, 32, ands, xors); }

int make_cursor(string patt, int x, int y)
{ HCURSOR c = imake_cursor(patt, x, y);
  thecursors.push_back(c);
  return thecursors.size()-1; }

window * window::set_cursor(int n)
{ if (n<0 || n>=thecursors.size())
    n=0;
  curcurs=thecursors[n];
  PostMessage(wnd, MESSAGE_CURSOR, 0, myindex);
  return this; }

window * set_cursor(int n)
{ return cw->set_cursor(n); }

static string icrossstr = "                                "
                          "                                "
                          "                                "
                          "                                "
                          "    BW                    BW    "
                          "     BW                  BW     "
                          "      BW                BW      "
                          "       BW              BW       "
                          "        BW            BW        "
                          "         BW          BW         "
                          "          BW        BW          "
                          "           BW      BW           "
                          "            BW    BW            "
                          "             BW  BW             "
                          "              BWBW              "
                          "               BW               "
                          "              BWBW              "
                          "             BW  BW             "
                          "            BW    BW            "
                          "           BW      BW           "
                          "          BW        BW          "
                          "         BW          BW         "
                          "        BW            BW        "
                          "       BW              BW       "
                          "      BW                BW      "
                          "     BW                  BW     "
                          "    BW                    BW    "
                          "                                "
                          "                                "
                          "                                "
                          "                                "
                          "                                ";

static string ihbarstr = "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "    B                      B    "
                         "    BW                     BW   "
                         "    BW                     BW   "
                         "    BW                     BW   "
                         "     BBBBBBBBBBBBBBBBBBBBBB W   "
                         "    B WWWWWWWWWWWWWWWWWWWWWB    "
                         "    BW                     BW   "
                         "    BW                     BW   "
                         "    BW                     BW   "
                         "     W                      W   "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                "
                         "                                ";

static string ivbarstr = "                                "
                         "                                "
                         "                                "
                         "                                "
                         "           BBBB BBBB            "
                         "            WWWB WWWW           "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "               BW               "
                         "           BBBB BBBB            "
                         "            WWWW WWWW           "
                         "                                "
                         "                                "
                         "                                ";

void window::init1()
{ wnd = NULL;
  dc=NULL;
  bitmap = NULL;
  bitmapdc = NULL;
  creationevent=NULL;
  if (curcurs==NULL)
    curcurs=LoadCursor(NULL, IDC_ARROW);
  if (thecursors.size()==0)
  { thecursors.push_back(LoadCursor(NULL, IDC_ARROW));
    thecursors.push_back(imake_cursor(icrossstr, 15, 15));
    thecursors.push_back(LoadCursor(NULL, IDC_HAND));
    thecursors.push_back(imake_cursor(ihbarstr, 15, 15));
    thecursors.push_back(imake_cursor(ivbarstr, 15, 15));
    thecursors.push_back(LoadCursor(NULL, IDC_HELP));
    thecursors.push_back(LoadCursor(NULL, IDC_WAIT));
    thecursors.push_back(LoadCursor(NULL, IDC_NO)); }
  edgex=0;
  edgey=0;
  color = RGB(0, 0, 0);
  penw = 1;
  brushcolor = color;
  textcolor = textcolor;
  pen = CreatePen(PS_SOLID, penw, color);
  brush = CreateSolidBrush(color);
  hregion=NULL;
  menu=NULL;
  currentfont=NULL;
  heading=0;
  flags=0;
  maxcharheightonline=0;
  pench=false;
  haswindow=false;
  hasbitmap=false;
  usewindow=false;
  usebitmap=false;
  autoclick=true;
  canusewindow=false;
  posint=true;
  isprinter=false;
  printended=false;
  extradata=NULL;
  if (semaphore==NULL)
    semaphore = CreateMutex(NULL, false, NULL);
  myindex=windows.size();
  windows.push_back(this);
  numwindows+=1; }

void window::init2()
{ prevclient = client;
  wantw = penw;
  wantc = color;
  xpos = (client.right-client.left)/2;
  ypos = (client.bottom-client.top)/2;
  currentfont = fontrec::defaultfont;
  bool gotfont = false;
  if (dc!=NULL)
  { SetPolyFillMode(dc, WINDING);
    SelectObject(dc, pen);
    SelectObject(dc, brush);
    SetTextColor(dc, color);
    MoveToEx(dc, xpos, ypos, NULL);
    SetBkMode(dc, TRANSPARENT);
    SelectObject(dc, currentfont);
    GetTextMetrics(dc, &fontdata);
    gotfont=true; }
  if (bitmapdc!=NULL)
  { SetPolyFillMode(bitmapdc, WINDING);
    SelectObject(bitmapdc, pen);
    SelectObject(bitmapdc, brush);
    SetTextColor(bitmapdc, color);
    MoveToEx(bitmapdc, xpos, ypos, NULL);
    SetBkMode(bitmapdc, TRANSPARENT);
    SelectObject(bitmapdc, currentfont);
    if (!gotfont)
      GetTextMetrics(bitmapdc, &fontdata); }
  set_font_size(dpiy/4); }

window::window(bool isscreen)
{ init1();
  if (isscreen)
  { dc = GetDC(NULL);
    dpix=GetDeviceCaps(dc, LOGPIXELSX);
    dpiy=GetDeviceCaps(dc, LOGPIXELSY);
    SetRect(&whole, 0, 0, GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES));
    client=whole;
    const char * usefontname = "times new roman";
    int usefontsize = dpiy/16;
    HFONT fh = CreateFont(usefontsize,
                          0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH,
                          widestring(usefontname));
    fontrec * fr = new fontrec(usefontname, usefontsize, FW_NORMAL, 900, fh);
    fontrec::defaultfont = fr;
    fontrec::fonts.push_back(fr);
    currentfont=fr;
    fr->use();
    haswindow = true;
    usewindow = true; }
  else
  { int tempsize=50;
    dpix=screen->dpix;
    dpiy=screen->dpiy;
    bitmap = CreateCompatibleBitmap(screen->dc, tempsize, tempsize);
    bitmapdc = CreateCompatibleDC(screen->dc);
    SelectObject(bitmapdc, bitmap);
    BitBlt(bitmapdc, 0, 0, tempsize, tempsize, NULL, 0, 0, WHITENESS);
    wnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, classname, windowname,
                         WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, tempsize,
                         tempsize, NULL, NULL, instance, NULL);
    GetWindowRect(wnd, &whole);
    GetClientRect(wnd, &client);
    dc = GetDC(wnd);
    SetWindowLong(wnd, GWL_USERDATA, myindex);
    haswindow = true;
    hasbitmap = true;
    usewindow = true;
    usebitmap = true;
    SetTimer(wnd, 1, 10, NULL); }
  init2(); }

window::window(int w, int h, int x, int y)
{ init1();
  if (x==-7777)
    x=(screen->whole.right-w)/2;
  if (y==-7777)
    y=(screen->whole.bottom-h)/2;
  SetRect(&client, x, y, x+w+1, y+h+1);
  creationevent = CreateEvent(NULL, false, false, NULL);
  ResetEvent(creationevent);
  PostMessage(babywindow->wnd, MESSAGE_CREATE, 0, myindex);
  WaitForSingleObject(creationevent, INFINITE);
  CloseHandle(creationevent);
  cw = this; }

void window::nonthreadcreate()
{ bitmap = CreateCompatibleBitmap(screen->dc, screen->whole.right-screen->whole.left,
                                              screen->whole.bottom-screen->whole.top);
  bitmapdc = CreateCompatibleDC(screen->dc);
  SelectObject(bitmapdc, bitmap);
  BitBlt(bitmapdc, 0, 0, screen->whole.right-screen->whole.left,
                         screen->whole.bottom-screen->whole.top, NULL, 0, 0, WHITENESS);
  int extstyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES;
  int style = WS_OVERLAPPEDWINDOW;
  whole=client;
  AdjustWindowRectEx(&whole, style, false, extstyle);
  wnd = CreateWindowEx(extstyle, classname, windowname, style,
                       whole.left+1, whole.top+1, whole.right-whole.left, whole.bottom-whole.top,
                       NULL, NULL, instance, NULL);
  GetWindowRect(wnd, &whole);
  GetClientRect(wnd, &client);
  dc = GetDC(wnd);
  dpix=GetDeviceCaps(dc, LOGPIXELSX);
  dpiy=GetDeviceCaps(dc, LOGPIXELSY);
  SetWindowLong(wnd, GWL_USERDATA, myindex);
  ShowWindow(wnd, SW_SHOW);
  SetWindowPos(wnd, HWND_TOPMOST, whole.left, whole.top, 0, 0,
                    SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
  GetWindowRect(wnd, &whole);
  GetClientRect(wnd, &client);
  haswindow = true;
  hasbitmap = true;
  usewindow = true;
  usebitmap = true;
  init2();
  SetEvent(creationevent); }

window::window(char printer)
{ init1();
  isprinter=true;
  PRINTDLG p;
  memset(&p, 0, sizeof PRINTDLG);
  p.lStructSize=sizeof PRINTDLG;
  p.hwndOwner=cw->wnd;
  p.Flags=PD_ALLPAGES | PD_COLLATE | PD_RETURNDC | PD_NOSELECTION | PD_NOPAGENUMS;
  p.nCopies=1;
  DOCINFO d;
  memset(&d, 0, sizeof DOCINFO);
  d.cbSize=sizeof DOCINFO;
  d.lpszDocName=(LPCTSTR)TEXT("Results");
  int err=PrintDlg(&p);
  if (err==0)
  { printf("PrintDlgEx failed\n");
    return; }
  err=StartDoc(p.hDC, &d);
  if (err<=0)
  { printf("StartDoc failed\n");
    return; }
  err=StartPage(p.hDC);
  if (err<=0)
  { printf("StartPage failed\n");
    EndDoc(p.hDC);
    return; }
  bitmapdc=p.hDC;
  cw = this;
  dpix=GetDeviceCaps(bitmapdc, LOGPIXELSX);
  dpiy=GetDeviceCaps(bitmapdc, LOGPIXELSY);
  edgex=GetDeviceCaps(bitmapdc, PHYSICALOFFSETX);
  edgey=GetDeviceCaps(bitmapdc, PHYSICALOFFSETY);
  client.left=0;
  client.right=GetDeviceCaps(bitmapdc, HORZRES);
  client.top=0;
  client.bottom=GetDeviceCaps(bitmapdc, VERTRES);
  whole=client;
  hasbitmap = true;
  usebitmap = true;
  init2(); }

window::window(bool offscreen, int w, int h)
{ init1();
  dpix=screen->dpix;
  dpiy=screen->dpiy;
  bitmap = CreateCompatibleBitmap(screen->dc, w, h);
  bitmapdc = CreateCompatibleDC(screen->dc);
  SelectObject(bitmapdc, bitmap);
  BitBlt(bitmapdc, 0, 0, w, h, NULL, 0, 0, WHITENESS);
  SetRect(&client, 0, 0, w, h);
  SetRect(&whole, 0, 0, w, h);
  hasbitmap = true;
  usebitmap = true;
  init2(); }

window * create_off_screen_area(int w, int h)
{ return new window(true, w, h); }

window::~window()
{ if (hregion!=NULL)
    DeleteObject(hregion);
  if (isprinter)
  { if (!printended)
    { EndPage(bitmapdc);
      EndDoc(bitmapdc); }
    DeleteDC(bitmapdc);
    return; }
  if (hasbitmap)
  { DeleteDC(bitmapdc);
    DeleteObject(bitmap); }
  if (haswindow)
  { ReleaseDC(wnd, dc); }
  DeleteObject(pen);
  DeleteObject(brush); }

void window::destroy()
{ if (haswindow)
    PostMessage(babywindow->wnd, MESSAGE_KILLIT, 0, myindex); }

void copy_area(int w, int h, window * dst, int xd, int yd, window * src, int xs, int ys)
{ HDC sdc=src->dc;
  if (src->hasbitmap)
    sdc=src->bitmapdc;
  if (sdc==NULL)
    return;
  if (dst->usebitmap)
    BitBlt(dst->bitmapdc, xd, yd, w, h, sdc, xs, ys, SRCCOPY);
  if (dst->usewindow)
  { WaitForSingleObject(window::semaphore, INFINITE);
    BitBlt(dst->dc, xd, yd, w, h, sdc, xs, ys, SRCCOPY);
    ReleaseMutex(window::semaphore); } }

void copy_area(int w, int h, int xd, int yd, int xs, int ys)
{ copy_area(w, h, cw, xd, yd, cw, xs, ys); }

void copy_area(int w, int h, int xd, int yd, window * src)
{ copy_area(w, h, cw, xd, yd, src, 0, 0); }

static window * preprintwin = NULL;

void start_printing()
{ preprintwin=cw;
  new window('P'); }

window * window::next_page()
{ if (isprinter)
  { EndPage(bitmapdc);
    StartPage(bitmapdc); }
  return this; }

void next_page()
{ cw->next_page(); }

window * window::finish_printing()
{ if (isprinter)
  { EndPage(bitmapdc);
    EndDoc(bitmapdc);
    printended=true; }
  return this; }

void finish_printing()
{ cw->finish_printing();
  delete cw;
  cw=preprintwin;
  preprintwin=NULL; }

int CALLBACK fontcallback(const LOGFONT *lplf, const TEXTMETRIC *lptm,
                          DWORD dwType, LPARAM lpData)
{ if (lplf->lfFaceName[0]!='@' && dwType==TRUETYPE_FONTTYPE)
    fontlist.push_back(properstring(lplf->lfFaceName));
  return 1; }

int consolestopper()
{ consolewanted=false;
  return rand(); }

void create_console()
{ if (conwin!=NULL)
    return;
  AllocConsole();
  SetConsoleTitle(contitle);
  Sleep(40);
  conwin = FindWindow(NULL, contitle);
  freopen("CONIN$", "r", stdin);
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);
  RECT conr;
  GetWindowRect(conwin, &conr);
  conr.top=screen->whole.bottom-(conr.bottom-conr.top);
  SetWindowPos(conwin, HWND_TOPMOST, 25, conr.top-25, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
  BringWindowToTop(conwin);
  set_console_caption("running..."); }

void set_console_caption(string s)
{ if (conwin!=NULL)
  { TCHAR str[1025];
    int n=s.length();
    if (n>1024) n=1024;
    for (int i=0; i<n; i+=1)
      str[i]=s[i];
    str[n]=0;
    SetWindowText(conwin, str); } }

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance, PSTR str, INT cmdshow)
{ instance = thisinstance;
  WNDCLASS wc;
  wc.style=CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc=windowproc;
  wc.cbClsExtra=0;
  wc.cbWndExtra=sizeof(DWORD);
  wc.hInstance=thisinstance;
  wc.hIcon=LoadIcon(thisinstance, IDI_ASTERISK);
  wc.hCursor=LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName=NULL;
  wc.lpszClassName=classname;
  RegisterClass(&wc);

  SystemParametersInfo(SPI_SETFONTSMOOTHING, true, 0,
                       SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
  SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, FE_FONTSMOOTHINGCLEARTYPE, 0,
                       SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

  screen = new window(true);
  cw = screen;
  if (consolewanted)
    create_console();

  EnumFonts(screen->dc, NULL, fontcallback, 0);

  eventsync = CreateMutex(NULL, false, NULL);
  threadsync = CreateEvent(NULL, false, false, NULL);
  ResetEvent(threadsync);
  deadsync = CreateEvent(NULL, false, false, NULL);
  ResetEvent(deadsync);
  waitsync = CreateEvent(NULL, false, false, NULL);
  ResetEvent(waitsync);
  windowsthread = CreateThread(NULL, 0, windowfunction, NULL, 0, & windowsthreadid);
  WaitForSingleObject(threadsync, INFINITE);
  ResetEvent(threadsync);
  programthread = CreateThread(NULL, 16<<20, threadfunction, NULL, 0, & programthreadid);
  WaitForSingleObject(threadsync, INFINITE);
  WaitForSingleObject(deadsync, INFINITE); }

int get_screen_width()
{ if (screen==NULL)
  { HDC dc = GetDC(NULL);
    int answer = GetDeviceCaps(dc, HORZRES);
    ReleaseDC(NULL, dc);
    return answer; }
  return screen->whole.right-screen->whole.left; }

int get_screen_height()
{ if (screen==NULL)
  { HDC dc = GetDC(NULL);
    int answer = GetDeviceCaps(dc, VERTRES);
    ReleaseDC(NULL, dc);
    return answer; }
  return screen->whole.bottom-screen->whole.top; }

void get_screen_size(int & w, int & h)
{ if (screen==NULL)
  { HDC dc = GetDC(NULL);
    w = GetDeviceCaps(dc, HORZRES);
    w = GetDeviceCaps(dc, VERTRES);
    ReleaseDC(NULL, dc); }
  else
  { w = screen->whole.right-screen->whole.left;
    h = screen->whole.bottom-screen->whole.top; } }

void hide_console()
{ if (conwin!=NULL)
    ShowWindow(conwin, SW_HIDE);
  if (cw!=NULL)
    cw->make_window_active(); }

void make_console_active()
{ if (conwin!=NULL)
  { ShowWindow(conwin, SW_SHOW);
    BringWindowToTop(conwin);
    SetForegroundWindow(conwin); }
  else
    create_console(); }

void show_console()
{ make_console_active(); }

window * window::make_window_active()
{ ShowWindow(wnd, SW_SHOW);
  BringWindowToTop(wnd);
  SetForegroundWindow(wnd);
  return this; }

window * make_window_active(window * w)
{ if (w==NULL)
    w=cw;
  return w->make_window_active(); }

void set_console_position(int x, int y)
{ if (conwin!=NULL)
    SetWindowPos(conwin, HWND_TOPMOST, x, y, 0, 0,
                         SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME); }

void set_console_size(int w, int h)
{ if (conwin!=NULL)
    SetWindowPos(conwin, HWND_TOPMOST, 0, 0, w, h,
                         SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME); }

window * window::hide_window()
{ if (haswindow)
    ShowWindow(wnd, SW_HIDE);
  return this; }

window * hide_window()
{ return cw->hide_window(); }

window * window::show_window()
{ if (haswindow)
  { SetWindowPos(wnd, HWND_TOPMOST, 0, 0, 0, 0,
                      SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
    ShowWindow(wnd, SW_SHOW); }
  return this; }

window * show_window()
{ return cw->show_window(); }

window * window::make(int w, int h, int x, int y)
{ window * r = new window(w, h, x, y);
  return r; }

window * make_window(int w, int h, int x, int y)
{ window * r = new window(w, h, x, y);
  return r; }

void window::set_window_data(windowdata * e)
{ extradata=e; }

windowdata * window::get_window_data()
{ return extradata; }

window * select(window * win)
{ cw=win;
  return cw; }

window * current_window()
{ return cw; }

window * window::set_window_position(int x, int y)
{ if (haswindow)
  { SetWindowPos(wnd, HWND_TOPMOST, x, y, whole.right-whole.left, whole.bottom-whole.top,
                      SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
    GetWindowRect(wnd, &whole);
    GetClientRect(wnd, &client); }
  return this; }

window * set_window_position(int x, int y)
{ return cw->set_window_position(x, y); }

window * window::get_window_position(int & x, int & y)
{ if (haswindow)
  { RECT r;
    GetWindowRect(wnd, &r);
    x = r.left;
    y = r.top; }
  return this; }

window * get_window_position(int & x, int & y)
{ return cw->get_window_size(x, y); }

int window::get_window_x_position()
{ RECT r;
  if (haswindow)
    GetWindowRect(wnd, &r);
  return r.left; }

int get_window_x_position()
{ return cw->get_window_x_position(); }

int window::get_window_y_position()
{ RECT r;
  if (haswindow)
    GetWindowRect(wnd, &r);
  return r.top; }

int get_window_y_position()
{ return cw->get_window_y_position(); }

window * window::set_window_size(int w, int h)
{ if (haswindow)
  { SetRect(&client, 0, 0, w+1, h+1);
    whole=client;
    int extstyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES;
    int style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectEx(&whole, style, false, extstyle);
    SetWindowPos(wnd, HWND_TOPMOST, 0, 0, whole.right-whole.left, whole.bottom-whole.top,
                      SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME);
    GetWindowRect(wnd, &whole);
    GetClientRect(wnd, &client); }
  return this; }

window * set_window_size(int w, int h)
{ return cw->set_window_size(w, h); }

window * window::set_full_window_size(int w, int h)
{ if (haswindow)
  { SetRect(&whole, 0, 0, w+1, h+1);
    SetWindowPos(wnd, HWND_TOPMOST, 0, 0, whole.right-whole.left, whole.bottom-whole.top,
                      SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME);
    GetWindowRect(wnd, &whole);
    GetClientRect(wnd, &client); }
  return this; }

window * set_full_window_size(int w, int h)
{ return cw->set_full_window_size(w, h); }

int window::get_window_width()
{ if (haswindow)
  { RECT r;
    GetClientRect(wnd, &r);
    return r.right-r.left+1; }
  else
    return client.right-client.left+1; }

int get_window_width()
{ return cw->get_window_width(); }

int window::get_full_window_width()
{ if (haswindow)
  { RECT r;
    GetWindowRect(wnd, &r);
    return r.right-r.left+1; }
  else
    return whole.right-whole.left+1; }

int get_full_window_width()
{ return cw->get_full_window_width(); }

int window::get_window_height()
{ if (haswindow)
  { RECT r;
    GetClientRect(wnd, &r);
    return r.bottom-r.top+1; }
  else
    return client.bottom-client.top+1; }

int get_window_height()
{ return cw->get_window_height(); }

int window::get_full_window_height()
{ if (haswindow)
  { RECT r;
    GetWindowRect(wnd, &r);
    return r.bottom-r.top+1; }
  else
    return whole.bottom-whole.top+1; }

int get_full_window_height()
{ return cw->get_full_window_height(); }

window * window::get_window_size(int & w, int & h)
{ if (haswindow)
  { RECT r;
    GetClientRect(wnd, &r);
    w = r.right-r.left+1;
    h = r.bottom-r.top+1; }
  else
  { w = client.right-client.left+1;
    h = client.bottom-client.top+1; }
  return this; }

window * get_window_size(int & w, int & h)
{ return cw->get_window_size(w, h); }

window * window::get_full_window_size(int & w, int & h)
{ if (haswindow)
  { RECT r;
    GetWindowRect(wnd, &r);
    w = r.right-r.left+1;
    h = r.bottom-r.top+1; }
  else
  { w = whole.right-whole.left+1;
    h = whole.bottom-whole.top+1; }
  return this; }

window * get_full_window_size(int & w, int & h)
{ return cw->get_full_window_size(w, h); }

int window::get_pixels_per_inch()
{ return dpix; }

int get_pixels_per_inch()
{ if (cw==NULL)
  { HDC dc = GetDC(NULL);
    int answer = GetDeviceCaps(dc, LOGPIXELSX);
    ReleaseDC(NULL, dc);
    return answer; }
  return cw->get_pixels_per_inch(); }

int window::get_vertical_pixels_per_inch()
{ return dpiy; }

int get_vertical_pixels_per_inch()
{ if (cw==NULL)
  { HDC dc = GetDC(NULL);
    int answer = GetDeviceCaps(dc, LOGPIXELSY);
    ReleaseDC(NULL, dc);
    return answer; }
  return cw->get_vertical_pixels_per_inch(); }

int window::get_left_edge()
{ return edgex; }

int get_left_edge()
{ return cw->get_left_edge(); }

int window::get_top_edge()
{ return edgey; }

int get_top_edge()
{ return cw->get_top_edge(); }

menu_reference window::add_menu(string name)
{ if (!haswindow)
    return NULL;
  if (menu==NULL)
  { int w, h;
    get_window_size(w, h);
    menu=CreateMenu();
    SetMenu(wnd, menu);
    DrawMenuBar(wnd);
    set_window_size(w, h); }
  TCHAR * ws = widestring(name);
  menu_reference newmenu=CreatePopupMenu();
  AppendMenu(menu, MF_POPUP, (int)newmenu, ws);
  delete[] ws;
  DrawMenuBar(wnd);
  return newmenu; }

menu_reference add_menu(string name)
{ return cw->add_menu(name); }

menu_reference window::add_sub_menu(menu_reference parent, string name)
{ if (!haswindow)
    return NULL;
  if (menu==NULL)
  { int w, h;
    get_window_size(w, h);
    menu=CreateMenu();
    SetMenu(wnd, menu);
    DrawMenuBar(wnd);
    set_window_size(w, h); }
  if (parent==NULL)
    parent=menu;
  TCHAR * ws = widestring(name);
  menu_reference newmenu=CreatePopupMenu();
  AppendMenu(parent, MF_POPUP, (int)newmenu, ws);
  delete[] ws;
  DrawMenuBar(wnd);
  return newmenu; }

menu_reference add_sub_menu(menu_reference parent, string name)
{ return cw->add_sub_menu(parent, name); }

void window::add_menu_item(menu_reference parent, string name, int code)
{ if (!haswindow)
    return;
  if (menu==NULL)
    add_menu("Menu");
  if (parent==NULL)
    parent=menu;
  TCHAR * ws = widestring(name);
  AppendMenu(parent, MF_STRING, code, ws);
  delete[] ws;
  DrawMenuBar(wnd);
  return; }

void add_menu_item(menu_reference parent, string name, int code)
{ cw->add_menu_item(parent, name, code); }

window * window::clear()
{ if (usebitmap)
  { BitBlt(bitmapdc, 0, 0, screen->whole.right-screen->whole.left,
                           screen->whole.bottom-screen->whole.top,
                           NULL, 0, 0, WHITENESS); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    BitBlt(dc, 0, 0, client.right-client.left,
                     client.bottom-client.top, NULL, 0, 0, WHITENESS);
    ReleaseMutex(semaphore); }
  return this; }

window * clear()
{ return cw->clear(); }

void window::floatify()
{ if (!posint)
    return;
  fxpos=xpos;
  fypos=ypos; }

window * window::move_to(int x, int y)
{ if (usebitmap)
    MoveToEx(bitmapdc, x, y, NULL);
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    MoveToEx(dc, x, y, NULL);
    ReleaseMutex(semaphore); }
  xpos=x;
  ypos=y;
  posint=true;
  return this; }

window * move_to(int x, int y)
{ return cw->move_to(x, y); }

window * window::move_relative(int dx, int dy)
{ if (posint)
    return move_to(xpos+dx, ypos+dy);
  return move_to(fxpos+dx, fypos+dy); }

window * move_relative(int dx, int dy)
{ return cw->move_relative(dx, dy); }

window * window::move_to(double x, double y)
{ move_to((int)(x+0.5), (int)(y+0.5));
  fxpos=x;
  fypos=y;
  posint=false;
  return this; }

window * move_to(double x, double y)
{ return cw->move_to(x, y); }

window * window::move_relative(double dx, double dy)
{ floatify();
  move_to(fxpos+dx, fypos+dy);
  return this; }

window * move_relative(double dx, double dy)
{ return cw->move_relative(dx, dy); }

window * window::get_position(int &x, int &y)
{ x=xpos;
  y=ypos;
  return this; }

window * get_position(int &x, int &y)
{ return cw->get_position(x, y); }

window * window::get_position(double &x, double &y)
{ floatify();
  x=fxpos;
  y=fypos;
  return this; }

window * get_position(double &x, double &y)
{ return cw->get_position(x, y); }

window * window::get_position(float &x, float &y)
{ floatify();
  x=fxpos;
  y=fypos;
  return this; }

window * get_position(float &x, float &y)
{ return cw->get_position(x, y); }

double window::get_x_position()
{ floatify();
  return fxpos; }

double get_x_position()
{ return cw->get_x_position(); }

double window::get_y_position()
{ floatify();
  return fypos; }

double get_y_position()
{ return cw->get_y_position(); }

window * window::draw_to(int x, int y)
{ if (penw!=wantw || color!=wantc)
    isetp();
  if (usebitmap)
  { LineTo(bitmapdc, x, y);
    LineTo(bitmapdc, x, y); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    LineTo(dc, x, y);
    LineTo(dc, x, y);
    ReleaseMutex(semaphore); }
  xpos=x;
  ypos=y;
  posint=true;
  return this; }

window * draw_to(int x, int y)
{ return cw->draw_to(x, y); }

window * window::draw_relative(int dx, int dy)
{ if (posint)
    return draw_to(xpos+dx, ypos+dy);
  return draw_to(fxpos+dx, fypos+dy); }

window * draw_relative(int dx, int dy)
{ return cw->draw_relative(dx, dy); }

window * window::draw_to(double x, double y)
{ draw_to((int)(x+0.5), (int)(y+0.5));
  fxpos=x;
  fypos=y;
  posint=false;
  return this; }

window * draw_to(double x, double y)
{ return cw->draw_to(x, y); }

window * window::draw_relative(double dx, double dy)
{ floatify();
  draw_to(fxpos+dx, fypos+dy);
  return this; }

window * draw_relative(double dx, double dy)
{ return cw->draw_relative(dx, dy); }

window * window::draw_point(int x, int y)
{ if (penw!=wantw || color!=wantc)
    isetp();
  if (usebitmap)
  { MoveToEx(bitmapdc, x, y, NULL);
    if (penw>1)
      LineTo(bitmapdc, x, y);
    else
      SetPixel(bitmapdc, x, y, color); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    MoveToEx(dc, x, y, NULL);
    if (penw>1)
      LineTo(dc, x, y);
    else
      SetPixel(dc, x, y, color);
    ReleaseMutex(semaphore); }
  xpos=x;
  ypos=y;
  posint=true;
  return this; }

window * draw_point(int x, int y)
{ return cw->draw_point(x, y); }

window * window::draw_point(double x, double y)
{ draw_point((int)(x+0.5), (int)(y+0.5));
  fxpos=x;
  fypos=y;
  posint=false;
  return this; }

window * draw_point(double x, double y)
{ return cw->draw_point(x, y); }

window * window::draw_point()
{ if (penw!=wantw || color!=wantc)
    isetp();
  if (usebitmap)
  { MoveToEx(bitmapdc, xpos, ypos, NULL);
    LineTo(bitmapdc, xpos, ypos); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    MoveToEx(dc, xpos, ypos, NULL);
    LineTo(dc, xpos, ypos);
    ReleaseMutex(semaphore); }
  return this; }

window * draw_point()
{ return cw->draw_point(); }

window * window::set_pixel_color(int x, int y, int c)
{ if (usebitmap)
  { SetPixel(bitmapdc, x, y, c); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SetPixel(dc, x, y, c);
    ReleaseMutex(semaphore); }
  return this; }

window * set_pixel_color(int x, int y, int c)
{ return cw->set_pixel_color(x, y, c); }

window * window::set_pixel_color(double x, double y, int c)
{ return set_pixel_color((int)(x+0.5), (int)(y+0.5), c); }

window * set_pixel_color(double x, double y, int c)
{ return cw->set_pixel_color(x, y, c); }

window * window::set_pixel_color(int c)
{ if (usebitmap)
  { SetPixel(bitmapdc, xpos, ypos, c); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SetPixel(dc, xpos, ypos, c);
    ReleaseMutex(semaphore); }
  return this; }

window * set_pixel_color(int c)
{ return cw->set_pixel_color(c); }

int window::get_pixel_color(int x, int y)
{ int c;
  if (usebitmap)
  { c=GetPixel(bitmapdc, x, y); }
  else if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    c=GetPixel(dc, x, y);
    ReleaseMutex(semaphore); }
  return c; }

int get_pixel_color(int x, int y)
{ return cw->get_pixel_color(x, y); }

int window::get_pixel_color(double x, double y)
{ return get_pixel_color((int)(x+0.5), (int)(y+0.5)); }

int get_pixel_color(double x, double y)
{ return cw->get_pixel_color(x, y); }

int window::get_pixel_color()
{ int c;
  if (usebitmap)
  { c=GetPixel(bitmapdc, xpos, ypos); }
  else if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    c=GetPixel(dc, xpos, ypos);
    ReleaseMutex(semaphore); }
  return c; }

int get_pixel_color()
{ return cw->get_pixel_color(); }

window * window::fill_rectangle(int x, int y, int wi, int he, int rgb)
{ if (rgb==-2)
    rgb=wantc;
  RECT rect;
  rect.left=x;
  rect.right=x+wi;
  rect.top=y;
  rect.bottom=y+he;
  HBRUSH b = brush;
  if (rgb!=brushcolor)
    b=CreateSolidBrush(rgb);
  if (usebitmap)
  { FillRect(bitmapdc, & rect, b); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    FillRect(dc, & rect, b);
    ReleaseMutex(semaphore); }
  if (b!=brush)
    DeleteObject(b);
  return this; }

window * fill_rectangle(int x, int y, int w, int h, int rgb)
{ return cw->fill_rectangle(x, y, w, h, rgb); }

window * window::fill_rectangle(double x, double y, double wi, double he, int rgb)
{ return fill_rectangle((int)(x+0.5), (int)(y+0.5), (int)(wi+0.5), (int)(he+0.5), rgb); }

window * fill_rectangle(double x, double y, double w, double h, int rgb)
{ return cw->fill_rectangle(x, y, w, h, rgb); }

window * window::set_heading_degrees(double h)
{ if (h>=0)
    heading=(int)(h*3600.0+0.5);
  else
    heading=(int)(h*3600.0-0.5);
  return this; }

window * set_heading_degrees(double h)
{ return cw->set_heading_degrees(h); }

static const double todegreesfactor = 180.0/acos(-1.0), toradiansfactor = acos(-1.0)/180.0;

window * window::set_heading_radians(double h)
{ return set_heading_degrees(h*todegreesfactor); }

window * set_heading_radians(double h)
{ return cw->set_heading_radians(h); }

double window::get_heading_degrees()
{ return heading/3600.0; }

double get_heading_degrees()
{ return cw->get_heading_degrees(); }

double window::get_heading_radians()
{ return heading/3600.0*toradiansfactor; }

double get_heading_radians()
{ return cw->get_heading_radians(); }

double window::distance_to(double x, double y)
{ if (posint)
  { fxpos=xpos;
    fypos=ypos; }
  return sqrt((x-fxpos)*(x-fxpos)+(y-fypos)*(y-fypos)); }

double distance_to(double x, double y)
{ return cw->distance_to(x, y); }

double window::direction_to_in_degrees(double x, double y)
{ floatify();
  return atan2(x-fxpos, fypos-y)*todegreesfactor; }

double direction_to_in_degrees(double x, double y)
{ return cw->direction_to_in_degrees(x, y); }

double window::direction_to_in_radians(double x, double y)
{ floatify();
  return atan2(x-fxpos, fypos-y); }

double direction_to_in_radians(double x, double y)
{ return cw->direction_to_in_radians(x, y); }

double window::direction_from_to_in_degrees(double x0, double y0, double x1, double y1)
{ return atan2(x1-x0, y0-y1)*todegreesfactor; }

double direction_from_to_in_degrees(double x0, double y0, double x1, double y1)
{ return atan2(x1-x0, y0-y1)*todegreesfactor; }

double window::direction_from_to_in_radians(double x0, double y0, double x1, double y1)
{ return atan2(x1-x0, y0-y1); }

double direction_from_to_in_radians(double x0, double y0, double x1, double y1)
{ return atan2(x1-x0, y0-y1); }

window * window::turn_left_by_degrees(double a)
{ if (a>=0)
  { heading-=(int)(a*3600.0+0.5);
    if (heading<0) heading+=1296000; }
  else
  { heading-=(int)(a*3600.0-0.5);
    if (heading>=1296000) heading-=1296000; }
  return this; }

window * turn_left_by_degrees(double a)
{ return cw->turn_left_by_degrees(a); }

window * window::turn_left_by_radians(double a)
{ return turn_left_by_degrees(a*todegreesfactor); }

window * turn_left_by_radians(double a)
{ return cw->turn_left_by_radians(a); }

window * window::turn_right_by_degrees(double a)
{ if (a>=0)
  { heading+=(int)(a*3600.0+0.5);
    if (heading>=1296000) heading-=1296000; }
  else
  { heading+=(int)(a*3600.0-0.5);
    if (heading<0) heading+=1296000; }
  return this; }

window * turn_right_by_degrees(double a)
{ return cw->turn_right_by_degrees(a); }

window * window::turn_right_by_radians(double a)
{ return turn_right_by_degrees(a*todegreesfactor); }

window * turn_right_by_radians(double a)
{ return cw->turn_right_by_radians(a); }

window * window::move_distance(int d)
{ if (posint)
  { if (heading==0)
      move_to(xpos, ypos-d);
    else if (heading==324000)
      move_to(xpos+d, ypos);
    else if (heading==648000)
      move_to(xpos, ypos+d);
    else if (heading==972000)
      move_to(xpos-d, ypos);
    else
    { double a = heading/3600.0*toradiansfactor;
      fxpos=xpos+d*sin(a);
      fypos=ypos-d*cos(a);
      move_to(fxpos, fypos); } }
  else
  { if (heading==0)
      fypos-=d;
    else if (heading==324000)
      fxpos+=d;
    else if (heading==648000)
      fypos+=d;
    else if (heading==972000)
      fxpos-=d;
    else
    { double a = heading/3600.0*toradiansfactor;
      fxpos+=d*sin(a);
      fypos-=d*cos(a); }
    move_to(fxpos, fypos); }
  return this; }

window * move_distance(int d)
{ return cw->move_distance(d); }

window * window::move_distance(double d)
{ if (posint)
  { fxpos=xpos;
    fypos=ypos;
    posint=false; }
  if (heading==0)
    fypos-=d;
  else if (heading==324000)
    fxpos+=d;
  else if (heading==648000)
    fypos+=d;
  else if (heading==972000)
    fxpos-=d;
  else
  { double a = heading/3600.0*toradiansfactor;
    fxpos+=d*sin(a);
    fypos-=d*cos(a); }
  move_to(fxpos, fypos);
  return this; }

window * move_distance(double d)
{ return cw->move_distance(d); }

window * window::draw_distance(int d)
{ if (posint)
  { if (heading==0)
      return draw_to(xpos, ypos-d);
    if (heading==324000)
      return draw_to(xpos+d, ypos);
    if (heading==648000)
      return draw_to(xpos, ypos+d);
    if (heading==972000)
      return draw_to(xpos-d, ypos);
    double a = heading/3600.0*toradiansfactor;
    return draw_to(xpos+d*sin(a), ypos-d*cos(a)); }
  if (heading==0)
    return draw_to(fxpos, fypos-d);
  if (heading==324000)
    return draw_to(fxpos+d, fypos);
  if (heading==648000)
    return draw_to(fxpos, fypos+d);
  if (heading==972000)
    return draw_to(fxpos-d, fypos);
  double a = heading/3600.0*toradiansfactor;
  return draw_to(fxpos+d*sin(a), fypos-d*cos(a)); }

window * draw_distance(int d)
{ return cw->draw_distance(d); }

window * window::draw_distance(double d)
{ if (posint)
  { fxpos=xpos;
    fypos=ypos;
    posint=false; }
  if (heading==0)
    return draw_to(fxpos, fypos-d);
  if (heading==324000)
    return draw_to(fxpos+d, fypos);
  if (heading==648000)
    return draw_to(fxpos, fypos+d);
  if (heading==972000)
    return draw_to(fxpos-d, fypos);
  double a = heading/3600.0*toradiansfactor;
  return draw_to(fxpos+d*sin(a), fypos-d*cos(a)); }

window * draw_distance(double d)
{ return cw->draw_distance(d); }

window * window::set_caption(string s)
{ if (haswindow)
  { TCHAR str[1025];
    int n=s.length();
    if (n>1024) n=1024;
    for (int i=0; i<n; i+=1)
      str[i]=s[i];
    str[n]=0;
    SetWindowText(wnd, str); }
  return this; }

window * set_caption(string s)
{ return cw->set_caption(s); }

string window::get_caption()
{ if (!haswindow)
    return "";
  TCHAR str[1025];
  GetWindowText(wnd, str, 1024);
  str[1024]=0;
  return properstring(str); }

string get_caption()
{ return cw->get_caption(); }

window * window::iwrite_tchar(TCHAR * str, int n, const char * anchor, bool updateposition)
{ if (textcolor!=wantc)
    isett();
  SIZE sz;
  if (!updateposition)
  { sz.cx=0;
    sz.cy=0; }
  else if (usebitmap)
    GetTextExtentPoint32(bitmapdc, str, n, &sz);
  else if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    GetTextExtentPoint32(dc, str, n, &sz);
    ReleaseMutex(semaphore); }
  else
  { sz.cx=1;
    sz.cy=1; }
  int xo=0, yo=0, asc=fontdata.tmAscent;
  if (anchor==direction::north_east || anchor==NULL)
  { yo=-fontdata.tmAscent; }
  else if (anchor==direction::north_west)
  { xo=-sz.cx; yo=-fontdata.tmAscent; }
  else if (anchor==direction::south_east)
  { }
  else if (anchor==direction::south_west)
  { xo=-sz.cx; }
  else if (anchor==direction::north)
  { xo=-sz.cx/2; yo=-(asc+fontdata.tmDescent); }
  else if (anchor==direction::south)
  { xo=-sz.cx/2; }
  else if (anchor==direction::west)
  { xo=-sz.cx; yo=-(asc+fontdata.tmDescent)/2; }
  else if (anchor==direction::east)
  { yo=-(asc+fontdata.tmDescent)/2; }
  else if (anchor==direction::center)
  { xo=-sz.cx/2; yo=-(asc+fontdata.tmDescent)/2; }
  if (posint && currentfont->angle==900)
  { if (usebitmap)
      TextOut(bitmapdc, xpos+xo, ypos+yo, str, n);
    if (usewindow)
    { WaitForSingleObject(semaphore, INFINITE);
      TextOut(dc, xpos+xo, ypos+yo, str, n);
      ReleaseMutex(semaphore); }
    if (updateposition)
      move_to(xpos+xo+sz.cx, ypos+yo+asc); }
  else
  { floatify();
    const double a = currentfont->angle*toradiansfactor/10.0;
    const double s=sin(a), c=cos(a);
    const double nfxpos = fxpos + yo * c + xo * s;
    const double nfypos = fypos + yo * s - xo * c;
    if (usebitmap)
      TextOut(bitmapdc, (int)(nfxpos+0.5), (int)(nfypos+0.5), str, n);
    if (usewindow)
    { WaitForSingleObject(semaphore, INFINITE);
      TextOut(dc, (int)(nfxpos+0.5), (int)(nfypos+0.5), str, n);
      ReleaseMutex(semaphore); }
    if (updateposition)
      move_to(nfxpos + sz.cx * s + asc * c, nfypos - sz.cx * c + asc * s); }
  if (sz.cy>maxcharheightonline)
    maxcharheightonline=sz.cy;
  return this; }

static TCHAR * thetchar = new TCHAR[100];
static int thetcharlen = 100;

window * window::write_string(string s, const char * anchor, bool updateposition)
{ int n=s.length();
  if (n>thetcharlen)
  { delete [] thetchar;
    thetcharlen = n;
    thetchar = new TCHAR [n]; }
  if (n>1024) n=1024;
  for (int i=0; i<n; i+=1)
    thetchar[i]=s[i];
  iwrite_tchar(thetchar, n, anchor, updateposition);
  return this; }

window * write_string(string s, const char * anchor, bool updateposition)
{ return cw->write_string(s, anchor, updateposition); }

window * window::write_string(string s, bool updateposition)
{ return write_string(s, NULL, updateposition); }

window * write_string(string s, bool updateposition)
{ return cw->write_string(s, NULL, updateposition); }




window * window::write_string(const char * s, const char * anchor, bool updateposition)
{ return write_string(string(s), anchor, updateposition); }

window * write_string(const char * s, const char * anchor, bool updateposition)
{ return cw->write_string(string(s), anchor, updateposition); }

window * window::write_string(const char * s, bool updateposition)
{ return write_string(string(s), NULL, updateposition); }

window * write_string(const char * s, bool updateposition)
{ return cw->write_string(string(s), NULL, updateposition); }




window * window::write_string(const void * p, const char * anchor, bool updateposition)
{ char s[20];
  sprintf(s, "%08X", p);
  return write_string(s, anchor, updateposition); }

window * write_string(const void * p, const char * anchor, bool updateposition)
{ return cw->write_string(p, anchor, updateposition); }

window * window::write_string(const void * p, bool updateposition)
{ return write_string(p, NULL, updateposition); }

window * write_string(const void * p, bool updateposition)
{ return cw->write_string(p, NULL, updateposition); }




window * window::write_char(int c, const char * anchor, bool updateposition)
{ TCHAR ch=c;
  iwrite_tchar(&ch, 1, anchor, updateposition);
  return this; }

window * write_char(int c, const char * anchor, bool updateposition)
{ return cw->write_char(c, anchor, updateposition); }

window * window::write_char(int c, bool updateposition)
{ return write_char(c, NULL, updateposition); }

window * write_char(int c, bool updateposition)
{ return cw->write_char(c, NULL, updateposition); }

window * window::write_string(char c, const char * anchor, bool updateposition)
{ return write_char(c, anchor, updateposition); }

window * write_string(char c, const char * anchor, bool updateposition)
{ return cw->write_char(c, anchor, updateposition); }

window * window::write_string(char c, bool updateposition)
{ return write_char(c, NULL, updateposition); }

window * write_string(char c, bool updateposition)
{ return cw->write_char(c, NULL, updateposition); }



window * window::write_int(int n, const char * anchor, bool updateposition)
{ char s[15];
  sprintf(s, "%d", n);
  return write_string(s, anchor, updateposition); }

window * write_int(int n, const char * anchor, bool updateposition)
{ return cw->write_int(n, anchor, updateposition); }

window * window::write_int(int n, bool updateposition)
{ return write_int(n, NULL, updateposition); }

window * write_int(int n, bool updateposition)
{ return cw->write_int(n, NULL, updateposition); }

window * window::write_string(int n, const char * anchor, bool updateposition)
{ return write_int(n, anchor, updateposition); }

window * write_string(int n, const char * anchor, bool updateposition)
{ return cw->write_int(n, anchor, updateposition); }

window * window::write_string(int n, bool updateposition)
{ return write_int(n, NULL, updateposition); }

window * write_string(int n, bool updateposition)
{ return cw->write_int(n, NULL, updateposition); }





window * window::write_double(double d, const char * anchor, bool updateposition)
{ char s[30];
  sprintf(s, "%g", d);
  return write_string(s, anchor, updateposition); }

window * write_double(double d, const char * anchor, bool updateposition)
{ return cw->write_double(d, anchor, updateposition); }

window * window::write_double(double d, bool updateposition)
{ return write_double(d, NULL, updateposition); }

window * write_double(double d, bool updateposition)
{ return cw->write_double(d, NULL, updateposition); }

window * window::write_string(double d, const char * anchor, bool updateposition)
{ return write_double(d, anchor, updateposition); }

window * write_string(double d, const char * anchor, bool updateposition)
{ return cw->write_double(d, anchor, updateposition); }

window * window::write_string(double d, bool updateposition)
{ return write_double(d, NULL, updateposition); }

window * write_string(double d, bool updateposition)
{ return cw->write_double(d, NULL, updateposition); }





window * window::write_float(double d, const char * anchor, bool updateposition)
{ return write_double(d, anchor, updateposition); }

window * write_float(double d, const char * anchor, bool updateposition)
{ return cw->write_double(d, anchor, updateposition); }

window * window::write_float(double d, bool updateposition)
{ return write_double(d, NULL, updateposition); }

window * write_float(double d, bool updateposition)
{ return cw->write_double(d, NULL, updateposition); }




window * window::write_bool(bool b, const char * anchor, bool updateposition)
{ if (b)
    return write_string("true", anchor, updateposition);
  else
    return write_string("false", anchor, updateposition); }

window * write_bool(bool b, const char * anchor, bool updateposition)
{ return cw->write_bool(b, anchor, updateposition); }

window * window::write_bool(bool b, bool updateposition)
{ return write_bool(b, NULL, updateposition); }

window * write_bool(bool b, bool updateposition)
{ return cw->write_bool(b, NULL, updateposition); }

window * window::write_string(bool b, const char * anchor, bool updateposition)
{ return write_bool(b, anchor, updateposition); }

window * write_string(bool b, const char * anchor, bool updateposition)
{ return cw->write_bool(b, anchor, updateposition); }

window * window::write_string(bool b, bool updateposition)
{ return write_bool(b, NULL, updateposition); }

window * write_string(bool b, bool updateposition)
{ return cw->write_bool(b, NULL, updateposition); }



window * window::write_new_line(int x, int extray)
{ move_to(x, ypos+maxcharheightonline+extray);
  maxcharheightonline=0;
  return this; }

window * write_new_line(int x, int extray)
{ return cw->write_new_line(x, extray); }

window * window::measure_string(string s, int & x, int & y)
{ TCHAR str[1024];
  int n=s.length();
  if (n>1024) n=1024;
  for (int i=0; i<n; i+=1)
    str[i]=s[i];
  SIZE sz;
  if (usebitmap)
    GetTextExtentPoint32(bitmapdc, str, n, &sz);
  else if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    GetTextExtentPoint32(dc, str, n, &sz);
    ReleaseMutex(semaphore); }
  else
  { sz.cx=1;
    sz.cy=1; }
  x=sz.cx;
  y=sz.cy;
  return this; }

window * measure_string(string s, int & x, int & y)
{ return cw->measure_string(s, x, y); }

window * window::measure_char(int c, int & x, int & y)
{ TCHAR s;
  s=c;
  SIZE sz;
  if (usebitmap)
    GetTextExtentPoint32(bitmapdc, &s, 1, &sz);
  else if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    GetTextExtentPoint32(dc, &s, 1, &sz);
    ReleaseMutex(semaphore); }
  else
  { sz.cx=1;
    sz.cy=1; }
  x=sz.cx;
  y=sz.cy;
  return this; }

window * measure_char(int c, int & x, int & y)
{ return cw->measure_char(c, x, y); }

void window::isetp()
{ penw=wantw;
  color=wantc;
  HPEN p = CreatePen(PS_SOLID, penw, color);
  if (usebitmap)
  { SelectObject(bitmapdc, p); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SelectObject(dc, p);
    ReleaseMutex(semaphore); }
  DeleteObject(pen);
  pen=p; }

void window::isett()
{ textcolor=wantc;
  if (usebitmap)
    SetTextColor(bitmapdc, textcolor);
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SetTextColor(dc, textcolor);
    ReleaseMutex(semaphore); } }

window * window::set_pen_width_color(int w, int rgb)
{ wantw=w;
  wantc=rgb;
  return this; }

window * set_pen_width_color(int w, int rgb)
{ return cw->set_pen_width_color(w, rgb); }

window * window::set_pen_width(int w)
{ wantw=w;
  return this; }

window * set_pen_width(int w)
{ return cw->set_pen_width(w); }

window * window::set_pen_width(double w)
{ return set_pen_width((int)(w+0.5)); }

window * set_pen_width(double w)
{ return cw->set_pen_width(w); }

int window::get_pen_width()
{ return wantw; }

int get_pen_width()
{ return cw->get_pen_width(); }

window * window::set_pen_color(int rgb)
{ wantc=rgb;
  return this; }

window * set_pen_color(int rgb)
{ return cw->set_pen_color(rgb); }

window * window::set_pen_color_int(int r, int g, int b)
{ return set_pen_color(RGB(r, g, b)); }

window * set_pen_color_int(int r, int g, int b)
{ return cw->set_pen_color(RGB(r, g, b)); }

int window::get_pen_color()
{ return wantc; }

int get_pen_color()
{ return cw->get_pen_color(); }

void split_color(int rgb, double & r, double & g, double & b)
{ r=(rgb&0xFF)/255.0;
  g=((rgb>>8)&0xFF)/255.0;
  b=((rgb>>16)&0xFF)/255.0; }

void split_color(int rgb, float & r, float & g, float & b)
{ r=(rgb&0xFF)/255.0;
  g=((rgb>>8)&0xFF)/255.0;
  b=((rgb>>16)&0xFF)/255.0; }

void split_color_int(int rgb, int & r, int & g, int & b)
{ r=rgb&0xFF;
  g=(rgb>>8)&0xFF;
  b=(rgb>>16)&0xFF; }

int make_color_int(int r, int g, int b)
{ return RGB(r, g, b); }

int make_color(double r, double g, double b)
{ if (r<0) r=0;
  else if (r>1) r=1;
  if (g<0) g=0;
  else if (g>1) g=1;
  if (b<0) b=0;
  else if (b>1) b=1;
  return RGB((int)(r*255), (int)(g*255), (int)(b*255)); }

window * window::set_pen_color(double r, double g, double b)
{ return set_pen_color(make_color(r, g, b)); }

window * set_pen_color(double r, double g, double b)
{ return cw->set_pen_color(make_color(r, g, b)); }

static const double deg360=1.0, deg60=1.0/6.0, deg180=0.5, deg240=2.0/3.0;

double antihls(double n1, double n2, double h)
{ int times=(int)(h/deg360);
  h-=times*deg360;
  if (h<0.0) h=h+deg360;
  if (h<deg60) return (n1+(n2-n1)*h/deg60);
  if (h<deg180) return n2;
  if (h<deg240) return (n1+(n2-n1)*(deg240-h)/deg60);
  return n1; }

void hlstorgb(double H, double L, double S, double &R, double &G, double &B)
{ double m1= L<=0.5 ? L+L*S : L+S-L*S;
  double m2=2*L-m1;
  R=antihls(m1,m2,H-deg60);
  G=antihls(m1,m2,H+deg180);
  B=antihls(m1,m2,H+deg60); }

void rgb_to_hls(double R, double G, double B, double &H, double &L, double &S)
{ double bst=R;
  if (G>bst) bst=G;
  if (B>bst) bst=B;
  double dst=R;
  if (G<dst) dst=G;
  if (B<dst) dst=B;
  double twol=bst+dst;
  L=twol/2.0;
  double delta=bst-dst;
  if (twol==0.0 || twol==2.0)
    S=0.0;
  else if (twol<=1.0)
    S=delta/twol;
  else
    S=delta/(2.0-twol);
  double h=0;
  if (delta==0.0) h=0.0;
  else if (R>=G && R>=B) h=(G-B)/delta;
  else if (G>=R && G>=B) h=2.0+(B-R)/delta;
  else h=4.0+(R-G)/delta;
  if (h<0.0) h+=6.0;
  H=h*deg60; }

int make_color_hls(double h, double l, double s)
{ if (l<0) l=0;
  else if (l>1) l=1;
  if (s<0) s=0;
  else if (s>1) s=1;
  double r, g, b;
  hlstorgb(h, l, s, r, g, b);
  return RGB((int)(r*255), (int)(g*255), (int)(b*255)); }

window * window::set_pen_color_hls(double h, double l, double s)
{ return set_pen_color(make_color_hls(h, l, s)); }

window * set_pen_color_hls(double h, double l, double s)
{ return cw->set_pen_color(make_color_hls(h, l, s)); }

void sleep(double t)
{ int ms=(int)(t*1000.0+0.5);
  if (ms<=0)
    ms=1;
  Sleep(ms); }

window * window::get_font_size(int & height, int & ascent, int & descent,
                               int & internalleading, int & externalleading)
{ height=fontdata.tmHeight;
  ascent=fontdata.tmAscent;
  descent=fontdata.tmDescent;
  internalleading=fontdata.tmInternalLeading;
  externalleading=fontdata.tmExternalLeading;
  return this; }

window * get_font_size(int & height, int & ascent, int & descent,
                       int & internalleading, int & externalleading)
{ return cw->get_font_size(height, ascent, descent, internalleading, externalleading); }

window * window::set_font(fontrec * f)
{ if (currentfont==f)
    return this;
  HFONT hf = fontrec::defaultfont->it;
  if (f!=NULL)
    hf=f->it;
  if (usebitmap)
  { SelectObject(bitmapdc, hf);
    GetTextMetrics(bitmapdc, &fontdata); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SelectObject(dc, hf);
    if (!usebitmap)
      GetTextMetrics(dc, &fontdata);
    ReleaseMutex(semaphore); }
  if (currentfont!=NULL)
    currentfont->abandon();
  currentfont=f;
  if (currentfont!=NULL)
    currentfont->use();
  return this; }

window * set_font(fontrec * f)
{ return cw->set_font(f); }

window * window::i_set_font(string name, int size, int style, int angle)
{ if (name=="")
    name=currentfont->name;
  if (size<=0)
    size=dpiy/4;
  if (style==-1)
    style=style::normal;
  return set_font(font(name, size, style, angle)); }

window * window::set_font(string name, int size, int style, double an)
{ int angle=0;
  if (an>=0)
    angle=(int)(an*10+0.5);
  else
    angle=(int)(an*10-0.5);
  return i_set_font(name, size, style, angle); }

window * set_font(string name, int size, int style, double angle)
{ return cw->set_font(name, size, style, angle); }

window * window::set_font_size(int size)
{ if (size<=0)
    size=dpiy/4;
  return i_set_font(currentfont->name, size, currentfont->style, currentfont->angle); }

window * set_font_size(int size)
{ return cw->set_font_size(size); }

window * window::set_font_style(int style)
{ if (style==-1)
    style=style::normal;
  return i_set_font(currentfont->name, currentfont->size, style, currentfont->angle); }

window * set_font_style(int style)
{ return cw->set_font_style(style); }

window * window::set_font_angle_degrees(double angle)
{ return set_font(currentfont->name, currentfont->size, currentfont->style, angle); }

window * set_font_angle_degrees(double angle)
{ return cw->set_font_angle_degrees(angle); }

window * window::set_font_angle_radians(double angle)
{ return set_font(currentfont->name, currentfont->size, currentfont->style, angle*todegreesfactor); }

window * set_font_angle_radians(double angle)
{ return cw->set_font_angle_radians(angle*todegreesfactor); }

shape::shape()
{ cap=0;
  pts=NULL;
  bts=NULL;
  clear(); }

shape::~shape()
{ delete[] pts;
  delete[] bts;
  cap=0; }

void shape::clear()
{ num=0;
  x0=0;
  y0=0; }

void shape::enlarge()
{ int newcap=cap*2;
  if (newcap<=num)
    newcap=num+16;
  POINT * npts = new POINT[newcap];
  BYTE * nbts = new BYTE[newcap];
  for (int i=0; i<num; i+=1)
  { npts[i]=pts[i];
    nbts[i]=bts[i]; }
  delete[] pts;
  delete[] bts;
  pts=npts;
  bts=nbts;
  cap=newcap; }

void shape::note_position(double x, double y)
{ if (num==0)
  { x0=x;
    y0=y; }
  if (num>=cap)
    enlarge();
  pts[num].x=(int)(x+0.5);
  pts[num].y=(int)(y+0.5);
  bts[num]=PT_LINETO;
  bts[0]=PT_MOVETO;
  num+=1; }

window * window::start_shape()
{ theshape.clear();
  return this; }

void start_shape()
{ theshape.clear(); }

window * window::note_position(double x, double y)
{ theshape.note_position(x, y);
  return this; }

void note_position(double x, double y)
{ theshape.note_position(x, y); }

window * window::note_position()
{ floatify();
  theshape.note_position(fxpos, fypos);
  return this; }

void note_position()
{ cw->note_position(); }

void shape::move_shape_to(double x, double y)
{ int dx, dy;
  if (x>x0)
    dx=(int)(x-x0+0.5);
  else
    dx=(int)(x-x0-0.5);
  if (y>y0)
    dy=(int)(y-y0+0.5);
  else
    dy=(int)(y-y0-0.5);
  for (int i=0; i<num; i+=1)
  { pts[i].x+=dx;
    pts[i].y+=dy; }
  x0=x;
  y0=y; }

window * window::move_shape_to(double x, double y)
{ theshape.move_shape_to(x, y);
  return this; }

void move_shape_to(double x, double y)
{ theshape.move_shape_to(x, y); }

void shape::scale_shape(double s)
{ for (int i=0; i<num; i+=1)
  { pts[i].x=(int)((pts[i].x-x0)*s+x0+0.5);
    pts[i].y=(int)((pts[i].y-y0)*s+y0+0.5); } }

window * window::scale_shape(double s)
{ theshape.scale_shape(s);
  return this; }

void scale_shape(double s)
{ theshape.scale_shape(s); }

window * window::draw_shape()
{ if (wantw!=penw || wantc!=color)
    isetp();
  if (theshape.num<1)
    return this;
  if (usebitmap)
  { PolyDraw(bitmapdc, theshape.pts, theshape.bts, theshape.num); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    PolyDraw(dc, theshape.pts, theshape.bts, theshape.num);
    ReleaseMutex(semaphore); }
  return this; }

void draw_shape()
{ cw->draw_shape(); }

window * window::fill_shape(int rgb)
{ if (theshape.num<1)
    return this;
  if (wantw!=penw || wantc!=color)
    isetp();
  if (rgb==-2)
    rgb=wantc;
  HBRUSH b = NULL;
  if (rgb!=brushcolor)
    b=CreateSolidBrush(rgb);
  int ov=theshape.bts[theshape.num-1];
  theshape.bts[theshape.num-1]=ov|PT_CLOSEFIGURE;
  if (usebitmap)
  { if (rgb!=brushcolor)
      SelectObject(bitmapdc, b);
    Polygon(bitmapdc, theshape.pts, theshape.num); }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    if (rgb!=brushcolor)
      SelectObject(dc, b);
    Polygon(dc, theshape.pts, theshape.num);
    ReleaseMutex(semaphore); }
  theshape.bts[theshape.num-1]=ov;
  if (rgb!=brushcolor)
  { DeleteObject(brush);
    brush=b;
    brushcolor=rgb; }
  return this; }

void fill_shape(int rgb)
{ cw->fill_shape(rgb); }

void window::set_restriction(HRGN h)
{ if (usebitmap)
    SelectClipRgn(bitmapdc, h);
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    SelectClipRgn(dc, h);
    ReleaseMutex(semaphore); }
  if (hregion!=NULL)
    DeleteObject(hregion);
  hregion=h; }

window * window::restrict_drawing_to_shape()
{ set_restriction(CreatePolygonRgn(theshape.pts, theshape.num, WINDING));
  return this; }

window * restrict_drawing_to_shape()
{ return cw->restrict_drawing_to_shape(); }

window * window::restrict_drawing_to_rectangle(int x, int y, int w, int h)
{ set_restriction(CreateRectRgn(x, y, x+w, y+h));
  return this; }

window * restrict_drawing_to_rectangle(int x, int y, int w, int h)
{ return cw->restrict_drawing_to_rectangle(x, y, w, h); }

window * window::unrestricted_drawing()
{ set_restriction(NULL);
  return this; }

window * unrestricted_drawing()
{ return cw->unrestricted_drawing(); }

window * window::enter_fast_mode()
{ if (!usewindow) return this;
  canusewindow=usewindow;
  usewindow=false;
  return this; }

window * enter_fast_mode()
{ return cw->enter_fast_mode(); }

window * window::catch_up()
{ if (usewindow || !canusewindow) return this;
  usewindow=canusewindow;
  RedrawWindow(wnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  return this; }

window * catch_up()
{ return cw->catch_up(); }

window * window::draw_image(image * im, int x0, int y0)
{ if (im==NULL)
    return this;
  int w=im->width, h=im->height;
  if (usebitmap)
  { for (int r=0; r<h; r+=1)
    { int * row = im->pixels[r];
      for (int c=0; c<w; c+=1)
        SetPixel(bitmapdc, x0+c, y0+r, row[c]); } }
  if (usewindow)
  { WaitForSingleObject(semaphore, INFINITE);
    for (int r=0; r<h; r+=1)
    { int * row = im->pixels[r];
      for (int c=0; c<w; c+=1)
        SetPixel(dc, x0+c, y0+r, row[c]); }
    ReleaseMutex(semaphore); }
  return this; }

window * draw_image(image * im, int x0, int y0)
{ return cw->draw_image(im, x0, y0); }

window * window::draw_image(image * im)
{ return draw_image(im, xpos, ypos); }

window * draw_image(image * im)
{ return cw->draw_image(im); }

window * window::draw_image_from_file(string s, int x0, int y0)
{ image * im = image_from_file(s);
  draw_image(im, x0, y0);
  delete im;
  return this; }

window * draw_image_from_file(string s, int x0, int y0)
{ return cw->draw_image_from_file(s, x0, y0); }

window * window::draw_image_from_file(string s)
{ return draw_image_from_file(s, xpos, ypos); }

window * draw_image_from_file(string s)
{ return cw->draw_image_from_file(s); }

void window::paint()
{ if (haswindow && hasbitmap)
  { WaitForSingleObject(semaphore, INFINITE);
    GetWindowRect(wnd, &whole);
    GetClientRect(wnd, &client);
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(wnd, &ps);
    BitBlt(dc, 0, 0, client.right-client.left, client.bottom-client.top,
           bitmapdc, 0, 0, SRCCOPY);
    EndPaint(wnd, &ps);
    ReleaseMutex(semaphore); } }

LRESULT CALLBACK windowproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{ static window * mouseisin = NULL;
  static int mousecountdown = 0;
  static int mousex=0, mousey=0;
  int code = 0;
  window * w = NULL;
  if (wnd!=NULL)
  { code = GetWindowLong(wnd, GWL_USERDATA);
    w = window::windows[code]; }
  static int count=0;
  switch (msg)
  { case MESSAGE_CREATE:
    { w = window::windows[lparam];
      w->nonthreadcreate();
      mouseisin=NULL;
      return 0; }
    case MESSAGE_KILLIT:
    { w = window::windows[lparam];
      if (w!=NULL)
        DestroyWindow(w->wnd);
      mouseisin=NULL;
      return 0; }
    case MESSAGE_CURSOR:
    { w = window::windows[lparam];
      if (w!=NULL)
        SetCursor(curcurs);
      return 0; }
    case WM_PAINT:
    { w->paint();
      return 0; }
    case WM_COMMAND:
    { if (ievent::eventmask & menu_selection)
        ievent::add(w, ievent::menu_selected, wparam, 0, 0);
      return 0; }
    case WM_CLOSE:
    { if (ievent::eventmask & window_close_button)
        ievent::add(w, ievent::close_window, 0, 0, 0);
      else
        DestroyWindow(wnd);
      return 0; }
    case WM_DESTROY:
    { if (window::windows[code]!=NULL)
      { window::numwindows-=1;
        if (window::numwindows<=0)
        { if (suspended)
            ResumeThread(programthread);
          TerminateThread(programthread, 1);
          PostQuitMessage(1); }
        delete window::windows[code];
        window::windows[code]=NULL; }
      return 0; }
    case WM_KEYDOWN:
    { if (wparam==8 || wparam==9 || wparam==13 || wparam==16 || wparam==17 ||
          wparam==20 || wparam==27 || wparam==32 || wparam>=48 && wparam<=57 ||
          wparam>=65 && wparam<=92 || wparam>=96 && wparam<=111 || wparam>=128)
        return 0;
      wparam+=128;
      if (wparam==174) wparam=127;
      /* NO BREAK DELIBERATELY */ }
    case WM_CHAR:
    { if (wparam==3)
      { if (suspended)
          ResumeThread(programthread);
        TerminateThread(programthread, 1);
        PostQuitMessage(1); }
      if (wparam==13)
        wparam=10;
      if (wparam==27)
      { if (running)
        { if (!suspended)
          { WaitForSingleObject(window::semaphore, INFINITE);
            SuspendThread(programthread);
            suspended=true;
            ReleaseMutex(window::semaphore);
            set_console_caption("FROZEN (press ESC)"); }
          else
          { ResumeThread(programthread);
            suspended=false;
            set_console_caption("running..."); } } }
      else
      { if (ievent::eventmask & key_typed)
          ievent::add(w, ievent::key_typed, 0, 0, wparam); }
      return 0; }
    case WM_SYSCHAR:
    { if (ievent::eventmask & key_typed)
        ievent::add(w, ievent::key_typed, 0, 0, 256+wparam);
      return 0; }
    case WM_LBUTTONDOWN:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      mouseisin=w;
      ievent::draggling=false;
      ievent::mousedown(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 0);
      return 0; }
    case WM_LBUTTONUP:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      if (ievent::draggling && mouseisin==w)
        ievent::mousedragged(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF);
      ievent::draggling=false;
      mouseisin=NULL;
      ievent::mouseup(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 0);
      return 0; }
    case WM_MBUTTONDOWN:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      mouseisin=w;
      ievent::draggling=false;
      ievent::mousedown(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 1);
      return 0; }
    case WM_MBUTTONUP:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      if (ievent::draggling && mouseisin==w)
        ievent::mousedragged(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF);
      mouseisin=NULL;
      ievent::draggling=false;
      ievent::mouseup(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 1);
      return 0; }
    case WM_RBUTTONDOWN:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      ievent::draggling=false;
      mouseisin=w;
      ievent::mousedown(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 2);
      return 0; }
    case WM_RBUTTONUP:
    { if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      if (ievent::draggling && mouseisin==w)
        ievent::mousedragged(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF);
      ievent::draggling=false;
      mouseisin=NULL;
      ievent::mouseup(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 2);
      return 0; }
    case WM_MOUSEMOVE:
    { if (!ievent::mouseisdown && (ievent::eventmask & mouse_movement)!=0)
        ievent::add(w, ievent::mouse_moved, lparam & 0xFFFF, (lparam>>16) & 0xFFFF, 0);
      if (mouseisin!=NULL)
      { if (mouseisin!=w)
        { ievent::draggling=false;
          ievent::mouseexit(mouseisin); }
        ievent::mousedragging(w, lparam & 0xFFFF, (lparam>>16) & 0xFFFF); }
      return 0; }
    case WM_TIMER:
    { if (mouseisin!=NULL)
      { POINT p;
        GetCursorPos(&p);
        ScreenToClient(mouseisin->wnd, &p);
        RECT & c = mouseisin->client;
        if (p.x<c.left || p.y<c.top || p.x>c.right || p.y>c.bottom)
        { ievent::mouseexit(mouseisin);
          mouseisin=NULL; } }
      ievent::tick();
      SetTimer(wnd, 1, 10, NULL);
      return 0; }
    case WM_SIZE:
    { if (wparam==SIZE_MINIMIZED)
      { w->flags|=window::flag_minimized;
        if (ievent::eventmask & window_changed)
          ievent::add(w, ievent::minimized, 0, 0, 0); }
      else if (w->flags & window::flag_minimized)
      { RECT r;
        GetClientRect(w->wnd, &r);
        int nw=r.right-r.left, nh=r.bottom-r.top;
        w->flags &=~ window::flag_minimized;
        if (ievent::eventmask & window_changed)
          ievent::add(w, ievent::restored, nw, nh, 0); }
      else if (w->flags & window::flag_hadasize)
      { RECT r;
        GetClientRect(w->wnd, &r);
        int nw=r.right-r.left, nh=r.bottom-r.top;
        if (nw!=w->prevclient.right-w->prevclient.left ||
            nh!=w->prevclient.bottom-w->prevclient.top)
        { if (ievent::eventmask & window_changed)
            ievent::add(w, ievent::resizing, nw, nh, 0); } }
      w->flags|=window::flag_hadasize;
      return 1; }
    case WM_EXITSIZEMOVE:
    { RECT r;
      GetWindowRect(w->wnd, &w->whole);
      GetClientRect(w->wnd, &r);
      w->flags &=~ window::flag_minimized;
      int nw=r.right-r.left, nh=r.bottom-r.top;
      if (nw!=w->prevclient.right-w->prevclient.left ||
          nh!=w->prevclient.bottom-w->prevclient.top)
      { if (ievent::eventmask & window_changed)
          ievent::add(w, ievent::resized, nw, nh, 0);
        w->client=r;
        w->prevclient=r; }
      return 0; }
    case WM_SETCURSOR:
    { if (curcurs!=NULL)
        SetCursor(curcurs);
      return 0; }
    case WM_DROPFILES:
    { DROPFILES * * dfp = (DROPFILES * *)wparam;
      DROPFILES * df = *dfp;
      string files="";
      if (df->fWide)
      { short int * p = (short int *)(((char *)df)+df->pFiles);
        while (*p!=0)
        { string fname="";
          while (*p!=0)
          { fname+=(char)(*p);
            p+=1; }
          if (files!="")
            files+=',';
          files+=fname;
          p+=1; } }
      else
      { char * p = ((char *)df)+df->pFiles;
        while (*p!=0)
        { string fname="";
          while (*p!=0)
          { fname+=(char)(*p);
            p+=1; }
          if (files!="")
            files+=',';
          files+=fname;
          p+=1; } }
      if (mouseisin!=w && mouseisin!=NULL)
        ievent::mouseexit(mouseisin);
      mouseisin=w;
      ievent::dropfiles(w, df->pt.x, df->pt.y, files);
      return 0; }
    default:
    { return DefWindowProc(wnd, msg, wparam, lparam); } } }

DWORD __stdcall windowfunction(LPVOID x)
{ babywindow = new window(false);
  SetEvent(threadsync);
  while (!quittened)
  { MSG msg;
    BOOL ok = GetMessage(&msg, NULL, 0, 0);
    if (!ok) break;
    TranslateMessage(&msg);
    DispatchMessage(&msg); }
  quittened=true;
  SetEvent(deadsync);
  return 0; }

DWORD __stdcall threadfunction(LPVOID x)
{ running=true;
  homedir=_getcwd(NULL, 0);
  if (!unrandom)
    srand((unsigned int)time(NULL));
  timerstart=clock();
  int peculiar = setjmp(jb);
  if (!peculiar)
  { set_console_caption("running...");
    main(); }
  running=false;
  set_console_caption("*** FINISHED ***");
  SetEvent(threadsync);
  return 0; }

void wait(double t)
{ int oldmask;
  int tm = (int)(t*100+0.5);
  if (tm<1) tm=1;
  WaitForSingleObject(eventsync, INFINITE);
  oldmask=ievent::eventmask;
  ievent::eventmask=timer_expired;
  countdown=tm;
  ReleaseMutex(eventsync);
  the_event=ievent::next(true);
  ievent::eventmask=oldmask; }

void wait_for(int mask, double t)
{ int oldmask, tm=0;
  if (t>=-1.0)
  { tm = (int)(t*100+0.5);
    if (tm<1) tm=1;
    mask |= timer_expired; }
  WaitForSingleObject(eventsync, INFINITE);
  oldmask=ievent::eventmask;
  ievent::eventmask=mask;
  countdown=tm;
  ReleaseMutex(eventsync);
  the_event = ievent::next(true);
  ievent::eventmask=oldmask; }

void clear_interesting_events(int mask)
{ ievent::eventmask=0; }

void set_interesting_events(int mask)
{ ievent::eventmask=mask; }

void set_interesting_event(int mask)
{ ievent::eventmask=mask; }

int get_interesting_events(int mask)
{ return ievent::eventmask; }

void add_to_interesting_events(int mask)
{ ievent::eventmask|=mask; }

void remove_from_interesting_events(int mask)
{ ievent::eventmask&=~mask; }

event wait_for_interesting_event(double t)
{ int oldmask, extramask=0, tm=0;
  if (t>=-1.0)
  { tm = (int)(t*100+0.5);
    if (tm<1) tm=1;
    extramask = timer_expired; }
  WaitForSingleObject(eventsync, INFINITE);
  oldmask=ievent::eventmask;
  ievent::eventmask|=extramask;
  countdown=tm;
  ReleaseMutex(eventsync);
  the_event = ievent::next(t!=0.0);
  ievent::eventmask=oldmask;
  event e;
  e.kind=the_event.kind;
  e.x=the_event.x_pos;
  e.y=the_event.y_pos;
  if (e.kind==event::mouse_left_dragged || e.kind==event::mouse_left_dragging ||
      e.kind==event::mouse_middle_dragged || e.kind==event::mouse_middle_dragging ||
      e.kind==event::mouse_right_dragged || e.kind==event::mouse_right_dragging)
  { e.x0=ievent::dragoriginx;
    e.y0=ievent::dragoriginx; }
  else
  { e.x0=e.x;
    e.y0=e.y; }
  if (e.kind==event::key_typed)
  { e.code=the_event.char_code;
    e.c=(char)e.code; }
  else if (e.kind==event::menu_selected)
  { e.code=e.x;
    e.x=0;
    e.c=0; }
  else
  { e.code=0;
    e.c=0; }
  if (e.kind==event::file_drop)
    e.files=the_event.currentfiles;
  else
    e.files="";
  return e; }

string event::description()
{ return ievent::name(kind); }

bool any_events()
{ return ievent::any(); }

bool wait_for_mouse_click(double t)
{ wait_for(mouse_clicked, t);
  return the_event.kind==ievent::mouse_left_click; }

char wait_for_key_typed(double t)
{ wait_for(key_typed, t);
  if (the_event.kind==ievent::key_typed)
    return the_event.char_code;
  else
    return 0; }

char wait_for_mouse_click_or_key_typed(double t)
{ wait_for(key_typed + mouse_clicked, t);
  if (the_event.kind==ievent::key_typed)
    return the_event.char_code;
  else
    return 0; }

char wait_for_key_typed_or_mouse_click(double t)
{ return wait_for_mouse_click_or_key_typed(t); }

bool mouse_was_clicked()
{ return the_event.kind==ievent::mouse_left_click; }

bool key_was_typed()
{ return the_event.kind==ievent::key_typed; }

int get_event_kind()
{ return the_event.kind; }

int what_happened()
{ return the_event.kind; }

string get_event_name()
{ return ievent::name(the_event.kind); }

int get_selected_menu()
{ return the_event.x_pos; }

int get_click_x()
{ return the_event.x_pos; }

int get_click_y()
{ return the_event.y_pos; }

int get_drag_origin_x()
{ return ievent::dragoriginx; }

int get_drag_origin_y()
{ return ievent::dragoriginy; }

char get_key_typed()
{ return the_event.char_code; }

void exit()
{ longjmp(jb, 1); }

void error_pop_up(string message, string title)
{ TCHAR * msg = widestring(message);
  TCHAR * ttl = widestring(title);
  MessageBox(NULL, msg, ttl,
             MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
  delete[] msg;
  delete[] ttl;
  exit(); }

bool yes_no_pop_up(string message, string title)
{ TCHAR * msg = widestring(message);
  TCHAR * ttl = widestring(title);
  int r = MessageBox(NULL, msg, ttl,
                     MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
  delete[] msg;
  delete[] ttl;
  return r==IDYES; }

bool ok_cancel_pop_up(string message, string title)
{ TCHAR * msg = widestring(message);
  TCHAR * ttl = widestring(title);
  int r = MessageBox(NULL, msg, ttl,
                     MB_OKCANCEL | MB_ICONQUESTION | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
  delete[] msg;
  delete[] ttl;
  return r==IDOK; }

void information_pop_up(string message, string title)
{ TCHAR * msg = widestring(message);
  TCHAR * ttl = widestring(title);
  MessageBox(NULL, msg, ttl,
             MB_OK | MB_ICONINFORMATION | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
  delete[] msg;
  delete[] ttl; }

string open_file_pop_up(string suggestedfilename, string initdirectory, string title)
{ OPENFILENAME fns;
  TCHAR filename[1000], initdir[1000], boxtitle[300];
  widestrcpy(filename, suggestedfilename, 999);
  if (initdirectory == "")
    GetCurrentDirectory(998, initdir);
  else
    widestrcpy(initdir, initdirectory, 999);
  widestrcpy(boxtitle, title, 299);
  ZeroMemory(&fns, sizeof(OPENFILENAME));
  fns.lStructSize=sizeof(fns);
  fns.hwndOwner=NULL;
  fns.hInstance=NULL;
  fns.lpstrFilter=TEXT("All\0*.*\0");
  fns.lpstrCustomFilter=NULL;
  fns.nMaxCustFilter=0;
  fns.nFilterIndex=1;
  fns.lpstrFile=filename;
  fns.nMaxFile=998;
  fns.lpstrFileTitle=NULL;
  fns.nMaxFileTitle=0;
  fns.lpstrInitialDir=initdir;
  fns.lpstrTitle=boxtitle;
  fns.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_ENABLESIZING;
  fns.nFileOffset=0;
  fns.nFileExtension=0;
  fns.lpstrDefExt=NULL;
  fns.lCustData=NULL;
  fns.lpfnHook=NULL;
  fns.lpTemplateName=NULL;
  int ok=GetOpenFileName(&fns);
  if (ok)
    return properstring(filename);
  else
    return ""; }

string save_as_pop_up(string suggestedfilename, string initdirectory, string defaultextension, string title)
{ OPENFILENAME fns;
  TCHAR filename[1000], initdir[1000], boxtitle[300], defex[100];
  widestrcpy(filename, suggestedfilename, 999);
  if (initdirectory == "")
    GetCurrentDirectory(998, initdir);
  else
    widestrcpy(initdir, initdirectory, 999);
  widestrcpy(boxtitle, title, 299);
  if (defaultextension!="" && defaultextension[0]=='.')
    defaultextension = defaultextension.substr(1);
  widestrcpy(defex, defaultextension, 99);
  fns.lStructSize=sizeof(fns);
  fns.hwndOwner=NULL;
  fns.hInstance=NULL;
  fns.lpstrFilter=NULL;
  fns.lpstrCustomFilter=NULL;
  fns.nMaxCustFilter=0;
  fns.nFilterIndex=0;
  fns.lpstrFile=filename;
  fns.nMaxFile=998;
  fns.lpstrFileTitle=NULL;
  fns.nMaxFileTitle=0;
  fns.lpstrInitialDir=initdir;
  fns.lpstrTitle=boxtitle;
  fns.Flags=OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
  fns.nFileOffset=0;
  fns.nFileExtension=0;
  fns.lpstrDefExt=defex;
  fns.lCustData=NULL;
  fns.lpfnHook=NULL;
  fns.lpTemplateName=NULL;
  int ok=GetSaveFileName(&fns);
  if (ok)
    return properstring(filename);
  else
    return ""; }

static vector<string> questions, answers;
static bool readparams = false;

static string trim(string s)
{ int n=s.length(), fns=-1, lns=-1;
  for (int i=0; i<n; i+=1)
  { if (s[i]>' ')
    { if (fns==-1)
        fns=i;
      lns=i; } }
  if (fns==-1)
    return "";
  if (lns==n-1)
    return s.substr(fns);
  return s.substr(fns, lns-fns+1); }

static bool same(string s, string t)
{ if (_stricmp(s.c_str(), t.c_str())==0)
    return true;
  return false; }

static string getparam(string name)
{ if (!readparams)
  { string nm=homedir+"\\parameters.txt";
    ifstream f(nm.c_str());
    if (f.fail())
    { error_pop_up("Can not read parameter file\n'parameters.txt'");
      exit(); }
    while (1)
    { string s;
      getline(f, s);
      if (f.fail()) break;
      int epos=-1, n=s.length();
      for (int i=0; i<n; i+=1)
      { if (s[i]=='=')
        { epos=i;
          break; } }
      if (epos==-1)
        continue;
      string q=s.substr(0, epos), a=s.substr(epos+1);
      q=trim(q);
      a=trim(a);
      questions.push_back(q);
      answers.push_back(a); }
    readparams=true;
    f.close(); }
  int n=questions.size();
  for (int i=0; i<n; i+=1)
  { if (same(name, questions[i]))
      return answers[i]; }
  error_pop_up(string("Parameter '")+name+"' is not defined\n");
  exit();
  return ""; }

string string_parameter(string name)
{ return getparam(name); }

char char_parameter(string name)
{ string s=getparam(name);
  int n=s.length();
  if (n==0)
    return ' ';
  if (n==1)
    return s[0];
  error_pop_up(string("Parameter '")+name+"' is not a single character\n"+s);
  exit();
  return ' '; }

int int_parameter(string name)
{ string s=getparam(name);
  char * stopper;
  int val = strtol(s.c_str(), &stopper, 10);
  if (*stopper!=0)
  { error_pop_up(string("Parameter '")+name+"' is not an int\n"+s);
    exit(); }
  return val; }

double double_parameter(string name)
{ string s=getparam(name);
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { error_pop_up(string("Parameter '")+name+"' is not a double\n"+s);
    exit(); }
  return val; }

double float_parameter(string name)
{ string s=getparam(name);
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { error_pop_up(string("Parameter '")+name+"' is not a float\n"+s);
    exit(); }
  return val; }

bool bool_parameter(string name)
{ string s=getparam(name);
  if (s=="1") return true;
  if (s=="0") return false;
  if (same(s, "true")) return true;
  if (same(s, "false")) return false;
  if (same(s, "yes")) return true;
  if (same(s, "no")) return false;
  if (same(s, "t")) return true;
  if (same(s, "f")) return false;
  if (same(s, "y")) return true;
  if (same(s, "n")) return false;
  error_pop_up(string("Parameter '")+name+"' is not a bool value\n"+s);
  exit();
  return false; }

int color_parameter(string name)
{ string s=getparam(name);
  for (int i=0; i<color::nn && i<color::nv; i+=1)
    if (same(s, color::names[i]))
      return color::values[i];
  error_pop_up(string("Parameter '")+name+"' is not recognised color name\n"+s);
  exit();
  return 0; }

void log_entry(string s)
{ log_entry("%s\n", s.c_str()); }

void log_entry(int n)
{ log_entry("%d\n", n); }

void log_entry(char c)
{ log_entry("%c\n", c); }

void log_entry(bool b)
{ if (b)
    log_entry("TRUE\n");
  else
    log_entry("FALSE\n"); }

void log_entry(double d)
{ log_entry("%f\n", d); }

void log_entry(const char *patt, ...)
{ if (logisdead)
    return;
  if (logfile==NULL)
  { logfile=fopen("log.txt", "w");
    if (logfile==NULL)
    { logisdead=true;
      bool ok=yes_no_pop_up("Can't create log file\nDo you want to continue anyway?");
      if (!ok)
        exit(); }
    else
    { __time64_t t;
      _time64(&t);
      fprintf(logfile, "Log started %s", _ctime64(&t)); } }
  va_list a;
  va_start(a, patt);
  vfprintf(logfile, patt, a);
  fflush(logfile); }

static int daysnorm[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
static int daysleap[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

double what_is_the_time()  // seconds since midnight 1st jan 2000, local time.
{ __timeb64 t;
  _ftime64(&t);
  struct tm * lt = _localtime64(&t.time);
  int year=1900+lt->tm_year;
  int days=year*365+(year-1)/4-(year-1)/100+(year-1)/400-730486;
  bool isly = year%4==0;
  if (year%100==0) isly=false;
  if (year%400==0) isly=true;
  if (isly)
    days+=daysleap[lt->tm_mon];
  else
    days+=daysnorm[lt->tm_mon];
  days+=lt->tm_mday;
  int secs=lt->tm_hour*60*60+lt->tm_min*60+lt->tm_sec;
  return days*24.0*60*60 + secs + t.millitm/1000.0; }

void start_timer()
{ timerstart=clock(); }

double read_timer()
{ clock_t t=clock();
  return (t-timerstart)/(double)CLOCKS_PER_SEC; }

int get_clock_time()
{ __timeb64 t;
  _ftime64(&t);
  struct tm * lt = _localtime64(&t.time);
  return lt->tm_hour*10000 + lt->tm_min*100 + lt->tm_sec; }

int get_calendar_date()
{ __timeb64 t;
  _ftime64(&t);
  struct tm * lt = _localtime64(&t.time);
  return (lt->tm_year+1900)*10000 + (lt->tm_mon+1)*100 + lt->tm_mday; }

void print(string s)
{ printf("%s", s.c_str()); }

void print(const char * s)
{ printf("%s", s); }

void print(int n)
{ printf("%d", n); }

void print(char c)
{ printf("%c", c); }

void print(double d)
{ printf("%g", d); }

void print(bool b)
{ if (b)
    printf("true");
  else
    printf("false"); }

void print_string(string s)
{ printf("%s", s.c_str()); }

void print_int(int n)
{ printf("%d", n); }

void print_char(char c)
{ printf("%c", c); }

void print_double(double d)
{ printf("%.16g", d); }

void print_float(double d)
{ printf("%.8g", d); }

void print_hexadecimal(int n)
{ printf("%9X", n); }

void print_binary(int n)
{ bool something=false;
  unsigned int m=0x80000000;
  for (int i=0; i<32; i+=1)
  { if (n&m)
    { printf("1");
      something=true; }
    else if (something)
      printf("0");
    m>>=1; }
  if (!something)
    printf("0"); }

void print_bool(bool b)
{ if (b)
    printf("true");
  else
    printf("false"); }

void new_line()
{ printf("\n"); }

static string readsomething()
{ string r="";
  int c=0;
  while (true)
  { c=getc(stdin);
    if (c==EOF || c>' ' && c!=',')
      break; }
  while (true)
  { if (c<=' ' || c==',')
      break;
    r+=(char)c;
    c=getc(stdin); }
  ungetc(c, stdin);
  return r; }

string read_string()
{ return readsomething(); }

string read_line()
{ string r="";
  while (true)
  { int c = getc(stdin);
    if (c=='\n' || c==EOF)
      break;
    r+=(char)c; }
  return r; }

int read_int()
{ string s=readsomething();
  char * stopper;
  int val = strtol(s.c_str(), &stopper, 10);
  if (*stopper!=0)
  { bool k=ok_cancel_pop_up(s+"\nis not an int value");
    if (!k) exit();
    val=0; }
  return val; }

double read_double()
{ string s=readsomething();
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { bool k=ok_cancel_pop_up(s+"\nis not a double value");
    if (!k) exit();
    val=0.0; }
  return val; }

double read_float()
{ string s=readsomething();
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { bool k=ok_cancel_pop_up(s+"\nis not a float value");
    if (!k) exit();
    val=0.0; }
  return val; }

bool read_bool()
{ string s=readsomething();
  if (s=="1") return true;
  if (s=="0") return false;
  if (same(s, "true")) return true;
  if (same(s, "false")) return false;
  if (same(s, "yes")) return true;
  if (same(s, "no")) return false;
  if (same(s, "t")) return true;
  if (same(s, "f")) return false;
  if (same(s, "y")) return true;
  if (same(s, "n")) return false;
  bool k=ok_cancel_pop_up(s+"\nis not a bool value");
  if (!k) exit();
  return false; }

void not_really_random(int n)
{ unrandom=true;
  srand(n); }

int random()
{ return (rand()<<15)|rand(); }

int random_in_range(int a, int b)
{ return a+random()%(b-a+1); }

static int nthreads=0;

thread::thread()
{ stack_size=0;
  state=0; }

DWORD WINAPI threadstarter(LPVOID pp)
{ thread * me = (thread *)pp;
  nthreads+=1;
  if (!unrandom)
  { int n=nthreads&7;
    for (int i=0; i<n; i+=1)
      random();
    srand(random() ^ (unsigned int)time(NULL)); }
  me->main();
  me->state=2;
  return 1; }

void thread::start()
{ if (state!=0)
    return;
  windowsidh = CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)threadstarter,
                           (void *)this, 0, &windowsidi); }

void thread::kill()
{ if (state!=0)
    TerminateThread(windowsidh, 1);
  state=2; }

void thread::sleep(double t)
{ int ms = (int)(t*1000.0+0.5);
  if (ms<=0) ms=1;
  Sleep(ms); }

void thread::main()
{ printf("You didn't define main for your thread\n"); }

semaphore::semaphore()
{ it = CreateMutex(NULL, false, NULL); }

semaphore::~semaphore()
{ CloseHandle(it);
  it=NULL; }

void semaphore::make_free()
{ ReleaseMutex(it); }

void semaphore::wait_until_free()
{ WaitForSingleObject(it, INFINITE); }

image::image(int w, int h, int * * p)
{ width=w;
  height=h;
  pixels=p; }

image::~image()
{ for (int i=0; i<height; i+=1)
    delete[] pixels[i];
  delete[] pixels; }

typedef unsigned char byte;

struct gifreader
{ static const int only_show_codes = 0;
  struct logical_screen_descriptor
  { int width;
    int height;
    bool is_global_colour_table;
    int colour_resolution;
    int global_colour_table_size;
    byte background_colour;
    byte aspect_ratio; };
  logical_screen_descriptor lsd;
  struct image_descriptor
  { unsigned short int left;
    unsigned short int top;
    unsigned short int width;
    unsigned short int height;
    bool is_local_colour_table;
    bool is_interlaced;
    int local_colour_table_size; };
  image_descriptor id;
  struct colour
  { byte r, g, b; };
  struct colour_table
  { colour c[256];
    int size;
    colour_table(); };
  colour_table * ct;
  int prefix[5000], addition[5000], clear_code;
  int onrow, oncol, pass, inpass, allskip, prerow;
  byte curr;
  int bitsleft, bitsgone, incb, codesize;
  int mask[9];
  bool block_was_zero;
  int readcb();
  int file;
  byte buffer[1024], block[257];
  int assembly[2000];
  int bsz, bptr, bempty;
  int lzw_minimum_code_size;
  int * * pixels;
  gifreader();
  void clear();
  bool openfile(string s);
  void closefile();
  void refill();
  byte readb();
  int read2b();
  int readbb(byte array[], int num);
  int reads(char s[], int num);
  image_descriptor readid();
  logical_screen_descriptor readlsd();
  void printlsd(logical_screen_descriptor & x);
  void printid(image_descriptor & x);
  void readct(colour_table & ct, int size);
  void printct(colour_table & ct);
  int get(int bsz);
  void pixel(int c);
  int printcode(int code);
  void decode();
  image * gifreader::read(); };

gifreader::gifreader()
{ clear();
  mask[0]=0x00;
  mask[1]=0x01;
  mask[2]=0x03;
  mask[3]=0x07;
  mask[4]=0x0F;
  mask[5]=0x1F;
  mask[6]=0x3F;
  mask[7]=0x7F;
  mask[8]=0xFF; }

void gifreader::clear()
{ bsz=0;
  bptr=0;
  bempty=0;
  lzw_minimum_code_size=0;
  onrow=0;
  oncol=0;
  pass=0;
  inpass=0;
  allskip=0;
  prerow=0;
  file=0; }

bool gifreader::openfile(string s)
{ clear();
  file=open(s.c_str(), O_RDONLY | O_BINARY);
  if (file<0)
    return false;
  return true; }

void gifreader::closefile()
{ if (file!=0)
    close(file);
  file=0; }

void gifreader::refill()
{ int n=::read(file, buffer, 1024);
  if (n<0) n=0;
  if (n<=0) bempty=1;
  bsz=n;
  bptr=0; }

byte gifreader::readb()
{ if (bptr>=bsz)
    refill();
  if (bempty)
    return 0;
  byte b=buffer[bptr];
  bptr+=1;
  return b; }

int gifreader::read2b()
{ int a=readb();
  return a+(readb()<<8); }

int gifreader::readbb(byte array[], int num)
{ int got=0;
  while (num>0)
  { if (bempty)
      return got;
    int ntg=bsz-bptr;
    if (ntg>num) ntg=num;
    for (int i=0; i<ntg; i+=1)
      array[got+i]=buffer[bptr+i];
    bptr+=ntg;
    got+=ntg;
    num-=ntg;
    if (num>0)
      refill(); }
  return got; }

int gifreader::reads(char s[], int num)
{ int n=readbb((byte *)s, num);
  s[n]=0;
  return n; }

gifreader::logical_screen_descriptor gifreader::readlsd()
{ logical_screen_descriptor x;
  x.width=read2b();
  x.height=read2b();
  int b=readb();
  x.background_colour=readb();
  x.aspect_ratio=readb();
  x.is_global_colour_table = (b & 0x80) != 0;
  x.colour_resolution = 1 << (((b & 0x70) >> 4) + 1);
  x.global_colour_table_size = 1 << ((b & 0x07) + 1);
  return x; }

void gifreader::printlsd(gifreader::logical_screen_descriptor & lsd)
{ printf("logical screeen width = %d, height = %d\n", lsd.width, lsd.height);
  printf("colour resolution = %d, ", lsd.colour_resolution);
  printf("size of global colour table = %d. ", lsd.global_colour_table_size);
  if (lsd.is_global_colour_table)
    printf("Has a global colour table\n");
  else
    printf("No global colour table\n"); }

gifreader::image_descriptor gifreader::readid()
{ image_descriptor x;
  x.left=read2b();
  x.top=read2b();
  x.width=read2b();
  x.height=read2b();
  int b=readb();
  x.is_local_colour_table = (b & 0x80) != 0;
  x.is_interlaced = (b & 0x40) != 0;
  x.local_colour_table_size = 1 << ((b & 0x07) + 1);
  return x; }

void gifreader::printid(gifreader::image_descriptor & x)
{ printf("left=%d top=%d width=%d height=%d\n", x.left, x.top, x.width, x.height);
  printf("size of local colour table = %d. ", x.local_colour_table_size);
  if (x.is_interlaced)
    printf("Interlaced, ");
  else
    printf("Non-interlaced, ");
  if (x.is_local_colour_table)
    printf("Has a local colour table\n");
  else
    printf("No local colour table\n"); }

gifreader::colour_table::colour_table()
{ size=0; }

void gifreader::readct(gifreader::colour_table & ct, int size)
{ for (int i=0; i<size; i+=1)
  { ct.c[i].r=readb();
    ct.c[i].g=readb();
    ct.c[i].b=readb(); }
  ct.size=size; }

void gifreader::printct(gifreader::colour_table & ct)
{ for (int i=0; i<ct.size; i+=1)
    printf("  %3d: R=%3d G=%3d B=%3d\n", i, ct.c[i].r, ct.c[i].g, ct.c[i].b); }

int gifreader::readcb()
{ if (incb==-1)
    return 0;
  if (incb==0)
  { incb=readb();
    if (incb==0)
    { incb=-1;
      block_was_zero=true;
      return 0; } }
  incb-=1;
  return readb(); }

int gifreader::get(int bsz)
{ int extra=bsz-bitsleft;
  if (extra<=0)
  { int val=(curr>>bitsgone) & mask[bsz];
    bitsgone+=bsz;
    bitsleft-=bsz;
    return val; }
  else if (extra<=8)
  { int val=(curr>>bitsgone) & mask[bitsleft];
    curr=readcb();
    val|=(curr & mask[extra])<<bitsleft;
    bitsgone=extra;
    bitsleft=8-extra;
    return val; }
  else
  { int val=(curr>>bitsgone) & mask[bitsleft];
    curr=readcb();
    extra-=8;
    val|=curr << bitsleft;
    curr=readcb();
    val|=(curr & mask[extra])<<(bitsleft+8);
    bitsgone=extra;
    bitsleft=8-extra;
    return val; } }

void gifreader::pixel(int c)
{ int r=onrow;
  if (id.is_interlaced)
  { if (oncol==id.width)
    { r-=allskip;
      if (pass==0)
      { r=r<<3;
        if (r>=id.height)
        { pass=1;
          allskip+=inpass;
          r=onrow-allskip;
          inpass=0; }
        else
          inpass+=1; }
      if (pass==1)
      { r=4+(r<<3);
        if (r>=id.height)
        { pass=2;
          allskip+=inpass;
          r=onrow-allskip;
          inpass=0; }
        else
          inpass+=1; }
      if (pass==2)
      { r=2+(r<<2);
        if (r>=id.height)
        { pass=3;
          allskip+=inpass;
          r=onrow-allskip;
          inpass=0; }
        else
          inpass+=1; }
      if (pass==3)
        r=1+r+r;
      prerow=r;
      onrow+=1;
      // printf("\nrow %d -> %d  ", onrow, r);
      // read_line();
      oncol=0; }
    else
      r=prerow; }
  else if (oncol==id.width)
  { onrow+=1;
    oncol=0;
    r=onrow; }
  // printf(" %d", oncol);
  pixels[r+id.top][oncol+id.left] = (ct->c[c].r)|(ct->c[c].g<<8)|(ct->c[c].b<<16);
  oncol+=1; }

int gifreader::printcode(int code)
{ int pos=0;
  while (code>=0)
  { assembly[pos]=addition[code];
    code=prefix[code];
    pos+=1; }
  pos-=1;
  int r=assembly[pos];
  while (pos>=0)
  { pixel(assembly[pos]);
    // printf(" %d", assembly[pos]);
    pos-=1; }
  return r; }

void gifreader::decode()
{ incb=0;
  curr=0;
  bitsleft=0;
  bitsgone=0;
  block_was_zero=false;
  onrow=-1;
  oncol=id.width;
  pass=0;
  inpass=0;
  allskip=0;
  prerow=0;
  codesize=lzw_minimum_code_size+1;
  clear_code=1<<lzw_minimum_code_size;
  // printf("000000000011111111112222222222333333333344444444445555555555\n");
  // printf("012345678901234567890123456789012345678901234567890123456789\n");
  int next_pot=2<<lzw_minimum_code_size;
  int end_code=clear_code+1;
  int next_code=clear_code+1;
  int last=0, clears=0;
  for (int i=0; i<clear_code; i+=1)
  { prefix[i]=-1;
    addition[i]=i; }
  while (!block_was_zero)
  { int c=get(codesize);
    // printf("<%d> ", c);
    if (c==clear_code)
    { // printf("clear at row %d col %d\n", onrow, oncol);
      clears+=1;
      codesize=lzw_minimum_code_size+1;
      next_pot=2<<lzw_minimum_code_size;
      next_code=clear_code+1;
      last=0; }
    else if (c==end_code)
    { // printf("\nend\n");
      break; }
    else if (c<clear_code)
    { if (next_code==end_code)
      { pixel(c);
        // printf(" %d\n", c);
        // printf(" %d", c);
        last=c;
        next_code+=1; }
      else
      { prefix[next_code]=last;
        addition[next_code]=c;
        last=c;
        pixel(c);
        // printf(" %d [%d]->%d,%d\n", c, next_code, prefix[next_code], addition[next_code]);
        // printf(" %d", c);
        next_code+=1; } }
    else if (c==next_code)
    { int fst=printcode(last);
      prefix[next_code]=last;
      addition[next_code]=fst;
      last=next_code;
      pixel(fst);
      // printf(" %d [%d]->%d,%d\n", fst, next_code, prefix[next_code], addition[next_code]);
      // printf(" %d", fst);
      next_code+=1; }
    else
    { int fst=printcode(c);
      prefix[next_code]=last;
      addition[next_code]=fst;
      last=c;
      // printf(" [%d]->%d,%d\n", next_code, prefix[next_code], addition[next_code]);
      next_code+=1; }
    if (next_code==next_pot)
    { codesize+=1;
      next_pot<<=1; } }
  while (!block_was_zero)
    readcb(); }

image * gifreader::read()
{ char version[7];
  reads(version, 6);
  lsd = readlsd();
  pixels = new int * [lsd.height];
  for (int i=0; i<lsd.height; i+=1)
    pixels[i] = new int [lsd.width];
  colour_table gct;
  ct = & gct;
  if (lsd.is_global_colour_table)
  // { printf("Global colour table:\n");
    readct(gct, lsd.global_colour_table_size);
  //   printct(gct); }
  int startplace=lseek(file, 0, SEEK_CUR)-bsz+bptr;
  int images=0, timesround=0;
  double delayatend=0;
  while (1)
  { int separator=readb();
    if (separator==0)
    { }
    else if (separator==0x3B)
    { // printf("End marker 0x3B\n");
      if (images>1)
      { images=0;
        timesround+=1;
        lseek(file, startplace, SEEK_SET);
        refill(); }
      else
        break; }
    else if (separator==0x2C)
    { // printf("Image marker 0x2C\n");
      images+=1;
      id = readid();
      // printid(id);
      colour_table lct;
      if (id.is_local_colour_table)
      { // printf("Local colour table:\n");
        readct(lct, id.local_colour_table_size);
        // printct(lct);
        ct = &lct; }
      lzw_minimum_code_size=readb();
      // printf("LZW minimum code size = %d\n", lzw_minimum_code_size);
      if (only_show_codes)
      { while (1)
        { int blocksize=readb();
          if (blocksize==0)
          { printf("End of image\n");
            break; }
          readbb(block, blocksize);
          printf("Block of %d codes:", blocksize);
          for (int i=0; i<blocksize; i+=1)
          { if (i%24==0) printf("\n   ");
            printf(" %02X", block[i]); }
          printf("\n"); } }
      else
      { decode();
        /* if (delayatend>0) Delay(delayatend); */
        delayatend=0; }
      ct = &gct; }
    else if (separator==0x21)
    { int kind=readb();
      if (kind==0xF9)
      { // printf("Graphic Control:\n");
        int b=readb();
        if (b==4)
        { b=readb();
          int disposal_method=(b>>2)&0x07;
          bool user_input_needed = (b & 0x02)!=0;
          bool transparency_used = (b & 0x01)!=0;
          int delay=read2b();
          delayatend=delay/100.0;
          int transparent_index=readb();
       /* if (disposal_method==1)
            printf("  Do not dispose, ");
          else if (disposal_method==2)
            printf("  Restore to background, ");
          else if (disposal_method==3)
            printf("  Restore to previous, ");
          else
            printf("  Unknown disposal method %d, ", disposal_method);
          if (user_input_needed)
            printf("User input needed, ");
          else
            printf("No wait for user input, ");
          if (transparency_used)
            printf("Transparency used\n  Transparent = %d\n", transparent_index);
          else
          { printf("No transparency\n");
            transparent_index=99999; }
          if (delay!=0)
            printf("  Delay by %d.%02d seconds after rendering\n", delay/100, delay%100); */ }
        else
        { printf("  block size should be 4, but it is %d\n", b);
          readbb(block, b);
          b=readb();
          if (b!=0)
            printf("  No block terminator present\n"); } }
      else if (kind==0xFE)
      { if (timesround==0) printf("Comment: ");
        while (1)
        { int blocksize=readb();
          if (blocksize==0)
            break;
          readbb(block, blocksize);
          block[blocksize]=0;
          if (timesround==0) printf("%s", block); }
        if (timesround==0) printf("\n"); }
      else if (kind==0x01)
      { printf("Plain Text:\n");
        int b=readb();
        if (b==12)
        { int left=read2b();
          int top=read2b();
          int width=read2b();
          int height=read2b();
          int char_width=readb();
          int char_height=readb();
          int foreground_colour=readb();
          int background_colour=readb();
          printf("  left=%d top=%d width=%d height=%d\n", left, top, width, height);
          printf("  charw=%d, charh=%d, foreground=%d, background=%d\n", char_width, char_height, foreground_colour, background_colour);
          printf("  text: ");
          while (1)
          { int blocksize=readb();
            if (blocksize==0)
              break;
            readbb(block, blocksize);
            block[blocksize]=0;
            printf("%s", block); }
          printf("\n"); }
        else
        { printf("  block size should be 12, but it is %d\n", b);
          readbb(block, b);
          b=readb();
          if (b!=0)
            printf("  No block terminator present\n"); } }
      else if (kind==0xFF)
      { if (timesround==0) printf("Application specific:\n");
        int b=readb();
        if (b==11)
        { char application_identifier[9];
          reads(application_identifier, 8);
          byte authentication_code[3];
          readbb(authentication_code, 3);
          if (timesround==0) printf("  application: %s, ", application_identifier);
          if (timesround==0) printf("authentication: %02X %02X %02X\n", authentication_code[0], authentication_code[1], authentication_code[2]);
          while (1)
          { int blocksize=readb();
            if (blocksize==0)
              break;
            readbb(block, blocksize); } }
        else
        { printf("  block size should be 12, but it is %d\n", b);
          readbb(block, b);
          b=readb();
          if (b!=0)
            printf("  No block terminator present\n"); } }
      else
      { printf("Unexpected extension kind %02X\n", kind);
        break; } }
    else
    { printf("Unexpected marker %02X\n", separator);
      break; } }
  closefile();
  return new image(lsd.width, lsd.height, pixels); }

struct bmpreader
{ static const int debugbmp=0;
  struct BMPHeader
  { short int rubbish;
    char letterB;
    char letterM;
    int filesize;
    int reserved;
    int dataoffset; };
  struct BMPInfoHeader
  { int forty;
    int width;
    int height;
    short int planes;
    short int bitsperpixel;
    int compression;
    int imagesize;
    int xpixelspermetre;
    int ypixelspermetre;
    int numcolours;
    int numimportantcolours; };
  struct BMPColour
  { int b, g, r, code; };
  BMPColour ctab[256];
  static const char * colourname;
  char * reason;
  int readthisrow, partstaken, lastbyte, remainingrep, remainingplain, atendline, atendfile, roundup;
  FILE * f;
  bmpreader();
  void clear();
  void printcolour(int r, int g, int b);
  void printcolour(BMPColour &c);
  void printcolour(int c);
  void startrow(void);
  void finishrow(void);
  int readcolour24(void);
  int readcolour8(void);
  int readcolour4(void);
  int readcolour1(void);
  int readcolour4comp(void);
  int readcolour8comp(void);
  bool openfile(string s);
  void closefile();
  image * read(); };

         //                       r 000000000111111111222222222
         //                       g 000111222000111222000111222
         //                       b 012012012012012012012012012
const char * bmpreader::colourname="_bBgccGcCrmmywSyLSRmMoPPYCW";

bmpreader::bmpreader()
{ clear(); }

void bmpreader::clear()
{ readthisrow=0;
  partstaken=0;
  f=NULL; }

void bmpreader::printcolour(int r, int g, int b)
{ if (r<64) r=0;
  else if (r<192) r=1;
  else r=2;
  if (g<64) g=0;
  else if (g<192) g=1;
  else g=2;
  if (b<64) b=0;
  else if (b<192) b=1;
  else b=2;
  putchar(colourname[r*9+g*3+b]); }

void bmpreader::printcolour(BMPColour &c)
{ printcolour(c.r, c.g, c.b); }

void bmpreader::printcolour(int c)
{ printcolour(c&255, (c>>8)&255, (c>>16)&255); }

void bmpreader::startrow(void)
{ readthisrow=0;
  partstaken=0;
  remainingrep=0;
  remainingplain=0;
  atendline=0;
  roundup=0; }

void bmpreader::finishrow(void)
{ int rem=readthisrow&3;
  readthisrow=0;
  partstaken=0;
  if (rem==0) return;
  while (rem<4)
  { fgetc(f);
    rem+=1; } }

int bmpreader::readcolour24(void)
{ int b=fgetc(f);
  int g=fgetc(f);
  int r=fgetc(f);
  readthisrow+=3;
  return (r|(g<<8)|(b<<16)); }

int bmpreader::readcolour8(void)
{ int c=fgetc(f);
  readthisrow+=1;
  return (ctab[c].code); }

int bmpreader::readcolour4(void)
{ int c;
  if (partstaken==0)
  { lastbyte=fgetc(f);
    c=(lastbyte>>4)&15;
    readthisrow+=1;
    partstaken=1; }
  else
  { partstaken=0;
    c=lastbyte&15; }
  return (ctab[c].code); }

int bmpreader::readcolour1(void)
{ if (partstaken==0)
  { lastbyte=fgetc(f);
    readthisrow+=1; }
  int c=(lastbyte>>(7-partstaken))&1;
  partstaken+=1;
  if (partstaken==8) partstaken=0;
  return (ctab[c].code); }

int bmpreader::readcolour4comp(void)
{ int n, c;
  if (remainingrep>0)
  { partstaken&=1;
    if (partstaken==0)
      c=(lastbyte>>4)&15;
    else
      c=lastbyte&15;
    partstaken+=1;
    remainingrep-=1;
    return ctab[c].code; }
  else if (remainingplain>0)
  { partstaken&=1;
    if (partstaken==0)
    { lastbyte=fgetc(f);
      readthisrow+=1;
      c=(lastbyte>>4)&15; }
    else
      c=lastbyte&15;
    partstaken+=1;
    remainingplain-=1;
    if (remainingplain==0 && roundup)
      fgetc(f);
    return ctab[c].code; }
  else if (atendline || atendfile)
    return ctab[0].code;
  else
  { n=fgetc(f);
    c=fgetc(f);
    if (n==EOF)
    { atendfile=1;
      return 0; }
    if (n>0)
    { remainingrep=n-1;
      lastbyte=c;
      c=(lastbyte>>4)&15;
      partstaken=1;
      return ctab[c].code; }
    else if (n==0)
    { if (c==0)
      { atendline=1;
        return ctab[0].code; }
      else if (c==1)
      { atendline=1;
        atendfile=1;
        return ctab[0].code; }
      else if (c==2)
      { int dx=fgetc(f);
        int dy=fgetc(f);
        reason="Compression includes a delta";
        atendline=1;
        atendfile=1;
        return 0; }
      else
      { remainingplain=c-1;
        roundup=(c/2)&1;
        lastbyte=fgetc(f);
        c=(lastbyte>>4)&15;
        partstaken=1;
        return ctab[c].code; } }
    else
      return ctab[0].code; } }

int bmpreader::readcolour8comp(void)
{ int n, c;
  if (remainingrep>0)
  { remainingrep-=1;
    return ctab[lastbyte].code; }
  else if (remainingplain>0)
  { lastbyte=fgetc(f);
    readthisrow+=1;
    remainingplain-=1;
    if (remainingplain==0 && roundup)
      fgetc(f);
    return ctab[lastbyte].code; }
  else if (atendline || atendfile)
    return ctab[0].code;
  else
  { n=fgetc(f);
    c=fgetc(f);
    if (n==EOF)
    { atendfile=1;
      return 0; }
    if (n>0)
    { remainingrep=n-1;
      lastbyte=c;
      return ctab[lastbyte].code; }
    else if (n==0)
    { if (c==0)
      { atendline=1;
        return ctab[0].code; }
      else if (c==1)
      { atendline=1;
        atendfile=1;
        return ctab[0].code; }
      else if (c==2)
      { int dx=fgetc(f);
        int dy=fgetc(f);
        reason="Compression includes a delta";
        atendline=1;
        atendfile=1;
        return 0; }
      else
      { remainingplain=c-1;
        roundup=c&1;
        lastbyte=fgetc(f);
        return ctab[lastbyte].code; } }
    else
      return ctab[0].code; } }

bool bmpreader::openfile(string s)
{ clear();
  f=fopen(s.c_str(), "rb");
  if (f==NULL)
    return false;
  return true; }

void bmpreader::closefile()
{ if (f!=NULL)
    fclose(f);
  f=NULL; }

image * bmpreader::read()
{ if (f==NULL)
    return NULL;
  if (debugbmp) printf("sizeof(BMPHeader)=%d, should be 16\n", sizeof(BMPHeader));
  if (debugbmp) printf("sizeof(BMPInfoHeader)=%d, should be 40\n", sizeof(BMPInfoHeader));
  BMPHeader prehdr;
  BMPInfoHeader hdr;
  int n=fread(&prehdr.letterB, 14, 1, f);
  if (n!=1)
  { reason="File too small, header missing";
    return NULL; }
  n=fread(&hdr, 40, 1, f);
  if (n!=1)
  { reason="File too small, header missing";
    return NULL; }
  if (prehdr.letterB!='B' || prehdr.letterM!='M')
  { reason="'BM' signature missing";
    return NULL; }
  if (debugbmp) printf("File Size is %d bytes\n", prehdr.filesize);
  if (debugbmp) printf("Reserved for zeros is %d\n", prehdr.reserved);
  if (debugbmp) printf("Data Offset is %d\n", prehdr.dataoffset);
  if (hdr.forty!=40)
  { reason="size 40 missing from header";
    return NULL; }
  int w=hdr.width, h=hdr.height, b=hdr.bitsperpixel,
      comp=hdr.compression, numc=hdr.numcolours;
  if (debugbmp) printf("width is %d pixels\n", w);
  if (debugbmp) printf("height is %d pixels\n", h);
  if (hdr.planes!=1)
  { reason="Number of planes is not 1";
    return NULL; }
  if (debugbmp) printf("%d planes\n", hdr.planes);
  if (debugbmp) printf("%d bits per pixel\n", b);
  if (b!=1 && b!=4 && b!=8 && b!=24)
  { reason="Bits per pixel has unsupported value";
    return NULL; }
  if (comp<0 || comp>2)
  { reason="Invalid compression code";
    return NULL; }
  if (debugbmp) printf("Compression code %d\n", comp);
  if (debugbmp) printf("resolution %d x %d pixels per metre\n", hdr.xpixelspermetre, hdr.ypixelspermetre);
  if (b==1)
    numc=2;
  else if (b==8 && numc==0)
    numc=256;
  else if (b==4 && numc==0)
    numc=16;
  else if (b>8)
    numc=0;
  if (debugbmp) printf("%d colours defined\n", numc);
  if (debugbmp) printf("first %d colours are important\n", hdr.numimportantcolours);
  for (int j=0; j<256; j+=1)
  { ctab[j].r=0;
    ctab[j].g=0;
    ctab[j].b=0; }
  if (numc>(1<<b))
  { reason="Too many colours for bits per pixel";
    return NULL; }
  for (int i=0; i<numc; i+=1)
  { int x=fgetc(f);
    if (x==EOF)
    { reason="File too small, colour table incomplete";
      return NULL; }
    ctab[i].b=x;
    x=fgetc(f);
    if (x==EOF)
    { reason="File too small, colour table incomplete";
      return NULL; }
    ctab[i].g=x;
    x=fgetc(f);
    if (x==EOF)
    { reason="File too small, colour table incomplete";
      return NULL; }
    ctab[i].r=x;
    x=fgetc(f);
    if (x==EOF)
    { reason="File too small, colour table incomplete";
      return NULL; }
    if (debugbmp) printf("Colour[%d] = r:%3d g:%3d b:%3d  = ", i, ctab[i].r, ctab[i].g, ctab[i].b);
    if (debugbmp) printcolour(ctab[i]);
    if (debugbmp) putchar('\n'); }
  for (int k=0; k<256; k+=1)
    ctab[k].code=ctab[k].r|(ctab[k].g<<8)|(ctab[k].b<<16);
  if (debugbmp) printf("\n");
  atendfile=0;
  int * * pix = new int * [h];
  for (int i=0; i<h; i+=1)
    pix[i] = new int [w];
  image * answer = new image(w, h, pix);
  if (comp==0)
  { if (b==24)
    { for (int row=h-1; row>=0; row-=1)
      { startrow();
        for (int col=0; col<w; col+=1)
        { int c=readcolour24();
          pix[row][col]=c; }
        finishrow(); } }
    else if (b==8)
    { for (int row=h-1; row>=0; row-=1)
      { startrow();
        for (int col=0; col<w; col+=1)
        { int c=readcolour8();
          pix[row][col]=c; }
        finishrow(); } }
    else if (b==4)
    { for (int row=h-1; row>=0; row-=1)
      { startrow();
        for (int col=0; col<w; col+=1)
        { int c=readcolour4();
          pix[row][col]=c; }
        finishrow(); } }
    else if (b==1)
    { for (int row=h-1; row>=0; row-=1)
      { startrow();
        for (int col=0; col<w; col+=1)
        { int c=readcolour1();
          pix[row][col]=c; }
        finishrow(); } } }
  else if (comp==2 && b==4)
  { for (int row=h-1; row>=0; row-=1)
    { startrow();
      for (int col=0; col<w; col+=1)
      { int c=readcolour4comp();
        pix[row][col]=c; }
      while (!atendline)
        readcolour4comp(); } }
  else if (comp==1 && b==8)
  { for (int row=h-1; row>=0; row-=1)
    { startrow();
      for (int col=0; col<w; col+=1)
      { int c=readcolour8comp();
        pix[row][col]=c; }
      while (!atendline)
        readcolour8comp(); } }
  else
  { reason="Compression scheme not supported";
    delete answer;
    return NULL; }
  if (debugbmp) putchar('\n');
  closefile();
  return answer; }

namespace jpeg
{
  const int debugh=0, debugb=0, debugt=0, debuga=0;

  FILE * filein;

  int putback1=0, putback2=0;

  int round(double v)
  { if (v<0)
      return (int)(v-0.5);
    else
      return (int)(v+0.5); }

  int getbyte()
  { if (putback1==0)
      return fgetc(filein);
    else if (putback1==1)
    { putback1=2;
      return 0xFF; }
    else
    { putback1=0;
      return putback2; } }

  int getword()
  { int a = getbyte();
    int b = getbyte();
    return (a<<8)|b; }

  void ignoresome(int n)
  { for (int i=0; i<n; i+=1)
      getbyte(); }

  void printsome(int n)
  { for (int i=0; i<n; i+=1)
    { if (i%16==0)
        printf("      ");
      int a = getbyte();
      if (a==-1)
        break;
      if (a==0xFF)
        printf("<%02X", a);
      else
        printf(" %02X", a);
      if (i%16==15)
        printf("\n"); }
    if (n%16!=0)
      printf("\n"); }

  struct bitstream
  { int b, nbits;
    bitstream();
    int getbit();
    int getbits(int n); };

  bitstream::bitstream()
  { putback1=0;
    nbits=0; }

  int bitstream::getbits(int n)
  { int v=0;
    for (int i=0; i<n; i+=1)
    { int b=getbit();
      if (b==-1)
        return -1;
      v=(v<<1)|b; }
    return v; }

  int bitstream::getbit()
  { if (nbits==0)
    { b=getbyte();
      if (debugb) printf("[%02X]", b);
      if (b==0xFF)
      { b=getbyte();
        if (debugb) printf("[%02X]", b);
        if (b==0)
          b=0xFF;
        else
        { putback1=1;
          putback2=b;
          return -1; } }
      if (b==-1)
        return -1;
      nbits=8; }
    nbits-=1;
    return (b>>nbits)&0x01; }

  int getbitnum(int nb, int v)
  { if (nb==0)
      return 0;
    int max=(1<<nb)-1, half=1<<(nb-1);
    if (v&half)
      return v;
    return v-max; }

  struct hufftree
  { int num;
    hufftree * ptr0, * ptr1;
    hufftree();
    ~hufftree();
    void print(char * prefix="");
    void add(const char * code, int val); };

  hufftree::hufftree()
  { ptr0=NULL;
    ptr1=NULL;
    num=-1; }

  hufftree::~hufftree()
  { delete ptr0;
    delete ptr1; }

  void hufftree::print(char * prefix)
  { static char dfprf[50];
    if (prefix!=dfprf)
    { strcpy(dfprf, prefix);
      print(dfprf); }
    else if (ptr0==NULL && ptr1==NULL)
      if (debugh) printf("%-25s %d\n", prefix, num);
    else
    { int len=strlen(prefix);
      if (ptr0!=0)
      { prefix[len]='0';
        prefix[len+1]=0;
        ptr0->print(prefix);
        prefix[len]=0; }
      if (ptr1!=0)
      { prefix[len]='1';
        prefix[len+1]=0;
        ptr1->print(prefix);
        prefix[len]=0; } } }

  void hufftree::add(const char * code, int val)
  { char c = code[0];
    if (c==0)
    { num=val;
      if (ptr0!=NULL)
        delete ptr0;
      ptr0=NULL;
      if (ptr1!=NULL)
        delete ptr1;
      ptr1=NULL; }
    else if (c=='0')
    { num=0;
      if (ptr0==NULL)
        ptr0=new hufftree();
      ptr0->add(code+1, val); }
    else if (c=='1')
    { num=0;
      if (ptr1==NULL)
        ptr1=new hufftree();
      ptr1->add(code+1, val); } }

  void getstuffed(bitstream & b, hufftree * dctree, hufftree * actree, int * block)
  { for (int i=0; i<64; i+=1)
      block[i]=0;
    hufftree * t = dctree;
    while (t->ptr0!=NULL || t->ptr1!=NULL)
    { int bit=b.getbit();
      if (bit==-1)
        return;
      if (debugh) printf("%d", bit);
      if (bit==0)
        t=t->ptr0;
      else
        t=t->ptr1;
      if (t==NULL)
      { printf("BIT NOT IN TREE\n");
        return; } }
    int n=b.getbits(t->num);
    block[0]=getbitnum(t->num, n);
    if (debugh)
      printf("  DC value %d bits, %d giving %d\n", t->num, n, getbitnum(t->num, n));
    int pos=0;
    while (1)
    { if (pos>=63)
      { if (debugh) printf("No (0,0) end marker\n");
        return; }
      t = actree;
      while (t->ptr0!=NULL || t->ptr1!=NULL)
      { int bit=b.getbit();
        if (bit==-1)
        { if (debugh) printf("EARLY MARKER\n");
          return; }
        if (debugh) printf("%d", bit);
        if (bit==0)
          t=t->ptr0;
        else
          t=t->ptr1;
        if (t==NULL)
        { printf("BIT NOT IN TREE\n");
          return; } }
      n=t->num;
      int nz=n>>4, sz=n&15;
      if (nz==0 && sz==0)
        return;
      if (nz==15 && sz==0)
        nz+=1;
      while (nz>0)
      { pos+=1;
        if (pos>63) break;
        block[pos]=0;
        nz-=1; }
      if (sz!=0)
      { n=b.getbits(sz);
        pos+=1;
        if (pos>63)
        { printf("\n**** Too many samples ****\n");
          return; }
        block[pos]=getbitnum(sz, n); }
      if (debugh)
        printf("  (%d zeros, %d bits) %d gives AC value %d\n", nz, sz, n, getbitnum(sz, n)); } }

  void printblock(int b[])
  { printf("[%d", b[0]);
    for (int i=1; i<30; i+=1)
      printf(",%d", b[i]);
    printf(",...");
    for (int i=58; i<64; i+=1)
      printf(",%d", b[i]);
    printf("]\n"); }

  struct qtable
  { double c[8][8]; };

  int zzx[64] = { 0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7 };
  int zzy[64] = { 0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7 };

  const double pi = acos(-1.0);

  double transf[8][8], alpha[8][8];

  void setup()
  { alpha[0][0]=0.5/4;
    for (int i=1; i<8; i+=1)
    { alpha[i][0]=sqrt(2.0)/8;
      alpha[0][i]=sqrt(2.0)/8; }
    for (int i=1; i<8; i+=1)
      for (int j=1; j<8; j+=1)
        alpha[i][j]=0.25;
    for (int n=0; n<8; n+=1)
      for (int k=0; k<8; k+=1)
        transf[n][k]=cos(pi*k/8.0*(n+0.5)); }

  void dct(int orig[][8], int result[][8], double quant[][8])
  { for (int i=0; i<8; i+=1)
      for (int j=0; j<8; j+=1)
        orig[i][j]-=128;
    for (int k1=0; k1<8; k1+=1)
    { for (int k2=0; k2<8; k2+=1)
      { double ans=0;
        for (int n1=0; n1<8; n1+=1)
          for(int n2=0; n2<8; n2+=1)
            ans+=alpha[k1][k2]*orig[n1][n2]*transf[n1][k1]*transf[n2][k2];
        result[k1][k2]=round(ans); } }
    for (int i=0; i<8; i+=1)
      for (int j=0; j<8; j+=1)
        result[i][j]=round(result[i][j]/quant[i][j]); }

  void idct(int orig[][8], int result[][8], double quant[][8])
  { for (int i=0; i<8; i+=1)
      for (int j=0; j<8; j+=1)
        orig[i][j]=(int)(orig[i][j]*quant[i][j]);
    for (int n1=0; n1<8; n1+=1)
    { for (int n2=0; n2<8; n2+=1)
      { double ans=0;
        for (int k1=0; k1<8; k1+=1)
          for(int k2=0; k2<8; k2+=1)
            ans+=alpha[k1][k2]*orig[k1][k2]*transf[n1][k1]*transf[n2][k2];
        result[n1][n2]=round(ans+128); } } }

  void enzigzag(int m[][8], int r[])
  { for (int p=0; p<64; p+=1)
      r[p]=m[zzy[p]][zzx[p]]; }

  void dezigzag(int r[], int m[][8])
  { for (int p=0; p<64; p+=1)
      m[zzy[p]][zzx[p]]=r[p]; }

  void dezigzag(int r[], double q[][8])
  { for (int p=0; p<64; p+=1)
      q[zzy[p]][zzx[p]]=r[p]; }

  void printflat(int result[])
  { int last=63;
    for (int p=63; p>=0; p-=1)
    { if (result[p]!=0)
      { last=p;
        break; } }
    for (int p=0; p<=last; p+=1)
      printf(" %d", result[p]);
    printf("\n"); }

  void print8x8(int result[][8])
  { for (int i=0; i<8; i+=1)
    { if (i==0) printf("  ["); else printf("   ");
      for (int j=0; j<8; j+=1)
        printf(" %3d", result[i][j]);
      if (i<7) printf("\n"); }
    printf("  ]\n"); }

  struct jpegparam
  { int ncomp, nstart, rstint, width, height, prec, xpos, ypos;
    int comptype[17], compxsamp[17], compysamp[17], compqtab[17], dcval[17];
    hufftree * hufftrees[33], * dctree[17], * actree[17];
    int * * image[17];
    qtable * qtab[17];
    jpegparam();
    ~jpegparam(); };

  jpegparam::jpegparam()
  { for (int i=0; i<17; i+=1)
    { dctree[i]=NULL;
      actree[i]=NULL;
      image[i]=NULL;
      qtab[i]=NULL; }
    for (int i=0; i<33; i+=1)
      hufftrees[i]=NULL; }

  jpegparam::~jpegparam()
  { int h=(height+7)&0x7FFFFFF8;
    for (int i=0; i<17; i+=1)
    { if (i==1)
        continue;
      delete qtab[i];
      if (image[i]!=NULL)
      { for (int j=0; j<h; j+=1)
          delete[] image[i][j]; }
      delete[] image[i]; }
    if (image[1]!=NULL)
    { for (int i=height; i<h; i+=1)
        delete[] image[1][i]; }
    for (int i=0; i<33; i+=1)
      delete hufftrees[i]; }

  void getblock(bitstream & b, jpegparam & j)
  { int blk[64], m2[8][8], m3[8][8];
    for (int cmpn=1; cmpn<=j.ncomp; cmpn+=1)
    { int * * im = j.image[cmpn];
      getstuffed(b, j.dctree[cmpn], j.actree[cmpn], blk);
      dezigzag(blk, m2);
      m2[0][0]+=j.dcval[cmpn];
      j.dcval[cmpn]=m2[0][0];
      idct(m2, m3, j.qtab[j.compqtab[cmpn]]->c);
      for (int r=0; r<8; r+=1)
        for (int c=0; c<8; c+=1)
          im[j.ypos+r][j.xpos+c]=m3[r][c]; }
    j.xpos+=8;
    if (j.xpos>=j.width)
    { j.xpos=0;
      j.ypos+=8; } }

  int * * newim(int rs, int cs)
  { rs=(rs+7)&0xFFFFFFF8;
    cs=(cs+7)&0xFFFFFFF8;
    int * * v = new int * [rs];
    for (int i=0; i<rs; i+=1)
      v[i]=new int [cs];
    return v; }

  void printqtab(qtable * q)
  { for (int r=0; r<8; r+=1)
    { if (r==0) printf("  { {"); else printf("    {");
      for (int c=0; c<8; c+=1)
      { if (c>0) printf(",");
        printf(" %3.0f", q->c[r][c]); }
      if (r==7) printf(" } }\n"); else printf(" },\n"); } }

  image * readjpeg(const char * fname)
  { filein=fopen(fname, "rb");
    if (filein==NULL)
      return NULL;
    jpegparam j;
    j.ncomp=1;
    j.nstart=0;
    j.rstint=0x7FFFFFFF;
    while (1)
    { int ff=getbyte();
      if (ff==-1)
      { printf("Unexpected end of file\n");
        return NULL; }
      else if (ff==0xFF)
      { int c = getbyte();
        if (c==0xD8)
        { if (debuga) printf("FFD8 start of image\n"); }
        else if (c==0xD9)
        { if (debuga) printf("FFD9 end of image\n");
          image * res = new image(j.width, j.height, j.image[1]);
          if (j.ncomp==1)
          { int * * v = j.image[1];
            for (int r=0; r<j.height; r+=2)
            { for (int c=0; c<j.width; c+=2)
              { int y=v[r][c];
                if (y<0) y=0; else if (y>255) y=255;
                v[r][c]=(r<<16)|(y<<8)|y; } } }
          else
          { int * * v1 = j.image[1];
            int * * v2 = j.image[2];
            int * * v3 = j.image[3];
            for (int r=0; r<j.height; r+=1)
            { for (int c=0; c<j.width; c+=1)
              { int y=v1[r][c], cb=v2[r][c]-128, cr=v3[r][c]-128;
                int red = (int)(y + 1.402*cr);
                int green = (int)(y - 0.34414*cb - 0.71414*cr);
                int blue = (int)(y + 1.772*cb);
                if (red<0) red=0; else if (red>255) red=255;
                if (blue<0) blue=0; else if (blue>255) blue=255;
                if (green<0) green=0; else if (green>255) green=255;
                v1[r][c]=(blue<<16)|(green<<8)|red; } } }
          return res; }
        else if (c==0xC0)
        { int sz=getword()-2;
          if (debuga) printf("FFC0 start of frame (baseline) (%d):\n", sz);
          j.prec=getbyte();
          sz-=1;
          if (debuga) printf("  Precision: %d bits per sample\n", j.prec);
          if (j.prec!=8)
          { printf("\n**** Can't handle %d bit precision ****\n", j.prec);
            return NULL; }
          j.height=getword();
          sz-=2;
          if (debuga) printf("  Height: %d pixels\n", j.height);
          j.width=getword();
          sz-=2;
          if (debuga) printf("  Width: %d pixels\n", j.width);
          j.ncomp=getbyte();
          sz-=1;
          if (debuga) printf("  Components: %d\n", j.ncomp);
          if (j.ncomp!=1 && j.ncomp!=3)
          { printf("\n**** Can't handle %d components ****\n", j.ncomp);
            return NULL; }
          for (int i=1; i<=j.ncomp; i+=1)
          { int cid=getbyte();
            sz-=1;
            if (debuga) printf("    component %d, %c%c: ", i, "  CC  "[cid], " YbrIQ"[cid]);
            if (i==1 && cid!=1)
            { printf("\n**** Can't handle component %d not being Y ****\n", i);
              return NULL; }
            if (i==2 && cid!=2)
            { printf("\n**** Can't handle component %d not being Cb ****\n", i);
              return NULL; }
            if (i==3 && cid!=3)
            { printf("\n**** Can't handle component %d not being Cr ****\n", i);
              return NULL; }
            j.image[i]=newim(j.height, j.width);
            j.comptype[i]=cid;
            int vertsf=getbyte();
            int horsf=vertsf>>4;
            vertsf&=15;
            sz-=1;
            if (debuga) printf("sampling factors %d in x, %d in y,  ", horsf, vertsf);
            if (horsf!=1 || vertsf!=1)
            { printf("\n**** Can't handle component sampling factor not 1 ****\n");
              return NULL; }
            j.compxsamp[i]=horsf;
            j.compysamp[i]=vertsf;
            int qtn=getbyte();
            sz-=1;
            if (debuga) printf("Quant table %d\n", qtn);
            j.compqtab[i]=qtn; }
          if (sz>0)
          { printf("  LEFT OVERS:\n");
            if (debuga) printsome(sz); else ignoresome(sz);
            return NULL; } }
        else if (c==0xC2)
        { int sz=getword()-2;
          printf("FFC2 start of frame (progressive) (%d):\n", sz);
          if (debuga) printsome(sz); else ignoresome(sz); }
        else if (c==0xC4)
        { int sz=getword()-2, num=0, numoflen[17];
          if (debuga) printf("FFC4 define huffman tables (%d):\n", sz);
          for (int t=0; sz>=16; t+=1)
          { hufftree * ht = new hufftree();
            int tn=getbyte();
            sz-=1;
            if (debuga) printf("  Table %d: %cC-%d ", t, "DA"[(tn>>4)&1], tn&0x0F);
            j.hufftrees[tn&0x1F]=ht;
            for (int i=1; i<=16; i+=1)
            { if (sz<=0)
              { printf("**RAN OUT**");
                return NULL; }
              numoflen[i]=getbyte();
              sz-=1;
              if (debugt)
              { if (i>1)
                  printf(", ");
                printf("%d", numoflen[i]); }
              num+=numoflen[i]; }
            if (debuga) printf("\n");
            int count[256];
            for (int i=0; i<256; i+=1)
              count[i]=0;
            char code[18];
            code[0]=0;
            for (int i=1; i<=16; i+=1)
            { code[i-1]='0';
              code[i]=0;
              for (int j=0; j<numoflen[i]; j+=1)
              { if (sz<=0)
                { printf("**RAN OUT**");
                  return NULL; }
                int v=getbyte();
                count[v]+=1;
                if (count[v]>1)
                { printf("******REPEATED******");
                  return NULL; }
                sz-=1;
                if (debugt) printf("        %3d: %-20s\n", v, code);
                ht->add(code, v);
                for (int p=i-1; p>=0; p-=1)
                { if (code[p]=='0')
                  { code[p]='1';
                    break; }
                  code[p]='0'; } } }
            if (debugt)
            { printf("tree as recorded:\n");
              ht->print(); } }
          if (sz>0)
          { printf("  LEFT OVERS (%d):\n", sz);
            if (debuga) printsome(sz); else ignoresome(sz);
            return NULL; } }
        else if (c==0xDB)
        { int sz=getword()-2;
          if (debuga) printf("FFDB define quantization tables (%d):\n", sz);
          while (sz>0)
          { int qtid=getbyte();
            sz-=1;
            int bits=16;
            if ((qtid&0xF0)==0)
              bits=8;
            qtid&=0x0F;
            if (debuga) printf("  Quantization table %d: %d bit precision\n", qtid, bits);
            if (bits!=8)
            { printf("\n**** Can't handle qtable precision not 8 ****\n");
              return NULL; };
            qtable * qt = new qtable;
            j.qtab[qtid]=qt;
            int blk[64];
            for (int i=0; i<64; i+=1)
            { int v=getbyte();
              sz-=1;
              if (bits==16)
              { v=v*256+getbyte();
                sz-=1; }
              blk[i]=v; }
            dezigzag(blk, qt->c); }
          if (sz>0)
          { printf("  LEFT OVERS\n");
            if (debuga) printsome(sz); else ignoresome(sz);
            return NULL; } }
        else if (c==0xDD)
        { int sz=getword()-2;
          j.rstint=getword();
          if (debuga) printf("FFDD define RSTn interval as %d\n", j.rstint);
          sz-=2; }
        else if (c==0xDA)
        { int sz=getword()-2;
          j.nstart+=1;
          if (debuga) printf("FFDA start of scan (size=%d) - Start %d\n", sz, j.nstart);
          int numcom=getbyte();
          if (numcom!=j.ncomp)
          { printf("\n**** number of components has changed ****\n");
            return NULL; }
          sz-=1;
          for (int i=1; i<=numcom; i+=1)
          { int id=getbyte();
            sz-=1;
            if (debuga) printf("  Component %d, %c%c: ", i, "  CC  "[id], " YbrIQ"[id]);
            if (id!=j.comptype[i])
            { printf("\n**** component type has changed ****\n");
              return NULL; }
            int tabs=getbyte();
            sz-=1;
            j.dctree[i]=j.hufftrees[tabs>>4];
            j.actree[i]=j.hufftrees[16+(tabs&15)];
            if (debuga) printf("  Q table %d\n", j.compqtab[i]);
            /* printqtab(j.qtab[j.compqtab[i]]); */ }
          if (debuga) printf("  Ignore three bytes\n");
          ignoresome(sz);
          j.xpos=0;
          j.ypos=0;
          for (int i=0; i<17; i+=1)
            j.dcval[i]=0;
          bitstream b;
          int nclumps = ((j.width+7)/8)*((j.height+7)/8);
          if (j.rstint<nclumps) nclumps=j.rstint;
          if (debuga) printf(" Getting %d clumps:\n", nclumps);
          for (int clump=0; clump<nclumps; clump+=1)
          { if (debuga) printf("  Clump %d at %d, %d\n", clump+1, j.xpos, j.ypos);
            getblock(b, j); } }
        else if (c==0xFE)
        { int sz=getword()-2;
          if (debuga)
          { printf("FFFE comment (%d):\n", sz);
            printsome(sz); }
          else
            ignoresome(sz); }
        else if ((c&0xF0)==0xD0)
        { j.nstart+=1;
          if (debuga) printf("FFDn Restart %d - Start %d\n", c&0x0F, j.nstart);
          for (int i=0; i<17; i+=1)
            j.dcval[i]=0;
          bitstream b;
          int nclumps = ((j.width+7)/8)*((j.height+7)/8);
          if (j.rstint<nclumps) nclumps=j.rstint;
          if (debuga) printf(" Getting %d clumps:\n", nclumps);
          for (int clump=0; clump<nclumps; clump+=1)
          { if (debuga) printf("  Clump %d at %d, %d\n", clump+1, j.xpos, j.ypos);
            getblock(b, j); } }
        else if ((c&0xF0)==0xE0)
        { int sz=getword()-2;
          if (debuga) printf("FFEn application specific %d (%d):\n", c&0x0F, sz);
          ignoresome(sz); }
        else
        { printf("UNEXPECTED 0x%02X after 0xFF\n", c);
          if (debuga) printsome(100);
          return NULL; } }
      else
      { printf("UNEXPECTED 0x%02X\n", ff);
        if (debuga) printsome(100);
        return NULL; } } } }

image * image_from_gif_file(string s)
{ static gifreader gr;
  bool ok=gr.openfile(s.c_str());
  if (!ok)
  { error_pop_up("Image file not found", s+": file not found");
    return NULL; }
  return gr.read(); }

image * image_from_bmp_file(string s)
{ static bmpreader bm;
  bool ok=bm.openfile(s.c_str());
  if (!ok)
  { error_pop_up("Image file not found", s+": file not found");
    return NULL; }
  return bm.read(); }

image * image_from_jpeg_file(string s)
{ static bool used=false;
  if (!used)
  { jpeg::setup();
    used=true; }
  image * im = jpeg::readjpeg(s.c_str());
  if (im==NULL)
    error_pop_up("Image file not found", s+": file not found");
  fclose(jpeg::filein);
  return im; }

image * image_from_file(string s)
{ int dp=-1, n=s.length();
  for (int i=0; i<n; i+=1)
    if (s[i]=='.')
      dp=i;
  if (dp<0)
    error_pop_up("Image file name error", s+": bad file anme for image\nGIF, BMP, JPG only");
  string e="";
  for (int i=dp+1; i<n; i+=1)
  { if (s[i]<'a')
      e+=(char)(s[i]+32);
    else
      e+=s[i]; }
  if (e=="gif")
    return image_from_gif_file(s);
  else if (e=="bmp")
    return image_from_bmp_file(s);
  else if (e=="jpg" || e=="jpeg")
    return image_from_jpeg_file(s);
  else
    error_pop_up("Image file name error", s+": bad file anme for image\nGIF, BMP, JPG only");
  return NULL; }
