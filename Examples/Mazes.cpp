#include "../Include/defGameEngine.hpp"

#include <stack>

class Maze : public def::GameEngine
{
public:
	Maze()
	{
		GetWindow()->SetTitle("Maze");
	}

private:
	std::stack<def::Vector2i> stack;
	std::vector<int> maze;
	int nVisited;

	enum
	{
		CELL_DIR_N = 0,
		CELL_DIR_E = 2,
		CELL_DIR_S = 4,
		CELL_DIR_W = 8,
		CELL_VISITED = 16
	};

	def::Vector2i vMazeSize;
	int nCellSize;

protected:
	bool OnUserCreate() override
	{
		vMazeSize = { 40, 25 };
		nCellSize = 3;

		// Reset maze
		maze.resize(vMazeSize.x * vMazeSize.y, 0);
		nVisited = 0;

		// Update stack
		stack.push({ 0, 0 });
		maze[0] = CELL_VISITED;
		nVisited = 1;

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		auto offset = [&](int ox, int oy)
			{
				def::Vector2i& top = stack.top();
				return (top.y + oy) * vMazeSize.x + (top.x + ox);
			};

		if (nVisited < vMazeSize.x * vMazeSize.y)
		{
			std::vector<int> vecNeighbors;
			def::Vector2i& vLast = stack.top();

			if (vLast.y > 0 && (maze[offset(0, -1)] & CELL_VISITED) == 0)
				vecNeighbors.push_back(0);

			if (vLast.x < vMazeSize.x - 1 && (maze[offset(1, 0)] & CELL_VISITED) == 0)
				vecNeighbors.push_back(1);

			if (vLast.y < vMazeSize.y - 1 && (maze[offset(0, 1)] & CELL_VISITED) == 0)
				vecNeighbors.push_back(2);

			if (vLast.x > 0 && (maze[offset(-1, 0)] & CELL_VISITED) == 0)
				vecNeighbors.push_back(3);

			if (vecNeighbors.empty())
			{
				// Go back
				stack.pop();
			}
			else
			{
				int nDirection = vecNeighbors[rand() % vecNeighbors.size()];

				// Create a path between the neighbor
				// and the current cell

				if (nDirection == 0) // North
				{
					maze[offset(0, -1)] |= CELL_DIR_S | CELL_VISITED;
					maze[offset(0, 0)] |= CELL_DIR_N;
					stack.push({ vLast.x, vLast.y - 1 });
				}

				if (nDirection == 1) // East
				{
					maze[offset(1, 0)] |= CELL_DIR_W | CELL_VISITED;
					maze[offset(0, 0)] |= CELL_DIR_E;
					stack.push({ vLast.x + 1, vLast.y });
				}

				if (nDirection == 2) // South
				{
					maze[offset(0, 1)] |= CELL_DIR_N | CELL_VISITED;
					maze[offset(0, 0)] |= CELL_DIR_S;
					stack.push({ vLast.x, vLast.y + 1 });
				}

				if (nDirection == 3) // West
				{
					maze[offset(-1, 0)] |= CELL_DIR_E | CELL_VISITED;
					maze[offset(0, 0)] |= CELL_DIR_W;
					stack.push({ vLast.x - 1, vLast.y });
				}

				nVisited++;
			}
		}


		Clear(def::DARK_GREEN);

		for (int i = 0; i < vMazeSize.x * vMazeSize.y; i++)
		{
			def::Vector2i p = { i % vMazeSize.x, i / vMazeSize.x };

			if (maze[i] & CELL_VISITED)
				FillRectangle(p * (nCellSize + 1) + 1, def::Vector2i(nCellSize, nCellSize), def::GREEN);

			for (int c = 0; c < nCellSize; c++)
			{
				if (maze[i] & CELL_DIR_S) Draw(p.x * (nCellSize + 1) + c + 1, p.y * (nCellSize + 1) + nCellSize + 1, def::GREEN);
				if (maze[i] & CELL_DIR_E) Draw(p.x * (nCellSize + 1) + nCellSize + 1, p.y * (nCellSize + 1) + c + 1, def::GREEN);
			}
		}

		return true;
	}
};

int main()
{
	Maze demo;

	demo.Construct(161, 101, 8, 8);
	demo.Run();

	return 0;
}
