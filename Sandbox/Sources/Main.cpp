#include "defGameEngine.hpp"

class Example : public def::GameEngine
{
public:
    Example()
    {
        GetWindow()->SetTitle("Example");
        UseOnlyTextures(true);
    }

protected:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        ClearTexture(def::BLACK);
        DrawTextureString(GetInput()->GetMousePosition(), "Hello, premake5!", def::PINK);

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
