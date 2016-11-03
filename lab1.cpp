#include "library.h"

// A. How long is a Year!
int seconds_in_year(int const seconds, int const minutes, int const hours, int const days)
{
int const seconds_in_year = seconds*minutes*hours*days;
return (seconds_in_year);
}

void how_long_is_a_year()
{
	int const seconds = 60;
	int const minutes = 60;
	int const hours = 24;
	int const days = 365;

	int const seconds_in_a_year = seconds_in_year(seconds,minutes,hours,days);
	print(" A year is ");
	print(seconds_in_a_year);
	print(" seconds long!");
}

// B. Drawing a pentagram or a six point star!
void draw_star(int const x, int const y, int const side, int const color)
{
	set_pen_color(color);
	set_heading_degrees(90);
	move_to(x,y);
	start_shape();
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144);  
	draw_distance(side); note_position();
	turn_right_by_degrees(144); 
	draw_distance(side);  note_position();
//	fill_shape();

}

void draw_triangle1(int const x1, int const y1, int const side,int const color)
{  
   set_pen_color(color);
   set_heading_degrees(90);
   move_to(x1,y1);
   draw_distance(side);
   turn_right_by_degrees(120);
   draw_distance(side);
   turn_right_by_degrees(120);
   draw_distance(side); 
}



void draw_triangle2(int const x2, int const y2, int const side,int const color)
{
   set_pen_color(color);
   set_heading_degrees(150);
   move_to(x2,y2);
   draw_distance(side);
   turn_right_by_degrees(120);
   draw_distance(side);
   turn_right_by_degrees(120);
   draw_distance(side); 
}

void draw_shapes()
{
// Pentagram:
	int const x = 50;
 	int const y = 100;
	int const side = 200;
	make_window(600,300);
	set_pen_width(3);
	set_pen_color(color::blue);
	draw_star(x,y,side,color::black);

// Star with even number of points:
	// first triangle
	int const x1 = 300;
	int const y1 = 100;
	draw_triangle1(x1,y1,side,color::blue);

	// second triangle
	int const x2 = 400;
	int const y2 = 50;
	draw_triangle2(x2,y2,side,color::blue);

}

// C. stick men round heads
void draw_chest(int const chest_size)
{
   set_heading_degrees(90);
   move_to(150,155);
   draw_distance(chest_size);
   turn_right_by_degrees(120);
   draw_distance(chest_size);
   turn_right_by_degrees(120);
   draw_distance(chest_size); 
}

void draw_stick_men(int const diameter,int const side)
{
	//face
	move_to(200,100);
	set_pen_width(diameter);
	draw_point();
	set_pen_width(diameter-8);
	set_pen_color(color::white);
	draw_point();
	
	//neck
	move_to(200,100+(diameter/2));
	set_pen_width(5);
	set_pen_color(color::black);
	set_heading_degrees(180);
	draw_distance(15);

	//chest
	int const chest_size = 100;
	draw_chest(chest_size);

	// side
	move_to(200,240);
	set_heading_degrees(180);
	draw_distance(60);
	
	//rigth leg
	move_to(200,300);
	set_heading_degrees(145);
	draw_distance(60);
	set_heading_degrees(180);
	draw_distance(60);
	set_heading_degrees(90);
	draw_distance(20);

	//left leg
	move_to(200,300);
	set_heading_degrees(215);
	draw_distance(60);
	set_heading_degrees(180);
	draw_distance(60);
	set_heading_degrees(270);
	draw_distance(20);

	// right arm
	move_to(250,155);
	set_heading_degrees(180);
	draw_distance(100);

	// left arm
	move_to(150,155);
	set_heading_degrees(300);
	draw_distance(50);
	set_heading_degrees(340);
	draw_distance(50);
}


void main()
{
	//Part A
	how_long_is_a_year();

	//Part B
	draw_shapes();

	//Part C
	int const diameter = 80;
	int const side = 100;
	make_window(400,500);
	draw_stick_men(diameter,side);
}
