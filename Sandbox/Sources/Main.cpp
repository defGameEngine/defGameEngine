#include "defGameEngine.hpp"

struct CustomLayer : def::Layer
{
    CustomLayer()
    {
        size = { 100, 100 };
    }

    bool OnUpdate(float deltaTime) override
    {
        Context().ClearTexture(def::CYAN);
        Context().DrawTextureRectangle({ 25, 25 }, { 50, 50 }, def::RED);
        return true;
    }
};

class Example : public def::GameEngine
{
public:
    Example()
    {
        UseOnlyTextures(true);
        Window().SetTitle("Example");
    }

    size_t id;

private:
    bool OnUserCreate() override
    {
        id = CreateLayer(new CustomLayer());
        return true;
    }

    bool OnConsoleCommand(const std::string& command, std::stringstream& output, def::Pixel& colour) override
    {
        if (command == "rand")
            output << rand();

        else if (command == "cls")
            Console().Clear();

        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        ClearTexture(def::WHITE);
        DrawTextureLine({ 0, 0 }, Input().GetMousePosition(), def::GREEN);

        if (Input().GetKeyState(def::Key::TAB).released)
            Console().Show(!Console().IsShown());

        return true;
    }

};

int main()
{
    Example demo;

    if (demo.Construct(256, 200, 4, 4))
        demo.Run();
}
