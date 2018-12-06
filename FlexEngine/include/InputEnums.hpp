#pragma once

#include "Helpers.hpp" // For RollingAverage

namespace flex
{
	namespace Input
	{
		enum class KeyAction
		{
			PRESS,
			RELEASE,
			REPEAT,

			_NONE
		};

		enum class KeyCode
		{
			KEY_SPACE,
			KEY_APOSTROPHE,
			KEY_COMMA,
			KEY_MINUS,
			KEY_PERIOD,
			KEY_SLASH,
			KEY_0,
			KEY_1,
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,
			KEY_SEMICOLON,
			KEY_EQUAL,
			KEY_A,
			KEY_B,
			KEY_C,
			KEY_D,
			KEY_E,
			KEY_F,
			KEY_G,
			KEY_H,
			KEY_I,
			KEY_J,
			KEY_K,
			KEY_L,
			KEY_M,
			KEY_N,
			KEY_O,
			KEY_P,
			KEY_Q,
			KEY_R,
			KEY_S,
			KEY_T,
			KEY_U,
			KEY_V,
			KEY_W,
			KEY_X,
			KEY_Y,
			KEY_Z,
			KEY_LEFT_BRACKET,
			KEY_BACKSLASH,
			KEY_RIGHT_BRACKET,
			KEY_GRAVE_ACCENT,
			KEY_WORLD_1,
			KEY_WORLD_2,

			KEY_ESCAPE,
			KEY_ENTER,
			KEY_TAB,
			KEY_BACKSPACE,
			KEY_INSERT,
			KEY_DELETE,
			KEY_RIGHT,
			KEY_LEFT,
			KEY_DOWN,
			KEY_UP,
			KEY_PAGE_UP,
			KEY_PAGE_DOWN,
			KEY_HOME,
			KEY_END,
			KEY_CAPS_LOCK,
			KEY_SCROLL_LOCK,
			KEY_NUM_LOCK,
			KEY_PRINT_SCREEN,
			KEY_PAUSE,
			KEY_F1,
			KEY_F2,
			KEY_F3,
			KEY_F4,
			KEY_F5,
			KEY_F6,
			KEY_F7,
			KEY_F8,
			KEY_F9,
			KEY_F10,
			KEY_F11,
			KEY_F12,
			KEY_F13,
			KEY_F14,
			KEY_F15,
			KEY_F16,
			KEY_F17,
			KEY_F18,
			KEY_F19,
			KEY_F20,
			KEY_F21,
			KEY_F22,
			KEY_F23,
			KEY_F24,
			KEY_F25,
			KEY_KP_0,
			KEY_KP_1,
			KEY_KP_2,
			KEY_KP_3,
			KEY_KP_4,
			KEY_KP_5,
			KEY_KP_6,
			KEY_KP_7,
			KEY_KP_8,
			KEY_KP_9,
			KEY_KP_DECIMAL,
			KEY_KP_DIVIDE,
			KEY_KP_MULTIPLY,
			KEY_KP_SUBTRACT,
			KEY_KP_ADD,
			KEY_KP_ENTER,
			KEY_KP_EQUAL,
			KEY_LEFT_SHIFT,
			KEY_LEFT_CONTROL,
			KEY_LEFT_ALT,
			KEY_LEFT_SUPER,
			KEY_RIGHT_SHIFT,
			KEY_RIGHT_CONTROL,
			KEY_RIGHT_ALT,
			KEY_RIGHT_SUPER,
			KEY_MENU,

			_NONE
		};

		enum class Mod
		{
			_NONE = 0,

			SHIFT = (1 << 0),
			CONTROL = (1 << 1),
			ALT = (1 << 2),
			SUPER = (1 << 3)
		};

		enum class MouseButton
		{
			MOUSE_BUTTON_1,
			MOUSE_BUTTON_2,
			MOUSE_BUTTON_3,
			MOUSE_BUTTON_4,
			MOUSE_BUTTON_5,
			MOUSE_BUTTON_6,
			MOUSE_BUTTON_7,
			MOUSE_BUTTON_8,

			LEFT = MOUSE_BUTTON_1,
			RIGHT = MOUSE_BUTTON_2,
			MIDDLE = MOUSE_BUTTON_3,

			_NONE = MOUSE_BUTTON_8 + 1
		};

		enum class GamepadButton
		{
			A = 0,
			B = 1,
			X = 2,
			Y = 3,
			LEFT_BUMPER = 4,
			RIGHT_BUMPER = 5,
			BACK = 6,
			START = 7,
			// 8 ?
			LEFT_STICK_DOWN = 9,
			RIGHT_STICK_DOWN = 10,
			D_PAD_UP = 11,
			D_PAD_RIGHT = 12,
			D_PAD_DOWN = 13,
			D_PAD_LEFT = 14,

			_NONE,
			_COUNT = _NONE
		};

		enum class GamepadAxis
		{
			LEFT_STICK_X = 0,
			LEFT_STICK_Y = 1,
			RIGHT_STICK_X = 2,
			RIGHT_STICK_Y = 3,
			LEFT_TRIGGER = 4,
			RIGHT_TRIGGER = 5,

			_NONE
		};

		enum class MouseAxis
		{
			X,
			Y,
			SCROLL_X,
			SCROLL_Y,

			_NONE
		};

		enum class Action
		{
			WALK_LEFT,
			WALK_RIGHT,
			WALK_FORWARD,
			WALK_BACKWARD,
			LOOK_H,
			LOOK_V,
			INTERACT,

			DBG_SWITCH_TO_NEXT_CAM,
			DBG_SWITCH_TO_PREV_CAM,
			DBG_ENTER_NEXT_SCENE,
			DBG_ENTER_PREV_SCENE,
			DBG_SAVE_SCENE,
			DBG_GIZMO_SELECT_TRANSLATE,
			DBG_GIZMO_SELECT_ROTATE,
			DBG_GIZMO_SELECT_SCALE,

			_NONE,
			COUNT = _NONE
		};

		struct Binding
		{
			KeyCode keyCode = KeyCode::_NONE;
			MouseButton mouseButton = MouseButton::_NONE;
			MouseAxis mouseAxis = MouseAxis::_NONE;
			GamepadButton gamepadButton = GamepadButton::_NONE;
			GamepadAxis gamepadAxis = GamepadAxis::_NONE;
			bool bPositive = true; // ?
		};

		struct Key
		{
			i32 down = 0; // A count of how many frames this key has been down for (0 means not down)
		};

		struct MouseDrag
		{
			glm::vec2 startLocation;
			glm::vec2 endLocation;
		};

		struct GamepadState
		{
			// Bitfield used to store gamepad button states for each player
			// 0 = up, 1 = down (See GamepadButton enum)
			u32 buttonStates = 0;
			u32 buttonsPressed = 0;
			u32 buttonsReleased = 0;

			// LEFT_STICK_X, LEFT_STICK_Y, RIGHT_STICK_X, RIGHT_STICK_Y, LEFT_TRIGGER, RIGHT_TRIGGER
			real axes[6];
			RollingAverage<real> averageRotationSpeeds;
			i32 framesToAverageOver = 10;
			real pJoystickX = 0.0f;
			real pJoystickY = 0.0f;
			i32 previousQuadrant = -1;
		};

	} // namespace Input
} // namespace flex