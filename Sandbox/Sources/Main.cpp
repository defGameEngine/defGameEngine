#include "defGameEngine.hpp"

class Example : public def::GameEngine
{
public:
    Example()
    {
        GetWindow()->SetTitle("Example");
    }

protected:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        def::Vector2i size = GetWindow()->GetScreenSize();

        for (int y = 0; y < size.y; y++)
            for (int x = 0; x < size.x; x++)
                Draw(x, y, def::Pixel(rand() % 256, rand() % 256, rand() % 256));
            
        return true;
    }

};

int main()
{
    Example demo;

    if (demo.Construct(256, 240, 4, 4))
        demo.Run();
}
