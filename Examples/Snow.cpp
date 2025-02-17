#include "../Include/defGameEngine.hpp"

class Snow : public def::GameEngine
{
public:
	Snow()
	{
		GetWindow()->SetTitle("Snow");
	}

private:
	struct Flake
	{
		def::Vector2f pos;
	
		float time = 0.0f;
		float speed = 1.0f;
	};

	std::list<Flake> flakes;
	int flakeRadius = 4;

	float speedDiff = 0.0f;

private:
	float rnd(float min, float max)
	{
		return (float)rand() / (float)RAND_MAX * (max - min) + min;
	}

protected:
	bool OnUserCreate() override
	{
		srand(time(nullptr));

		flakes.resize(1024);

		for (auto& f : flakes)
		{
			f.pos = { rnd(0.0f, GetWindow()->GetScreenWidth()), float(-flakeRadius) };
			f.speed = rnd(100.0f, 200.0f);
		}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetInput()->GetKeyState(def::Key::UP).held)
			speedDiff += deltaTime * 100.0f;

		if (GetInput()->GetKeyState(def::Key::DOWN).held)
			speedDiff -= deltaTime * 100.0f;

		speedDiff = std::max(0.0f, speedDiff);

		for (auto& f : flakes)
		{
			f.time += deltaTime;
			f.pos.y = f.time * (f.speed + speedDiff);

			if (f.pos.y >= GetWindow()->GetScreenHeight() + flakeRadius)
			{
				f.pos = { rnd(-speedDiff / 10.0f, GetWindow()->GetScreenWidth()), float(-flakeRadius) };
				f.time = 0.0f;
			}
		}

		Clear(def::BLACK);

		for (auto& f : flakes)
		{
			f.pos.x += deltaTime * speedDiff / 10.0f;

			float c = abs(1.0f - 1.0f / f.speed * 200.0f);
			FillCircle(f.pos + def::Vector2f(f.pos.x + cos(f.time * f.speed * 0.1f), 0.0f), (float)flakeRadius * c, def::Pixel::Float(c, c, c));
		}

		return true;
	}

};

int main()
{
	Snow app;

	app.Construct(1280, 720, 1, 1);
	app.Run();

	return 0;
}
