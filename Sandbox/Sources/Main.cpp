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
        DrawString(10, 10, "Hello, premake5!", def::PINK, 2, 2);

        return true;
    }

};

int main()
{
    Example demo;

    if (demo.Construct(256, 240, 4, 4))
        demo.Run();

    return 0;
}
