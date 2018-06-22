#include "library.h"
double const g = 32.174;
double const pi = acos(-1.0);
// 
/*
// 1. calculation
//equations
double height(double const v, double const t) //works
{
	double const h = (v*t) - (0.5*g*(t*t));
	return(h);
}

double flight_time(double const v) // works
{
	double const f_t = (2*(v/g));
	return(f_t);
}

// 2. Tabulation
// imputs all the values in a table
void tabulation(int const v, int const t)
{
	int const max_t = 12;
	if (t >= 0 && t<=max_t)
	{
		print(" after "); if (t < 10) print(" "); print(t); print(" seconds, height is "); print(height(v, t)); print(" feet"); new_line();
		wait(0.01);
		tabulation(v, t + 1);
	}
}

// 3. Interaction
int ask_for_velocity()
{
	print(" Set your initial velocity: ");
	int const velocity = read_int();
	print(" Calculating data..."); new_line();
	//wait(1); 
	print(" This might take a while..."); new_line();
	//wait(3);
	print(" Okey! here you go"); new_line(); new_line();
	wait(1);
	print(" Table:"); new_line();
	//wait(0.5);
	return(velocity);
}

void calc_imput_velocity()
{
	tabulation(ask_for_velocity(), 0);
	
}

// 4. visual representation
//veritcally represents the heigth of an object
void visual_rep(int const v, int const t)
{
	int const max_t = 12;
	if (t >= 0 && t <= max_t)
	{
		move_to(50.0, 700-height(v,t)); set_pen_width(10); set_pen_color(1-(t/13.0), 0,0+(t/13.0)); draw_point();
		visual_rep(v, t + 1);
	}
}

// 5. First Flight and 6. A nice arc

void trajectory_old(double const x, double const y, int const v, double const t)
{
	// adjust the y multiplyer in move_to() to adjust angle
	//adjust the value added to t for pixel density
	int const max_t = 14;
	double const scale = 700 / max_t;
	if (t >= 0 && t <= max_t)
	{
		move_to(x-x + t*60, y - height(v, t)/3); set_pen_width(8); set_pen_color(1 - (t / 13.0), 0, 0 + (t / 13.0)); draw_point();
		trajectory_old(x, y, v, t + 0.01);
	}
	//void main()
	//{
		//int const x = 800;
		//int const y = 300;
		//make_window(x, y);
		//int const v = 200;
		//int const t = 0;
		//trajectory(x,y, v, t);
	//}
	
}
*/
//______________ Imported Cannon Code___________
// This is my original cannon code, in lab3.cpp I updated it so that the cannon
// is drawn for all degrees
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
	double const circumference = 2 * radius * pi;
	double const side = circumference / 360;
	set_heading_degrees(90);
	move_to(x, y - radius);
	set_pen_color_int(102, 50, 0);
	start_shape();
	line_and_turn(360, side, 1);
	fill_shape();
	set_pen_color(0,0,0);
}

void draw_cannon_body(double const x, double const y, double const radius, int const a, int const size)
{
	// constants
	
	double const w1 = 70 * size / 100; // longest width of the cannon
	double const w2 = 60 * size / 100; // smallest width of the cannon
	double const L1 = 70 * size / 100; // smaller length of the cannon barrel
	double const L2 = 210 * size / 100; // longer length of the cannon barrel
	double const b = a*sin((w1 - w2) / (2.0*(L1 + L2))); //When the cannon is aimed at angle a, the heading for the bottom line is (a-b)
	set_pen_width(4);
	set_pen_color_int(154, 150, 147);
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
	//double const radius = (360 * (size / 10)) / (2 * pi);
	//draw_cannon_body1(x, y, radius, a);
	draw_cannon_body(x, y, radius, a, size);
	draw_circle(x, y, radius);
}

double return_fire_x(int const x, int const y, double a, double const radius, int const size)
{
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
{
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
//______________________________________________

// 7. take the battle to the enemy
	// natural visuals
void draw_background(int const window_x, int const window_y)
{
	move_to(0, 0);
	start_shape();
	set_pen_color_int(205, 254, 255);
	set_heading_degrees(90);
	draw_distance(window_x); note_position();
	turn_right_by_degrees(90);
	draw_distance(window_y); note_position();
	turn_right_by_degrees(90);
	draw_distance(window_x); note_position();
	turn_right_by_degrees(90);
	draw_distance(window_y); note_position();
	fill_shape();
	set_pen_color(0, 0, 0);
}

void draw_ground(int const window_x, int const window_y)
{

	set_pen_color(color::light_green);
	set_pen_width(4);
	start_shape();
	set_heading_degrees(0);
	move_to(0, window_y);
	draw_distance(50); note_position();
	turn_right_by_degrees(90);
	draw_distance(window_x); note_position();
	turn_right_by_degrees(90);
	draw_distance(50); note_position();
	turn_right_by_degrees(90);
	draw_distance(window_x); note_position();
	fill_shape();
	set_pen_color(color::black);
}

void draw_sun(int const window_x, int const window_y)
{
	set_pen_color(color::yellow);
	set_pen_width(4);
	start_shape();
	set_heading_degrees(90);
	move_to(random_in_range(25, 1350), window_y - random_in_range(375, 425));
	draw_distance(100); note_position();
	turn_right_by_degrees(90);
	draw_distance(100); note_position();
	turn_right_by_degrees(90);
	draw_distance(100); note_position();
	turn_right_by_degrees(90);
	draw_distance(100); note_position();
	fill_shape();
	set_pen_color(color::black);
}

void draw_cloud_square(int const w_x, int const w_y)
{
	set_pen_color_int(212, 212, 212);
	set_pen_width(4);
	start_shape();
	set_heading_degrees(90);
	move_to(w_x, w_y);
	draw_distance(40); note_position();
	turn_right_by_degrees(90);
	draw_distance(40); note_position();
	turn_right_by_degrees(90);
	draw_distance(40); note_position();
	turn_right_by_degrees(90);
	draw_distance(40); note_position();
	fill_shape();
	set_pen_color(color::black);
}

void draw_cloud(int const window_x, int const window_y)
{
	int const w_x = random_in_range(10, 1400);
	int const w_y = window_y - random_in_range(250, 460);
	draw_cloud_square(w_x, w_y);
	draw_cloud_square(w_x + 15, w_y - 25);

	draw_cloud_square(w_x + 30, w_y);
}

void draw_N_clouds(int const window_x, int const window_y, int const N)
{
	if (N > 0)
	{
		draw_cloud(window_x, window_y);
		draw_N_clouds(window_x, window_y, N-1);
	}
}

void draw_rand_clouds(int const window_x,int const window_y)
{
	int const N = random_in_range(0,6);
	draw_N_clouds(window_x, window_y, N);
}

void connacon(int const window_x, int const window_y)
{
	move_to(window_x / 2.0, window_y - 460.0);
	set_pen_color(color::black);
	set_font_size(40);
	write_string("CONNACON", direction::center);
}
void you_won(int const window_x, int const window_y)
{
	move_to(window_x / 2.0, window_y - 410.0);
	set_pen_color(color::black);
	set_font_size(40);
	write_string("Mission Accomplished.", direction::center);
}


	// man made objects
void flashing_wind_obj(int const hq_win_x, int const hq_win_y, int const N)
{

	if (N>0)
	{
		wait(0.01);
		set_pen_color(color::red);
		set_pen_width(4);
		start_shape();
		move_to(hq_win_x, hq_win_y);
		set_heading_degrees(0);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		turn_right_by_degrees(90);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		fill_shape();
		set_pen_color(color::black);
		wait(0.01);
		set_pen_color(color::black);
		set_pen_width(4);
		start_shape();
		move_to(hq_win_x, hq_win_y);
		set_heading_degrees(0);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		turn_right_by_degrees(90);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		fill_shape();
		set_pen_color(color::black);
		flashing_wind_obj(hq_win_x, hq_win_y, N - 1);
	}
	else if (N==0)
	{
		set_pen_color(color::light_grey);
		set_pen_width(4);
		start_shape();
		move_to(hq_win_x, hq_win_y);
		set_heading_degrees(0);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		turn_right_by_degrees(90);
		draw_distance(15); note_position();
		turn_right_by_degrees(90);
		draw_distance(42); note_position();
		fill_shape();
		set_pen_color(color::black);
	}
}
void flashing_window(int const hq_win_x, int const hq_win_y)
{
	int const N = 100;
	flashing_wind_obj(hq_win_x, hq_win_y+15, N);
}


double const return_hq_window_x(int const window_y, int const range)
{
	int const x = range;
	int const y = window_y - 60;
	move_to(x, window_y - 25);
	set_heading_degrees(0);
	move_distance(85);
	turn_right_by_degrees(90);
	move_distance(120);
	turn_right_by_degrees(90);
	move_distance(85); 
	move_to(x+10, y-15);
	return(get_x_position());
}

double const return_hq_window_y(int const window_y, int const range)
{
	int const x = range;
	int const y = window_y - 60;
	move_to(x, window_y - 25);
	set_heading_degrees(0);
	move_distance(85);
	turn_right_by_degrees(90);
	move_distance(120);
	turn_right_by_degrees(90);
	move_distance(85);
	move_to(x + 10, y-15);
	return(get_y_position());
}

void draw_hq_window1(int const x, int const y)
{
	set_pen_color(color::white);
	set_pen_width(4);
	start_shape();
	move_to(x, y);
	set_heading_degrees(0);
	draw_distance(15); note_position();
	turn_right_by_degrees(90);
	draw_distance(42); note_position();
	turn_right_by_degrees(90);
	draw_distance(15); note_position();
	turn_right_by_degrees(90);
	draw_distance(42); note_position();
	fill_shape();
	set_pen_color(color::black);
}

void draw_hq_window2(int const x, int const y)
{
	set_pen_color_int(67, 133,244);
	set_pen_width(4);
	start_shape();
	move_to(x, y);
	set_heading_degrees(0);
	draw_distance(15); note_position();
	turn_right_by_degrees(90);
	draw_distance(42); note_position();
	turn_right_by_degrees(90);
	draw_distance(15); note_position();
	turn_right_by_degrees(90);
	draw_distance(42); note_position();
	fill_shape();
	set_pen_color(color::black);
}

void draw_head_quater(int const window_y, int const range)
{
	int const x = range;
	int const y = window_y - 60;
	set_pen_color_int(67, 40, 17);
	set_pen_width(4);
	start_shape();
	move_to(x, window_y - 25); note_position();
	set_heading_degrees(0);
	draw_distance(85); note_position();
	turn_right_by_degrees(90);
	draw_distance(120); note_position();
	turn_right_by_degrees(90);
	draw_distance(85); note_position();
	fill_shape();
	set_pen_color(color::black);
	//windows
	draw_hq_window1(x + 10, y);
	draw_hq_window2(x + 67, y);
}

void draw_cannon_stand(int const window_y)
{
	set_pen_color_int(146, 92, 52);
	set_pen_width(4);
	start_shape();
	set_heading_degrees(0);
	set_pen_width(4);
	move_to(25, window_y - 25); note_position();
	draw_distance(35); note_position();
	turn_right_by_degrees(90);
	draw_distance(125); note_position();
	turn_right_by_degrees(90);
	draw_distance(35); note_position();
	fill_shape();
	set_pen_color(color::black);
}

int hq_range()
{
	int const r = random_in_range(750, 1290);
	return(r);
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
{
	//double const max_t = 2 * v / g;

	if ((yt - calc_height(v, t, angle) / 1.5) < (window_y - 10))
	{
		wait(0.01);
		draw_to(xt+calc_distance(v, t, angle), (yt - calc_height(v, t, angle)/1.5)); set_pen_width(5);
		set_pen_color(1 - (t / 13.0), 0, 0 + (t / 13.0));
		if (xt + calc_distance(v, t, angle) >= hq_win_x-2 && xt + calc_distance(v, t, angle) <= hq_win_x + 42 && yt - calc_height(v, t, angle) / 1.5 >= hq_win_y && yt - calc_height(v, t, angle) / 1.5 <= hq_win_y + 20)
		{
			 new_line(); new_line();
			 flashing_window(hq_win_x, hq_win_y); you_won(window_x, window_y);
			print(" If you still have amo left, try hitting the renforced window."); new_line();
			print(" You probably wont destroy it but its worth a shot."); new_line(); new_line();
			print(" Otherwise feel free to start the mission at a higher difficulty lvl."); new_line(); new_line();
		}
		else if (xt + calc_distance(v, t, angle) >= hq_win_x+47 && xt + calc_distance(v, t, angle) <= hq_win_x + 114 && yt - calc_height(v, t, angle) / 1.5 >= hq_win_y && yt - calc_height(v, t, angle) / 1.5 <= hq_win_y + 20)
		{
			print(" You've hit the reinforced window..."); new_line();
			print(" There seems to be a slight dent but nothing to promissing"); new_line(); new_line();
		}
		else if (xt + calc_distance(v, t, angle) >= hq_win_x - 10 && xt + calc_distance(v, t, angle) <= hq_win_x + 110 && yt - calc_height(v, t, angle) / 1.5 >= hq_win_y+60 && yt - calc_height(v, t, angle) / 1.5 <= hq_win_y -15)
		{
			trajectory_loop(xt, yt, v, t + 0.1, angle,window_x, window_y, hq_win_x, hq_win_y);
		}
		else
		{
			trajectory_loop(xt, yt, v, t + 0.1, angle, window_x, window_y, hq_win_x, hq_win_y);
		}
	}
}

void trajectory(double const xt, double const yt, int const v, double const t, double const a, int const window_x, int const window_y, double const hq_win_x, double const hq_win_y)
{
	// adjust the y multiplyer in move_to() to adjust angle
	//adjust the value added to t for pixel density
	move_to(xt, yt);
	trajectory_loop(xt, yt, v, t, a, window_x, window_y, hq_win_x, hq_win_y);
	
	
	/*void main()
	{
	int const x = 800;
	int const y = 500;
	make_window(x, y);
	int const v = 200;
	int const t = 0;
	trajectory(x, y, v, t);
	*/
}

double ask_angle()
{
	print(" Imput desired angle: ");
	double const a = (read_double()); new_line();
	return (a);
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
	print(" The goal of this mission is to hit the enemy base at its weakest point."); new_line();
	print(" A recent attack managed to damadge but not destroy the enemy HQ."); new_line();
	print(" The right window is damage and has no explosive resistant glass."); new_line(); new_line();
	print(" Your job is to hit the HQ's rigth window with a cannon ball to destroy it."); new_line(); new_line();
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
	
	if (diff == 1) return(5);
	if (diff == 2) return(3);
	if (diff == 3) return(1);
	if (diff == 4) return(-1);
	else
	{
		return(0);
		print(" You imputed something other than 1, 2, 3 or 4!"); new_line();
		print(" please try again"); new_line(); new_line();
		cannacon_difficulty_lvl();
	}
	
}

void number_of_tries(double const xt, double const yt, double const a, int const difficulty,int const window_x, int const window_y, double const hq_win_x, double const hq_win_y)
{


	if (difficulty > 1)
	{
		show_console();
		double const v = ask_velocity();
		hide_console();
		trajectory(xt, yt, v, 0, a, window_x, window_y, hq_win_x, hq_win_y);
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
	else if (difficulty == -1 )
	{
		int const angle2 = random_in_range(20,80);
		return(angle2);
	}
	else
	{
		return(0); print("Error check difficulty_3 function");
	}
}


	// 8. The Game
void main()
{
	set_console_position(350, 0);
	set_console_size(900, 330);
	cannon_game_start();
	// variables
	int const window_x = 1430;
	int const window_y = 500;
	int const difficulty = cannacon_difficulty_lvl();
	int const size = 40;
	double const radius = 55 * size / 100;
	int const x = 75;
	int const y = window_y-64-radius;
	int const a = lvl4_hard(difficulty);
	double const xt = return_fire_x(x, y,90 - a, radius, size); 
	double const yt = return_fire_y(x, y,90 - a, radius, size); 
	int const range = hq_range();
	double const hq_win_x = return_hq_window_x(window_y, range); 
	double const hq_win_y = return_hq_window_y(window_y, range); 
	make_window(window_x, window_y);
	set_window_position(0, 325);

	// visuals section
	draw_background(window_x, window_y);
	draw_sun(window_x, window_y);
	draw_ground(window_x, window_y);
	draw_rand_clouds(window_x, window_y);
	connacon(window_x, window_y);
	
	

	// objects section
	draw_cannon_stand(window_y);
	draw_head_quater(window_y, range);
	draw_cannon(x, y, 90-a, radius, size);
	number_of_tries(xt, yt, a, difficulty, window_x, window_y, hq_win_x, hq_win_y);

}
