#define DEF_GEOMETRY2D_IMPL
#include "../defGeometry2D/defGeometry2D.hpp"

#include "../Include/defGameEngine.hpp"

template <class T>
struct Dynamic
{
	T model;
	def::Vector2f velocity;
};

struct Ball : Dynamic<def::circle<float>> {};

struct Tile : Dynamic<def::rect<float>>
{
	bool isRedundant = false;
};

class BreakOut : public def::GameEngine
{
public:
	BreakOut()
	{
		GetWindow()->SetTitle("Break Out");
	}

private:
	Ball ball;

	std::list<Tile> tiles;
	std::list<Tile>::iterator bat;

protected:
	bool OnUserCreate() override
	{
		ball.model.pos = GetWindow()->GetScreenSize() / 2;
		ball.velocity = { 0.5f, 0.8f };
		ball.model.radius = 5.0f;

		Tile b;
		b.model.pos = GetWindow()->GetScreenSize() * def::Vector2f(0.5f, 0.75f);
		b.model.size = { 20.0f, 5.0f };
	
		tiles.push_back(b);
		bat = tiles.begin();

		def::Vector2f origin = GetWindow()->GetScreenSize() * 0.25f;
		def::Vector2f tileSize = { 20.0f, 10.0f };

		def::Vector2i count = GetWindow()->GetScreenSize() * def::Vector2f(0.5f, 0.2f) / tileSize;

		// Add tiles
		for (int i = 0; i < count.x; i++)
			for (int j = 0; j < count.y; j++)
			{
				Tile tile;

				tile.model.pos = origin + (tileSize + 1) * def::Vector2f(i, j);
				tile.model.size = tileSize;

				tiles.push_back(tile);
			}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetInput()->GetKeyState(def::Key::LEFT).held) bat->model.pos.x -= 150.0f * deltaTime;
		if (GetInput()->GetKeyState(def::Key::RIGHT).held) bat->model.pos.x += 150.0f * deltaTime;

		def::Vector2f potentialPos = ball.model.pos + ball.velocity * deltaTime * 200.0f;

		if (potentialPos.x < ball.model.radius)
			ball.velocity.x *= -1.0f;

		if (potentialPos.x >= (float)GetWindow()->GetScreenWidth() - ball.model.radius)
			ball.velocity.x *= -1.0f;

		if (potentialPos.y < ball.model.radius)
			ball.velocity.y *= -1.0f;

		if (potentialPos.y >= (float)GetWindow()->GetScreenHeight() - ball.model.radius)
			ball.velocity.y *= -1.0f;

		for (auto it = tiles.begin(); it != tiles.end(); it++)
		{
			uint32_t i = 0;

			while (i < def::rect<float>::SIDES)
			{
				if (def::intersects(ball.model, it->model.side(i)).size() > 0)
				{
					if (it != bat)
						it->isRedundant = true;

					break;
				}

				i++;
			}

			switch (i)
			{
			case def::rect<float>::side::LEFT: ball.velocity.x *= -1.0f; break;
			case def::rect<float>::side::TOP: ball.velocity.y *= -1.0f; break;
			case def::rect<float>::side::RIGHT: ball.velocity.x *= -1.0f; break;
			case def::rect<float>::side::BOTTOM: ball.velocity.y *= -1.0f; break;
			}
		}

		ball.model.pos += ball.velocity * deltaTime * 150.0f;

		tiles.remove_if([](const Tile& t) { return t.isRedundant; });

		Clear(def::BLACK);

		FillCircle(ball.model.pos, ball.model.radius, def::YELLOW);
		FillRectangle(bat->model.pos, bat->model.size, def::BLUE);

		for (auto it = std::next(tiles.begin()); it != tiles.end(); it++)
			FillRectangle(it->model.pos, it->model.size, def::GREEN);

		return true;
	}
};

int main()
{
	BreakOut app;

	app.Construct(512, 480, 2, 2);
	app.Run();

	return 0;
}
