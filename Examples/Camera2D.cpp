#include "../Include/defGameEngine.hpp"

#define DGE_AFFINE_TRANSFORMS
#include "../Extensions/DGE_AffineTransforms.hpp"

#define DGE_CAMERA2D
#include "../Extensions/DGE_Camera2D.hpp"

class AffineTransforms : public def::GameEngine
{
public:
	AffineTransforms()
	{
		GetWindow()->SetTitle("Affine Transforms");
	}

	def::AffineTransforms at;
	def::Camera2D camera;

	def::Vector2f pos;
	def::Vector2f size;

	def::Sprite* spr;

protected:
	bool OnUserCreate() override
	{
		camera.Initialise({ 0, 0 }, GetWindow()->GetScreenSize(), def::Camera2D::Mode::LazyLock);

		size = { 20, 20 };

		spr = new def::Sprite("assets/kart.png");

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetInput()->GetScrollDelta() > 0) at.Zoom(1.1f, pos);
		if (GetInput()->GetScrollDelta() < 0) at.Zoom(0.9f, pos);

		Clear(def::DARK_BLUE);

		//pos += (at.ScreenToWorld(GetMousePos()) - pos) * deltaTime;

		if (GetInput()->GetKeyState(def::Key::A).held) pos.x -= 500.0f * deltaTime;
		if (GetInput()->GetKeyState(def::Key::D).held) pos.x += 500.0f * deltaTime;
		if (GetInput()->GetKeyState(def::Key::W).held) pos.y -= 500.0f * deltaTime;
		if (GetInput()->GetKeyState(def::Key::S).held) pos.y += 500.0f * deltaTime;

		at.SetOffset(camera.Update(pos, deltaTime));
		
		at.DrawSprite({ 0, 0 }, spr);
		at.FillRectangle(pos - size / 2, size, def::RED);

		return true;
	}

};

int main()
{
	AffineTransforms app;

	if (app.Construct(1280, 720, 1, 1, false, true))
		app.Run();

	return 0;
}
