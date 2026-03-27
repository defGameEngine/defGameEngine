#include "defGameEngine.hpp"

class Example : public def::GameEngine
{
public:
    Example()
    {
        Window().SetTitle("Example");
    }

    size_t id;

private:
    bool OnUserCreate() override
    {
        id = CreateLayer({ 0, 0 }, { 100, 100 });
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
        PickLayer(id);
        DrawCircle(50, 50, 10, def::RED);
        PickLayer(1);

        Clear(def::BLACK);
        DrawLine(0, 0, Input().GetMouseX(), Input().GetMouseY(), def::GREEN);

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
