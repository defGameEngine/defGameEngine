#include "../Include/defGameEngine.hpp"

#include <algorithm>

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		GetWindow()->SetTitle("Sample");
	}

	def::Vector2f scale = { 1.0f, 1.0f };
	def::Vector2f offset = { 0, 0 };

	def::Vector2i panStart;

	std::vector<def::Vector2f> data;

	def::Vector2f ScreenToWorld(const def::Vector2i& p)
	{
		return (def::Vector2f(p) + offset) / scale;
	}

	def::Vector2i WorldToScreen(const def::Vector2f& p)
	{
		return p * scale - offset;
	}

protected:
	bool OnUserCreate() override
	{
		scale = def::Vector2f(GetWindow()->GetScreenWidth(), GetWindow()->GetScreenHeight());

		auto randf = [](float min, float max)
		{
			return min + (float)rand() / float(max - min);
		};

		data.resize(100);

		for (int i = 0; i < data.size(); i++)
			data[i] = def::Vector2f(i, 1.0f + sin(i));

		return true;
	}

	void Plot(const def::Vector2f& pos, const def::Vector2f& size, const std::vector<def::Vector2f>& data, const def::Pixel& col = def::WHITE)
	{
		auto minX = std::min_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.x < b.x; });
		auto maxX = std::max_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.x < b.x; });

		auto minY = std::min_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.y < b.y; });
		auto maxY = std::max_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.y < b.y; });

		def::Vector2f diff(maxX->x - minX->x, maxY->y - minY->y);
		def::Vector2f step(1.0f / diff);

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::Vector2i p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::Vector2i p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		for (const auto& p : data)
		{
			def::Vector2i wp = WorldToScreen(pos + def::Vector2f(0.05f, 0.05f) + p / diff * (size - 0.1f));
			FillCircle(wp, 2, col);
		}

		def::Vector2i p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	void Scatter(const def::Vector2f& pos, const def::Vector2f& size, std::vector<def::Vector2f> data, const def::Pixel& col = def::WHITE)
	{
		auto minX = std::min_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.x < b.x; });
		auto maxX = std::max_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.x < b.x; });

		auto minY = std::min_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.y < b.y; });
		auto maxY = std::max_element(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return a.y < b.y; });

		def::Vector2f diff(maxX->x - minX->x, maxY->y - minY->y);
		def::Vector2f step(1.0f / diff);

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::Vector2i p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::Vector2i p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		std::sort(data.begin(), data.end(), [](const def::Vector2f& a, const def::Vector2f& b) { return b.x < a.x; });

		def::Vector2f op = WorldToScreen(pos + def::Vector2f(0.05f, 0.05f) + data[0] / diff * (size - 0.1f));
		for (size_t i = 1; i < data.size(); i++)
		{
			def::Vector2i np = WorldToScreen(pos + def::Vector2f(0.05f, 0.05f) + data[i] / diff * (size - 0.1f));
			DrawLine(op, np, col);
			op = np;
		}

		def::Vector2i p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	template <typename T>
	void Bar(const def::Vector2f& pos, const def::Vector2f& size, const std::vector<T>& data, const def::Pixel& col = def::WHITE)
	{
		auto min = std::min_element(data.begin(), data.end(), [](const T& a, const T& b) { return a < b; });
		auto max = std::max_element(data.begin(), data.end(), [](const T& a, const T& b) { return a < b; });

		float diff = *max - *min;
		def::Vector2f step = 1.0f / size;

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::Vector2i p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::Vector2i p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		float barWidth = (size.x - 0.1f) / data.size();

		for (size_t i = 0; i < data.size(); i++)
		{
			def::Vector2i wp = WorldToScreen(pos + def::Vector2f(0.05f, 0.05f) + barWidth * i + def::Vector2f(0.0f, size.y));
			FillRectangle(wp, def::Vector2i(barWidth, data[i] / diff * size.y), col);
		}

		def::Vector2i p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetInput()->GetButtonState(def::Button::LEFT).pressed) panStart = offset + GetInput()->GetMousePosition();
		if (GetInput()->GetButtonState(def::Button::LEFT).held) offset = panStart - GetInput()->GetMousePosition();

		if (GetInput()->GetKeyState(def::Key::UP).held) scale *= 1.01f;
		if (GetInput()->GetKeyState(def::Key::DOWN).held) scale *= 0.99f;

		Clear(def::WHITE);
		//Bar<float>({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 2.0f, 20.0f, 31.0f, 10.0f, 15.0f }, def::BLACK);
		Plot({ 0.0f, 1.1f }, { 1.0f, 1.0f }, data, def::BLACK);
		Scatter({ 0.0f, 2.2f }, { 1.0f, 1.0f }, data, def::BLACK);

		return true;
	}
};

int main()
{
	Sample demo;

	demo.Construct(800, 600, 1, 1);
	demo.Run();

	return 0;
}
