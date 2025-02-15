#pragma once

#ifndef DGE_KEY_STATE_HPP
#define DGE_KEY_STATE_HPP

namespace def
{
	struct KeyState
	{
		KeyState();
		KeyState(bool held, bool released, bool pressed);

		bool held;
		bool released;
		bool pressed;
	};
}

#endif
