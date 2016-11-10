#include "library.h"

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

void draw_button(int const Xpos, int const Ypos, int const size, string const string)
{
	set_pen_color(color::black);
	rect(Xpos-2, Ypos-2, size + 4,size + 4);
	set_pen_color(color::light_grey);
	rect(Xpos, Ypos, size, size);
	set_pen_color(color::black);
	move_to(Xpos + (size / 2), Ypos + (size / 2));
	set_font_size(size);
	write_string(string, direction::center);
}

void row_of_buttons(int const rows, int const Xpos, int const Ypos, int const size, string const s[])
{	// this function draws a row of buttons
	int N = rows;
	int const but_size = size / 4;
	int separation = 0;
	int s_print = 0;
	while (N > 0)
	{
		draw_button(Xpos + separation, Ypos, but_size,s[s_print]);
		separation = separation + but_size + (but_size / 3);
		s_print = s_print + 1;
		N = N - 1;
	}
}

void block_of_buttons(int const floors, int const rows, int const Xpos, int const Ypos, int const size)
{
	string const s0[4] = { "+", "7", "8", "9" };
	string const s1[4] = { "-", "4", "5", "6" };
	string const s2[4] = { "*", "1", "2", "3" };
	string const s3[4] = { "/", "=", "0", "C" };
	// the int real_size converts the size given by the user to be the real size of the output in pixels 
	int N = floors;
	int separation = 0;
	int const but_size = size / 4;
	while (N > 0)
	{
		if (N == 4) row_of_buttons(rows, Xpos, Ypos + separation, size, s0);
		else if (N == 3) row_of_buttons(rows, Xpos, Ypos + separation, size, s1);
		else if (N == 2) row_of_buttons(rows, Xpos, Ypos + separation, size, s2);
		else row_of_buttons(rows, Xpos, Ypos + separation, size, s3);
		separation = separation + (but_size / 3) + but_size;
		N = N - 1;
	}
}

int find_row(int const Xpos, int const Ypos, int const size)
{
		double const ratio = size / 280.0;
		int const but_size = size / 4;
		int const sep = size / 16 + 7 * ratio;
		int const total = but_size + sep;
		int const initial_space = Ypos + 140 * ratio;
		int y = get_click_y();
		int row = (y - (Ypos + 140 * ratio)) / total;
		if ((y - (initial_space) > total*row && y - initial_space < total*(1 + row) - sep)) return row;
		else return-1;
}

int find_column(int const Xpos, int const Ypos, int const size)
{
		double const ratio = size / 280.0;
		int const but_size = size / 4;
		int const sep = size / 16 +7*ratio;
		int const total = but_size + sep;
		int const initial_space = Xpos + 15 * ratio;
		int x = get_click_x();
		int column = (x - (initial_space)) / total;
		if ((x - (initial_space) > total*(column) && x - (initial_space) < total*(1 + column) - sep))return column;
		else return-1;
}

void display(int const Xpos, int const Ypos, int const size, string s)
{
	set_pen_color(color::black);
	double const ratio = (size / 280.0);
	int const but_size = size / 4;
	rect(Xpos + 15 * ratio, Ypos + 15 * ratio, 345 * ratio, 100 * ratio);
	set_font_size(80 * ratio);
	move_to(Xpos + 345 * ratio, Ypos + 15 * ratio + 10 * ratio);
	set_pen_color(color::green);
	write_string(s, direction::south_west);
}

void display(int const Xpos, int const Ypos, int const size, int i)
{
	set_pen_color(color::black);
	double const ratio = (size / 280.0);
	int const but_size = size / 4;
	rect(Xpos + 15 * ratio, Ypos + 15 * ratio, 345 * ratio, 100 * ratio);
	set_font_size(80 * ratio);
	move_to(Xpos + 345 * ratio, Ypos + 15 * ratio + 10 * ratio);
	set_pen_color(color::green);
	write_string(i, direction::south_west);
}

int but_code(int const Xpos, int const Ypos, int const size)
{
	// -1 == C ; -2 == = ; -3 == * ; -4 == / ;  -5 == + ; -6 == - ;
	// based on order if opperations
	int const s0[4] = { -5, 7, 8, 9 };
	int const s1[4] = { -6, 4, 5, 6 };  
	int const s2[4] = { -3, 1, 2, 3 }; 
	int const s3[4] = { -4, -2, 0, -1 }; 
	int const row = find_row(Xpos, Ypos, size);
	int const column = find_column(Xpos, Ypos, size);
	if (column >= 0 && row >= 0)
	{
		if (row == 0) return(s0[column]);
		else if (row == 1) return(s1[column]);
		else if (row == 2) return(s2[column]);
		else return(s3[column]);
	}
	else return(-13);
}

void draw_calculator(int const Xpos, int const Ypos, int const size)
{
	double const ratio = (size / 280.0);
	display(Xpos, Ypos, size,"");
	block_of_buttons(4, 4, Xpos + 15 * ratio, Ypos + 140 * ratio, size);
}

void calc_code(int const Xpos, int const Ypos, int const size)
{
	int memory = 0;
	int memory2 = 0;
	int opperation = 0;
	int previous_opperation = 0;
	int code = 0;
	int i = 0;
	bool b = 1;
	while (true)
	{
		while (code >= 0 || code == -13)
		{
			wait_for_mouse_click();
			code = but_code(Xpos, Ypos, size);
			if (code >= 0 && i != 0 && code != -13)
			{
				memory2 = memory2 * 10 + code;
				//print(memory2);
				if (memory2 >= 99999999 || memory <= -9999999)
				{
					display(Xpos, Ypos, size, "# too large");
					opperation = -1;
					memory = 0;
				}
				else
				{
					display(Xpos, Ypos, size, memory2);
				}
			}
			else if (code >= 0 && i == 0 && code != -13)
			{
				memory = memory * 10 + code;
				if (memory >= 99999999)
				{
					display(Xpos, Ypos, size, "# too large");
					opperation = -1;
					memory = 0;
				}
				else
				{
					display(Xpos, Ypos, size, memory);
				}
			}
			if (code<0 && code != -13)
			{
				previous_opperation = opperation;
				opperation = code;
			}
		}
		
		if (opperation == -1) // C
		{
			memory = 0;
			memory2 = 0;
			i = -1;
			display(Xpos, Ypos, size, "0");
		}
		else if (previous_opperation == -2) // =
		{
			display(Xpos, Ypos, size, memory);
		}
		else if (previous_opperation == -3) // *
		{
			memory = memory * memory2;
			display(Xpos, Ypos, size, memory);
		}
		else if (previous_opperation == -4) // /
		{
			if (memory2 == 0) display(Xpos, Ypos, size, "undefined");
			else
			{
				memory = memory / memory2;
				display(Xpos, Ypos, size, memory);
			}
		}
		else if (previous_opperation == -5) // +
		{
			memory = memory + memory2;
			display(Xpos, Ypos, size, memory);
		}
		else if (previous_opperation == -6) // -
		{
			memory = memory - memory2;
			display(Xpos, Ypos, size, memory);
		}
		code = 0;
		memory2 = 0;
		i++;
	}
}

void calculator(int const Xpos, int const Ypos, int const size)
{
	draw_calculator(Xpos, Ypos,size);
	calc_code(Xpos, Ypos, size);
}

void main()
{
	// you can scale the calculator by changing size, Xpos and Ypos
	// I plan to fix the multiplication and addition overflow in the future
	int const size = 280; // original 280
	int const Xpos = 0;
	int const Ypos = 0;
	make_window(380, 505);
	calculator(Xpos, Ypos, size);
}
