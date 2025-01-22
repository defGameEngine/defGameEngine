#include "../Include/defGameEngine.hpp"

#define DGE_AFFINE_TRANSFORMS
#include "../Extensions/DGE_AffineTransforms.hpp"

struct Shape;

struct Node
{
	Shape* parent;
	def::Vector2i pos;
};

struct Shape
{
	std::vector<Node> nodes;

	size_t maxNodes;
	bool expired = false;

	virtual void DrawYourself(def::AffineTransforms& vendor) = 0;

	Node* GetNextNode(const def::Vector2f& pos)
	{
		if (nodes.size() == maxNodes)
			return nullptr;

		Node n;
		n.parent = this;
		n.pos = pos;
		nodes.push_back(n);

		return &nodes.back();
	}

	void DrawNodes(def::AffineTransforms& vendor)
	{
		for (const auto& n : nodes)
			vendor.FillTextureCircle(n.pos, 2, def::RED);
	}

	Node* HitNode(const def::Vector2i& vPos)
	{
		for (auto& n : nodes)
		{
			if (n.pos == vPos)
				return &n;
		}

		return nullptr;
	}
};

struct Line : Shape
{
	Line()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::AffineTransforms& vendor) override
	{
		vendor.FillTextureCircle(nodes[0].pos, 2, def::RED);
		vendor.FillTextureCircle(nodes[1].pos, 2, def::RED);

		vendor.DrawTextureLine(nodes[0].pos, nodes[1].pos, def::WHITE);
		vendor.DrawTextureLine(nodes[0].pos, nodes[1].pos, def::WHITE);
	}
};

struct Circle : Shape
{
	Circle()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::AffineTransforms& vendor) override
	{
		vendor.DrawTextureLine(nodes[0].pos, nodes[1].pos, def::WHITE);

		uint32_t radius = uint32_t((nodes[1].pos - nodes[0].pos).Length());
		vendor.DrawTextureCircle(nodes[0].pos, radius);
	}
};

struct Rect : Shape
{
	Rect()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::AffineTransforms& vendor) override
	{
		def::Vector2i p1 = nodes[0].pos;
		def::Vector2i p2 = nodes[1].pos;
		if (p1 > p2) std::swap(p1, p2);

		vendor.DrawTextureRectangle(p1, p2 - p1, def::WHITE);
	}
};

struct Curve : Shape
{
	Curve()
	{
		nodes.reserve(3);
		maxNodes = 3;
	}

	virtual void DrawYourself(def::AffineTransforms& vendor) override
	{
		if (nodes.size() == 2)
			vendor.DrawTextureLine(nodes[0].pos, nodes[1].pos, def::WHITE);

		if (nodes.size() == 3)
		{
			vendor.DrawTextureLine(nodes[0].pos, nodes[1].pos, def::WHITE);
			vendor.DrawTextureLine(nodes[1].pos, nodes[2].pos, def::WHITE);

			def::Vector2i op = nodes[0].pos;
			def::Vector2i np = op;

			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				np = (1 - t) * (1 - t) * nodes[0].pos + 2 * (1 - t) * t * nodes[1].pos + t * t * nodes[2].pos;
				vendor.DrawTextureLine(op, np);
				op = np;
			}
		}
	}
};

class SimpleCAD : public def::GameEngine
{
public:
	SimpleCAD()
	{
		SetTitle("SimpleCAD");
		UseOnlyTextures(true);
	}

	virtual ~SimpleCAD()
	{
		for (auto& shape : shapes)
			delete shape;
	}

public:
	void DrawSelectedArea()
	{
		if (selectedArea.second != def::Vector2i(-1, -1))
		{
			def::Vector2i first = selectedArea.first;
			def::Vector2i second = selectedArea.second;

			if (first > second)
				std::swap(first, second);

			at.FillTextureRectangle(first, second - first, def::Pixel(255, 255, 255, 122));
		}
	}

	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetMouse(def::Button::WHEEL).pressed)
			at.StartPan(GetMousePos());

		if (GetMouse(def::Button::WHEEL).held)
			at.UpdatePan(GetMousePos());

		if (GetMouseWheelDelta() > 0)
			at.Zoom(1.1f, GetMousePos());

		if (GetMouseWheelDelta() < 0)
			at.Zoom(0.9f, GetMousePos());

		def::Vector2i cursor = at.ScreenToWorld(GetMousePos());

		if (GetKey(def::Key::L).pressed)
		{
			tempShape = new Line();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::C).pressed)
		{
			tempShape = new Circle();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::R).pressed)
		{
			tempShape = new Rect();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::B).pressed)
		{
			tempShape = new Curve();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetMouse(def::Button::RIGHT).pressed)
		{
			for (const auto& shape : shapes)
			{
				Node* node = shape->HitNode(cursor);

				if (node)
				{
					selected = node;
					break;
				}
			}

			if (!selected)
			{
				selectedArea.first = cursor;
				selectedArea.second = { -1, -1 };
			}
		}

		if (GetMouse(def::Button::RIGHT).held)
		{
			if (!selected)
				selectedArea.second = cursor;
		}

		if (GetKey(def::Key::D).pressed)
		{
			if (selectedArea.second != def::Vector2i(-1, -1))
			{
				for (int32_t x = selectedArea.first.x; x <= selectedArea.second.x; x++)
					for (int32_t y = selectedArea.first.y; y <= selectedArea.second.y; y++)
					{
						for (const auto& shape : shapes)
						{
							Node* node = shape->HitNode({ x, y });

							if (node)
								node->parent->expired = true;
						}
					}

				selectedArea.first = { -1, -1 };
				selectedArea.second = { -1, -1 };
			}

			if (tempShape)
			{
				delete tempShape;
				tempShape = nullptr;
			}
			else
			{
				if (selected)
				{
					selected->parent->expired = true;
					selected = nullptr;
				}
			}
		}

		if (selected)
			selected->pos = cursor;

		if (GetMouse(def::Button::LEFT).released)
		{
			if (tempShape)
			{
				selected = tempShape->GetNextNode(cursor);
				if (!selected)
				{
					shapes.push_back(tempShape);
					tempShape = nullptr;
				}
			}
			else
				selected = nullptr;
		}

		ClearTexture(def::DARK_BLUE);

		at.DrawTextureCircle(cursor, 2, def::DARK_GREY);

		def::Vector2i origin = at.GetOrigin();
		def::Vector2i end = at.GetEnd();

		at.DrawTextureLine({ ScreenWidth() / 2, origin.y }, { ScreenWidth() / 2, end.y }, def::GREY);
		at.DrawTextureLine({ origin.x, ScreenHeight() / 2 }, { end.x, ScreenHeight() / 2 }, def::GREY);

		for (size_t i = 0; i < shapes.size(); i++)
		{
			auto shape = shapes[i];

			if (shape->expired)
			{
				shapes.erase(shapes.begin() + i);
				continue;
			}

			shape->DrawYourself(at);
			shape->DrawNodes(at);
		}

		if (tempShape)
		{
			tempShape->DrawYourself(at);
			tempShape->DrawNodes(at);
		}

		DrawSelectedArea();

		return true;
	}

private:
	std::vector<Shape*> shapes;

	Shape* tempShape = nullptr;
	Node* selected = nullptr;

	std::pair<def::Vector2i, def::Vector2i> selectedArea;

	def::AffineTransforms at;

};

int main()
{
	SimpleCAD demo;

	demo.Construct(1280, 720, 1, 1);
	demo.Run();

	return 0;
}
