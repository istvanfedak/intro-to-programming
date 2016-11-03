#include "library.h"
double const pi = acos(-1.0);

//A1
void basic_repetitive_function(int const A, int const B)
{
	if (A <= B)
	{
		print(" ");
		print(A);
		basic_repetitive_function(A + 1, B);
	}
	
}

//A2
void print_kph(int const A)
{
	double const kph =(int)(A*1.609344 + 0.5);
	print(kph);
}
void print_mpm(int const A)
{
	double mpm =(A/60.0); // I could figure out another way so I made mpm non constant
	mpm*= 100;
	int mpm2 = (int)(mpm);
	double const mpm_2dec = mpm2/100.0;
	print(mpm_2dec);
}
void multiples_of_five_numbers(int const A, int const B)
{
	if (A <= B)
	{
		print(" "); print(A); print(" mph is "); print_kph(A); print(" or "); print_mpm(A); print(" mpm"); new_line();
		multiples_of_five_numbers(A + 5, B);
	}

}

//B
void draw_stars(int const A)
{
	if (A > 0)
	{
		print(" ");
		print("*");
		draw_stars(A-1);
	}

}
void draw_dots(int const B)
{
	if (B > 0)
	{
		print(" ");
		print(".");
		draw_dots(B - 1);
	}
}
void draw_dots_stars(int const B, int const A)
{
	draw_dots(B); draw_stars(A); draw_dots(B);
}
//_________________________________________________

void count_up(int const x, int const y)
{
	if (x > 0)
	{
		print(" "); print(y); //new_line();
		count_up(x-1,y+2);
	}
}
void count_down(int const x)
{
	if (x > 0)
	{
		print(" "); print(x); //new_line();
		count_down(x - 1);
	}
}

//________________________________________________
void draw_sequence(int const x, int const y)
{
	if (x > 0)
	{
		print(" ");  print(x); print(" "); print(y); new_line();
		draw_sequence(x-1,y+2);
	}
}

void triangle(int const B, int const A)
{
	if (B > 0)
	{
		draw_dots_stars(B, A); new_line();
		triangle(B - 1, A + 2);
	}
}

//C
/* //////////////// Documentation /////////////////////////////////////////////////

Since library.h interprets angles differently:
- regular two dimensional cartesian angles are measured from the x axis counter clockwise
Ex: 0* is positive horizontal, 90* is positive vertical, 180* is negative horizontal and 270* is negative vertical

- In library.h angles are measured from the y axis clockwise
Ex: 0* is positive vertical, 90* is positive horizontal, 180* is negative vertical and 270* is negative horizontal

Also all geometrical calculations (sin,cos, etc) use radians instead of degrees
- All your angles should be in radians if using


//---------- calculation section ----------------
angl: parametter of the draw_cannon function. It is an angle specified by user measured from the x axis counter clockwise
- in degrees
angle: it is the angle tangent to the firing angle "a"
- in degrees

r: radius of wheel
w1: the width of the back of the cannon
w2: width of the front of the cannon

l1: the distance from the back of the cannon to the wheel’s axle
l2: the rest of the length from the wheel's axle to the front of the cannon
l3: l1+l2 -- distance from the front of the cannon to the back of the cannon

a: the aiming angle.
- in radians
b: When the cannon is aimed at angle a, the heading for the bottom line is (a-b)
- in radians
r: radius of the wheel

(xg, yg): the position where the wheel touches the ground
(xc, yc): the position for the center of the wheel/ fulcrum
(xp, yp): the easiest point to start drawing the body of the cannon from, coordinates
(xe, ye): the point where the ball pops out when it is fired

d: the distance between points P and E
g: the angle from point P to point E if the cannon lies flat as in the third diagram
- in radians
*/
////////////////////////////////////////////////////////////////////////////////////////////


void line_and_turn(const int lines, const double angle, const double length) {
	if (lines>0) {
		turn_right_by_degrees(angle);
		draw_distance(length); note_position();
		line_and_turn(lines - 1, angle, length);
	}
}

//draws circle with center (xc, yc)
void draw_circle(const double xc, const double yc, const double radius) {
	start_shape();
	double const circumference = 2 * radius * pi;
	double const side = circumference / 360;
	set_heading_degrees(90);
	move_to(xc, yc - radius);
	line_and_turn(360, 1.0, side);
	fill_shape();
}

void draw_cannon_body(const double xg, const double yg, const double angl, const double r) {
	const double angle = 0 - angl;
	const double w1 = 70.0;
	const double w2 = 35.0;
	const double L1 = 50.0;
	const double L2 = 150.0;
	const double a = (90 - angl) / 180.0*pi;// aming angle in radians
	const double xc = xg;
	const double yc = yg - r;
	const double b = asin((w1 - w2) / 2 / (L1 + L2));
	const double xp = xc - L1 * sin(a - b);
	const double yp = yc + L1 * cos(a - b);
	const double len = (L1 + L2) * cos(b);
	const double d = sqrt(len*len + w1*w1 / 4);
	const double g = asin(w1 / 2 / d);
	const double xe = xp + d * sin(a - g);
	const double ye = yp - d * cos(a - g);

	//Drawing the body
	start_shape();
	move_to(xp, yp); //moves pen to easiest starting position
	set_heading_degrees(angle);
	draw_distance(w1);
	note_position();

	turn_right_by_degrees(90 + b*(180 / pi));
	draw_distance(L1 + L2);
	note_position();

	set_heading_degrees(angle + 180); //if a is zero w2 is vertical but in the opposite direction as w1
	draw_distance(w2);
	note_position();

	turn_right_by_degrees(90 - b*(180 / pi));
	draw_distance(L2 - r);
	move_distance(2 * r); //this is so you dont see the line going through the wheel
	draw_distance(L1 - r);
	note_position();



	//moving to firing position
	move_to(xe, ye);
	set_heading_degrees(90 - angl);
	draw_distance(10); //cannon ball
	move_to(xg, yg);

	fill_shape();
}

void draw_cannon(const double xg, const double yg, const double angl, const double r){
	set_pen_color(color::dark_grey);
	draw_cannon_body(xg, yg, angl, r);

	set_pen_color(color::brown);
	draw_circle(xg, yg - r, r);

	set_pen_color(color::black); //resents color to default
}

void main()
{
	print("Part A:\n");
	basic_repetitive_function(0, 10);
	new_line();
	new_line();
	print("Part A2:\n");
	multiples_of_five_numbers(10, 25);
	new_line();
	print("Part B\n");
	triangle(5, 1);
	// Part C
	const double xg = 200;
	const double yg = 400;
	const double r = 40;
	new_line();
	print("Part C\n");
	print("Input cannon angle: ");
	const double a = read_double();

	make_window(800, 800);
	draw_cannon(xg, yg, a, r);
}
