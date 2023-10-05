#pragma once

namespace Spark {
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// GLFW3.h 
#define SPK_KEY_SPACE           ::Spark::Key::Space
#define SPK_KEY_APOSTROPHE      ::Spark::Key::Apostrophe    /* ' */
#define SPK_KEY_COMMA           ::Spark::Key::Comma         /* , */
#define SPK_KEY_MINUS           ::Spark::Key::Minus         /* - */
#define SPK_KEY_PERIOD          ::Spark::Key::Period        /* . */
#define SPK_KEY_SLASH           ::Spark::Key::Slash         /* / */
#define SPK_KEY_0               ::Spark::Key::D0
#define SPK_KEY_1               ::Spark::Key::D1
#define SPK_KEY_2               ::Spark::Key::D2
#define SPK_KEY_3               ::Spark::Key::D3
#define SPK_KEY_4               ::Spark::Key::D4
#define SPK_KEY_5               ::Spark::Key::D5
#define SPK_KEY_6               ::Spark::Key::D6
#define SPK_KEY_7               ::Spark::Key::D7
#define SPK_KEY_8               ::Spark::Key::D8
#define SPK_KEY_9               ::Spark::Key::D9
#define SPK_KEY_SEMICOLON       ::Spark::Key::Semicolon     /* ; */
#define SPK_KEY_EQUAL           ::Spark::Key::Equal         /* = */
#define SPK_KEY_A               ::Spark::Key::A
#define SPK_KEY_B               ::Spark::Key::B
#define SPK_KEY_C               ::Spark::Key::C
#define SPK_KEY_D               ::Spark::Key::D
#define SPK_KEY_E               ::Spark::Key::E
#define SPK_KEY_F               ::Spark::Key::F
#define SPK_KEY_G               ::Spark::Key::G
#define SPK_KEY_H               ::Spark::Key::H
#define SPK_KEY_I               ::Spark::Key::I
#define SPK_KEY_J               ::Spark::Key::J
#define SPK_KEY_K               ::Spark::Key::K
#define SPK_KEY_L               ::Spark::Key::L
#define SPK_KEY_M               ::Spark::Key::M
#define SPK_KEY_N               ::Spark::Key::N
#define SPK_KEY_O               ::Spark::Key::O
#define SPK_KEY_P               ::Spark::Key::P
#define SPK_KEY_Q               ::Spark::Key::Q
#define SPK_KEY_R               ::Spark::Key::R
#define SPK_KEY_S               ::Spark::Key::S
#define SPK_KEY_T               ::Spark::Key::T
#define SPK_KEY_U               ::Spark::Key::U
#define SPK_KEY_V               ::Spark::Key::V
#define SPK_KEY_W               ::Spark::Key::W
#define SPK_KEY_X               ::Spark::Key::X
#define SPK_KEY_Y               ::Spark::Key::Y
#define SPK_KEY_Z               ::Spark::Key::Z
#define SPK_KEY_LEFT_BRACKET    ::Spark::Key::LeftBracket   /* [ */
#define SPK_KEY_BACKSLASH       ::Spark::Key::Backslash     /* \ */
#define SPK_KEY_RIGHT_BRACKET   ::Spark::Key::RightBracket  /* ] */
#define SPK_KEY_GRAVE_ACCENT    ::Spark::Key::GraveAccent   /* ` */
#define SPK_KEY_WORLD_1         ::Spark::Key::World1        /* non-US #1 */
#define SPK_KEY_WORLD_2         ::Spark::Key::World2        /* non-US #2 */

#define SPK_KEY_ESCAPE          ::Spark::Key::Escape
#define SPK_KEY_ENTER           ::Spark::Key::Enter
#define SPK_KEY_TAB             ::Spark::Key::Tab
#define SPK_KEY_BACKSPACE       ::Spark::Key::Backspace
#define SPK_KEY_INSERT          ::Spark::Key::Insert
#define SPK_KEY_DELETE          ::Spark::Key::Delete
#define SPK_KEY_RIGHT           ::Spark::Key::Right
#define SPK_KEY_LEFT            ::Spark::Key::Left
#define SPK_KEY_DOWN            ::Spark::Key::Down
#define SPK_KEY_UP              ::Spark::Key::Up
#define SPK_KEY_PAGE_UP         ::Spark::Key::PageUp
#define SPK_KEY_PAGE_DOWN       ::Spark::Key::PageDown
#define SPK_KEY_HOME            ::Spark::Key::Home
#define SPK_KEY_END             ::Spark::Key::End
#define SPK_KEY_CAPS_LOCK       ::Spark::Key::CapsLock
#define SPK_KEY_SCROLL_LOCK     ::Spark::Key::ScrollLock
#define SPK_KEY_NUM_LOCK        ::Spark::Key::NumLock
#define SPK_KEY_PRINT_SCREEN    ::Spark::Key::PrintScreen
#define SPK_KEY_PAUSE           ::Spark::Key::Pause
#define SPK_KEY_F1              ::Spark::Key::F1
#define SPK_KEY_F2              ::Spark::Key::F2
#define SPK_KEY_F3              ::Spark::Key::F3
#define SPK_KEY_F4              ::Spark::Key::F4
#define SPK_KEY_F5              ::Spark::Key::F5
#define SPK_KEY_F6              ::Spark::Key::F6
#define SPK_KEY_F7              ::Spark::Key::F7
#define SPK_KEY_F8              ::Spark::Key::F8
#define SPK_KEY_F9              ::Spark::Key::F9
#define SPK_KEY_F10             ::Spark::Key::F10
#define SPK_KEY_F11             ::Spark::Key::F11
#define SPK_KEY_F12             ::Spark::Key::F12
#define SPK_KEY_F13             ::Spark::Key::F13
#define SPK_KEY_F14             ::Spark::Key::F14
#define SPK_KEY_F15             ::Spark::Key::F15
#define SPK_KEY_F16             ::Spark::Key::F16
#define SPK_KEY_F17             ::Spark::Key::F17
#define SPK_KEY_F18             ::Spark::Key::F18
#define SPK_KEY_F19             ::Spark::Key::F19
#define SPK_KEY_F20             ::Spark::Key::F20
#define SPK_KEY_F21             ::Spark::Key::F21
#define SPK_KEY_F22             ::Spark::Key::F22
#define SPK_KEY_F23             ::Spark::Key::F23
#define SPK_KEY_F24             ::Spark::Key::F24
#define SPK_KEY_F25             ::Spark::Key::F25

/* Keypad */
#define SPK_KEY_KP_0            ::Spark::Key::KP0
#define SPK_KEY_KP_1            ::Spark::Key::KP1
#define SPK_KEY_KP_2            ::Spark::Key::KP2
#define SPK_KEY_KP_3            ::Spark::Key::KP3
#define SPK_KEY_KP_4            ::Spark::Key::KP4
#define SPK_KEY_KP_5            ::Spark::Key::KP5
#define SPK_KEY_KP_6            ::Spark::Key::KP6
#define SPK_KEY_KP_7            ::Spark::Key::KP7
#define SPK_KEY_KP_8            ::Spark::Key::KP8
#define SPK_KEY_KP_9            ::Spark::Key::KP9
#define SPK_KEY_KP_DECIMAL      ::Spark::Key::KPDecimal
#define SPK_KEY_KP_DIVIDE       ::Spark::Key::KPDivide
#define SPK_KEY_KP_MULTIPLY     ::Spark::Key::KPMultiply
#define SPK_KEY_KP_SUBTRACT     ::Spark::Key::KPSubtract
#define SPK_KEY_KP_ADD          ::Spark::Key::KPAdd
#define SPK_KEY_KP_ENTER        ::Spark::Key::KPEnter
#define SPK_KEY_KP_EQUAL        ::Spark::Key::KPEqual

#define SPK_KEY_LEFT_SHIFT      ::Spark::Key::LeftShift
#define SPK_KEY_LEFT_CONTROL    ::Spark::Key::LeftControl
#define SPK_KEY_LEFT_ALT        ::Spark::Key::LeftAlt
#define SPK_KEY_LEFT_SUPER      ::Spark::Key::LeftSuper
#define SPK_KEY_RIGHT_SHIFT     ::Spark::Key::RightShift
#define SPK_KEY_RIGHT_CONTROL   ::Spark::Key::RightControl
#define SPK_KEY_RIGHT_ALT       ::Spark::Key::RightAlt
#define SPK_KEY_RIGHT_SUPER     ::Spark::Key::RightSuper
#define SPK_KEY_MENU            ::Spark::Key::Menu