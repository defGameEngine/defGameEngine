#include "../Include/KeyState.hpp"

namespace def
{
	KeyState::KeyState() : held(false), released(false), pressed(false)
	{

	}

	KeyState::KeyState(bool held, bool released, bool pressed)
		: held(held), released(released), pressed(pressed)
	{

	}
}