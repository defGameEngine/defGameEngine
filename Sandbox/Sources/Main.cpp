#include "defGameEngine.hpp"

struct CustomLayer : def::Layer
{
    CustomLayer()
    {
        size = { 100, 100 };

        pos[0] = { 20, 20 };
        pos[1] = { 100, 100 };
        pos[2] = { 20, 100 };
    }

    bool OnUpdate(float deltaTime) override
    {
        auto& input = Context().Input();

        if (input.GetButtonState(def::Button::LEFT).held)
        {
            def::Vector2f mouse = input.GetMousePosition();

            for (int i = 0; i < 3; i++)
            {
                if (def::Vector2f(mouse - pos[i]).Length2() <= 4.0f)
                    selected = &pos[i];
            }

            if (selected)
                *selected = mouse;
        }

        if (input.GetButtonState(def::Button::LEFT).released)
            selected = nullptr;

        if (!selected)
        {
            int scrolls = input.GetScrollDelta();

            if (scrolls != 0)
            {
                float theta = 200.0f * scrolls * Context().GetDeltaTime();

                float c = cosf(theta);
                float s = sinf(theta);

                def::Vector2f center = (pos[0] + pos[1] + pos[2]) / 3.0f;

                for (int i = 0; i < 3; i++)
                {
                    def::Vector2f old = pos[i] - center;

                    pos[i].x = center.x + old.x * c - old.y * s;
                    pos[i].y = center.y + old.x * s + old.y * c;
                }
            }
        }

        Context().ClearTexture(def::GREY);
        Context().GradientTextureTriangle(pos[0], pos[1], pos[2], def::RED, def::GREEN, def::BLUE);

        for (int i = 0; i < 3; i++)
            Context().FillTextureCircle(pos[i], 2, def::YELLOW);

        return true;
    }

    def::Vector2f pos[3];
    def::Vector2f* selected = nullptr;

    float angle = 0.0f;
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

    if (demo.Construct(256, 200, 4, 4, false, true))
        demo.Run();
}