#include "defGameEngine.hpp"

class Example : public def::GameEngine
{
public:
	Example()
	{
		Window().SetTitle("Hello, DGE!");
	}

protected:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float) override
	{
		for (int i = 0; i < Window().GetScreenHeight(); ++i)
			for (int j = 0; j < Window().GetScreenWidth(); ++j)
				Draw(j, i, def::Pixel(rand() % 256, rand() % 256, rand() % 256));

		return true;
	}

};

int main()
{
	Example app;

	if (app.Construct(256, 240, 4, 4))
		app.Run();
}
