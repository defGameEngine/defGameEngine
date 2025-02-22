#ifndef DGE_CAMERA2D_HPP
#define DGE_CAMERA2D_HPP

#include "../Include/defGameEngine.hpp"

namespace def
{
	class Camera2D
	{
	public:
		enum class Mode
		{
			None,
			Lock,
			LazyLock,
			BorderLock
		};

	public:
		Camera2D() = default;
		Camera2D(const Vector2f& pos, const Vector2f& viewArea, const Mode mode = Mode::None);

		void Initialise(const Vector2f& pos, const Vector2f& viewArea, const Mode mode = Mode::None);
		Vector2f Update(Vector2f& target, float deltaTime);

		void SetMode(const Mode mode);
		void SetPosition(const Vector2f& pos);

		Mode GetMode() const;
		Vector2f GetPosition() const;

	private:
		Mode m_Mode;
		Vector2f m_ViewArea;
		Vector2f m_Position;

	};
}

#ifdef DGE_CAMERA2D
#undef DGE_CAMERA2D

namespace def
{
	Camera2D::Camera2D(const Vector2f& pos, const Vector2f& viewArea, const Mode mode)
	{
		Initialise(pos, viewArea, mode);
	}

	void Camera2D::Initialise(const Vector2f& pos, const Vector2f& viewArea, const Mode mode)
	{
		m_Mode = mode;
		m_ViewArea = viewArea;
		m_Position = pos;
	}

	Vector2f Camera2D::Update(Vector2f& target, float deltaTime)
	{
		switch (m_Mode)
		{
		case Camera2D::Mode::Lock:
			m_Position = target;
			break;

		case Camera2D::Mode::LazyLock:
			m_Position += (target - m_Position) * deltaTime;
			break;

		case Camera2D::Mode::BorderLock:
			m_Position = (target / m_ViewArea).Floor() * m_ViewArea + m_ViewArea * 0.5f;
			break;

		}

		return m_Position - m_ViewArea * 0.5f;
	}

	void Camera2D::SetMode(const Camera2D::Mode mode)
	{
		m_Mode = mode;
	}

	void Camera2D::SetPosition(const Vector2f& pos)
	{
		m_Position = pos;
	}

	Camera2D::Mode Camera2D::GetMode() const
	{
		return m_Mode;
	}

	Vector2f Camera2D::GetPosition() const
	{
		return m_Position;
	}
}

#endif

#endif
