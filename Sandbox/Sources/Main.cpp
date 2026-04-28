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

    def::Texture* tex;

private:
    bool OnUserCreate() override
    {
        id = CreateLayer(new CustomLayer());

        tex = new def::Texture("blocks.png", { 30.0f, 50.0f }, { 300.0f, 200.0f });

        return true;
    }

    bool OnConsoleCommand(const std::string& command, std::stringstream& output, def::Pixel& colour) override
    {
        if (command == "rand")
            output << "Hi!\n\n\nYou've got: " << rand();

        else if (command == "cls")
            Console().Clear();

        else if (command == "fullscreen")
            Window().EnableFullscreen(!Window().IsFullScreen());

        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        ClearTexture(def::WHITE);
        DrawTextureLine({ 0, 0 }, Input().GetMousePosition(), def::GREEN);

        if (Input().GetKeyState(def::Key::K1).pressed)
            SetSampleMethod(def::Sprite::SampleMethod::LINEAR);

        if (Input().GetKeyState(def::Key::K2).pressed)
            SetSampleMethod(def::Sprite::SampleMethod::BILINEAR);

        if (Input().GetKeyState(def::Key::K3).pressed)
            SetWrapMethod(def::Sprite::WrapMethod::NONE);

        if (Input().GetKeyState(def::Key::K4).pressed)
            SetWrapMethod(def::Sprite::WrapMethod::REPEAT);

        if (Input().GetKeyState(def::Key::K5).pressed)
            SetWrapMethod(def::Sprite::WrapMethod::MIRROR);

        if (Input().GetKeyState(def::Key::K6).pressed)
            SetWrapMethod(def::Sprite::WrapMethod::CLAMP);

        DrawTexture({ 0.0f, 0.0f }, tex);

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