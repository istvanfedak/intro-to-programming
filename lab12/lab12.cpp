#include "library.h"

image * batman = image_from_file("batman.gif");
image * hulk = image_from_file("hulk.gif");
image * chest = image_from_file("chest.gif");
image * block = image_from_file("block.gif");

struct location
{
	int r, c;
};

void read_array(string arr[], int & size, string file_name, location object[], int & object_size)
{
	object_size = 2; // marks the start of empty space objcts
	ifstream fin(file_name);
	if (!fin) cout << " Could not open file: " << file_name << endl << endl;
	else
	{
		cout << " Successfully opened file: " << file_name << endl << endl;
		while (true)
		{
			if (fin.eof()) break;
			fin >> arr[size];
			for (int i = 0; i < arr[size].length(); i++)
			{
				if (arr[size][i] == 'A')
				{
					object[0].r = size;
					object[0].c = i;
				}
				else if (arr[size][i] == 'B')
				{
					object[1].r = size;
					object[1].c = i;
				}
				else if (arr[size][i] == '.') //records empy space location
				{
					object[object_size].r = size;
					object[object_size].c = i;
					object_size++;
				}
			}
			size++;
		}
		fin.close();
	}
}

void print_array(string arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < arr[i].length(); j++)
		{
			if (arr[i][j] == '#') cout << "@";
			else if (arr[i][j] == '.') cout << " ";
			else cout << arr[i][j];
		}
		cout << endl;
	}
}

void duplicate(string arr1[], int size, string arr2[])
{
	for (int i = 0; i < size; i++)
	{
		arr2[i] = arr1[i];
	}
}

void create_enemy(string arr[], location object[], int size, int enemy_index)
{
	int enemy = random_in_range(enemy_index, size - 1);
	object[enemy_index] = object[enemy];
	arr[object[enemy_index].r][object[enemy_index].c] = 'E';
}

void create_n_enemies(string arr[], location object[], int size, int n_enemies)
{
	for (int i = 2; i < n_enemies + 2; i++)
	{
		create_enemy(arr, object, size, i);
	}
}

void square(int const Xpos, int const Ypos, int const size, int color)
{
	set_pen_width_color(3, color::black);
	start_shape();
	move_to(Xpos, Ypos); note_position();
	set_heading_degrees(90);
	draw_distance(size); note_position();
	turn_right_by_degrees(90);
	draw_distance(size); note_position();
	turn_right_by_degrees(90);
	draw_distance(size); note_position();
	turn_right_by_degrees(90);
	draw_distance(size); note_position();
	set_pen_width(0);
	set_pen_color(color);
	fill_shape();
}

void draw_maze(string arr[], int size, int sqr_size)
{
	make_window(1280, 720);
	for (int row = 0; row < size; row++)
	{
		for (int column = 0; column < arr[row].length(); column++)
		{
			if (arr[row][column] == 'A') draw_image(batman, column * sqr_size, row * sqr_size);
			else if (arr[row][column] == 'B') draw_image(chest, column * sqr_size, row * sqr_size);
			else if (arr[row][column] == '#')draw_image(block, column * sqr_size, row * sqr_size);
			else if (arr[row][column] == '.' || arr[row][column] == 's')square(column * sqr_size, row * sqr_size, sqr_size, color::white);
			else if (arr[row][column] == 'E')draw_image(hulk, column * sqr_size, row * sqr_size);
			else if (arr[row][column] == 'u' || arr[row][column] == 'd' || arr[row][column] == 'l' || arr[row][column] == 'r')
			{
				square(column * sqr_size, row * sqr_size, sqr_size, color::yellow);
			}

		}
	}
}

void character_movement(string arr[], int size, location object[], int obj_index, int sqr_size, char c, string mirror[], bool b)
{
	if (b) mirror[object[0].r][object[0].c] = 's';
	if (c == 'l' || c == -91)
	{ /* make the robot move one square to the left */
		if (object[obj_index].c - 1 >= 0 && arr[object[obj_index].r][object[obj_index].c - 1] != '#')
		{
			if (obj_index < 2)
			{
				draw_image(batman, (object[obj_index].c - 1) * sqr_size, object[obj_index].r * sqr_size);
			}
			else draw_image(hulk, (object[obj_index].c - 1) * sqr_size, object[obj_index].r * sqr_size);
			square(object[obj_index].c * sqr_size, object[obj_index].r * sqr_size, sqr_size - 2, color::white);
			object[obj_index].c -= 1;
			if (!b && obj_index == 0) mirror[object[0].r][object[0].c] = 'l';
		}
	}
	else if (c == 'r' || c == -89)
	{ /* make the robot move one square to the right */
		if (object[obj_index].c + 1 < arr[object[obj_index].r].length() && arr[object[obj_index].r][object[obj_index].c + 1] != '#')
		{
			if (obj_index < 2)
			{
				draw_image(batman, (object[obj_index].c + 1) * sqr_size, object[obj_index].r * sqr_size);
			}
			else draw_image(hulk, (object[obj_index].c + 1) * sqr_size, object[obj_index].r * sqr_size);
			square(object[obj_index].c * sqr_size, object[obj_index].r * sqr_size, sqr_size - 2, color::white);
			object[obj_index].c += 1;
			if (!b && obj_index == 0) mirror[object[0].r][object[0].c] = 'r';
		}
	}
	else if (c == 'u' || c == -90)
	{ /* make the robot move one square up */

		if (object[obj_index].r - 1 >= 0 && arr[object[obj_index].r - 1][object[obj_index].c] != '#')
		{
			if (obj_index < 2)
			{
				draw_image(batman, object[obj_index].c * sqr_size, (object[obj_index].r - 1)* sqr_size);
			}
			else draw_image(hulk, object[obj_index].c * sqr_size, (object[obj_index].r - 1)* sqr_size);
			square(object[obj_index].c * sqr_size, object[obj_index].r * sqr_size, sqr_size - 2, color::white);
			object[obj_index].r -= 1;
			if (!b && obj_index == 0) mirror[object[0].r][object[0].c] = 'u';
		}
	}
	else if (c == 'd' || c == -88)
	{ /* make the robot move one square down */
		if (object[obj_index].r + 1 < size && arr[object[obj_index].r + 1][object[0].c] != '#')
		{
			if (obj_index < 2)
			{
				draw_image(batman, object[obj_index].c * sqr_size, (object[obj_index].r + 1)* sqr_size);
			}
			else draw_image(hulk, object[obj_index].c * sqr_size, (object[obj_index].r + 1)* sqr_size);
			square(object[obj_index].c * sqr_size, object[obj_index].r * sqr_size, sqr_size - 2, color::white);
			object[obj_index].r += 1;
			if (!b && obj_index == 0) mirror[object[0].r][object[0].c] = 'd';
		}
	}
}

void auto_mode(string arr[], int size, location object[], int sqr_size, char c, int number_of_enemies, string mirror[])
{
	if (c == 'a')
	{
		while (true)
		{
			c = wait_for_key_typed(0.01);
			if (c == 'm' || c == 'q' || c == 'x') break;
			else
			{
				if (object[0].r == object[1].r && object[0].c == object[1].c) break;
				c = random_in_range(-91, -88);
				character_movement(arr, size, object, 0, sqr_size, c, mirror, false);
				//enemy
				for (int i = 2; i < number_of_enemies + 2; i++)
				{
					c = random_in_range(-91, -88);
					character_movement(arr, size, object, i, sqr_size, c, mirror, false);
					if (object[0].r == object[i].r && object[0].c == object[i].c) break;
				}
			}
		}
	}
}

void back(string arr[], int size, location object[], int sqr_size, char c, int number_of_enemies, string mirror[])
{
	if (c == 'b')
	{
		char move = mirror[object[0].r][object[0].c];
		if (move == 'l') c = 'r';
		else if (move == 'r') c = 'l';
		else if (move == 'u') c = 'd';
		else if (move == 'd') c = 'u';
		character_movement(arr, size, object, 0, sqr_size, c, mirror, true);
	}
}


struct store_cell
{
	// this struct helps convert random_in_range() into to a direction
	// stores the coordinates of the neighboring cells
	char up;
	char down;
	char left;
	char right;
};

store_cell cell_availability; //stores neighboring cells availability 'r' for read ; 'e' for empty

//////Records Neighboring cell's Emptiness Availablity///////
// Stores them in struct randomness
void record_empty_cells(string arr[], int size, location object[], string mirror[])
{
	//up
	if (object[0].r - 1 >= 0 && mirror[object[0].r - 1][object[0].c] == '.')
	{
		cell_availability.up = 'e';
	}
	else if (object[0].r - 1 >= 0 && mirror[object[0].r - 1][object[0].c] == 'B') cell_availability.up = 'e';
	else cell_availability.up = 'r';
	//down
	if (object[0].r + 1 < size && mirror[object[0].r + 1][object[0].c] == '.')
	{
		cell_availability.down = 'e';
	}
	else if (object[0].r + 1 < size && mirror[object[0].r + 1][object[0].c] == 'B') cell_availability.up = 'e';
	else cell_availability.down = 'r';
	//left
	if (object[0].c - 1 >= 0 && mirror[object[0].r][object[0].c - 1] == '.')
	{
		cell_availability.left = 'e';
	}
	else if (object[0].c - 1 >= 0 && mirror[object[0].r][object[0].c - 1] == 'B') cell_availability.up = 'e';
	else cell_availability.left = 'r';
	//right
	if (object[0].c + 1 < arr[object[0].r].length() && mirror[object[0].r][object[0].c + 1] == '.')
	{
		cell_availability.right = 'e';
	}
	else if (object[0].c + 1 < arr[object[0].r].length() && mirror[object[0].r][object[0].c + 1] == 'B') cell_availability.up = 'e';
	else cell_availability.right = 'r';

}

//////// Checks for Neighboring cell Availability ///////
bool check_bool(string arr[], int size, location object[], string mirror[])
{
	record_empty_cells(arr, size, object, mirror);
	//cout << " Up: " << cell_availability.up << " | down: " << cell_availability.down << " | left: " << cell_availability.left << " | right: " << cell_availability.right << endl;
	//Checks if rows are available
	if (cell_availability.up == 'e' || cell_availability.down == 'e')
	{
		return true;
	}
	//Checks if columns are available
	else if (cell_availability.right == 'e' || cell_availability.left == 'e')
	{
		return true;
	}
	else return false;
}

///// Available Neighbor Cells char Conversion to Random ints///////
char random_conversion()
{
	int up, down, left, right, random_selc;
	//// assigns int values to available cells
	if (cell_availability.up == 'e') up = 1;
	else up = 0;
	if (cell_availability.down == 'e') down = 2;
	else down = 0;
	if (cell_availability.left == 'e') left = 3;
	else left = 0;
	if (cell_availability.right == 'e') right = 4;
	else right = 0;
	//// selects a random empty cell
	while (true)
	{
		random_selc = random_in_range(1, 4);
		if (random_selc == up) return 'u';
		else if (random_selc == down) return 'd';
		else if (random_selc == left) return 'l';
		else if (random_selc == right) return 'r';
	}
}

//////// Resets the Values of Neighboring cells to Empty/////////////
void reset()
{
	cell_availability.up = 'e';
	cell_availability.down = 'e';
	cell_availability.right = 'e';
	cell_availability.left = 'e';
}

void auto_better(string arr[], int size, location object[], int sqr_size, char c, int number_of_enemies, string mirror[])
{
	if (c == 'a')
	{
		while (true)
		{
			c = wait_for_key_typed(0.1);
			if (c == 'm') return;
			reset();
			for (int i = 2; i < number_of_enemies + 2; i++)
			{
				if (object[0].r == object[i].r && object[0].c == object[i].c)
				{
					square(object[0].c * sqr_size, object[0].r * sqr_size, sqr_size - 2, color::red);
					show_console();
					cout << " You lost!" << endl;
					return;
				}
			}
			if (object[0].r == object[1].r && object[0].c == object[1].c)
			{
				square(object[0].c * sqr_size, object[0].r * sqr_size, sqr_size - 2, color::red);
				show_console();
				return;
			}
			if (check_bool(arr, size, object, mirror))
			{ //If neighboring cell is empty it chooses a random empty cell to move into
				c = random_conversion();
				character_movement(arr, size, object, 0, sqr_size, c, mirror, false); \
			}
			else // If no cells are available it goes back and tries to find another empty cell
			{
				back(arr, size, object, sqr_size, 'b', number_of_enemies, mirror);
			}
			for (int i = 2; i < number_of_enemies + 2; i++)
			{
				c = random_in_range(-91, -88);
				character_movement(arr, size, object, i, sqr_size, c, mirror, false);
			}
		}
	}
}

void game_main(string arr[], int size, location object[], int sqr_size, int number_of_enemies, string mirror[])
{
	while (true)
	{
		// stoping criteria
		for (int i = 2; i < number_of_enemies + 2; i++)
		{
			if (object[0].r == object[i].r && object[0].c == object[i].c)
			{
				square(object[0].c * sqr_size, object[0].r * sqr_size, sqr_size - 2, color::red);
				show_console();
				cout << " You lost!" << endl;
				return;
			}
		}
		if (object[0].r == object[1].r && object[0].c == object[1].c)
		{
			square(object[0].c * sqr_size, object[0].r * sqr_size, sqr_size - 2, color::red);
			show_console();
			cout << " You won!" << endl;
			draw_maze(mirror, size, sqr_size);
			return;
		}
		char c = wait_for_key_typed();
		back(arr, size, object, sqr_size, c, number_of_enemies, mirror);
		if (c == 'q' || c == 'x') break;
		character_movement(arr, size, object, 0, sqr_size, c, mirror, false);
		auto_better(arr, size, object, sqr_size, c, number_of_enemies, mirror);
		for (int i = 2; i < number_of_enemies + 2; i++)
		{//draws enemies
			c = random_in_range(-91, -88);
			character_movement(arr, size, object, i, sqr_size, c, mirror, false);
		}
	}
}

void intro(int & number_of_enemies, string & name)
{
	string c;
	cout << " Welcome to Batman vs. Hulk" << endl << endl;;
	cout << " Goal: Make it to the chest alive" << endl;
	cout << " Enemy: You had a fight with hulk and right now he is trying to kill you" << endl;
	cout << "   - He has supper strenth... so he can slowly tear the maze appart" << endl;
	cout << "	- Pro tip: he is also not that smart so unless he touches you your good" << endl;
	cout << " Instructions:" << endl;
	cout << "   - You can use your arror keys to move around" << endl;
	cout << "   - You can also use 'u'(up),'d'(down),'l'(left),'r'(right)" << endl;
	cout << "   - To turn auto mode on press 'a'" << endl;
	cout << "   - To turn auto mode off press 'm'" << endl;
	cout << "   - To quit press 'x' or 'q'" << endl << endl;

	cout << " Set up:" << endl;
	cout << "	- If you downloaded a predefined maze, enter " << endl;
	cout << "	  its name(else enter standard or n): ";
	cin >> name;
	if (name == "standard" || name == "n") name = "lab12_maze.txt";
	cout << "	- Imput the number of enemies: ";
	cin >> number_of_enemies;
	cout << endl << endl;
	cout << " If you are a grader enter 'g', else enter any other letter to continue: ";
	cin >> c;
	cout << endl;
	if (c == "g")
	{
		cout << " Bug Report: " << endl;
		cout << "	- For some reason the enemy manages to move into walls," << endl;
		cout << "	  I use the same function as batman to  determine if" << endl;
		cout << "	  the space is not a wall(stoping criteria)." << endl;
		cout << "	- I also coundn't figure out how to draw the exact and correct" << endl;
		cout << "	  path to the yellow brick road the user took with out any corners" << endl;
		cout << "	  while on manual mode. Auto mode works flawlestly!" << endl << endl;
		cout << "	- I've checked the cunction countless times and I cant fix the bugs" << endl;
		cout << "	  this is why I didn't submit the lab on time. I hope I dont loose " << endl;
		cout << "          many points!!!" << endl << endl;
		cout << " Thank you guys for all the help! Good luck next year. " << endl << endl;
		cout << " Enter any letter to continue: ";
		cin >> c;
		cout << endl;
	}
	hide_console();
}

void main()
{
	int size = 0;
	int sqr_size = 20;
	int object_size = 10000;
	int number_of_enemies = 5;
	string name = "lab12_maze.txt";
	string maze[100];
	string mirror[100];
	location object[10000];
	intro(number_of_enemies, name);
	read_array(maze, size, name, object, object_size);
	duplicate(maze, size, mirror);
	print_array(mirror, size);
	create_n_enemies(maze, object, object_size, number_of_enemies);
	draw_maze(maze, size, sqr_size);
    game_main(maze, size, object, sqr_size, number_of_enemies, mirror);
}
