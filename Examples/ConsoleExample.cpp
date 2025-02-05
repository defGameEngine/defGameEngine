#include "../Include/defGameEngine.hpp"

struct Ball
{
	def::Vector2f pos;
	def::Vector2f vel;
	
	def::Pixel col;
};

class Demo : public def::GameEngine
{
public:
	Demo()
	{
		SetTitle("Demo");
		UseOnlyTextures(true);
	}

private:
	std::vector<Ball> balls;

	int ballRadius = 5;
	float ballSpeed = 200.0f;

private:
	float RandFloat(float start, float end)
	{
		return start + (float)rand() / (float)RAND_MAX * (end - start);
	}

	void AddBall(const def::Vector2f& pos, const def::Vector2f& vel)
	{
		balls.push_back({ pos, vel, def::Pixel(rand() % 256, rand() % 256, rand() % 256) });
	}

protected:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnConsoleCommand(const std::string& command, std::stringstream& output, def::Pixel& colour) override
	{
		std::stringstream ss(command);

		std::string com;
		ss >> com;

		if (com == "add")
		{
			int count = 0;
			ss >> count;

			for (int i = 0; i < count; i++)
				AddBall(GetScreenSize() / 2, { RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f) });

			output << "Added " << count << " balls";
		}
		else if (com == "remove")
		{
			int count = 0;
			ss >> count;

			for (int i = 0; i < count; i++)
				balls.pop_back();

			output << "Removed " << count << " balls";
		}
		else if (com == "count")
		{
			output << "Balls count = " << balls.size();
		}
		else if (com == "clear")
		{
			ClearConsole();
		}
		else
		{
			output << "Unexpected command";
			colour = def::RED;
		}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetMouse(def::Button::LEFT).pressed)
			AddBall(GetScreenSize() / 2, { RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f) });
		
		if (GetKey(def::Key::TAB).pressed)
			ShowConsole(true);

		if (GetKey(def::Key::ESCAPE).pressed)
			ShowConsole(false);

		for (auto& ball : balls)
		{
			if (ball.pos.x <= ballRadius || ball.pos.x >= ScreenWidth() - ballRadius)
			{
				ball.vel.x *= -1.0f;
				ball.pos.x = std::clamp(ball.pos.x, (float)ballRadius, float(ScreenWidth() - ballRadius - 1));
			}

			if (ball.pos.y <= ballRadius || ball.pos.y >= ScreenHeight() - ballRadius)
			{
				ball.vel.y *= -1.0f;
				ball.pos.y = std::clamp(ball.pos.y, (float)ballRadius, float(ScreenHeight() - ballRadius - 1));
			}

			ball.pos += ballSpeed * ball.vel * deltaTime;
		}

		ClearTexture(def::DARK_BLUE);

		for (const auto& ball : balls)
			FillTextureCircle(ball.pos, ballRadius, ball.col);

		return true;
	}

};

int main()
{
	Demo app;

	if (app.Construct(256, 240, 4, 4, false, true))
		app.Run();

	return 0;
}
