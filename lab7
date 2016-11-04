#include "library.h"

double const g = 32.174;
double const pi = acos(-1.0);

void line_and_turn(int const N, double const side, int const angle)
{
	if (N > 0)
	{
		draw_distance(side); note_position();
		turn_right_by_degrees(angle);
		line_and_turn(N - 1, side, angle);
	}
}
	
void draw_circle(double const x, double const y, double const radius)
{
	double const circumference = 2 * radius * 3.14;
	double const side = circumference / 360;
	set_heading_degrees(90);
	move_to(x, y - radius);
	start_shape();
	line_and_turn(360, side, 1);
	fill_shape();
	set_pen_color(0, 0, 0);
}

void rect(int const Xpos, int const Ypos, int const width, int const height)
{
	start_shape();
	move_to(Xpos, Ypos); note_position();
	set_heading_degrees(90);
	draw_distance(width); note_position();
	turn_right_by_degrees(90);
	draw_distance(height); note_position();
	turn_right_by_degrees(90);
	draw_distance(width); note_position();
	turn_right_by_degrees(90);
	draw_distance(height); note_position();
	fill_shape();
}


//-------------Flag Code Imported -----------------------
//White background for the flag
void draw_flag_background(int const Xpos, int const Ypos, int const width, int const height, int const color)
{
	set_pen_color(color);
	rect(Xpos, Ypos, width, height);
}

//initial stripe
void draw_rectangle(int const Xpos, double const Ypos, double const width, int const color)
{
	double const height = width*(10 / 247.0);
	set_pen_color(color);
	rect(Xpos, Ypos, width, height);
}

//function that draws read stripes
void draw_stripes(int const Xpos, int const Ypos, double const width, int const color)
{
	double const y1 = width*(10 / 247.0);//height
	draw_rectangle(Xpos, Ypos + 0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*2.0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*4.0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*6.0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*8.0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*10.0, width, color::dark_red);
	draw_rectangle(Xpos, Ypos + y1*12.0, width, color::dark_red);
}


void draw_star_background(int const Xpos, int const Ypos, double const width, int const color)
{
	int const width_backg = width*(15 / 38.0);
	int const height = width*(70 / 247.0);
	set_pen_color(color);
	rect(Xpos, Ypos, width_backg, height);

}

// I decided to draw points because it looked better
void draw_star(double const Xpos, double const Ypos, double const side, int const color)
{
	set_pen_color(color);
	move_to(Xpos, Ypos);
	set_pen_width(side * 2);
	draw_point();
}

void draw_six_stars(int const Xpos, double const x, double const y_int, double const side, int const color)

{
	draw_star(Xpos + x*(30 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(78 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(126 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(174 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(222 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(270 / 760.0), y_int, side, color);
}

void draw_five_stars(int const Xpos, double const x, double const y_int, double const side, int const color)

{
	draw_star(Xpos + x*(54 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(102 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(150 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(198 / 760.0), y_int, side, color);
	draw_star(Xpos + x*(246 / 760.0), y_int, side, color);
}

void draw_all_stars(int const Xpos, int const Ypos, int const width)
{
	double const side = width*(9.15 / 760.0);
	double const y_int = width*(7 / 247.0);

	draw_six_stars(Xpos, width, Ypos + y_int, side, color::white);
	draw_five_stars(Xpos, width, Ypos + 2 * y_int, side, color::white);
	draw_six_stars(Xpos, width, Ypos + 3 * y_int, side, color::white);
	draw_five_stars(Xpos, width, Ypos + 4 * y_int, side, color::white);
	draw_six_stars(Xpos, width, Ypos + 5 * y_int, side, color::white);
	draw_five_stars(Xpos, width, Ypos + 6 * y_int, side, color::white);
	draw_six_stars(Xpos, width, Ypos + 7 * y_int, side, color::white);
	draw_five_stars(Xpos, width, Ypos + 8 * y_int, side, color::white);
	draw_six_stars(Xpos, width, Ypos + 9 * y_int, side, color::white);

}

void draw_flag(int const Xpos, int const Ypos, int const size)
{
	double const width = size; //initially x = 760
	double const height = width*(400 / 760.0);
	draw_flag_background(Xpos, Ypos, width, height, color::white);
	draw_stripes(Xpos, Ypos, width, color::dark_red);
	draw_star_background(Xpos, Ypos, width, color::dark_blue);
	draw_all_stars(Xpos, Ypos, width);
}

//---------------------------------------------------------------

//-------START OF LAB 7!!!!--------------------------------------
void draw_sky(int const time)
{
	
	if (time >= 6 && time < 8) // sunrise
	{
		
		set_pen_color_int(58, 73, 115);
		rect(0, 0, 980, 300);
		set_pen_color_int(108, 82, 108);
		rect(0, 300, 980, 240);
		set_pen_color_int(251, 93, 67);
		rect(0, 540, 980, 180);
	}
	else if (time >= 8 && time < 10) // early morning
	{
		
		set_pen_color_int(118, 223, 255);
		rect(0, 0, 980, 300);
		set_pen_color_int(150, 223, 255);
		rect(0, 300, 980, 240);
		set_pen_color_int(170, 223, 255);
		rect(0, 540, 980, 180);
	}
	else if (time >= 10 && time < 12) // late morning
	{
		
		set_pen_color_int(140, 223, 255);
		rect(0, 0, 980, 300);
		set_pen_color_int(120, 223, 255);
		rect(0, 300, 980, 240);
		set_pen_color_int(100, 223, 255);
		rect(0, 540, 980, 180);
	}
	else if (time >= 12 && time <14) // early after noon
	{
		
		set_pen_color_int(120, 223, 255);
		rect(0, 0, 980, 300);
		set_pen_color_int(100, 223, 255);
		rect(0, 300, 980, 240);
		set_pen_color_int(90, 213, 255);
		rect(0, 540, 980, 180);
	}
	else if (time >= 14 && time <16) // afternoon
	{
		
		set_pen_color_int(100, 213, 255);
		rect(0, 0, 980, 300);
		set_pen_color_int(90, 203, 255);
		rect(0, 300, 980, 240);
		set_pen_color_int(70, 203, 255);
		rect(0, 540, 980, 180);
	}
	else if (time >= 16 && time <18) //dusk, sun sets on the west
	{
		
		set_pen_color_int(70, 203, 255);
		rect(0, 0, 980, 300);
		set_pen_color_int(65, 140, 200);
		rect(0, 300, 980, 240);
		set_pen_color_int(40, 100, 150);
		rect(0, 540, 980, 180);
	}
	else if (time >= 18 && time <20) // moon rise
	{
		
		set_pen_color_int(70, 90, 180);
		rect(0, 0, 980, 300);
		set_pen_color_int(50, 80, 150);
		rect(0, 300, 980, 240);
		set_pen_color_int(40, 73, 125);
		rect(0, 540, 980, 180);
	}
	else // nightime
	{
		
		set_pen_color_int(1, 23, 81);
		rect(0, 0, 980, 300);
		set_pen_color_int(6, 42, 98);
		rect(0, 300, 980, 240);
		set_pen_color_int(36, 74, 124);
		rect(0, 540, 980, 180);
	}
	set_pen_color_int(0, 0, 0);
}

void draw_sun_and_moon(int const time)
{
	set_pen_color_int(253, 240, 14); // sun
	if (time >= 6 && time < 8) draw_circle(480, 630, 75);
	else if (time >= 8 && time < 10) draw_circle(480, 420, 75);
	else if (time >= 10 && time < 12) draw_circle(480, 150, 75);

	set_pen_color_int(200, 200, 200); // moon
	if (time >= 18 && time < 20) draw_circle(480, 630, 75);
	else if (time >= 20 && time < 22) draw_circle(480, 420, 75);
	else if (time >= 22 && time < 24) draw_circle(480, 150, 75);
}

void draw_points(int const n)
{
	if (n > 0) //drawing stars using recursion
	{
		move_to(random_in_range(0, 980), random_in_range(0, 720));
		set_pen_color(random_in_range(240, 255), random_in_range(240, 255), random_in_range(240, 255));
		set_pen_width(random_in_range(1, 5));
		draw_point(); 
		draw_points(n - 1);
	}
}

void draw_stars(int const time) 
{
	int const n = random_in_range(160, 240); //number of stars in the sky
	if (time>=18 && time<=24) draw_points(n);
	else if (time >= 1 && time <= 6) draw_points(n);
}

void draw_cloud(int const n)
{
	if (n > 0 && n <= 6) //recursive function that draws clouds
	{
		int const x = random_in_range(0, 820);
		int const y = random_in_range(0, 520);
		double const size = 160;
		set_pen_color_int(255, 255, 255);
		rect(x, y, 160, 40);
		rect(x + 40, y - 10, 80, 60);
		draw_cloud(n - 1);
	}
}

void draw_clouds(int const time)
{
	if (time >= 1 && time<6) draw_cloud(random_in_range(0, 6));
	else if (time >= 12 && time<18) draw_cloud(random_in_range(0, 6));
	else if (time == 24) draw_cloud(random_in_range(0, 6));
	else draw_cloud(random_in_range(0, 8));
}

void draw_grass(int const time)
{
	int const n = random_in_range(45, 60);
	if (time >= 6 && time<10) set_pen_color_int(184, 203, 100);
	else if (time >= 10 && time<14) set_pen_color_int(164, 203, 110);
	else if (time >= 14 && time<18) set_pen_color_int(124, 203, 130);
	else set_pen_color_int(60, 100, 70);
	rect(0, 720 - n, 980, n);
}

void draw_road(int const time, int const n)
{
	if (time >= 6 && time<10) set_pen_color_int(122, 122, 122);
	else if (time >= 10 && time<14) set_pen_color_int(160, 160, 160);
	else if (time >= 14 && time<18) set_pen_color_int(140, 140, 140);
	else set_pen_color_int(70, 70, 70);
	rect(-1, 720 - n, 981, n);
}

void draw_background(int const time)
{
	draw_sky(time);
	draw_stars(time);
	draw_sun_and_moon(time);
	draw_clouds(time);
	draw_grass(time);
}

void window(int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue)
{
	int const random_selection = random_in_range(0, 15);//this ranve states the probability of a window to have curtains
	set_pen_color_int(red, green, blue);
	rect(Xpos, Ypos, width, height);
	if (random_selection <= 2) 
	{	// this function draws the curtains
		set_pen_color_int(225, 25, 41);
		rect(Xpos, Ypos, width / 3, height);
		rect(Xpos + 2 * (width / 3)+2, Ypos, width / 3, height);
	}
}

void row_of_windows(int const rows, int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue)
{	// this function draws a row of windows
	int N = rows;
	int separation = width*(15/64.0);
	while (N > 0)
	{
		window( Xpos + separation, Ypos, width, height, red , green, blue);
		separation = separation + width + (width/3);
		N = N - 1;
	}
}

void block_of_windows(int const floors, int const rows, int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue)
{
	int N = floors;
	int separation = height/2;
	while (N > 0)
	{
		row_of_windows(rows, Xpos, Ypos + separation, width, height, red, green, blue);
		separation = separation + 2*height;
		N = N - 1;
	}
}

void draw_block(int const floors, int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue)
{
	if (width >= 25 && width < 50) 
	{
		int const rows = 1;
		int const win_width = (width / (rows))*(16 / 22.0); // this variable converts the total width of the block of windows to an individual width
		int const win_height = height / (2 * floors); // the same is done for the height
		block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);
	}
	else if (width >= 50 && width < 100)
	{
		int const rows = random_in_range(1, 2);
		int const win_width = (width / (rows))*(16 / 22.0);
		int const win_height = height / (2 * floors);
		block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);
	}
	else if (width >= 100 && width < 150)
	{
		int const rows = random_in_range(2, 3);
		int const win_width = (width / (rows))*(16 / 22.0);
		int const win_height = height / (2 * floors);
		block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);
	}
	else if (width >= 150 && width < 250)
	{
		int const rows = random_in_range(3, 5);
		int const win_width = (width / (rows))*(16 / 22.0);
		int const win_height = height / (2 * floors);
		block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);

	}
	else
	{
		int const rows = random_in_range(4, 7);
		if (rows < 5)
		{
			int const win_width = (width / (rows))*(8 / 11.0);
			int const win_height = height / (2 * floors);
			block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);
		}
		else
		{
			int const win_width = (width / (rows))*(3 / 4.0);
			int const win_height = height / (2 * floors);
			block_of_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, red, green, blue);
		}

	}
}

void row_of_windows_and_door(int const rows, int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue, int const door_random)
{	// this function randomly creates a door instead of a window
	int N = rows;
	int separation = width*(15 / 64.0);
	while (N > 0)
	{
		if (N == door_random)
		{
			int const door_color = random_in_range(1, 2);
			if (door_color == 1) set_pen_color_int(random_in_range(140, 200), random_in_range(180, 240), random_in_range(80, 140));
			else if (door_color == 2) set_pen_color_int(random_in_range(180, 240), random_in_range(0, 60), random_in_range(0, 60));
			rect(Xpos + separation, Ypos, width, height*(3.1/2.0));
		}
		else window(Xpos + separation, Ypos, width, height, red, green, blue);
		separation = separation + width + (width / 3);
		N = N - 1;
	}
}

void block_of_house_windows(int const floors, int const rows, int const Xpos, int const Ypos, int const width, int const height, int const red, int const green, int const blue)
{ // this function makes the last row of the couse to have a door
	int N = floors;
	int separation = height / 2;
	while (N > 0)
	{
		if (N == 1)
		{
			int const door_random = random_in_range(1, rows);
			row_of_windows_and_door(rows, Xpos, Ypos + separation, width, height, red, green, blue,door_random);

		}
		else
		{
			row_of_windows(rows, Xpos, Ypos + separation, width, height, red, green, blue);
			separation = separation + 2 * height;
		}
		N = N - 1;
	}
}

void draw_house(int const floors, int const Xpos, int const Ypos, int const width, int const height)
{	// draws the house size depending on the width
	int const color = random_in_range(100, 200);
	if (width < 150)
	{
		int const rows = 2;
		int const win_width = (width / (rows))*(16 / 22.0);
		int const win_height = height / (2 * floors);
		block_of_house_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, color, color, color);
	}
	else
	{
		int const rows = 3;
		int const win_width = (width / (rows))*(16 / 22.0);
		int const win_height = height / (2 * floors);
		block_of_house_windows(floors, rows, Xpos, Ypos - height, win_width, win_height, color, color, color);
	}
}

void roof(int const Xpos, int const Ypos, int const width, int const height)
{
	int const random = random_in_range(1, 2); // this int determines the color of the roof
	set_pen_color_int(148, 108, 73);
	int const extra_width = 15;
	int const tri_hi = 40;
	int const hype = sqrt((tri_hi*tri_hi) + (((width + 2*extra_width) / 2.0)*((width + 2*extra_width) / 2.0)));
	double const angle = tan(tri_hi / ((width + 2*extra_width) / 2.0))*(180 / acos(-1.0));
	if (random == 1)
	{
		set_pen_color_int(148, 108, 73);
		move_to(Xpos - extra_width, Ypos - height);
		start_shape();
		set_heading_degrees(90);
		turn_left_by_degrees(angle);
		move_distance(hype); note_position();
		turn_right_by_degrees(180 - 2 * (90 - angle)+1);
		move_distance(hype+6); note_position();
		turn_right_by_degrees(180 - angle);
		move_distance(width + 2 * extra_width); note_position();
		fill_shape();
	}
	else
	{
		set_pen_color_int(154, 155, 53);
		move_to(Xpos - extra_width, Ypos - height);
		start_shape();
		set_heading_degrees(90);
		turn_left_by_degrees(angle);
		draw_distance(hype); note_position();
		turn_right_by_degrees(180 - 2 * (90 - angle));
		draw_distance(hype); note_position();
		turn_right_by_degrees(180 - angle);
		draw_distance(width + 2 * extra_width); note_position();
		fill_shape();
	}
}

void chimney(int const Xpos, int const Ypos, int const width, int const height)
{
	set_pen_color_int(100, 102, 101);
	int const chim_width = width / 10;
	int const chim_height = height / 4;
	rect(Xpos+5, Ypos - height - chim_height, chim_width, chim_height);
}

void flag(int const Xpos, int const Ypos, int const width, int const height)
{
	set_pen_color_int(141, 70, 19);
	rect(Xpos + 5, Ypos+5 - height, 5, 10);
	turn_right_by_degrees(90);
	move_distance(2);
	set_heading_degrees(0);
	set_pen_width(3);
	draw_distance(150);
	int const X = get_x_position();
	int const Y = get_y_position();
	draw_flag(X+3, Y, 125);
}

void watch_tower(int const Xpos, int const Ypos, int const width, int const height)
{
	int const separation = 27;
	int const color = random_in_range(50, 125);
	set_pen_color_int(color, color, color);
	rect(Xpos - separation, Ypos - height * 2, width / 3, height * 2);
	int const block_width = (width / 3) / 5;
	rect(Xpos - separation, Ypos - height * 2 - block_width, block_width, block_width);
	rect(Xpos - separation + 2 * block_width, Ypos - height * 2 - block_width, block_width, block_width);
	rect(Xpos - separation + (width / 3) - block_width, Ypos - height * 2 - block_width, block_width, block_width);
	set_pen_color(color::black);
	rect(Xpos - separation + block_width, Ypos - height * 1.7 - block_width, block_width, block_width * 3);
	rect(Xpos - separation + (width / 3) - 2 * block_width, Ypos - height * 1.7 - block_width, block_width, block_width * 3);
}

void building_condition(int const Xpos, int const Ypos, int const width, int const height)
{ //this function draws a building according to the parameters of width and height that are giving
	int const red = random_in_range(0, 50);
	int const green = random_in_range(0, 50);
	int const blue = random_in_range(0, 50);
	set_pen_color_int(random_in_range(50, 200), random_in_range(50, 100), random_in_range(50, 200));
	rect(Xpos, Ypos - height, width, height);
	if (height >= 100 && height < 150) // 1 floor house
	{
		int const randi = random_in_range(1, 3); // the int randi decides what other factor will the house have
		if (randi == 1)
		{
			chimney(Xpos, Ypos, width, height);
			roof(Xpos, Ypos, width, height);
			int const floors = 1;
			draw_house(floors, Xpos, Ypos, width, height);
		}
		else if (randi == 2)
		{
			roof(Xpos, Ypos, width, height);
			flag(Xpos, Ypos, width, height);
			int const floors = 1;
			draw_house(floors, Xpos, Ypos, width, height);
		}
		else
		{
			
			watch_tower(Xpos, Ypos, width, height);
			set_pen_color_int(random_in_range(50, 200), random_in_range(50, 100), random_in_range(50, 200));
			rect(Xpos, Ypos - height, width, height);
			roof(Xpos, Ypos, width, height);
			int const floors = 1;
			draw_house(floors, Xpos, Ypos, width, height);
		}
	}
	else if (height >= 150 && height < 250) // 2 floor house
	{	
		int const randi = random_in_range(1, 3);
		if (randi == 1)
		{
			chimney(Xpos, Ypos, width, height);
			roof(Xpos, Ypos, width, height);
			int const floors = 2;
			draw_house(floors, Xpos, Ypos, width, height);
		}
		else if (randi == 2)
		{
			roof(Xpos, Ypos, width, height);
			flag(Xpos, Ypos, width, height);
			int const floors = 2;
			draw_house(floors, Xpos, Ypos, width, height);
		}
		else
		{
			watch_tower(Xpos, Ypos, width, height);
			set_pen_color_int(random_in_range(50, 200), random_in_range(50, 100), random_in_range(50, 200));
			rect(Xpos, Ypos - height, width, height);
			roof(Xpos, Ypos, width, height);
			int const floors = 2;
			draw_house(floors, Xpos, Ypos, width, height);
		}
	}
	else if (height >= 250 && height < 300)
	{
		int const floors = random_in_range(3, 4);
		draw_block(floors, Xpos, Ypos, width, height, red, green, blue);
	}
	else if (height >= 300 && height < 400)
	{
		int const floors = random_in_range(4, 5);
		draw_block(floors, Xpos, Ypos, width, height, red, green, blue);
	}
	else if (height >= 400 && height < 500)
	{
		int const floors = random_in_range(5, 6);
		draw_block(floors, Xpos, Ypos, width, height, red, green, blue);
	}
	else
	{
		int const floors = random_in_range(6, 7);
		draw_block(floors, Xpos, Ypos, width, height, red, green, blue);
	}
}

void draw_building(int const Xpos, int const Ypos, int const width)
{ // this function, depending on the width of the building, determines the height
	if (width > 150) 
	{
		int const height = random_in_range(100, 620);
		building_condition(Xpos, Ypos, width, height);
	}
	else// houses with a width less than 150 looked horrible.
	{   // in order to prevent houses from being drawn there is a minimum height of 250
		int const height = random_in_range(250, 620);
		building_condition(Xpos, Ypos, width, height);
	}
}

void city_sceene(int const Xpos,int const Ypos)
{ // this function draws random buildings
	int separate = random_in_range(55, 70);
	int separator = separate;
	// I needed to functions to measure the separation of the buildings
	int width = random_in_range(25, 350);
	int Xdist = 980-separate- Xpos; // establishes the maximum distance left
	while (Xdist > width)
	{
		draw_building(Xpos+separator, Ypos, width);
		separator = separator + width+separate;
		Xdist = Xdist - width - separate;
		if (Xdist<250) width = random_in_range(25, 120);
		else width = random_in_range(25, 300);
		separate = random_in_range(30, 70);
	}
}

void draw_tree(int const Xpos,int const Ypos)
{	//draws tbe tree's trunk
	int const stem_height = random_in_range(175, 250);
	int const stem_width = random_in_range(10, 20);
	set_pen_color_int(141, 70, 19);
	rect(Xpos, Ypos-stem_height, stem_width, stem_height);
	// dsets the conditions of the trees
	int const leave_width = random_in_range(75, 160);
	int const leave_height = (2*stem_height)/2;
	int const X_lim_left = Xpos - leave_width / 2;
	int const X_lim_right = Xpos + leave_width / 2;
	int const y_lim_up = Ypos - stem_height - leave_height / 2;
	int const y_lim_down = Ypos - stem_height + leave_height / 2;
	int num_leaves = 1000;
	int red;
	int green;
	int blue;
	set_pen_width(12);
	// draws the trees
	while (num_leaves > 0)
	{
		red = random_in_range(35, 100);
		green = random_in_range(150, 250);
		blue = random_in_range(10, 50);
		set_pen_color_int(red, green, blue);
		move_to(random_in_range(X_lim_left, X_lim_right), random_in_range(y_lim_up, y_lim_down));
		draw_point();
		num_leaves = num_leaves - 1;
	}
}

void draw_trees(int const Xpos, int const Ypos)
{	// draws trees recursively
	int num_trees = random_in_range(0, 5); // dictates the total number of trees
	int Xrange;
	while (num_trees > 0)
	{
		Xrange = random_in_range(80, 900);
		draw_tree(Xrange, Ypos);
		num_trees = num_trees - 1;
	}
}

void draw_bush(int const Xpos, int const Ypos)
{ // bushes are the same as trees except that they dont have stems
	int const leave_width = random_in_range(25, 75);
	int const leave_height = random_in_range(25, 75);
	int const X_lim_left = Xpos - leave_width;
	int const X_lim_right = Xpos + leave_width;
	int const y_lim_up = Ypos - leave_height;
	int const y_lim_down = Ypos ;
	int num_leaves = 150;
	int red;
	int green;
	int blue;
	set_pen_width(14);
	while (num_leaves > 0)
	{
		red = random_in_range(35, 100);
		green = random_in_range(150, 250);
		blue = random_in_range(10, 50);
		set_pen_color_int(red, green, blue);
		move_to(random_in_range(X_lim_left, X_lim_right), random_in_range(y_lim_up, y_lim_down));
		draw_point();
		num_leaves = num_leaves - 1;
	}
}

void draw_bushes(int const Xpos, int const Ypos)
{
	int num_bushes = random_in_range(0, 5);// determins the maximum number of trees
	int Xrange;
	while (num_bushes > 0)
	{
		Xrange = random_in_range(55, 930);
		draw_bush(Xrange, Ypos);
		num_bushes = num_bushes - 1;
	}
}


//-------------Lab4 Imported code-----------------------

void draw_cannon_body(double const x, double const y, double const radius, int const a, int const size)
{
	// constants
	double const w1 = 70 * size / 100; // longest width of the cannon
	double const w2 = 60 * size / 100; // smallest width of the cannon
	double const L1 = 70 * size / 100; // smaller length of the cannon barrel
	double const L2 = 210 * size / 100; // longer length of the cannon barrel
	double const b = a*sin((w1 - w2) / (2.0*(L1 + L2))); //When the cannon is aimed at angle a, the heading for the bottom line is (a-b)
	set_pen_width(4);
	set_pen_color_int(54, 50, 47);
	// main build
	start_shape();
	move_to(x, y); note_position();
	set_heading_degrees(a - b);
	move_distance(radius); note_position();
	draw_distance(L2 - radius); note_position();
	turn_left_by_degrees(90 - b);
	draw_distance(w2); note_position();
	turn_left_by_degrees(90 - b);
	draw_distance(L1 + L2); note_position();
	turn_left_by_degrees(90 + b);
	draw_distance(w1); note_position();
	turn_left_by_degrees(90 + b);
	draw_distance(L1 - radius); note_position();
	fill_shape();
	set_pen_color(0, 0, 0);
}

void draw_cannon(int const x, int const y, int const a, double const radius, int const size)
{
	draw_cannon_body(x, y, radius, a, size);
	set_pen_color_int(102, 50, 0);
	draw_circle(x, y, radius);
}

double return_fire_x(int const x, int const y, double a, double const radius, int const size)
{	// X postition of the front of the barell of the cannon
	double const w1 = 70 * size / 100; // longest width of the cannon
	double const w2 = 60 * size / 100; // smallest width of the cannon
	double const L1 = 100 * size / 100; // smaller length of the cannon barrel
	double const L2 = 210 * size / 100; // longer length of the cannon barrel
	double const b = a*sin((w1 - w2) / (2.0*(L1 + L2))); //When the cannon is aimed at angle a, the heading for the bottom line is (a-b)
	move_to(x, y);
	set_heading_degrees(a - b);
	move_distance(radius);
	draw_distance(L2 - radius);
	turn_left_by_degrees(90 - b);
	draw_distance(w2 / 2.0);
	set_heading_degrees(a);
	return(get_x_position());
}

double return_fire_y(int const x, int const y, double a, double const radius, int const size)
{	// Y postition of the front of the barell of the cannon
	double const w1 = 70 * size / 100; // longest width of the cannon
	double const w2 = 60 * size / 100; // smallest width of the cannon
	double const L1 = 100 * size / 100; // smaller length of the cannon barrel
	double const L2 = 210 * size / 100; // longer length of the cannon barrel
	double const b = a*sin((w1 - w2) / (2.0*(L1 + L2))); //When the cannon is aimed at angle a, the heading for the bottom line is (a-b)
	move_to(x, y);
	set_heading_degrees(a - b);
	move_distance(radius);
	draw_distance(L2 - radius);
	turn_left_by_degrees(90 - b);
	draw_distance(w2 / 2.0);
	set_heading_degrees(a);
	return(get_y_position());
}

void connacon(int const Xpos, int const Ypos)
{	// draws connacon in the graphical window
	int const window_x = Xpos / 2.0;
	int const window_y = Ypos - 680;
	move_to(window_x, window_y);
	set_pen_color(color::dark_red);
	set_font_size(40);
	write_string("CONNACON", direction::center);
}

void you_won(int const window_x, int const window_y)
{	// lets the player know they won
	move_to(window_x / 2.0, window_y - 630.0);
	set_pen_color(color::dark_red);
	set_font_size(40);
	write_string("Mission Accomplished.", direction::center);
}

//made objects
//this spaceship code is not imported
void random_spaceship(int const Xpos, int const Ypos)
{
	int const radius = 50;
	draw_circle(Xpos, Ypos, radius + 2);
	set_pen_color_int(202, 254, 255);
	draw_circle(Xpos, Ypos, radius);
	set_heading_degrees(180);
	move_distance(radius + 2);
	set_heading_degrees(270);
	int const width = (radius * 4) / 3;
	move_distance(width);
	int const X = get_x_position();
	int const Y = get_y_position();
	set_pen_color_int(128, 128, 128);
	rect(X, Y, width * 2, radius / 2);
	row_of_windows(4, X, Y + 3, 25, (radius / 2) - 6, 252, 182, 21);
}

void random_spaceship_blown(int const Xpos, int const Ypos)
{	// this function draws the spaceship as it blows up
	int num = 30; // determines the number of times the spaceship is redrawn
	while (num > 0)
	{
		if (num == 10 && num == 8 && num == 6 && num == 4 && num == 2)
		{	//yellow spaceship
			int const radius = 50;
			draw_circle(Xpos, Ypos, radius + 2);
			set_pen_color_int(255, 240, 14);
			draw_circle(Xpos, Ypos, radius);
			set_heading_degrees(180);
			move_distance(radius + 2);
			set_heading_degrees(270);
			int const width = (radius * 4) / 3;
			move_distance(width);
			int const X = get_x_position();
			int const Y = get_y_position();
			set_pen_color_int(128, 128, 128);
			rect(X, Y, width * 2, radius / 2);
			set_pen_color_int(255, 240, 14);
			row_of_windows(4, X, Y + 3, 25, (radius / 2) - 6, 252, 182, 21);
		}
		else
		{	//red spaceship
			int const radius = 50;
			draw_circle(Xpos, Ypos, radius + 2);
			set_pen_color_int(173, 16, 0);
			draw_circle(Xpos, Ypos, radius);
			set_heading_degrees(180);
			move_distance(radius + 2);
			set_heading_degrees(270);
			int const width = (radius * 4) / 3;
			move_distance(width);
			int const X = get_x_position();
			int const Y = get_y_position();
			set_pen_color_int(128, 128, 128);
			rect(X, Y, width * 2, radius / 2);
			set_pen_color_int(255, 240, 14);
			row_of_windows(4, X, Y + 3, 25, (radius / 2) - 6, 252, 182, 21);
		}
		num = num - 1;
	}
}

// Calculations
double calc_height(double const v, double const t, double const a)// works
{
	double const h = (v*t*sin(a*pi / 180.0) - (0.5*g*(t*t)));
	return(h);
}

double calc_distance(double const v, double const t, double const a)// works
{
	double const d = (v*t*cos(a*pi / 180.0));
	return(d);
}

void print_you_won()
{
	new_line(); print(" Mission accomplished.");
}

void trajectory_loop(double const xt, double const yt, int const v, double const t, double const angle, int const window_x, int const window_y, double const hq_win_x, double const hq_win_y)
{	// this function determines the trajectory of the cannon ball as it flies through the air
	if ((yt - calc_height(v, t, angle) / 1.5) < (window_y - 10))
	{
		wait(0.01);
		draw_to(xt + calc_distance(v, t, angle), (yt - calc_height(v, t, angle) / 1.5)); set_pen_width(5);
		set_pen_color(1 - (t / 13.0), 0, 0 + (t / 13.0));
		// this if statement is the stoping criteria for the cannon hitting the object
		if (xt + calc_distance(v, t, angle) >= hq_win_x - 2 && xt + calc_distance(v, t, angle) <= hq_win_x + 132 && yt - calc_height(v, t, angle) / 1.5 >= hq_win_y && yt - calc_height(v, t, angle) / 1.5 <= hq_win_y + 40)
		{
			new_line(); new_line();
			random_spaceship_blown(hq_win_x + 66, hq_win_y); you_won(window_x, window_y);
			print(" If you still have amo left, try hitting him again..."); new_line();
			print(" With these aliens you never know..."); new_line(); new_line();
			print(" Otherwise feel free to try the mission at a higher difficulty lvl."); new_line(); new_line();
		}
		else trajectory_loop(xt, yt, v, t + 0.1, angle, window_x, window_y, hq_win_x, hq_win_y);
	}
}

void trajectory(double const xt, double const yt, int const v, double const t, double const a, int const window_x, int const window_y, double const hq_win_x, double const hq_win_y)
{
	move_to(xt, yt); // starting position of the arc
	trajectory_loop(xt, yt, v, t, a, window_x, window_y, hq_win_x, hq_win_y);
}

double ask_angle()
{
	print(" Imput desired angle: ");
	double const a = (read_double()); new_line();
	if (a>=50)return (a);
	else
	{
		cout << "This angle is too small try something 50* or more" << endl;
		ask_angle();
	}
}

double ask_velocity()
{
	print(" Imput desired velocity: ");
	double const v = (read_double()); new_line();
	return (v);
}

void instructions()
{
	print(" Instructions:"); new_line(); new_line();
	print(" The goal of this mission is to hit the allien terorizing the town."); new_line();
	print(" A recent attack managed to damadge but not destroy the ship."); new_line();
	print(" Your job is to hit that intruder with all you got!!!"); new_line(); new_line();
	print(" You can adjusted your cannon in two different ways:"); new_line(); new_line();
	print("     1) You can imput the initial angle your cannon will fire"); new_line();
	print("     2) You can imput the strength your cannon's projectiles (initial velocity)"); new_line(); new_line();
	print(" Difficulty levels:"); new_line();
	print("   - There are four dificulty levels each marked by a number"); new_line();
	print("   - You will be asked to imput the difficulty level number"); new_line(); new_line();
	print("      1 is the easiest difficulty level, allowing user to have five "); new_line();
	print("        cannon balls"); new_line();
	print("      2 is the moderate difficulty level, allowing user to have three"); new_line();
	print("        cannon balls"); new_line();
	print("      3 is the semi-hard difficulty level, allowing user to have one"); new_line();
	print("        cannon ball"); new_line();
	print("      4 is the hardest difficulty level, allowing user to have one cannon ball"); new_line();
	print("        and a random angle is assigned"); new_line(); new_line();
	print(" Good luck on your mission."); new_line(); new_line();
}

void cannon_game_start()
{
	print(" Welcome to Cannacon"); new_line(); new_line();
	cout << " Sorry to disturb you from your usual grading... but the city need your help!" << endl; new_line();
	cout << " To disable this message for grading purposes delete alien_game(); from main..." << endl; new_line();
	print(" To read the instructions on how to play enter either 1 for yes or 0 for no: "); int const instruc = read_int(); new_line();
	if (instruc == 1) instructions();
	else if (instruc == 0)
	{
		print(" Good luck!"); new_line(); new_line();
	}
	else
	{
		print(" Opps! you imputed something other than 1 or 0... "); new_line();
		print(" Im assumming you want the instructions "); new_line();  new_line();
		instructions();
	}
}

int cannacon_difficulty_lvl()
{
	print(" Set difficulty level: "); int const diff = read_int(); new_line(); print(" Thank you!"); new_line(); new_line();
	// converts the difficulty level into number of tries the player has
	if (diff == 1) return(5);
	if (diff == 2) return(3);
	if (diff == 3) return(1);
	if (diff == 4) return(-1);
	else
	{
		print(" You imputed something other than 1, 2, 3 or 4!"); new_line();
		print(" please try again"); new_line(); new_line();
		cannacon_difficulty_lvl();
	}
}

void number_of_tries(double const xt, double const yt, double const a, int const difficulty, int const window_x, int const window_y, double const hq_win_x, double const hq_win_y)
{	// recursive function that tells the player how many tries they have
	// it also draws the trajectory of the cannon ball
	if (difficulty > 1)
	{
		show_console();
		double const v = ask_velocity();
		hide_console();
		trajectory(xt, yt, v, 0, a, window_x, window_y, hq_win_x, hq_win_y);//
		wait(0.1);
		print(" You have "); print(difficulty - 1); print(" cannon balls left left!"); new_line(); new_line();
		number_of_tries(xt, yt, a, difficulty - 1, window_x, window_y, hq_win_x, hq_win_y);
	}
	else if (difficulty > 0 && difficulty < 2)
	{
		show_console();
		double const v = ask_velocity();
		hide_console();
		trajectory(xt, yt, v, 0, a, window_x, window_y, hq_win_x, hq_win_y);
		wait(0.1);
		print(" You have "); print(difficulty - 1); print(" cannon ball left!"); new_line(); new_line();
		number_of_tries(xt, yt, a, difficulty - 1, window_x, window_y, hq_win_x, hq_win_y);
	}
	if (difficulty == -1)
	{
		show_console();
		double const v = ask_velocity();
		hide_console();
		trajectory(xt, yt, v, 0, a, window_x, window_y, hq_win_x, hq_win_y);
		wait(0.1);
		print(" You have "); print(difficulty - 1); print(" cannon ball left!"); new_line(); new_line();
		number_of_tries(xt, yt, a, difficulty - 1, window_x, window_y, hq_win_x, hq_win_y);
	}
	else if (difficulty == 0)
	{
		show_console(); print(" You have run out of amo...");
	}
}

int lvl4_hard(int const difficulty)
{
	if (difficulty >= 1 && difficulty <= 5)
	{
		int const angle1 = ask_angle();
		return(angle1);
	}
	else if (difficulty == -1)
	{
		int const angle2 = random_in_range(50, 80);
		return(angle2);
	}
	else
	{
		return(0); print("Error check difficulty_3 function");
	}
}

void alien_game()
{
	hide_window();
	show_console();
	set_console_position(0, 0);
	cannon_game_start();
	// variables
	int const window_x = 980;
	int const window_y = 720;
	int const difficulty = cannacon_difficulty_lvl();
	int const size = 40;
	double const radius = 55 * size / 100;
	int const x = 65;
	int const y = window_y - 10 - radius;
	int const a = lvl4_hard(difficulty);
	double const xt = return_fire_x(x, y, 90 - a, radius, size);
	double const yt = return_fire_y(x, y, 90 - a, radius, size);
	int const range = random_in_range(600, 900);
	double const hq_win_x = range - 66;
	double const hq_win_y = 200;
	show_window();
	// visuals section
	connacon(window_x, window_y);
	// objects section
	draw_cannon(x, y, 90 - a, radius, size);
	random_spaceship(range, 200);
	number_of_tries(xt, yt, a, difficulty, window_x, window_y, hq_win_x, hq_win_y);
}
//------------------------------------------------------

void main()
{
	int const time = random_in_range(1, 24); //24 hour clock
	int const road_height = random_in_range(10, 40);
	int const game_randomness = random_in_range(0, 10); //controls how often you get to play
	make_window(980, 720);
	draw_background(time);
	city_sceene(0, 720-road_height);
	draw_trees(0, 720 - road_height);
	draw_bushes(0, 707 - road_height);
	draw_road(time, road_height);
	if (game_randomness == 0) alien_game();//diable this line for ease of grading
}
