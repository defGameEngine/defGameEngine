#include "../Include/defGameEngine.hpp"

using namespace def;

Vector2f RandomGradient(const Vector2i& i)
{
	uint32_t a = i.x, b = i.y;
	a *= 3284157443u; b ^= a << 16 | a >> 16;
	b *= 1911520717u; a ^= b << 16 | b >> 16;
	a *= 2048419325u;

	float random = (float)a * 3.14159f / float(~(~0u >> 1));
	return { cosf(random), sinf(random) };
}

float DotProductGridGradient(const Vector2i& i, const Vector2f& p)
{
	return (p - Vector2f(i)).DotProduct(RandomGradient(i));
}

// Returns float value (0.0f - 1.0f)
float PerlinNoise2D(const Vector2f& p)
{
	Vector2i i0 = p.Floor();
	Vector2i i1 = i0 + 1.0f;

	Vector2f w = p - Vector2f(i0);

	float ix0 = std::lerp(
		DotProductGridGradient(i0, p),
		DotProductGridGradient({ i1.x, i0.y }, p),
		w.x);

	float ix1 = std::lerp(
		DotProductGridGradient({ i0.x, i1.y }, p),
		DotProductGridGradient(i1, p),
		w.x);

	return std::lerp(ix0, ix1, w.y);
}

class PerlinNoise : public def::GameEngine
{
public:
	PerlinNoise()
	{
		GetWindow()->SetTitle("Perlin Noise");
	}

private:
	void UpdateMap()
	{
		for (int i = 0; i < GetWindow()->GetScreenWidth(); i++)
			for (int j = 0; j < GetWindow()->GetScreenHeight(); j++)
			{
				float n = 0.0f;

				float frequency = 1.0f;
				float amplitude = 1.0f;

				for (int o = 0; o < octaves; o++)
				{
					n += PerlinNoise2D(Vector2f(i, j) / (Vector2f)GetWindow()->GetScreenSize() * frequency) * amplitude;

					frequency *= 2.0f;
					amplitude *= 0.5f;
				}

				map[j * GetWindow()->GetScreenWidth() + i] = std::clamp(n * 1.2f, -1.0f, 1.0f) * 0.5f + 0.5f;
			}
	}

private:
	int octaves = 12;
	float* map = nullptr;

protected:
	bool OnUserCreate() override
	{
		int size = GetWindow()->GetScreenWidth() * GetWindow()->GetScreenHeight();

		map = new float[size];
		memset(map, 0, sizeof(float) * size);

		UpdateMap();

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetInput()->GetKeyState(def::Key::LEFT).pressed) octaves--;
		if (GetInput()->GetKeyState(def::Key::RIGHT).pressed) octaves++;
		octaves = std::clamp(octaves, 0, 32);

		if (GetInput()->GetKeyState(def::Key::SPACE).pressed)
			UpdateMap();

		for (int i = 0; i < GetWindow()->GetScreenWidth(); i++)
			for (int j = 0; j < GetWindow()->GetScreenHeight(); j++)
			{
				float n = map[j * GetWindow()->GetScreenWidth() + i];
				uint8_t col = uint8_t(n * 12.0f);

				def::Pixel pix;

				switch (col)
				{
				case 0: pix = BLACK; break;

				case 1: pix = DARK_BROWN; break;
				case 2: pix = BROWN; break;
				case 3: pix = DARK_RED; break;
				case 4: pix = RED; break;

				case 5: pix = DARK_PURPLE; break;
				case 6: pix = DARK_BLUE; break;
				case 7: pix = PURPLE; break;
				case 8: pix = BLUE; break;

				case 9:  pix = DARK_ORANGE; break;
				case 10: pix = ORANGE; break;
				case 11: pix = BEIGE; break;
				case 12: pix = YELLOW; break;
				}

				Draw(i, j, pix);
			}

		DrawString(2, 2, "Octaves = " + std::to_string(octaves), def::CYAN);

		return true;
	}

};

int main()
{
	PerlinNoise demo;

	demo.Construct(1280, 720, 1, 1);
	demo.Run();

	return 0;
}
