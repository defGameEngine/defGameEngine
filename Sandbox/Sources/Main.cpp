#include "defGameEngine.hpp"

class Example : public def::GameEngine
{
public:
    Example()
    {
        GetWindow()->SetTitle("Example");
    }

private:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        return true;
    }

};

int main()
{
    Example demo;

    if (demo.Construct(256, 240, 4, 4))
        demo.Run();
}
