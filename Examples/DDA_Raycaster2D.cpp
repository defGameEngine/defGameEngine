#include "../Include/defGameEngine.hpp"

class Raycasting : public def::GameEngine
{
public:
	Raycasting()
	{
		GetWindow()->SetTitle("Raycasting");
	}

	virtual ~Raycasting()
	{
		delete[] map;
	}

private:
	def::Vector2i tileSize;
	def::Vector2i tilesCount;

	def::Vector2f start;
	def::Vector2f end;

	bool* map;

	float pointSpeed = 100.0f;

protected:
	void ControlPoint(def::Vector2f& point, const float deltaTime, const def::Key up, const def::Key down, const def::Key left, const def::Key right)
	{
		if (GetInput()->GetKeyState(up).held)
			point.y -= pointSpeed * deltaTime;

		if (GetInput()->GetKeyState(down).held)
			point.y += pointSpeed * deltaTime;

		if (GetInput()->GetKeyState(left).held)
			point.x -= pointSpeed * deltaTime;

		if (GetInput()->GetKeyState(right).held)
			point.x += pointSpeed * deltaTime;
	}

	void SetTile(const def::Vector2i& pos, const bool value)
	{
		map[pos.y * tilesCount.x + pos.x] = value;
	}

	bool GetTile(const def::Vector2i& pos)
	{
		return map[pos.y * tilesCount.x + pos.x];
	}

	bool OnUserCreate() override
	{
		tileSize = { 8, 8 };
		tilesCount = GetWindow()->GetScreenSize() / tileSize;

		map = new bool[tilesCount.x * tilesCount.y] { false };

		start = { 10, 10 };
		end = { 20, 10 };

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		ControlPoint(start, deltaTime, def::Key::W, def::Key::S, def::Key::A, def::Key::D);
		ControlPoint(end, deltaTime, def::Key::UP, def::Key::DOWN, def::Key::LEFT, def::Key::RIGHT);

		if (GetInput()->GetButtonState(def::Button::RIGHT).pressed)
		{
			def::Vector2i tilePos = GetInput()->GetMousePosition() / tileSize;

			SetTile(tilePos, !GetTile(tilePos));
		}

		// Perform DDA algorithm
		// thank you, https://lodev.org/cgtutor/raycasting.html

		def::Vector2f rayStart = start / tileSize;
		def::Vector2f rayDirection = (end / tileSize - rayStart).Normalise();
		def::Vector2f stepSize = (1.0f / rayDirection).Abs();

		def::Vector2f side, step;
		def::Vector2i mapPos = rayStart;

		if (rayDirection.x < 0)
		{
			step.x = -1;
			side.x = (rayStart.x - (float)mapPos.x) * stepSize.x;
		}
		else
		{
			step.x = 1;
			side.x = (float(mapPos.x + 1) - rayStart.x) * stepSize.x;
		}

		if (rayDirection.y < 0)
		{
			step.y = -1;
			side.y = (rayStart.y - (float)mapPos.y) * stepSize.y;
		}
		else
		{
			step.y = 1;
			side.y = (float(mapPos.y + 1) - rayStart.y) * stepSize.y;
		}

		bool tileFound = false;

		float distance = 0.0f;
		float maxDistance = 64.0f;

		while (!tileFound && distance < maxDistance)
		{
			if (side.x < side.y)
			{
				mapPos.x += step.x;
				distance = side.x;
				side.x += stepSize.x;
			}
			else
			{
				mapPos.y += step.y;
				distance = side.y;
				side.y += stepSize.y;
			}

			if (mapPos.x >= 0 && mapPos.y >= 0 && mapPos.x < tilesCount.x && mapPos.y < tilesCount.y)
			{
				if (GetTile(mapPos))
					tileFound = true;
			}
		}

		def::Vector2f intersectionPoint;

		if (tileFound)
			intersectionPoint = rayStart + rayDirection * distance;

		Clear(def::BLACK);

		def::Vector2i p;
		for (; p.y < tilesCount.y; p.y++)
			for (p.x = 0; p.x < tilesCount.x; p.x++)
			{
				if (GetTile(p))
					FillRectangle(p * tileSize, tileSize, def::BLUE);
			}

		DrawLine(start, end, def::GREY);

		if (tileFound)
			DrawCircle(intersectionPoint * tileSize, 3, def::CYAN);

		FillCircle(start, 3, def::RED);
		FillCircle(end, 3, def::GREEN);

		return true;
	}
};

int main()
{
	Raycasting demo;

	if (demo.Construct(241, 241, 4, 4))
		demo.Run();

	return 0;
}
