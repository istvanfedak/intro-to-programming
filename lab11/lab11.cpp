#include "library.h"

//////////////////////////////////////////////////////////////
//////////////////// Random Maze Generator Code //////////////
//////////////////////////////////////////////////////////////
/*
Author: Istvan von Fedak
Algorigthm: Depth-first Search Random Maze Generator
Version 1.0
Date 4/6/2016
*/
/*
Improvement list
	- Makes the maze spawn a B location in a random end point
	- Make the maze store all the coordinates of the end points
	- Fix the scaling on the maze size
	- Make the maze recognize all unread locations
*/

////////////////// User parameters//////////////////////////
// As of right now... it only works with rows and colums having the same size
////////////////////User parameters///////////////////////////
//max number that fits on screen is 27
int const size = 23;//has to be an odd number
//int const n_rows = 13;
//int const n_columns = 13;
int const maze_width = size * 3, maze_length = size * 3;
char maze[maze_width][maze_length];

///////////////////// Structs ///////////////////////
struct pos
{
	// This struct helps me keep the start and end coordinates of the maze
	int column;
	int row;
};

struct store_cell
{
	// this struct helps convert random_in_range() into to a direction
	// stores the coordinates of the neighboring cells
	char up;
	char down;
	char left;
	char right;
};

pos A_pos; // start of the maze
pos B_pos; // end of the maze
pos E_pos;
store_cell cell_availability; //stores neighboring cells availability 'r' for read ; 'e' for empty

///////////////Generates Empty Maze Array///////////////////
/*
* creates grids of cells
* the following characters indicate walls: + - |
* the following character indicates an empty space: e
* 'A' stands for start
* 'B' could be used for finish
*/
void generate_blank_maze_array()
{

	for (int k = 0; k < maze_length; k++)
	{
		for (int h = 0; h < maze_width; h++)
		{
			if (k % 2 != 0)
			{
				if (h % 2 == 0) maze[k][h] = '#';
				else maze[k][h] = 'e';
				if (k == maze_length - 2 && h == maze_width - 2)
				{
					A_pos.row = k;
					A_pos.column = h;
					maze[k][h] = 'A';
				}
				if (k == 3 && h == 3)
				{
					B_pos.row = k;
					B_pos.column = h;
					//blank_maze[k][h] = 'B';
					//if you enable 'B' then it will always be sorounded by a wall
				}
			}
			else
			{
				maze[k][h] = '#';
			}
		}
	}
}

///////////////// Prints the maze ///////////////////////
void print_maze()
{
	for (int k = 0; k < maze_length; k++)
	{

		for (int h = 0; h < maze_width; h++)
		{
			cout << maze[k][h];
			/*
			if (maze[k][h] == '+' || maze[k][h] == '-' || maze[k][h] == '|' || maze[k][h] == '#')
			{
				// you can customize your walls by changing this character
				cout << "#";
			}
			else if (maze[k][h] == 'e' || maze[k][h] == 'r' || maze[k][h] == '.')
			{
				//You can change the character that represents empty space
				cout << " ";
			}
			else cout << maze[k][h];
			//*/
		}
		cout << endl;
	}
}


//////Records Neighboring cell's Emptiness Availablity///////
// Stores them in struct randomness
void record_empty_cells(int c, int r)
{
	if (r - 2 > 0 && maze[r - 2][c] == 'e')//up
	{
		cell_availability.up = 'e';
	}
	else cell_availability.up = 'r';
	if (r + 2 < maze_length && maze[r + 2][c] == 'e')//down
	{
		cell_availability.down = 'e';
	}
	else cell_availability.down = 'r';
	if (c - 2 > 0 && maze[r][c - 2] == 'e')//left
	{
		cell_availability.left = 'e';
	}
	else cell_availability.left = 'r';
	if (c + 2 < maze_width && maze[r][c + 2] == 'e')//right
	{
		cell_availability.right = 'e';
	}
	else cell_availability.right = 'r';
}

/*//// Available Neighbor Cells char Conversion to Random ints///////
int random_conversion()
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
		if (random_selc == up)
		{
			return 1;
			break;
		}
		else if (random_selc == down)
		{
			return 2;
			break;
		}
		else if (random_selc == left)
		{
			return 3;
			break;
		}
		else if (random_selc == right)
		{
			return 4;
			break;
		}
		else if ((up + down + left + right) == 0)
		{
			cout << "Boolean error!" << endl;
			/*
			* The sum of all movements should not be 0 because of the
			*  check_bool() function. If "Boolean error!" apears on your
			*  console you should check that function
			//*
			return 5;
			break;
		}
		
	}
}//*/
///// Available Neighbor Cells char Conversion to Random ints///////
int random_conversion(int c, int r)
{
	int up, down, left, right, random_selc;
	//// assigns int values to available cells
	if (r - 2 > 0 && maze[r - 2][c] == 'e') up = 1;
	else up = 0;
	if (r + 2 < maze_length && maze[r + 2][c] == 'e') down = 2;
	else down = 0;
	if (c - 2 > 0 && maze[r][c - 2] == 'e') left = 3;
	else left = 0;
	if (c + 2 < maze_width && maze[r][c + 2] == 'e') right = 4;
	else right = 0;

	//// selects a random empty cell
	while (true)
	{
		random_selc = random_in_range(1, 4);
		if (random_selc == up)
		{
			return 1;
			break;
		}
		else if (random_selc == down)
		{
			return 2;
			break;
		}
		else if (random_selc == left)
		{
			return 3;
			break;
		}
		else if (random_selc == right)
		{
			return 4;
			break;
		}
		else if ((up + down + left + right) == 0)
		{
			cout << "Boolean error!" << endl;
			/*
			* The sum of all movements should not be 0 because of the
			*  check_bool() function. If "Boolean error!" apears on your
			*  console you should check that function
			//*/
			return 5;
			break;
		}

	}
}



//////// Checks for Neighboring cell Availability ///////
bool check_bool(int c, int r)
{
	record_empty_cells(c, r);
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
	/*	If none unvisited cells available then the cell is considered a dead-end,
	this will cause the function to backtracks through the path until it
	reaches a cell with an unvisited neighbour cell or the start 'A'*/
	else return false;
}

//////// Resets the Values of Neighboring cells to Empty/////////////
void reset()
{
	cell_availability.up = 'e';
	cell_availability.down = 'e';
	cell_availability.right = 'e';
	cell_availability.left = 'e';
}

///////// Converts Random Maze to a Simpler Format///////
void convert_maze()
{
	for (int k = 0; k < maze_length; k++)
	{
		for (int h = 0; h < maze_width; h++)
		{
			if (maze[k][h] == 'e')
			{
				maze[k][h] = '#';
			}
			else if (maze[k][h] == 'r')
			{
				maze[k][h] = '.';
			}
			if (k == 3 && h == 3)
			{
				maze[k][h] = 'B';
			}
		}
	}
}

////////////////// Creates Random Maze Array /////////////////////
// 'r' stands as a read cell
// '.' stands as a path
void generate_random_maze_array(int c, int r)
{
	int history_size = 1000;
	int moves_history[1000];// The moves history array records all the previous movements
	int i = 0;
	while (true)
	{
		if (check_bool(c, r))
		{ //If neighboring cell is empty it chooses a random empty cell to move into
			moves_history[i] = random_conversion(c,r);

			if (moves_history[i] == 1)//up
			{
				maze[r - 2][c] = 'r';
				maze[r - 1][c] = '.';
				r -= 2;
			}
			else if (moves_history[i] == 2) // down
			{
				maze[r + 2][c] = 'r';
				maze[r + 1][c] = '.';
				r += 2;
			}
			else if (moves_history[i] == 3) // left
			{
				maze[r][c - 2] = 'r';
				maze[r][c - 1] = '.';
				c -= 2;
			}
			else if (moves_history[i] == 4) // right
			{
				maze[r][c + 2] = 'r';
				maze[r][c + 1] = '.';
				c += 2;
			}
			//if (moves_history[i] == 5) continue;
			i++;
			reset();
		}
		else // If no cells are available it goes back and tries to find another empty cell
		{
			if (moves_history[i - 1] == 1)
			{
				r += 2;
				i -= 1;
			}
			else if (moves_history[i - 1] == 2)
			{
				r -= 2;
				i -= 1;
			}

			else if (moves_history[i - 1] == 3)
			{
				c += 2;
				i -= 1;
			}
			else if (moves_history[i - 1] == 4)
			{
				c -= 2;
				i -= 1;
			}
			else if (i == 0) break;// if it returns back to the start the maze is complete
			reset();
		}
	}
	convert_maze();
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void generate_enemy()
{
	int seed = random_in_range(1, size * 3 - 3);
	E_pos.column = seed;
	E_pos.row = seed;
	maze[seed][seed] = 'E';
}
void enemy_movement()
{
	
}

void rect(int const Xpos, int const Ypos, int const width, int const height, int color)
{
	set_pen_width_color(3, color::black);
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
	set_pen_width(0);
	set_pen_color(color);
	fill_shape();
}

void block_of_rects(char maze[], int const Xpos, int const Ypos, int const block_size, int const height, int const i, int const j, int const color)
{
	int X_size = 0;
	int Y_size = 0;
	for (int n = 0; n < i; n++)
	{
		X_size = 0;
		for (int m = 0; m < j; m++)
		{
			rect(Xpos + X_size, Ypos + Y_size, block_size, block_size, color);
			X_size += block_size;
		}
		Y_size += block_size;
	}
}

void resizer(double const i, double const j, int & block_size)
{
	int win_width = get_window_width();
	int win_height = get_window_height();
	for (int bs = 40; true; bs -= 1)
	{
		if (j * bs+150 <= 620 && i * bs+150 <= 620)
		{
			block_size = bs;
			break;
		}
		else if (bs < 4)
		{
			cout << "sorry the size of the windo is too small to be playable\n";
			move_to(10, 10);
			set_pen_color(color::red);
			set_pen_width(5);
			write_string("sorry the size of the windo is too small to be playable", direction::south_east);
			block_size = 0;
			break;
		}
	}

}

void main()
{
	cout << "Sorry for the lack of UI\n";
	cout << " type: x, q, ESC to quit\n";
	cout << " To move: w is up, s is down, a is left, d is right\n";
	cout << " activate auto mode by typing A \n";
	cout << " to get out of auto mode type m \n";
	cout << " to increase the size of the maze go to the top, \n";
	cout << " change the int size to an odd int \n";
	cout << " i'm still working to perfect the maze generator\n";
	cout << " if you have any questions feel free to contact me \n";

	generate_blank_maze_array();
	//print_maze();
	generate_random_maze_array(A_pos.row, A_pos.column);
	generate_enemy();
	//print_maze();
	//cout << " Random Maze:" << endl;
	//print_maze();

	/*
	int const n = 80;
	int i = 0, j = 0;
	char maze[n][n];

	ifstream fin("themaze.txt");
	if (fin.fail())
	{
		cout << "Could not open file. ";
	}
	else
	{
		while (true)
		{
			j = 0;
			do
			{

				if (fin.eof())break;
				fin >> maze[i][j];
				j++;
				//cout << "test";
			} while (fin.peek() != '\n');
			if (fin.eof())
			{
				i++;
				break;
			}
			i++;
		}
	}
	fin.close();
	//*/

	//////////////////////Part 3 ///////////////////////////
	int x = 760, y = 760;
	int block_size;
	make_window(x, y);
	resizer(maze_length, maze_width, block_size);
	// drawing the maze
	int Xpos = 0, Ypos = 0;
	int X_size = Xpos, Y_size = Ypos;
	//int const Xpos = 0, Ypos = 0;
	for (int n = 0; n < maze_length; n++)
	{
		X_size = Xpos;
		for (int m = 0; m < maze_width; m++)
		{
			move_to(0, block_size);
			//cout << "test";
			if (maze[n][m] == '#')
				rect(X_size, Y_size, block_size, block_size, color::dark_grey);
			else if (maze[n][m] == '.')
				rect(X_size, Y_size, block_size, block_size, color::white);
			else if (maze[n][m] == 'A')
				rect(X_size, Y_size, block_size, block_size, color::green);
			else if (maze[n][m] == 'B')
				rect(X_size, Y_size, block_size, block_size, color::dark_blue);
			else if (maze[n][m] == 'E')
				rect(X_size, Y_size, block_size, block_size, color::red);
			X_size += block_size + 6;
		}
		Y_size += block_size + 6;
	}
	//////////////////////////////////////////
	//////////////////Part 4 and 5////////////
	block_size += 6;
	int const m_size = 1000;
	char memory[m_size];
	for (int i = 0; true;i++)
	{
		char c = wait_for_key_typed();
		//enemy
		while (true)
		{
			int r = random_in_range(1, 4);
			if (r == 1)//up
			{ 
				if (maze[E_pos.row - 1][E_pos.column] != '#' && maze[E_pos.row - 1][E_pos.column] != 'B')
				{
					rect(E_pos.column*block_size, E_pos.row*block_size - block_size, block_size - 6, block_size - 6, color::red);
					rect(E_pos.column*block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::white);
					E_pos.row--;
					break;
				}
				else break;
			}
			if (r == 2)//down
			{ 
				if (maze[E_pos.row + 1][E_pos.column] != '#' && maze[E_pos.row + 1][E_pos.column] != 'B')
				{
					rect(E_pos.column*block_size, E_pos.row*block_size + block_size, block_size - 6, block_size - 6, color::red);
					rect(E_pos.column*block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::white);
					E_pos.row++;
					break;
				}
				else break;
			}
			if (r == 3)//left
			{ 
				if (maze[E_pos.row][E_pos.column - 1] != '#' && maze[E_pos.row][E_pos.column - 1] != 'B')
				{
					rect(E_pos.column*block_size - block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::red);
					rect(E_pos.column*block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::white);
					E_pos.column--;
					break;
				}
				else break;
			}
			if (r == 4)//down
			{
				if (maze[E_pos.row][E_pos.column + 1] != '#' && maze[E_pos.row][E_pos.column + 1] != 'B')
				{
					rect(E_pos.column*block_size + block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::red);
					rect(E_pos.column*block_size, E_pos.row*block_size, block_size - 6, block_size - 6, color::white);
					E_pos.column++;
					break;
				}
				else break;
			}
		}
		memory[i] = c;
		if (c == 'A')
		{//Auto mode!
			while (true)
			{
				char c = wait_for_key_typed(0.02);
				if (maze[A_pos.row][A_pos.column] == maze[B_pos.row][B_pos.column])
				{
					rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::orange);
					wait(1);
					show_console();
					cout << "you won!";
					break;
				}
				if (A_pos.row == E_pos.row && A_pos.column == E_pos.column)
				{
					cout << "you Lost";
					break;
				}
				if (c == 'm') break;
				else
				{
					int direction = random_in_range(1, 4);
					if (direction == 1)
					{ /* makes the robot move one square up */
						if (maze[A_pos.row - 1][A_pos.column] != '#')//&& maze[A_pos.row - 1][A_pos.column]!='B')
						{
							rect(A_pos.column*block_size, A_pos.row*block_size - block_size, block_size - 6, block_size - 6, color::green);
							rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
							//maze[A_pos.row][A_pos.column] = 'B';
							A_pos.row--;

						}
					}
					else if (direction == 2)
					{ /* makes the robot move one square down */
						if (maze[A_pos.row + 1][A_pos.column] != '#')
						{
							rect(A_pos.column*block_size, A_pos.row*block_size + block_size, block_size - 6, block_size - 6, color::green);
							rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
							A_pos.row++;
						}
					}
					else if (direction == 3)
					{ /* makes the robot move one square left */
						if (maze[A_pos.row][A_pos.column-1] != '#')
						{
							rect(A_pos.column*block_size - block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::green);
							rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
							A_pos.column--;
						}
					}
					else if (direction == 4)
					{ /* makes the robot move one square right */
						if (maze[A_pos.row][A_pos.column + 1] != '#')
						{
							rect(A_pos.column*block_size + block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::green);
							rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
							A_pos.column++;
						}
					}

				}
			}
		}
		//Manual Mode
		else
		{
			if (A_pos.row == E_pos.row && A_pos.column == E_pos.column)
			{
				cout << "you Lost";
				break;
			}
			if (maze[A_pos.row][A_pos.column] == maze[B_pos.row][B_pos.column])
			{
				rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::orange);
				wait(1);
				show_console();
				cout << "you won!";
				break;
			}
			if (c == 'l' || c == 'a' || c == '-91')
			{ /* makes the robot move one square to the left */
				if (maze[A_pos.row][A_pos.column - 1] != '#')
				{
					rect(A_pos.column*block_size - block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::green);
					rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
					A_pos.column--;
				}
			}
			else if (c == 'r' || c == 'd' || c == '-89')
			{ /* makes the robot move one square to the right */
				if (maze[A_pos.row][A_pos.column + 1] != '#')
				{
					rect(A_pos.column*block_size + block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::green);
					rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
					A_pos.column++;
				}
			}
			else if (c == 'u' || c == 'w' || c == '-90')
			{ /* makes the robot move one square up */
				if (maze[A_pos.row - 1][A_pos.column] != '#')
				{
					rect(A_pos.column*block_size, A_pos.row*block_size - block_size, block_size - 6, block_size - 6, color::green);
					rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
					A_pos.row--;
				}
			}
			else if (c == 'd' || c == 's' || c == '-88')
			{ /* makes the robot move one square down */
				if (maze[A_pos.row + 1][A_pos.column] != '#')
				{
					rect(A_pos.column*block_size, A_pos.row*block_size + block_size, block_size - 6, block_size - 6, color::green);
					rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::light_blue);
					A_pos.row++;
				}
			}
			else if (c == 'x' || c == 'q' || c == 'ESC')
			{
				char v;
				show_console();
				cout << "Are you sure you want to quit? (type 'y' to quit): ";
				cin >> v;
				if (v == 'y')
				{
					cout << " Thank you! have a great day";
					break;
				}
				else hide_console();
			}
		}
		if (maze[A_pos.row][A_pos.column] == maze[B_pos.row][B_pos.column])
		{
			rect(A_pos.column*block_size, A_pos.row*block_size, block_size - 6, block_size - 6, color::orange);
			wait(1);
			show_console();
			cout << "you won!";
			break;
		}
		//*
		if (A_pos.row == E_pos.row && A_pos.column == E_pos.column)
		{
			cout << "you Lost";
			break;
		}
		//*/
	}

}
