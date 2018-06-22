# include "library.h"

int const pi = acos(-1);

// calendar date
int year(int const date) // This function obtains the year form the calendar date and returns its interval value
{
	int const year = date/10000;
	return(year);
}

int month(int const date)// This function obtains the month form the calendar date and returns its interval value
{
	int const month_date= date%10000;
	int const month = month_date/100;
	return(month);
}

int day(int const date) // This function obtains the day form the calendar date and returns its interval value
{
	int const day = date % 100;
	return(day);
}

string ordinal_day(int const date) // This function returns the ordinal suffix of the numbers
{
	int const cardinal_day = day(date);
	if (cardinal_day % 10 == 1) return("st");
	else if (cardinal_day % 10 == 2) return("nd");
	else if (cardinal_day % 10 == 3) return("rd");
	else if (cardinal_day % 10 >= 4) return("th");
	else if (cardinal_day % 10 == 0) return("th");
	else return("error ordinal_day");

}

string month_name(int const date) // This function converts month dates into month names
{
	int const normal_month = month(date);
	if (normal_month == 1) return("January ");
	if (normal_month == 2) return("February ");
	if (normal_month == 3) return("March ");
	if (normal_month == 4) return("April ");
	if (normal_month == 5) return("May ");
	if (normal_month == 6) return("June ");
	if (normal_month == 7) return("July ");
	if (normal_month == 8) return("August ");
	if (normal_month == 9) return("September ");
	if (normal_month == 10) return("October ");
	if (normal_month == 11) return("November ");
	if (normal_month == 12) return("December ");
	else return("error month_name"); // If something is wrong this error message would be displayed
}

void display_date(int const x, int const y, int const date, int const size) // This function displays the date in the window
{
	int const text_size = 60 * (size / 200.0);
	int const txt_x = x - 210 * (size / 200.0);
	int const txt_y = y + (650 - (750 / 2.0))*(size / 200.0);
	if (day(date) == 1) // This set of "if" statements erraces the previous month
	{
		// These set of "if" statements convert the month you have into the previous month so it could be displayed in white,
		// this also helps prevent the screen from flickering. Becuase each month has a different 

		set_pen_color(color::white);

		// Jan, Mar, May, Jul, Aug, Oct
		if (month(date - 100) == 1 || month(date - 100) == 3 || month(date - 100) == 5 || month(date - 100) == 7 || month(date - 100) == 8 || month(date - 100) == 10) 
		{
			
			set_font_size(text_size);
			move_to(txt_x, txt_y);
			write_string(day(31), direction::center);
			move_to(txt_x + 34 * text_size / 40, txt_y);
			write_string(ordinal_day(31), direction::center);
			move_to(txt_x + 140 * text_size / 40, txt_y);
			write_string(month_name(date - 100), direction::center);
			move_to(txt_x + 260 * text_size / 40, txt_y);
			write_string(year(date), direction::center);
		}

		// Apr, Jun, Sep, Nov
		else if (month(date - 100) == 4 || month(date - 100) == 6 || month(date - 100) == 9 || month(date - 100) == 11)
		{
			set_font_size(text_size);
			move_to(txt_x, txt_y);
			write_string(day(30), direction::center);
			move_to(txt_x + 34 * text_size / 40, txt_y);
			write_string(ordinal_day(30), direction::center);
			move_to(txt_x + 140 * text_size / 40, txt_y);
			write_string(month_name(date - 100), direction::center);
			move_to(txt_x + 260 * text_size / 40, txt_y);
			write_string(year(date), direction::center);
		}

		// December is a special case. Since we are erracing december, we have to errace the previous year
		else if (month(date - 100) == 0) //Dec
		{
			set_font_size(text_size);
			move_to(txt_x, txt_y);
			write_string(day(31), direction::center);
			move_to(txt_x + 34 * text_size / 40, txt_y);
			write_string(ordinal_day(31), direction::center);
			move_to(txt_x + 140 * text_size / 40, txt_y);
			write_string(month_name(1200), direction::center);
			move_to(txt_x + 260 * text_size / 40, txt_y);
			write_string(year(date - 10000), direction::center);
		}

		//Feb... leap year formula!
		else if (month(date - 100) == 2) 
		{
			if ((year(date) % 4 == 0 && !(year(date) % 100 == 0)))
			{
				set_pen_color(color::white);
				set_font_size(text_size);
				move_to(txt_x, txt_y);
				write_string(day(29), direction::center);
				move_to(txt_x + 34 * text_size / 40, txt_y);
				write_string(ordinal_day(date - 29), direction::center);
				move_to(txt_x + 140 * text_size / 40, txt_y);
				write_string(month_name(date - 100), direction::center);
				move_to(txt_x + 260 * text_size / 40, txt_y);
				write_string(year(date), direction::center);
			}
			else if (year(date) % 400 == 0)
			{
				set_font_size(text_size);
				move_to(txt_x, txt_y);
				write_string(day(29), direction::center);
				move_to(txt_x + 34 * text_size / 40, txt_y);
				write_string(ordinal_day(date - 29), direction::center);
				move_to(txt_x + 140 * text_size / 40, txt_y);
				write_string(month_name(date - 100), direction::center);
				move_to(txt_x + 260 * text_size / 40, txt_y);
				write_string(year(date), direction::center);
			}
			else
			{
				set_font_size(text_size);
				move_to(txt_x, txt_y);
				write_string(day(28), direction::center);
				move_to(txt_x + 34 * text_size / 40, txt_y);
				write_string(ordinal_day(28), direction::center);
				move_to(txt_x + 140 * text_size / 40, txt_y);
				write_string(month_name(date - 100), direction::center);
				move_to(txt_x + 260 * text_size / 40, txt_y);
				write_string(year(date), direction::center);
			}
		}	
	}

	// Erraces the rest of the days in the month. Since we already erraced the month in the previous step
	// we dont have to worry about erracing it again.
	else
	{
		set_pen_color(color::white);
		set_font_size(text_size);
		move_to(txt_x, txt_y);
		write_string(day(date-1), direction::center);
		move_to(txt_x + 34 * text_size / 40, txt_y);
		write_string(ordinal_day(date-1), direction::center);
		move_to(txt_x + 140 * text_size / 40, txt_y);
		write_string(month_name(date-1), direction::center);
		move_to(txt_x + 260 * text_size / 40, txt_y);
		write_string(year(date-1), direction::center);
	}
	
	// Draws the current date
	set_pen_color(color::black);
	move_to(txt_x, txt_y);
	write_string(day(date), direction::center);
	move_to(txt_x + 34 * text_size / 40, txt_y);
	write_string(ordinal_day(date), direction::center);
	move_to(txt_x + 140 * text_size / 40, txt_y);
	write_string(month_name(date), direction::center);
	move_to(txt_x + 260 * text_size / 40, txt_y);
	write_string(year(date), direction::center);
}

// clock time
int normal_hour_int(int const time) // This function converts continental hours into normal hours
{
	int const continental_hour = time / 10000;
	if (continental_hour <= 12)
	{
		int const hour_am = continental_hour;
		return(hour_am);
	}

	else
	{
		int const hour_pm = continental_hour - 12;
		return(hour_pm);
	}

}

string normal_hour_string(int const time) // This function adds the appropriate periods of a 12-hour clock
{
	int const continental_hour = time / 10000;
	if (continental_hour <= 12)
	{
		return(" a.m.");
	}
	else if (continental_hour > 12)
	{
		return(" p.m.");
	}

}

int minute(int const time) // This function obtains the minutes from the clock date
{
	int const month_date = time % 10000;
	int const minute = month_date / 100;
	return(minute);
}

string minute_zero(int const time) // This function adds a zero if the conditions are satisfied
{
	if (minute(time) >= 0 && minute(time) < 10) return("0");
	else return("");
}

int second(int const time) // This function converts the obtained clock time to return seconds
{
	int const second = time % 100;
	return(second);
}

void display_time(int const x, int const y, int const time, int const size) // This function display's the time in the window
{
	int const text_size = 80*(size/200.0);
	int const txt_x = (x - 130 * (size / 200.0));
	int const txt_y = (y - (750 / 2.0 - 100)*(size / 200.0));
	set_pen_color(color::white);
	set_font_size(text_size);

	//the purpose of the following invisible texts is to prevent flickering
	move_to(txt_x-60, txt_y); 
	write_string(normal_hour_int(time - 10000), direction::center); 

	// This function erraces the previous time 
	if (minute(time) >= 0 && minute(time-100) < 10) // this if statement adds a zero to the minutes
	{
		move_to(txt_x, txt_y);
		write_string(normal_hour_int(time - 10000), direction::center);
		move_to(txt_x + 25 * text_size / 60, txt_y);
		write_string(":", direction::center);

		move_to(txt_x + 55 * text_size / 60, txt_y);
		write_string(minute_zero(time-100), direction::center);

		move_to(txt_x + 80 * text_size / 60, txt_y);
		write_string(minute(time-100), direction::center);

		move_to(txt_x + 150 * text_size / 60, txt_y);
		write_string(normal_hour_string(time-100), direction::center);
	}

	else
	{
		move_to(txt_x, txt_y);
		write_string(normal_hour_int(time - 10000), direction::center);
		move_to(txt_x + 30 * text_size / 60, txt_y);
		write_string(":", direction::center);

		move_to(txt_x + 65 * text_size / 60, txt_y);
		write_string(minute(time-100), direction::center);

		move_to(txt_x + 150 * text_size / 60, txt_y);
		write_string(normal_hour_string(time-100), direction::center);
	}

	// This function displays the current time 
	set_pen_color(color::black);
	move_to(txt_x, txt_y);


	if (minute(time) >= 0 && minute(time) < 10)// this if statement adds a zero to the minutes
	{
		write_string(normal_hour_int(time), direction::center);
		move_to(txt_x + 25 * text_size / 60, txt_y);
		write_string(":", direction::center);

		move_to(txt_x + 55 * text_size / 60, txt_y);
		write_string(minute_zero(time), direction::center);

		move_to(txt_x + 80 * text_size / 60, txt_y);
		write_string(minute(time), direction::center);

		move_to(txt_x + 150 * text_size / 60, txt_y);
		write_string(normal_hour_string(time), direction::center);
	}
	else
	{
		write_string(normal_hour_int(time), direction::center);
		move_to(txt_x + 30 * text_size / 60, txt_y);
		write_string(":", direction::center);

		move_to(txt_x + 65 * text_size / 60, txt_y);
		write_string(minute(time), direction::center);

		move_to(txt_x + 150 * text_size / 60, txt_y);
		write_string(normal_hour_string(time), direction::center);
	}

}

// draws clock
void angle_and_turn(int const x, int const y, int const N, double const angle, double const size) // This function uses recurion to draw the clocks dial
{
	if (N > 0)
	{
		move_to(x, y);
		if (N % 5 == 0)
		{
			set_pen_width(4*size/200.0);
			move_distance(size - (10 * size / 200.0));
			draw_distance(22 * size / 200.0);
		}
		else if (N % 5 != 0)
		{
			set_pen_width(3 * size / 200.0);
			move_distance(size);
			draw_distance(12 * size / 200.0);
		}
		turn_right_by_degrees(angle);
		angle_and_turn(x, y, N - 1, angle, size);
	}
}

void draw_dial(double const x, double const y, double const size) // This function draws the complete dial
{
	double const N = 60;
	double const angle = 360 / N;
	set_heading_degrees(0);
	angle_and_turn(x, y, N, angle, size);

}

void draw_second_hand(int const x, int const y, int const time, int const size) // This function draws the second hand
{
	move_to(x, y);

	if ((second(time) -1) % 5 == 0) // draws back the clock dial
	{
		set_pen_color(color::black);
		move_to(x, y);
		set_heading_degrees(6 * second(time)-6);
		set_pen_width(4 * size / 200.0);
		move_distance(size - (10 * size / 200.0));
		draw_distance(22 * size / 200.0);
		set_pen_color(color::white);
		turn_right_by_degrees(180);
		move_distance(30 * size / 200.0);
		set_pen_width(6 * size / 200.0);
		draw_distance(22 * size / 200.0);
		set_pen_color(color::black);
	}

	if (second(time) % 5 == 0) // draws the second hand position for the big ticks
	{
		set_pen_color(color::red);
		move_to(x, y);
		set_heading_degrees(6 * second(time));
		set_pen_width(4 * size / 200.0);
		move_distance(size - (10 * size / 200.0));
		draw_distance(22 * size / 200.0);
		turn_right_by_degrees(180);
		move_distance(30 * size / 200.0);
		set_pen_width(6 * size / 200.0);
		draw_distance(22 * size / 200.0);
		set_pen_color(color::black);
	}

	// Erraces the second dial
	set_pen_color(color::black);
	move_to(x, y);
	set_heading_degrees(6 * second(time) - 6);
	set_pen_width(3 * size / 200.0);
	move_distance(size);
	draw_distance(12 * size / 200.0);
	set_pen_color(color::white);
	turn_right_by_degrees(180);
	move_distance(30 * size / 200.0);
	set_pen_width(6 * size / 200.0);
	draw_distance(22 * size / 200.0);

	// Draws the second dial
	set_pen_color(color::red);
	move_to(x, y);
	set_heading_degrees(6 * second(time));
	set_pen_width(3 * size / 200.0);
	move_distance(size);
	draw_distance(12 * size / 200.0);
	turn_right_by_degrees(180);
	move_distance(30 * size / 200.0);
	set_pen_width(6 * size / 200.0);
	draw_distance(22 * size / 200.0);
	set_pen_color(color::black);
}

void draw_minute_hand(int const x, int const y, int const time, int const size) //This function draws the watches minute hand
{
	// Erraces the previous minute
	set_pen_color(color::white);
	set_pen_width(8 * size / 200.0);
	move_to(x, y);
	set_heading_degrees(6 * minute(time) - 6);
	draw_distance(size - (48 * size / 200.0));
	move_to(x, y);
	turn_right_by_degrees(180);
	draw_distance(22 *size / 200.0);

	// Draws the minute hand
	set_pen_color(color::black);
	set_pen_width(8 * size / 200.0);
	move_to(x, y);
	set_heading_degrees(6 * minute(time));
	draw_distance(size - (48 * size / 200.0));
	move_to(x, y);
	turn_right_by_degrees(180);
	draw_distance(22 * size / 200.0);


}

void draw_hour_hand(int const x, int const y, int const time, int const size) // This function draws the hour hand on the watch
{
	// Erraces the previous movement
	set_pen_color(color::white);
	set_heading_degrees((30 * normal_hour_int(time)) + 0.5*minute(time)-0.5);
	set_pen_width(12 * size / 200.0);
	move_to(x, y);
	draw_distance(size - (78 * size / 200.0));
	move_to(x, y);
	turn_right_by_degrees(180);
	draw_distance(15 * size / 200.0);

	// Draws the hour hand
	set_pen_color(color::black);
	set_heading_degrees((30 * normal_hour_int(time)) + 0.5*minute(time));
	set_pen_width(12 * size / 200.0);
	move_to(x, y);
	draw_distance(size - (78 * size / 200.0));
	move_to(x, y);
	turn_right_by_degrees(180);
	draw_distance(15 * size / 200.0);
}


void main()
{
	// part 2
	int const time = get_clock_time();
	int const date = get_calendar_date();
	print("  year: ");  print(year(date));   new_line();
	print(" month: ");  print(month(date));  new_line();
	print("   day: ");  print(day(date));    new_line();
	print("  hour: ");  print(normal_hour_int(time)); print(normal_hour_string(time));  new_line();
	print("minute: ");  print(minute(time)); new_line();


	// part 6 complete product
	double const size = 200; // The clock is scalable... try changing the size
	int const x = 250; // You can move the center of the clock left and right by changing this value... try changing it
	int const y = 750/2.0; // You can move the center of the clock up and down by changing this value... try changing it

	make_window(500, 750);
	draw_dial(x, y, size); // Function draws the clock's dial
	
	while (true)
	{
		wait(0.1);
		int const time = get_clock_time();
		int const date = get_calendar_date();

		// Functions display date and time
		display_time(x, y, time, size);
		display_date(x, y, date, size);

		// Functions draw the clock's hands
		draw_second_hand(x, y, time, size);
		draw_minute_hand(x, y, time, size);
		draw_hour_hand(x, y, time, size);
	}
}
