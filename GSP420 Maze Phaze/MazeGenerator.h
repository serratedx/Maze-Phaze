#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "DataStructures.h"
#include <vector>
#include <fstream>

enum Directions
{
	UP = 0,
	RIGHT,
	DOWN,
	LEFT
};

struct MazeCell
{
	int x, y;		// x,y position of the cell
	bool visited;	// whether or not the cell has been visited
	bool N,E,S,W;	// existence of north, east, south, and west walls

	MazeCell()
		: x(0), y(0),
		visited(false),
		N(true),E(true),
		S(true),W(true)
	{
	}

	MazeCell& operator=(MazeCell& newCell)
	{
		x = newCell.x;
		y = newCell.y;
		visited = newCell.visited;
		N = newCell.N;
		E = newCell.E;
		S = newCell.S;
		W = newCell.W;
		return *this; 
	}
};

class MazeGenerator
{
private:

	int						MazeWidthNodes;		// number of maze nodes in x direction
	int						MazeHeightNodes;	// number of maze nodes in y direction
	Array2D<char>			MazeBoard;			// the board!

	Array2D<MazeCell>		TheMazeCells;		// 2D array of maze cells
	std::vector<MazeCell>	TheCellStack;		// 1D stack of maze cells
	int						TotalCellCount;		// total number of maze cells
	int						VisitedCellCount;	// number of maze cells visited
	int						MazeWidthCells;		// number of cells in x direction
	int						MazeHeightCells;	// number of cells in y direction

	int						TileSize;			// size of a tile (node) in the maze

	bool					Started;			// whether or not the generator has started
	bool					Finished;
	bool					LoadFailed;			// did loading from file fail?

public:

	// default constructor
	MazeGenerator()
	{
		MazeGenerator(16, 16);
	}

	MazeGenerator(char* filename)
		: MazeWidthNodes(0), MazeHeightNodes(0),
		TotalCellCount(0), VisitedCellCount(0),
		MazeWidthCells(0), MazeHeightCells(0),
		TileSize(1),
		Started(false), Finished(true), LoadFailed(false)
	{
		if (!LoadFromFile(filename))
		{
			LoadFailed = true;
		}
		MazeWidthCells = (MazeWidthNodes - 1) / 2;
		MazeHeightCells = (MazeHeightNodes - 1) / 2;
		TheMazeCells.Resize(MazeWidthCells, MazeHeightCells);
		for (int h = 0; h < MazeHeightCells; h++)
		{
			for (int w = 0; w < MazeWidthCells; w++)
			{
				TheMazeCells.Get(w, h).x			= w;
				TheMazeCells.Get(w, h).y			= h;
				TheMazeCells.Get(w, h).visited		= true;

				TheMazeCells.Get(w, h).N			= true;
				TheMazeCells.Get(w, h).E			= true;
				TheMazeCells.Get(w, h).S			= true;
				TheMazeCells.Get(w, h).W			= true;
				
				if (MazeBoard.Get(w*2+1,h*2) == ' ') 
					TheMazeCells.Get(w, h).N			= false;
				if (MazeBoard.Get(w*2+2,h*2+1) == ' ') 
					TheMazeCells.Get(w, h).E			= false;
				if (MazeBoard.Get(w*2+1,h*2+2) == ' ') 
					TheMazeCells.Get(w, h).S			= false;
				if (MazeBoard.Get(w*2,h*2+1) == ' ') 
					TheMazeCells.Get(w, h).W			= false;

				
			}
		}
		TotalCellCount = MazeWidthCells * MazeHeightCells;
		TheCellStack.resize(TotalCellCount);
	}

	// constructor with specified maze width and height
	MazeGenerator(int width, int height)
	{
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;

		MazeWidthNodes = width;
		MazeHeightNodes = height;
		MazeBoard.Resize(MazeWidthNodes, MazeHeightNodes);
		for (int row = 0; row < MazeHeightNodes; row++)
		{
			for (int col = 0; col < MazeWidthNodes; col++)
			{
				if ( (col % 2 == 1 && row % 2 == 1) 
					&& !(col == MazeWidthNodes-1 || row == MazeHeightNodes-1) 
					)
				{
					MazeBoard.Get(col, row) = 'O';
				}
				else
				{
					MazeBoard.Get(col, row) = 'W';
				}
			}
		}

		MazeWidthCells = (MazeWidthNodes - 1) / 2;
		MazeHeightCells = (MazeHeightNodes - 1) / 2;
		TheMazeCells.Resize(MazeWidthCells, MazeHeightCells);

		for (int h = 0; h < MazeHeightCells; h++)
		{
			for (int w = 0; w < MazeWidthCells; w++)
			{
				TheMazeCells.Get(w, h).x			= w;
				TheMazeCells.Get(w, h).y			= h;
				TheMazeCells.Get(w, h).visited		= false;
				TheMazeCells.Get(w, h).N			= true;
				TheMazeCells.Get(w, h).E			= true;
				TheMazeCells.Get(w, h).S			= true;
				TheMazeCells.Get(w, h).W			= true;
			}
		}

		TotalCellCount = MazeWidthCells * MazeHeightCells;
		TheCellStack.resize(TotalCellCount);
		Start();
	}

	// default destructor
	~MazeGenerator()
	{
		TheCellStack.~vector();
		TheMazeCells.~Array2D();
		MazeBoard.~Array2D();
	}

	// get the value of a node on the board
	char GetPoint(unsigned int col, unsigned int row)
	{
		return MazeBoard.Get(col, row);
	}

	// start the maze generation
	bool Start()
	{
		Started = true;
		Finished = false;
		VisitedCellCount = 1;

		TheMazeCells.Get(0, 0).visited = true;

		MazeBoard.Get(1,1) = ' ';

		TheCellStack.push_back(TheMazeCells.Get(0, 0));
		return true;
	}

	// run one frame of the maze generator
	void Frame()
	{
		int direction;		// enumerated direction value
		if (VisitedCellCount < TotalCellCount)
		{
			direction = ChooseADirection(TheCellStack.back());
			if (direction == -1)
			{
				// no valid moves for this cell
				TheCellStack.pop_back();
			}
			else
			{
				MakeMove(TheCellStack.back(), direction);
				VisitedCellCount++;
			}
		}
		else
		{
			Finished = true;
		}
	}

	int ChooseADirection(MazeCell currentCell)
	{
		// four valid moves: UP, RIGHt, DOWN, LEFT
		bool validMoves[4];
 		std::vector<int> remainingMoves;

		// initialize all moves to valid
		for (int i = 0; i < 4; i++)
			validMoves[i] = true;

		// Start eliminating moves
		////
		//	Eliminate moves by checking for map borders
		//	and by checking if there are any walls left
		////
		// check for top border or if the north wall is gone
		if ( (currentCell.y == 0) || (currentCell.N == false) )
			validMoves[UP] = false;
		// check for right border or if the east wall is gone
		if ( (currentCell.x == MazeWidthCells-1) || (currentCell.E == false) )
			validMoves[RIGHT] = false;
		// check for bottom border or if the south wall is gone
		if ( (currentCell.y == MazeHeightCells-1) || (currentCell.S == false) )
			validMoves[DOWN] = false;
		// check for left border or if the west wall is gone
		if ( (currentCell.x == 0) || (currentCell.W == false) )
			validMoves[LEFT] = false;

		////
		//	Eliminate remaining moves by checking 
		//	if adjacent cells have been visited
		////
		// check whether or not cell above has been visited
		// *excessive checking in order to prevent memory access violations
		if (validMoves[UP])
			if (TheMazeCells.Get(currentCell.x, currentCell.y - 1).visited)
				validMoves[UP] = false;
		// check whether or not cell to the right has been visited
		if (validMoves[RIGHT])
			if (TheMazeCells.Get(currentCell.x + 1, currentCell.y).visited)
				validMoves[RIGHT] = false;
		// check whether or not cell above has been visited
		if (validMoves[DOWN])
			if (TheMazeCells.Get(currentCell.x, currentCell.y + 1).visited)
				validMoves[DOWN] = false;
		// check whether or not cell above has been visited
		if (validMoves[LEFT]) 
			if (TheMazeCells.Get(currentCell.x - 1, currentCell.y - UP).visited)
				validMoves[LEFT] = false;

		////
		//	Add remaing moves to stack
		////
		int counter = 0;
		if (validMoves[UP])
		{
			remainingMoves.push_back(UP);
			counter++;
		}

		if (validMoves[RIGHT])
		{
			remainingMoves.push_back(RIGHT);
			counter++;
		}

		if (validMoves[DOWN])
		{
			remainingMoves.push_back(DOWN);
			counter++;
		}

		if (validMoves[LEFT])
		{
			remainingMoves.push_back(LEFT);
			counter++;
		}

		if (counter == 0)
		{
			return -1;
		}

		for (int i = rand()%counter; i > 0; i--)
		{
			remainingMoves.pop_back();
		}

		int moveDirection = remainingMoves.back();

		return moveDirection;
	}

	MazeCell MakeMove(MazeCell fromCell, int direction)
	{
		MazeCell toCell;

		switch(direction)
		{
		case UP:
			fromCell.N		= false;
			toCell.S		= false;
			toCell.x		= fromCell.x;
			toCell.y		= fromCell.y - 1;
			toCell.visited	= true;
			MazeBoard.Get(toCell.x * 2 + 1, toCell.y * 2 + 2) = ' ';
			break;

		case RIGHT:
			fromCell.E		= false;
			toCell.W		= false;
			toCell.x		= fromCell.x + 1;
			toCell.y		= fromCell.y;
			toCell.visited	= true;
			MazeBoard.Get(toCell.x * 2, toCell.y * 2 + 1) = ' ';
			break;

		case DOWN:
			fromCell.S		= false;
			toCell.N		= false;
			toCell.x		= fromCell.x;
			toCell.y		= fromCell.y + 1;
			toCell.visited	= true;
			MazeBoard.Get(toCell.x * 2 + 1, toCell.y * 2) = ' ';
			break;

		case LEFT:
			fromCell.W		= false;
			toCell.E		= false;
			toCell.x		= fromCell.x - 1;
			toCell.y		= fromCell.y;
			toCell.visited	= true;
			MazeBoard.Get(toCell.x * 2 + 2, toCell.y * 2 + 1) = ' ';
			break;
		}

		TheMazeCells.Get(fromCell.x, fromCell.y) = fromCell;
		TheMazeCells.Get(toCell.x, toCell.y) = toCell;

		int tempXPos = toCell.x * 2 + 1;
		int tempYPos = toCell.y * 2 + 1;
		MazeBoard.Get(tempXPos, tempYPos) = ' ';
		
		TheCellStack.push_back(TheMazeCells.Get(toCell.x, toCell.y));

		return toCell;
	}

	// return a cell
	MazeCell GetCell(int x, int y)
	{
		return TheMazeCells.Get(x, y);
	}

	// return maze width in nodes
	int GetMazeWidthNodes()
	{
		return MazeWidthNodes;
	}
	// return maze height in nodes
	int GetMazeHeightNodes()
	{
		return MazeHeightNodes;
	}

	// return maze width in cells
	int GetMazeWidthCells()
	{
		return MazeWidthCells;
	}

	// return maze height in cells
	int GetMazeHeightCells()
	{
		return MazeHeightCells;
	}

	MazeCell CurrentCell()
	{
		return TheCellStack.back();
	}

	bool IsDone()
	{
		return Finished;
	}

	bool DidLoadFail()
	{
		return LoadFailed;
	}

	void SaveToFile(char* filename)
	{
		std::ofstream a_file(filename, std::ios::trunc);
		a_file << "Width: " << this->MazeWidthNodes << "\n";
		a_file << "Height: " << this->MazeHeightNodes << "\n";
		a_file << "Tile Size: " << this->TileSize << "\n";
		for (int r = 0; r < MazeHeightNodes; r++)
		{
			for (int c = 0; c < MazeWidthNodes; c++)
			{
				a_file << this->GetPoint(c,r);
			}
			a_file << "\n";
		}
		a_file << "\0";
		a_file.close();
	}

	bool LoadFromFile(char* filename)
	{
		char *buffer;
		std::ifstream a_file(filename);
		if (a_file.is_open())
		{
			bool gotWidth = false;
			bool gotHeight = false;
			bool gotTileSize = false;
			int digit = 0;
			int tWidth = 0;
			int tHeight = 0;
			int tTileSize = 0;
			
			buffer = new char[256];
			//get the width
			while (!gotWidth)
			{
				a_file.get(buffer[digit]);
				if(buffer[digit] >= '0' && buffer[digit] <= '9')
				{
					digit++;
				}
				else if (buffer[digit] == '\n')
				{
					gotWidth = true;
				}
			}
			tWidth = atoi(buffer);
			delete buffer;

			buffer = new char[256];
			digit = 0;
			//get the height
			while (!gotHeight)
			{
				a_file.get(buffer[digit]);
				if(buffer[digit] >= '0' && buffer[digit] <= '9')
				{
					digit++;
				}
				else if (buffer[digit] == '\n')
				{
					gotHeight = true;
				}
			}
			tHeight = atoi(buffer);
			delete buffer;

			buffer = new char[256];
			digit = 0;
			//get the tile size
			while (!gotTileSize)
			{
				a_file.get(buffer[digit]);
				if(buffer[digit] >= '0' && buffer[digit] <= '9')
				{
					digit++;
				}
				else if (buffer[digit] == '\n')
				{
					gotTileSize = true;
				}
			}
			tTileSize = atoi(buffer);
			delete buffer;
			if (tTileSize < 1)
				tTileSize = 1;

			MazeWidthNodes = tWidth;
			MazeHeightNodes = tHeight;
			TileSize = tTileSize;
			MazeBoard.Resize(MazeWidthNodes, MazeHeightNodes);
			buffer = new char[(tWidth+1) * tHeight];
			int r = 0;
			digit = 0;
			while(!a_file.eof())
			{
				a_file.get(buffer[digit]);
				if(!(buffer[digit] == '\n') && digit < tWidth)
				{
					MazeBoard.Get(digit, r) = buffer[digit];
					digit++;
				}
				else
				{
					r++;
					digit = 0;
				}
				if (r >= tHeight)
					break;
			}
			delete buffer;
			buffer = NULL;
		}
		else
		{
			return false;
		}
		a_file.close();
		return true;
	}

	void SetTileSize(int s)
	{
		if (s < 1)
			s = 1;
		TileSize = s;
		return;
	}

	int GetTileSize()
	{
		return TileSize;
	}
};

#endif