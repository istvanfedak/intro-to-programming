//This program lets you graph data from a .txt file in a specific format

# include "library.h"

// ----------------- conversions --------------------
double  celsius_to_fahrenheit(double celcius){
	return (celcius*(9.0 / 5.0)) + 32;
}

double mm_to_10s_of_inch(double mm){
	return mm/2.54;
}

double m_s_to_mi_hr(double i){
	// (m/s)*(100cm/m)*(1in/2.54cm)*(1ft/12in)*(1mi/5280ft)*(60s/1min)*(60min/1hr) = 2.237
	return i*2.237;
}

//---------------------------------------------------

//----------------- Imported code from Lab6----------
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

//--------------------------------------------------

	// Graphing section
void draw_temp_graph(int const Xpos, int const Ypos, int const width, int const height, string const file_name, string const type)
{
	set_pen_width(4);
	ifstream fin(file_name);
	if (fin.fail())
	{
		cout << "can't open " << file_name << endl;
	}
	else
	{
		cout << file_name << " opened properly" << endl;

		double year, month, day, min_t, avg_t, max_t, new_snow, snow_melted, max_wind;
		fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
		double Xpos_scale = day_of_the_year(year, month, day)*(width / year_length(year)) + Xpos;
		double Ypos_scale = (height / 170.0);
		//initial conversions
		min_t = celsius_to_fahrenheit(min_t);
		avg_t = celsius_to_fahrenheit(avg_t);
		max_t = celsius_to_fahrenheit(max_t);

		int previous_day = 0;

		if (type == "max_t")
		{
			move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - max_t * Ypos_scale);
		}
		else if (type == "min_t")
		{
			move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - min_t * Ypos_scale);
		}
		else if (type == "avg_t")
		{
			move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - avg_t * Ypos_scale);
		}

		while (true)
		{
			if (fin.eof()) break;
			Xpos_scale = day_of_the_year(year, month, day)*(width / (double)year_length(year)) + Xpos;

			if (type == "max_t")
			{
				if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || max_t>120 || max_t<-50)
				{
					move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - max_t * Ypos_scale);
				}
				else draw_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - max_t * Ypos_scale);
			}
			else if (type == "min_t")
			{
				if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || min_t>120 || min_t<-50)
				{
					move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - min_t * Ypos_scale);
				}
				else draw_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - min_t * Ypos_scale);
			}
			else if (type == "avg_t")
			{
				if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || avg_t>120 || avg_t<-50)
				{
					move_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - avg_t * Ypos_scale);
				}
				else draw_to(Xpos_scale, Ypos + (height - 50 * Ypos_scale) - avg_t * Ypos_scale);
			}
			previous_day = day;
			fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
			//conversions
			min_t = celsius_to_fahrenheit(min_t);
			avg_t = celsius_to_fahrenheit(avg_t);
			max_t = celsius_to_fahrenheit(max_t);
		}
		fin.close();
	}
}

void draw_new_snow(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	set_pen_width(4);
	ifstream fin(file_name);
	if (fin.fail())
	{
		cout << "can't open " << file_name << endl;
	}
	else
	{
		double year, month, day, min_t, avg_t, max_t, new_snow, snow_melted, max_wind;
		fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
		//initial conversion
		new_snow = mm_to_10s_of_inch(new_snow);
		
		double X_scale = width / (double)year_length(year);
		double Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;
		double Ypos_scale = (height / 50.0);

		move_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - new_snow * Ypos_scale);

		int previous_day = 0;
		while (true)
		{
			if (fin.eof()) break;
			Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;

			if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || new_snow>40 || new_snow<-1)
			{
				move_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - new_snow * Ypos_scale);
			}
			else draw_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - new_snow * Ypos_scale);
			previous_day = day;

			fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
			//conversion
			new_snow = mm_to_10s_of_inch(new_snow);
		}
	}
}

void draw_snow_melted(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	set_pen_width(4);
	ifstream fin(file_name);
	if (fin.fail())
	{
		cout << "can't open " << file_name << endl;
	}
	else
	{
		double year, month, day, min_t, avg_t, max_t, new_snow, snow_melted, max_wind;
		fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
		//initial conversion 
		snow_melted = mm_to_10s_of_inch(snow_melted);
		
		double X_scale = width / (double)year_length(year);
		double Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;
		double Ypos_scale = (height / 50.0);

		move_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - snow_melted * Ypos_scale);

		int previous_day = 0;
		while (true)
		{
			if (fin.eof()) break;
			Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;

			if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || snow_melted>40 || snow_melted<-1)
			{
				move_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - snow_melted * Ypos_scale);
			}
			else draw_to(Xpos_scale, Ypos + (height - 11 * Ypos_scale) - snow_melted * Ypos_scale);
			previous_day = day;

			fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
			//conversion 
			snow_melted = mm_to_10s_of_inch(snow_melted);
		}
	}
}

void draw_max_wind(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	set_pen_width(4);
	ifstream fin(file_name);
	if (fin.fail())
	{
		cout << "can't open " << file_name << endl;
	}
	else
	{
		double year, month, day, min_t, avg_t, max_t, new_snow, snow_melted, max_wind;
		fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
		//initial conversion
		max_wind = m_s_to_mi_hr(max_wind);

		double X_scale = width / (double)year_length(year);
		double Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;
		double Ypos_scale = (height / 80.0);

		move_to(Xpos_scale, Ypos + height  - max_wind * Ypos_scale);

		int previous_day = 0;
		while (true)
		{
			if (fin.eof()) break;
			Xpos_scale = day_of_the_year(year, month, day)*X_scale + Xpos;

			if (day_of_the_year(year, month, previous_day) + 1 != day_of_the_year(year, month, day) || max_wind>80 || max_wind<-1)
			{
				move_to(Xpos_scale, Ypos + height - max_wind * Ypos_scale);
			}
			else draw_to(Xpos_scale, Ypos + height - max_wind * Ypos_scale);
			previous_day = day;

			fin >> year >> month >> day >> min_t >> avg_t >> max_t >> new_snow >> snow_melted >> max_wind;
			//conversion
			max_wind = m_s_to_mi_hr(max_wind);
		}
	}
}


	// Background drawing section
void draw_temp_background(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	move_to(Xpos, Ypos);
	set_pen_width(3);

	string const s[12] = { "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D" };
	double Xpos_scale = (width /12.0);

	//X_axis
	int k = 0;
	double m = width / 24.0;
	for (int i = 0; i <= width; i += Xpos_scale)
	{
		move_to(m+Xpos, Ypos + height+0.0);
		set_pen_width(5);
		if(k<12) write_string(s[k], direction::south);
		set_pen_width(3);
		move_to(i + Xpos, Ypos + height);
		draw_distance(height);
		k++;
		m += width / 12.0;
	}

	//Y_axis
	move_to(Xpos, Ypos + height);
	set_heading_degrees(90);
	int j = -50;
	for (int i = 0; i < height; i += (height / 17.0))
	{	
		move_to(Xpos - 15, Ypos + height - i);
		set_pen_width(5);
		write_string(j, direction::west);
		
		set_pen_width(3);
		move_to(Xpos, Ypos + height - i);
		draw_distance(width);
		j += 10;
	}
}

void draw_snow_background(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	move_to(Xpos, Ypos);
	set_pen_width(3);

	string const s[12] = { "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D" };
	double Xpos_scale = (width / 12.0);

	//X_axis
	int k = 0;
	double m = width / 24.0;
	for (int i = 0; i <= width; i += Xpos_scale)
	{
		move_to(m + Xpos, Ypos + height + 0.0);
		set_pen_width(5);
		if (k<12) write_string(s[k], direction::south);
		set_pen_width(3);
		move_to(i + Xpos, Ypos + height);
		draw_distance(height);
		k++;
		m += width / 12.0;
	}

	//Y_axis
	move_to(Xpos, Ypos + height);
	set_heading_degrees(90);
	int j = -10;
	for (int i = 0; i <= height; i += (height / 10.0))
	{
		move_to(Xpos - 15, Ypos + height - i);
		set_pen_width(5);
		write_string(j, direction::west);

		set_pen_width(3);
		move_to(Xpos, Ypos + height - i);
		draw_distance(width);
		j += 5;
	}
}

void draw_wind_background(int const Xpos, int const Ypos, int const width, int const height, string const file_name)
{
	move_to(Xpos, Ypos);
	set_pen_width(3);

	string const s[12] = { "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D" };
	double Xpos_scale = (width / 12.0);

	//X_axis
	int k = 0;
	double m = width / 24.0;
	for (int i = 0; i <= width; i += Xpos_scale)
	{
		move_to(m + Xpos, Ypos + height + 0.0);
		set_pen_width(5);
		if (k<12) write_string(s[k], direction::south);
		set_pen_width(3);
		move_to(i + Xpos, Ypos + height);
		draw_distance(height);
		k++;
		m += width / 12.0;
	}

	//Y_axis
	move_to(Xpos, Ypos + height);
	set_heading_degrees(90);
	int j = 0;
	for (int i = 0; i <= height; i += (height / 8.0))
	{
		move_to(Xpos - 15, Ypos + height - i);
		set_pen_width(5);
		write_string(j, direction::west);

		set_pen_width(3);
		move_to(Xpos, Ypos + height - i);
		draw_distance(width);
		j += 10;
	}
}

	// user interface section
void find_graph(string & graph_in, string graph_types[])
{
	
	cout << endl << " What would you be graphing today(type list if unsure):: ";
	cin >> graph_in;
	if (graph_in == "list" || graph_in == "min_temp" || graph_in == "avg_temp" || graph_in == "max_temp" || graph_in == "snow_fall" || graph_in == "precipitation" || graph_in == "wind_speed"|| graph_in == "min_max_temp")
	{
		if (graph_in == "list")
		{
			for (int i = 0; i < 7; i++)
			{
				cout << "   " << graph_types[i] << endl;
			}
			new_line();
			cout << endl << " What would you be graphing today:: ";
			cin >> graph_in;
			if (graph_in == "list")
			{
				cout << endl << " Opps... I already gave you a list!!" << endl;
				find_graph(graph_in, graph_types);
			}
		}
	}
	else
	{
		cout << endl << " Opps... I don't recognize that value ";
		find_graph(graph_in, graph_types);
	}
}

string grapher_start(string & graph_in, string graph_types[])
{
	string s;
	/*string const colors[26] = { "1-black", "2-dark_grey", "3-grey", "4-light_grey", "5-white", "6-dark_red", "red", "7-light_red", "8-pink", "9-dark_blue", "10-blue", "11-light_blue", "12-cyan", "13-dark_green", "14-green", "15-light_green", "16-lime_green", "17-yellow", "18-sodium_d", "19-orange", "20-brown", "21-indigo", "22-mauve", "23-violet", "24-purple", "25-magenta" };*/
	/*string const color_int[26] = { "black", "dark_grey", "grey", "light_grey", "white", "dark_red", "red", "light_red", "pink", "dark_blue", "blue", "light_blue", "cyan", "dark_green", "green", "light_green", "lime_green", "yellow", "sodium_d", "orange", "brown", "indigo", "mauve", "violet", "purple", "magenta" };*/
	cout << " Welcome to Weather Grapher!" << endl << endl;
	cout << " Our new grapher600x app is the most up to date with current rabbit standards" << endl;
	cout << " If you are a new user type help for instructions, else enter 'n'" << endl;
	cout << " ";
	cin >> s;
	if (s == "help")
	{
		cout << endl << " Intructions of use: " << endl;
		cout << " Weather Grapher is an application that helps users graph weather data" << endl << endl;
		cout << " You can obtain the following six graphs:" << endl;
		cout << " 1) The Minimum Temperature recorded on that day (Fahrenheit)" << endl;
		cout << "      - type 'min_temp' to graph" << endl;
		cout << " 2) The Average Temperature recorded during that day (Fahrenheit)" << endl;
		cout << "      - type 'avg_temp' to graph" << endl;
		cout << " 3) The Maximum Temperature recorded on that day (Fahrenheit)" << endl;
		cout << "      - type 'max_temp' to graph" << endl;
		cout << " 4) The Depth of new Snow fall (tenths of an inch)" << endl;
		cout << "      - type 'snow_fall' to graph" << endl;
		cout << " 5) Total amount of Precipitation, incl. snow melted (tenths of an inch)" << endl;
		cout << "      - type 'precipitation' to graph" << endl;
		cout << " 6) The Maximum Wind speed observed (miles per hour)" << endl;
		cout << "      - type 'wind_speed' to graph" << endl << endl;

		cout << " You can also pick a custom color for your graph" << endl;
		cout << " The colors are in RGB" << endl << endl;;
	}
	string ret = "d";
	cout << endl << " Enter the name of the file you wnat to graph(input d for default):: ";
	cin >> ret;
	//graph
	find_graph(graph_in, graph_types);
	return ret;

}

void main()
{
	string graph_types[7] = { "min_temp", "avg_temp", "max_temp", "snow_fall", "precipitation", "wind_speed", "min_max_temp"};
	string graph_in;
	string file_name = grapher_start(graph_in, graph_types);
	if (file_name == "d") file_name = "lab9_data.txt";
	
	//--------------finding color---------------
	int red, green, blue, red2, green2, blue2;
	cout << endl << " Thank you!" << endl << endl;
	cout << "Imput your red, green, and blue colors (ex: 2 3 4):: ";
	cin >> red;
	cout << " ";
	cin >> green;
	cout << " ";
	cin >> blue;
	cout << endl << " Thank you!" << endl << endl;
	if (graph_types[6] == graph_in)
	{
		cout << "Imput your second color red, green, and blue colors (ex: 2 3 4):: ";
		cin >> red2;
		cout << " ";
		cin >> green2;
		cout << " ";
		cin >> blue2;
		cout << endl << " Thank you!" << endl << endl;
	}
	//--------------------------------------------

	make_window(1280, 720);
	set_pen_width(5);
	if (graph_types[0] == graph_in)
	{
		draw_temp_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_temp_graph(200, 200, 600, 400, file_name, "min_t");
	}
	if (graph_types[1] == graph_in)
	{
		draw_temp_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_temp_graph(200, 200, 600, 400, file_name, "avg_t");
	}
	if (graph_types[2] == graph_in)
	{
		draw_temp_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_temp_graph(200, 200, 600, 400, file_name, "max_t");
	}
	if (graph_types[3] == graph_in)
	{
		draw_snow_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_new_snow(200, 200, 600, 400, file_name);
	}
	if (graph_types[4] == graph_in)
	{
		draw_snow_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_snow_melted(200, 200, 600, 400, file_name);
	}
	if (graph_types[5] == graph_in)
	{
		draw_wind_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_max_wind(200, 200, 600, 400, file_name);
	}
	if (graph_types[6] == graph_in)
	{
		draw_temp_background(200, 200, 600, 400, file_name);
		set_pen_color_int(red, green, blue);
		draw_temp_graph(200, 200, 600, 400, file_name, "max_t");
		set_pen_color_int(red2, green2, blue2);
		draw_temp_graph(200, 200, 600, 400, file_name, "min_t");
	}
}
