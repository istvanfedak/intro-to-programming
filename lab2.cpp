#include "library.h"
double const pi = acos(-1.0);

// 1) screen shot of a star and multiple stars
void draw_star1(double const x, double const y, double const side, int const color)
{
	set_pen_color(color);
	set_heading_degrees(180);
	move_to(x,y);
	draw_point(x,y);
	double const radius = (cos(72.0*pi/180)*side)/sin((180.0/5)*pi/180);
	move_distance(radius);
	start_shape();
	
	turn_right_by_degrees(54);
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144); 
	draw_distance(side);  note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	fill_shape();
}

// Section 1!

void draw_stars()
{
	double const x = 200;
	double const side = x*0.4;
 	double const y = x;
	make_window(600,400);
	set_pen_width(3);
	set_pen_color(color::blue);
	draw_star1(x,y,side, color::dark_blue);
	draw_star1(x,y,side/2.0, color::white);
	draw_star1(x,y,side/4.0, color::dark_red);
	draw_star1(x*2.1,y*0.9,side/4.0, color::orange);
	draw_star1(x*1.7,y*1.2,side/3.0, color::dark_green);
	draw_star1(x*1.9,y*0.5,side/2.4, color::dark_green);
}

//___________________________________________________________________________________________________________________________
// Second section of the Lab! Here we will be drawing the US service flag
//*
void draw_star2(double const x, double const y, int const color)
{
	double const side = x*(50/600.0);
	set_pen_width(2);
	set_pen_color(color);
	set_heading_degrees(180);
	move_to(x/2.0,y/2.0);
	draw_point(x/2.0,y/2.0);
	double const radius = (cos(72.0*pi/180)*(side))/sin((180.0/5)*pi/180);
	move_distance(radius);
	start_shape();
	
	turn_right_by_degrees(54);
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144); 
	draw_distance(side);  note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	fill_shape();
}

void draw_rectangle_a(double const x, double const y,int const color)
{
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(x*(1/60.0),x*(1/60.0));
	start_shape();
	turn_right_by_degrees(0);
	draw_distance(x*(29/30.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(71/120.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(29/30.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(71/120.0)); note_position();
	fill_shape();
}

void draw_rectangle_b(double const x, double const y,int const color)
{
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(x*(2/15.0),x*(2/15.0));

	start_shape();
	turn_right_by_degrees(0);
	draw_distance(x*(11/15.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(43/120.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(11/15.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(43/120.0)); note_position();
	fill_shape();
}

void draw_service_flag()
{
	double const x = 600; //initially x = 600
	double const y= x*(375/600.0);
	make_window(x,y);
	draw_rectangle_a(x, y, color::dark_red);
	draw_rectangle_b(x,y, color::white);
	draw_star2(x,y,color::dark_blue);

	
}

//__________________________________________________________________________________________________________________________________
// Section 3

//White background for the flag
void draw_background(int const x, int const y,int const color)
{
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(0,0);
	start_shape();
	turn_right_by_degrees(0);
	draw_distance(x); note_position();
	turn_right_by_degrees(90);
	draw_distance(y); note_position();
	turn_right_by_degrees(90);
	draw_distance(x); note_position();
	turn_right_by_degrees(90);
	draw_distance(y); note_position();
	fill_shape();
}

//initial stripe
void draw_rectangle( double const x, double const b,int const color)
{
	double const a = 0;
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(a,b);
	start_shape();
	turn_right_by_degrees(0);
	draw_distance(x); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(10/247.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(10/247.0)); note_position();
	fill_shape();
}

//function that draws read stripes
void draw_stripes(double const x, double const y1, int const color)
{
	draw_rectangle(x,0,color::dark_red);
	draw_rectangle(x,y1*2.0,color::dark_red);
	draw_rectangle(x,y1*4.0,color::dark_red);
	draw_rectangle(x,y1*6.0,color::dark_red);
	draw_rectangle(x,y1*8.0,color::dark_red);
	draw_rectangle(x,y1*10.0,color::dark_red);
	draw_rectangle(x,y1*12.0,color::dark_red);
}


void draw_star_background(double const x, int const color)
{
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(0,0);
	start_shape();
	turn_right_by_degrees(0);
	draw_distance(x*(15/38.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(70/247.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(15/38.0)); note_position();
	turn_right_by_degrees(90);
	draw_distance(x*(70/247.0)); note_position();
	fill_shape();
}

//This function helps me find the diameter of the star so I can calculate the ratios
double radius_of_star(double const side)
{
	double const radius = (cos(72.0*pi/180)*(side))/sin((180.0/5)*pi/180);
	double const radius_of_the_star = (side*cos(18*pi/180))+(radius*sin(54*pi/180));
	return(radius_of_the_star);
}

//this star has a center at (x2,y2)!!
void draw_star(double const x2, double const y2, double const side, int const color)
{
	set_pen_width(1);
	set_pen_color(color);
	set_heading_degrees(180);
	move_to(x2,y2);
	// this formula finds the radius of the pentagon with in the star. Makes it easier to locate the center of the star
	double const radius = (cos(72.0*pi/180)*(side))/sin((180.0/5)*pi/180);
	move_distance(radius);
	start_shape();
	
	turn_right_by_degrees(54);
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144); 
	draw_distance(side);  note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_left_by_degrees(72);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	fill_shape();
}


void draw_six_stars(double const x, double const y_int, double const side, int const color)
{
	draw_star(x*(30/760.0),y_int,side,color);
	draw_star(x*(78/760.0),y_int,side,color);
	draw_star(x*(126/760.0),y_int,side,color);
	draw_star(x*(174/760.0),y_int,side,color);
	draw_star(x*(222/760.0),y_int,side,color);
	draw_star(x*(270/760.0),y_int,side,color);
}

void draw_five_stars(double const x, double const y_int, double const side, int const color)

{
	draw_star(x*(54/760.0),y_int,side,color);
	draw_star(x*(102/760.0),y_int,side,color);
	draw_star(x*(150/760.0),y_int,side,color);
	draw_star(x*(198/760.0),y_int,side,color);
	draw_star(x*(246/760.0),y_int,side,color);
}

void draw_us_flag()
{
	double const x = 760; //initially x = 760
	double const y= x*(400/760.0);
	double const side = x*(9.15/760.0);
	double const y_int = x*(7/247.0) ;
	make_window(x,y);

	draw_background(x,y,color::white);

	draw_stripes(x,x*(10/247.0),color::dark_red);

	draw_star_background(x, color::dark_blue);

	draw_six_stars(x,y_int,side,color::white);
	draw_five_stars(x,2*y_int,side,color::white);
	draw_six_stars(x,3*y_int,side,color::white);
	draw_five_stars(x,4*y_int,side,color::white);
	draw_six_stars(x,5*y_int,side,color::white);
	draw_five_stars(x,6*y_int,side,color::white);
	draw_six_stars(x,7*y_int,side,color::white);
	draw_five_stars(x,8*y_int,side,color::white);
	draw_six_stars(x,9*y_int,side,color::white);

	double const d = 2.0*radius_of_star(side);
	print(d); // helps me find the diameter of the star
}

void main(){
	draw_stars();
	draw_service_flag();
	draw_us_flag();
}
