#include "Pch.hpp"
#include "Sprite.hpp"
#include "StbImage.hpp"
#include "Utils.hpp"

#include <algorithm>

namespace def
{
	Sprite::Sprite(const Vector2i& size)
	{
		Create(size);
	}

	Sprite::Sprite(std::string_view fileName)
	{
		Load(fileName);
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::Create(const Vector2i& size)
	{
		Assert(size.x > 0 && size.y > 0, "[Sprite.Create Error] Width and height should be > 0");

		pixels.clear();
		this->size = size;

		pixels.resize(size.x * size.y);
		std::fill(pixels.begin(), pixels.end(), BLACK);
	}

	void Sprite::Load(std::string_view fileName)
	{
		Assert(!stbi_is_hdr(fileName.data()), "[stb_image Error] can't load an HDR file");

		uint8_t* data = stbi_load(fileName.data(), &size.x, &size.y, NULL, 4);
		Assert(data, "[stb_image Error] ", SAFE_STBI_FAILURE_REASON());

		pixels.clear();
		pixels.resize(size.x * size.y);

		size_t length = size.x * size.y * 4;
		for (size_t i = 0; i < length; i += 4)
		{
			size_t j = i / 4;
			pixels[j].r = data[i + 0];
			pixels[j].g = data[i + 1];
			pixels[j].b = data[i + 2];
			pixels[j].a = data[i + 3];
		}

		stbi_image_free(data);
	}

	void Sprite::Save(std::string_view fileName, const FileType type) const
	{
		int err = 0;

		switch (type)
		{
		case FileType::BMP: err = stbi_write_bmp(fileName.data(), size.x, size.y, 4, pixels.data()); break;
		case FileType::PNG: err = stbi_write_png(fileName.data(), size.x, size.y, 4, pixels.data(), size.x * 4); break;
		case FileType::JPG: err = stbi_write_jpg(fileName.data(), size.x, size.y, 4, pixels.data(), 100); break;
		case FileType::TGA: err = stbi_write_tga(fileName.data(), size.x, size.y, 4, pixels.data()); break;
		case FileType::TGA_RLE:
		{
			stbi_write_tga_with_rle = 1;
			err = stbi_write_tga(fileName.data(), size.x, size.y, 4, pixels.data());
			stbi_write_tga_with_rle = 0;
		}
		break;

		}

		Assert(err == 1, "[stb_image_write Error] Code: ", std::to_string(err).c_str());
	}

	bool Sprite::SetPixel(int x, int y, const Pixel& col)
	{
		if (x >= 0 && y >= 0 && x < size.x && y < size.y)
		{
			pixels[y * size.x + x] = col;
			return true;
		}

		return false;
	}

	bool Sprite::SetPixel(const Vector2i& pos, const Pixel& col)
	{
		return SetPixel(pos.x, pos.y, col);
	}

	Pixel Sprite::GetPixel(int x, int y, const WrapMethod wrap) const
	{
		auto GetPixel = [&pixels = pixels, &size = size](int x, int y)
			{
				return pixels[y * size.x + x];
			};

		switch (wrap)
		{
		case WrapMethod::NONE:
		{
			if (x >= 0 && y >= 0 && x < size.x && y < size.y)
				return GetPixel(x, y);
		}
		break;

		case WrapMethod::REPEAT:
		return GetPixel(abs(x) % size.x, abs(y) % size.y);

		case WrapMethod::MIRROR:
		{
			int mx = (x < 0) ? size.x - 1 - abs(x) % size.x : abs(x) % size.x;
			int my = (y < 0) ? size.y - 1 - abs(y) % size.y : abs(y) % size.y;

			return GetPixel(mx, my);
		}

		case WrapMethod::CLAMP:
		{
			int cx = std::clamp(x, 0, size.x - 1);
			int cy = std::clamp(y, 0, size.y - 1);

			return GetPixel(cx, cy);
		}

		}

		return BLACK;
	}

	Pixel Sprite::GetPixel(const Vector2i& pos, const WrapMethod wrap) const
	{
		return GetPixel(pos.x, pos.y, wrap);
	}

	void Sprite::SetPixelData(const Pixel& col)
	{
		std::fill(pixels.begin(), pixels.end(), col);
	}

	Pixel Sprite::Sample(float x, float y, const SampleMethod sample, const WrapMethod wrap) const
	{
		return Sample({ x, y }, sample, wrap);
	}

	Pixel Sprite::Sample(const Vector2f& pos, const SampleMethod sample, const WrapMethod wrap) const
	{
		Vector2f denorm = pos * Vector2f(size);

		switch (sample)
		{
		case SampleMethod::LINEAR:
		return GetPixel(denorm, wrap);

		case SampleMethod::BILINEAR:
		{
			Vector2f cell = denorm.Floor();
			Vector2f offset = denorm - cell;

			Pixel tl = GetPixel(cell + Vector2f(0, 0), wrap);
			Pixel tr = GetPixel(cell + Vector2f(1, 0), wrap);
			Pixel bl = GetPixel(cell + Vector2f(0, 1), wrap);
			Pixel br = GetPixel(cell + Vector2f(1, 1), wrap);

			Pixel topCol = tr * offset.x + tl * (1.0f - offset.x);
			Pixel bottomCol = br * offset.x + bl * (1.0f - offset.x);

			return bottomCol * offset.y + topCol * (1.0f - offset.y);
		}

		case SampleMethod::TRILINEAR:
		{
			Vector2i center = (denorm - Vector2f(0.5f, 0.5f)).Floor();
			Vector2f offset = denorm - Vector2f(0.5f, 0.5f) - Vector2f(center);

			struct Pixelf
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
				float a = 0.0f;
			};

			Pixelf splineX[4][4];

			Vector2i s;
			for (s.y = 0; s.y < 4; s.y++)
				for (s.x = 0; s.x < 4; s.x++)
				{
					Pixel p = GetPixel(center + s - 1, wrap);
					splineX[s.y][s.x] = { (float)p.r, (float)p.g, (float)p.b, (float)p.a };
				}

			Vector2f t = offset;
			Vector2f tt = t * t;
			Vector2f ttt = tt * tt;

			Vector2f q[4];
			q[0] = 0.5f * (-1.0f * ttt + 2.0f * tt - t);
			q[1] = 0.5f * (3.0f * ttt - 5.0f * tt + 2.0f);
			q[2] = 0.5f * (-3.0f * ttt + 4.0f * tt + t);
			q[3] = 0.5f * (ttt - tt);

			Pixelf splineY[4];

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					splineY[i].r += splineX[i][j].r * q[j].x;
					splineY[i].g += splineX[i][j].g * q[j].x;
					splineY[i].b += splineX[i][j].b * q[j].x;
					splineY[i].a += splineX[i][j].a * q[j].x;
				}

			Pixelf pix;
			for (int i = 0; i < 4; i++)
			{
				pix.r += splineY[i].r * q[i].y;
				pix.g += splineY[i].g * q[i].y;
				pix.b += splineY[i].b * q[i].y;
				pix.a += splineY[i].a * q[i].y;
			}

			return Pixel(
				ClampFloatToUint8(pix.r),
				ClampFloatToUint8(pix.g),
				ClampFloatToUint8(pix.b),
				ClampFloatToUint8(pix.a)
			);
		}

		}

		return NONE;
	}
}
