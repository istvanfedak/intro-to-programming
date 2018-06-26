#include "library.h"
#include <iostream>
using namespace std;

int month_length(int const year, int const month)
{
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		return(31);
	}

	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		return(30);
	}

	else if (month == 2)
	{
		if (year % 4 == 0 && !(year % 100 == 0)) return(29);

		else if (year % 400 == 0) return(29);

		else return(28);
	}
	else return(0);
}

int sum_month_days(int const year, int const month)
{
	if (month>1)
	{
		int const total_month_days = month_length(year, month - 1) + sum_month_days(year, month - 1);
		return(total_month_days);
	}
	else return(0);
}

int day_of_the_year(int const year, int const month, int const day)
{

	if (month == 1) return(day);
	else if (month > 1 || month <= 12)
	{
		int const day_of_the_year = sum_month_days(year, month);
		return(day_of_the_year + day);
	}
	else return(0);
}

int year_length(int const year)
{
	if (year >= 0)
	{
		if (year % 4 == 0 && !(year % 100 == 0)) return(366);
		else if (year % 400 == 0) return(366);
		else return(365);
	}
	else return(0);
}

int sum_year_days(int const year)
{
	if (year >= 0)
	{
		int const days_in_years = year_length(year - 1) + sum_year_days(year - 1);
		return(days_in_years);
	}
	else return(0);
}

int day_of_the_century(int const year, int const month, int const day)
{
	//int const first_day_of_month = 1;
	if (year == 0) return(day_of_the_year(year, month, day));
	else if (year > 0)
	{
		int const day_of_the_century = sum_year_days(year) + day_of_the_year(year,month,day);
		return(day_of_the_century);
	}
	else return(0);
}

int day_of_the_week(int const year, int const month, int const day)
{
	return((day_of_the_century(year, month, day) + 5) % 7);
}

void new_calendar_line()
{
	print("-----------------------------"); new_line();
}

int month_string_length(int const month)
{
	if (month == 1)
	{
		string str = "January ";
		return(str.size());
	}
	else if (month == 2)
	{
		string str = "February ";
		return(str.size());
	}
	else if (month == 3)
	{
		string str = "March ";
		return(str.size());
	}
	else if (month == 4)
	{
		string str = "April ";
		return(str.size());
	}
	else if (month == 5)
	{
		string str = "May ";
		return(str.size());
	}
	else if (month == 6)
	{
		string str = "June ";
		return(str.size());
	}
	else if (month == 7)
	{
		string str = "July ";
		return(str.size());
	}
	else if (month == 8)
	{
		string str = "August ";
		return(str.size());
	}
	else if (month == 9)
	{
		string str = "September ";
		return(str.size());
	}
	else if (month == 10)
	{
		string str = "October ";
		return(str.size());
	}
	else if (month == 11)
	{
		string str = "November ";
		return(str.size());
	}
	else if (month == 12)
	{
		string str = "December ";
		return(str.size());
	}
	else return(0);
}

int year_string_length(int const year)
{
	if (year < 10) return(1);
	else if (year < 100) return(2);
	else if (year < 1000) return(3);
	else if (year < 10000) return(4);
	else return(0);
}

int initial_string_size_adjustment(int const year, int const month)
{
	// I use 30 to accommodate for dividing odd integers by 2
	int const size = year_string_length(year) + month_string_length(month);
	int const half_size = (year_string_length(year) + month_string_length(month)) / 2;
	if (size == (half_size * 2)) return((29-size)/2);
	else if (size != (half_size * 2)) return((30 - size) / 2);
	else return(0);
}

int final_string_size_adjustment(int const year, int const month)
{
	int const size = year_string_length(year) + month_string_length(month);
	int const half_size = (year_string_length(year) + month_string_length(month)) / 2;
	if (size == (half_size * 2)) return((30 - size) / 2);
	else if (size != (half_size * 2)) return((29 - size) / 2);
	else return(0);
}

void print_space(int const n)
{
	if (n > 1)
	{
		print(" ");
		print_space(n - 1);
	}
}

void print_month(int const year, int const month)
{
	int const i = initial_string_size_adjustment(year, month);
	int const f = final_string_size_adjustment(year, month);
	if (month == 1)
	{
		print("|"); print_space(i); print("January "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 2)
	{
		print("|"); print_space(i); print("February "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 3)
	{
		print("|"); print_space(i); print("March "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 4)
	{
		print("|"); print_space(i); print("April "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 5)
	{
		print("|"); print_space(i); print("May "); print(year); print_space(f); print("|");  new_line();
	}
	if (month == 6)
	{
		print("|"); print_space(i); print("June "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 7)
	{
		print("|"); print_space(i); print("July "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 8)
	{
		print("|"); print_space(i); print("August "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 9)
	{
		print("|"); print_space(i); print("September "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 10)
	{
		print("|"); print_space(i); print("October "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 11)
	{ 
		print("|"); print_space(i); print("November "); print(year); print_space(f); print("|"); new_line();
	}
	if (month == 12)
	{
		print("|"); print_space(i); print("December "); print(year); print_space(f); print("|"); new_line();
	}
}

void print_week_header()
{
	print("|Sun Mon Tue Wed Thu Fri Sat|"); new_line();
}

void print_calendar_header(int const year, int const month)
{
	new_calendar_line();
	print_month(year, month);
	new_line();
	print_week_header();
	new_calendar_line();
}

void align_top_row(int const year, int const month, int const N)
{
	if (day_of_the_week(year, month, N) == 0)
	{
		print("| "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 1)
	{
		print("|   | "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 2)
	{
		print("|   |   | "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 3)
	{
		print("|   |   |   | "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 4)
	{
		print("|   |   |   |   | "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 5)
	{
		print("|   |   |   |   |   | "); print(N); print(" ");
	}
	else if (day_of_the_week(year, month, N) == 6)
	{
		print("|   |   |   |   |   |   | "); print(N); print(" ");
	}
}

void align_bottom_row(int const year, int const month, int const N)
{
	if (day_of_the_week(year, month, N) == 0)
	{
		print("| "); print(N); print("|   |   |   |   |   |   |");
		new_line();  new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 1)
	{
		print("| "); print(N); print("|   |   |   |   |   |");
		new_line(); new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 2)
	{
		print("| "); print(N); print("|   |   |   |   |");
		new_line(); new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 3)
	{
		print("| "); print(N); print("|   |   |   |");
		new_line(); new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 4)
	{
		print("| ");  print(N); print("|   |   |");
		new_line(); new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 5)
	{
		print("| "); print(N); print("|   |");
		new_line(); new_calendar_line();
	}
	else if (day_of_the_week(year, month, N) == 6)
	{
		print("| "); print(N); 
	}
}

void print_nice(int const year, int const month, int const N)
{
	if (N == 1)
	{
		align_top_row(year, month, N);
	}
	else if (N >= 2 && N <= 9)
	{
		print("| "); print(N); print(" ");
	}
	else if (N >= 10 && N < month_length(year, month))
	{
		print("| "); print(N); print("");
	}
	else if (N == month_length(year, month))
	{
		align_bottom_row(year, month, N);
	}
	else print("error print_nice");

}

void calendar_body_recursion(int const year, int const month, int const N)
{
	if (N > 0)
	{
		calendar_body_recursion(year, month, N - 1);
		if (day_of_the_week(year, month, N) == 6)
		{
			print_nice(year, month, N);
			print("|"); new_line();
			new_calendar_line(); 
		}
		else if (day_of_the_week(year, month, N) != 6)
		{
			print_nice(year, month, N);
		}
	}
}

void print_calendar_body(int const year, int const month)
{
	int const number_of_days = month_length(year, month);
	calendar_body_recursion(year, month, number_of_days);
}

void print_calendar(int const year, int const month)
{
	new_line();//better to have a space before the calendar
	print_calendar_header(year, month);
	print_calendar_body(year, month);
}

int main()
{
	cout << "Enter your desired year \n";
	int const year = read_int(); // get_calendar_date()/10000; //random_in_range(0,10000);
	cout << "Enter your desired month \n";
	int const month = read_int(); //get_calendar_date()/100%100; //random_in_range(1,12);
	print_calendar(year, month);
	new_line();
	return 0;
}
