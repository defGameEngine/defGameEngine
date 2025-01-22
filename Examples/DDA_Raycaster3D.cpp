#include "../Include/defGameEngine.hpp"

#include <algorithm>

struct Object
{
	def::Vector2f pos;
	def::Vector2f vel;

	float speed;

	uint32_t type;
	uint32_t id;

	bool toRemove = false;
};

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

	virtual ~RayCasting()
	{
		delete tiles;
		delete[] depthBuffer;
	}

private:
	enum Objects
	{
		BLUESTONE,
		COLORSTONE,
		EAGLE,
		GREYSTONE,
		MOSSY,
		PILLAR,
		PURPLESTONE,
		REDBRICK,
		WOOD,
		BARREL,
		BULLET
	};

	enum Side
	{
		FLOOR,
		CEILING,
		WALL
	};

	def::Sprite* tiles = nullptr;

	int mapCellSize = 3;

	int floorId = Objects::GREYSTONE;
	int ceilingId = Objects::WOOD;

	std::string map;

	def::Vector2i mapSize = { 32, 32 };
	def::Vector2i texSize = { 64, 64 };

	def::Vector2f playerPos = { 7.0f, 3.0f };
	def::Vector2f playerVel = { -1.0, 0.0f };
	def::Vector2f playerPlane = { 0.0f, 0.66f };

	float moveSpeed = 5.0f;
	float rotSpeed = 3.0f;
	float depth = 16.0f;

	std::vector<Object> objects;
	float* depthBuffer = nullptr;

protected:
	bool OnUserCreate() override
	{
		tiles = new def::Sprite("Assets/tileset.png");

		map =
			"77777777777777777.........777777"
			"7..............................7"
			"7...8...............8..........7"
			"7...8...............8..........7"
			"7...88..............88.........7"
			"7....8...............8.........7"
			"7..............................7"
			"7..88..............88..........7"
			"7.......8888888.........88888887"
			"7..............................7"
			"...........8...............8...7"
			".........888.............888...7"
			"...............................2"
			"...............................7"
			"...............................7"
			"............88888..............7"
			"...............................7"
			"7..............................7"
			"7...6...............6..........7"
			"7...6...............6..........7"
			"7...66..............66.........7"
			"7....6...............6.........7"
			"7..............................7"
			"7..66..............66..........7"
			"7.......6666666.........66666667"
			"7..............................."
			"7..........6...............6...."
			"7........666.............666...."
			"7..............................."
			"7..............................."
			"7..............................."
			"777777..................77777777";

		objects.push_back({ {8.5f, 8.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		objects.push_back({ {7.5f, 7.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		objects.push_back({ {10.0f, 3.0f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });

		depthBuffer = new float[ScreenWidth()];

		return true;
	}

	bool PointOnMap(int x, int y)
	{
		return x >= 0 && y >= 0 && x < mapSize.x && y < mapSize.y;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// Remove redundant objects
		auto removeIter = std::remove_if(objects.begin(), objects.end(), [](const Object& o) { return o.toRemove; });
		if (removeIter != objects.end()) objects.erase(removeIter);

		if (GetKey(def::Key::W).held)
		{
			def::Vector2f vel = playerVel * moveSpeed * deltaTime;

			if (PointOnMap(int(playerPos.x + vel.x), (int)playerPos.y) && map[(int)playerPos.y * mapSize.x + int(playerPos.x + vel.x)] == '.')
				playerPos.x += vel.x;

			if (PointOnMap((int)playerPos.x, int(playerPos.y + vel.y)) && map[int(playerPos.y + vel.y) * mapSize.x + (int)playerPos.x] == '.')
				playerPos.y += vel.y;
		}

		if (GetKey(def::Key::S).held)
		{
			def::Vector2f vel = playerVel * moveSpeed * deltaTime;

			if (PointOnMap(int(playerPos.x - vel.x), (int)playerPos.y) && map[(int)playerPos.y * mapSize.x + int(playerPos.x - vel.x)] == '.')
				playerPos.x -= vel.x;

			if (PointOnMap((int)playerPos.x, int(playerPos.y - vel.y)) && map[int(playerPos.y - vel.y) * mapSize.x + (int)playerPos.x] == '.')
				playerPos.y -= vel.y;
		}

		if (GetKey(def::Key::A).held)
		{
			float oldVelX = playerVel.x;
			float oldPlaneX = playerPlane.x;

			playerVel.x = playerVel.x * cos(rotSpeed * deltaTime) - playerVel.y * sin(rotSpeed * deltaTime);
			playerVel.y = oldVelX * sin(rotSpeed * deltaTime) + playerVel.y * cos(rotSpeed * deltaTime);

			playerPlane.x = playerPlane.x * cos(rotSpeed * deltaTime) - playerPlane.y * sin(rotSpeed * deltaTime);
			playerPlane.y = oldPlaneX * sin(rotSpeed * deltaTime) + playerPlane.y * cos(rotSpeed * deltaTime);
		}

		if (GetKey(def::Key::D).held)
		{
			float oldVelX = playerVel.x;
			float oldPlaneX = playerPlane.x;

			playerVel.x = playerVel.x * cos(-rotSpeed * deltaTime) - playerVel.y * sin(-rotSpeed * deltaTime);
			playerVel.y = oldVelX * sin(-rotSpeed * deltaTime) + playerVel.y * cos(-rotSpeed * deltaTime);

			playerPlane.x = playerPlane.x * cos(-rotSpeed * deltaTime) - playerPlane.y * sin(-rotSpeed * deltaTime);
			playerPlane.y = oldPlaneX * sin(-rotSpeed * deltaTime) + playerPlane.y * cos(-rotSpeed * deltaTime);
		}

		// Check for collision
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				if (o1.id != o2.id)
				{
					if (o1.pos.Round() == o2.pos.Round())
					{
						if (o1.type == Objects::BULLET || o2.type == Objects::BULLET)
						{
							o1.toRemove = true;
							o2.toRemove = true;
						}
					}
				}
			}
		}

		Clear(def::BLACK);

		// Perform DDA raycast algorithm
		for (int x = 0; x < ScreenWidth(); x++)
		{
			float playerAngle = 2.0f * (float)x / (float)ScreenWidth() - 1.0f;

			def::Vector2f rayDir = playerVel + playerPlane * playerAngle;
			def::Vector2f distance = (1.0f / rayDir).Abs();

			bool doesHitWall = false;
			bool noWall = false;
			int side = 0;

			def::Vector2i step;
			def::Vector2f fromCurrentDistance;
			def::Vector2i mapPos = playerPos;

			float distanceToWall;

			if (rayDir.x < 0.0f)
			{
				step.x = -1;
				fromCurrentDistance.x = (playerPos.x - (float)mapPos.x) * distance.x;
			}
			else
			{
				step.x = 1;
				fromCurrentDistance.x = ((float)mapPos.x + 1.0f - playerPos.x) * distance.x;
			}

			if (rayDir.y < 0.0f)
			{
				step.y = -1;
				fromCurrentDistance.y = (playerPos.y - (float)mapPos.y) * distance.y;
			}
			else
			{
				step.y = 1;
				fromCurrentDistance.y = ((float)mapPos.y + 1.0f - playerPos.y) * distance.y;
			}

			while (!doesHitWall && !noWall)
			{
				if (fromCurrentDistance.x < fromCurrentDistance.y)
				{
					fromCurrentDistance.x += distance.x;
					mapPos.x += step.x;
					side = 0;
				}
				else
				{
					fromCurrentDistance.y += distance.y;
					mapPos.y += step.y;
					side = 1;
				}

				if (mapPos.y < 0 || mapPos.y >= mapSize.y || mapPos.x < 0 || mapPos.x >= mapSize.x)
				{
					distanceToWall = depth;
					noWall = true;
				}

				if (!noWall && std::isdigit(map[mapPos.y * mapSize.x + mapPos.x]))
					doesHitWall = true;
			}

			if (side == 0)
				distanceToWall = fromCurrentDistance.x - distance.x;
			else
				distanceToWall = fromCurrentDistance.y - distance.y;

			int lineHeight = int((float)ScreenHeight() / distanceToWall);

			int ceilingPos = std::max(-lineHeight + ScreenHeight() / 2, 0);
			int floorPos = std::min(lineHeight + ScreenHeight() / 2, ScreenHeight() - 1);

			float testPoint;
			float texStep, texPos;

			if (side == 0)
				testPoint = playerPos.y + rayDir.y * distanceToWall;
			else
				testPoint = playerPos.x + rayDir.x * distanceToWall;

			testPoint -= floorf(testPoint);

			def::Vector2i tex = { int(testPoint * (float)texSize.x), 0 };

			if ((side == 0 && rayDir.x > 0.0f) || (side == 1 && rayDir.y < 0.0f))
				tex.x = texSize.x - tex.x - 1;

			texStep = (float)texSize.y / (float)lineHeight / 2;
			texPos = float(ceilingPos - ScreenHeight() / 2 + lineHeight) * texStep;

			for (int y = 0; y <= floorPos; y++)
			{
				if (y <= ceilingPos) // ceiling and floor
				{
					float planeZ = float(ScreenHeight() / 2) / float(ScreenHeight() / 2 - y);

					def::Vector2f planePoint = playerPos + 2.0f * rayDir * planeZ;
					def::Vector2f planeSample = planePoint - planePoint.Floor();

					def::Vector2i texPos = (planeSample * texSize).Min(texSize);

					Draw(x, y, tiles->GetPixel(ceilingId * texSize.x + texPos.x, texPos.y)); // ceiling
					Draw(x, ScreenHeight() - y, tiles->GetPixel(floorId * texSize.x + texPos.x, texPos.y)); // floor
				}
				else if (y > ceilingPos && !noWall) // wall
				{
					tex.y = (int)texPos % (texSize.y - 1);
					texPos += texStep;

					Draw(x, y, tiles->GetPixel(((int)map[mapPos.y * mapSize.x + mapPos.x] - 48) * texSize.x + tex.x, tex.y));
				}
			}

			depthBuffer[x] = distanceToWall;
		}

		// Update and draw textured objects
		for (auto& o : objects)
		{
			o.pos += o.vel * o.speed * deltaTime;

			if (o.pos.Floor() >= def::Vector2f(0, 0) && o.pos.Floor() < mapSize && !std::isdigit(map[(int)o.pos.y * mapSize.x + (int)o.pos.x]))
			{
				def::Vector2f objectPos = o.pos - playerPos;

				float invDet = 1.0f / (playerPlane.x * playerVel.y - playerPlane.y * playerVel.x);

				def::Vector2f transform =
				{
					invDet * (playerVel.y * objectPos.x - playerVel.x * objectPos.y),
					invDet * (-playerPlane.y * objectPos.x + playerPlane.x * objectPos.y)
				};

				float aspectRatio = transform.x / transform.y;

				def::Vector2i objectScreenPos = { int(float(ScreenWidth() / 2) * (1.0f + aspectRatio)), ScreenHeight() / 2 };
				int objectScreenSize = int((float)ScreenHeight() / transform.y);

				def::Vector2i ceilingPos = def::Vector2i(objectScreenSize, objectScreenSize) / -2 + objectScreenPos;
				def::Vector2i floorPos = def::Vector2i(objectScreenSize, objectScreenSize) / 2 + objectScreenPos;

				ceilingPos = ceilingPos.Max(def::Vector2i(0, 0)).Min(GetScreenSize());
				floorPos = floorPos.Max(def::Vector2i(0, 0)).Min(GetScreenSize());

				SetPixelMode(def::Pixel::Mode::MASK);

				for (int x = ceilingPos.x; x < floorPos.x; x++)
				{
					int texX = (ScreenWidth() * (x - (-objectScreenSize / 2 + objectScreenPos.x)) * texSize.x / objectScreenSize) / ScreenWidth();

					if (transform.y >= 0 && x >= 0 && x < ScreenWidth() && transform.y < depthBuffer[x])
					{
						for (int y = ceilingPos.y; y < floorPos.y; y++)
						{
							int d = y * ScreenWidth() - ScreenHeight() * ScreenWidth() / 2 + objectScreenSize * ScreenWidth() / 2;
							int texY = (d * texSize.y / objectScreenSize) / ScreenWidth();

							Draw(x, y, tiles->GetPixel((int)o.type * texSize.x + texX, texY));
							depthBuffer[x] = transform.y;
						}
					}
				}

				SetPixelMode(def::Pixel::Mode::DEFAULT);
			}
			else
				o.toRemove = true;
		}

		// Draw map
		for (int x = 0; x < mapSize.x; x++)
			for (int y = 0; y < mapSize.y; y++)
			{
				if (map[y * mapSize.x + x] == '.')
					FillRectangle(x * 2, y * 2, 2, 2, def::GREY);
				else
					FillRectangle(x * 2, y * 2, 2, 2, def::WHITE);
			}

		FillRectangle((int)playerPos.x * 2, (int)playerPos.y * 2, 2, 2, def::YELLOW);

		if (GetMouse(def::Button::LEFT).pressed)
		{
			Object o;
			o.pos = playerPos;
			o.vel = playerVel;
			o.speed = 5.0f;
			o.type = Objects::BULLET;
			o.id = objects.size();
			objects.push_back(o);
		}

		return true;
	}

};

int main()
{
	RayCasting demo;

	demo.Construct(1024, 768, 1, 1);
	demo.Run();

	return 0;
}
