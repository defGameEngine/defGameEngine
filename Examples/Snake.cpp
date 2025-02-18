#include "../Include/defGameEngine.hpp"

#include <list>

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		GetWindow()->SetTitle("Sample");
	}

protected:
	def::Vector2i apple;
	int dir;
	bool isDead;

	int score = 0;
	int record = 0;

	std::list<def::Vector2i> snake;

	def::Vector2i bodyPartSize = { 16, 16 };
	def::Vector2i worldSize;

	float delay = 0.5f;
	float timer = 0.0f;

protected:
	bool OnUserCreate() override
	{
		worldSize = GetWindow()->GetScreenSize() / bodyPartSize;

		NewGame();

		return true;
	}

	void NewGame()
	{
		isDead = false;

		apple = worldSize / def::Vector2i(4, 2);
		dir = 0;

		def::Vector2i snakePos = worldSize / 2;
		snake =
		{
			snakePos,
			snakePos + def::Vector2i(1, 0),
			snakePos + def::Vector2i(2, 0),
			snakePos + def::Vector2i(3, 0),
		};

		if (record < score)
			record = score;

		score = 0;

		srand(time(0));
	}

	void DrawBodyPart(const def::Vector2i& pos, const def::Pixel& col)
	{
		FillRectangle(pos * bodyPartSize, bodyPartSize, col);
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (isDead)
		{
			if (GetInput()->GetKeyState(def::Key::SPACE).pressed)
				NewGame();

			DrawString(GetWindow()->GetScreenSize() / def::Vector2i(3, 2), "GAME OVER! PRESS SPACE.");
			return true;
		}

		if (GetInput()->GetKeyState(def::Key::A).pressed) dir = 0;
		if (GetInput()->GetKeyState(def::Key::W).pressed) dir = 1;
		if (GetInput()->GetKeyState(def::Key::D).pressed) dir = 2;
		if (GetInput()->GetKeyState(def::Key::S).pressed) dir = 3;

		if (timer >= delay)
		{
			switch (dir)
			{
			case 0: snake.push_front({ snake.front().x - 1, snake.front().y }); break;
			case 1: snake.push_front({ snake.front().x, snake.front().y - 1 }); break;
			case 2: snake.push_front({ snake.front().x + 1, snake.front().y }); break;
			case 3: snake.push_front({ snake.front().x, snake.front().y + 1 }); break;
			}

			if (snake.front() == apple)
			{
				score++;

				apple = def::Vector2i(rand(), rand()) % worldSize;
				snake.push_back(snake.back());
			}

			for (auto i = snake.begin(); i != snake.end(); i++)
			{
				if (i != snake.begin() && *i == *snake.begin())
					isDead = true;
			}

			if (snake.front().x < 0 || snake.front().y < 0 || snake.front().x >= worldSize.x || snake.front().y >= worldSize.y)
				isDead = true;

			snake.pop_back();

			timer = 0.0f;
		}
		timer += deltaTime;

		Clear(def::GREEN);

		DrawString(0, 0, "Score: " + std::to_string(score));
		DrawString(0, 10, "Record: " + std::to_string(record));

		for (auto& s : snake)
			DrawBodyPart(s, isDead ? def::RED : def::BLUE);

		DrawBodyPart(snake.front(), isDead ? def::DARK_RED : def::WHITE);
		DrawBodyPart(apple, def::DARK_RED);

		return true;
	}
};

int main()
{
	Sample demo;

	demo.Construct(384, 240, 4, 4);
	demo.Run();

	return 0;
}
