#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/resource.h>

using namespace std;

int get_clock_time()
{ time_t t;
  time(&t);
  struct tm * lt = localtime(&t);
  return lt->tm_hour*10000 + lt->tm_min*100 + lt->tm_sec; }

int get_calendar_date()
{ time_t t;
  time(&t);
  struct tm * lt = localtime(&t);
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
{ printf("%g", d); }

void print_float(double d)
{ print_double(d); }

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

string _x_readsomething()
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
{ return _x_readsomething(); }

string read_line()
{ string r="";
  while (true)
  { int c = getc(stdin);
    if (c=='\n' || c==EOF)
      break;
    r+=(char)c; }
  return r; }

int read_int()
{ string s=_x_readsomething();
  char * stopper;
  int val = strtol(s.c_str(), &stopper, 10);
  if (*stopper!=0)
  { cerr << "\n'" << s << "' is not an int value\n";
    exit(1); }
  return val; }

double read_double()
{ string s=_x_readsomething();
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { cerr << "\n'" << s << "' is not a double value\n";
    exit(1); }
  return val; }

double read_float()
{ string s=_x_readsomething();
  char * stopper;
  double val = strtod(s.c_str(), &stopper);
  if (*stopper!=0)
  { cerr << "\n'" << s << "' is not a float value\n";
    exit(1); }
  return val; }

bool _x_same(string s, const char * t)
{ return strcasecmp(s.c_str(), t)==0; }

bool read_bool()
{ string s=_x_readsomething();
  if (s=="1") return true;
  if (s=="0") return false;
  if (_x_same(s, "true")) return true;
  if (_x_same(s, "false")) return false;
  if (_x_same(s, "yes")) return true;
  if (_x_same(s, "no")) return false;
  if (_x_same(s, "t")) return true;
  if (_x_same(s, "f")) return false;
  if (_x_same(s, "y")) return true;
  if (_x_same(s, "n")) return false;
  cerr << "\n'" << s << "' is not a bool value.\n";
  exit(1);
  return false; }

int _x_randomificate()
{ srandomdev();
  return 0; }

const int _x_pointless = _x_randomificate();

void not_really_random(int n=1289)
{ srandom(n); }

int random_in_range(int a, int b)
{ if (b<a)
    return random()%(a-b+1)+b;
  else
    return random()%(b-a+1)+a; }

void exit()
{ exit(0); }

double get_cpu_time()
{ struct rusage ruse;
  getrusage(RUSAGE_SELF, &ruse);
  return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0 + 
         ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0; }
