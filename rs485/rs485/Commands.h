namespace Commands
{
	enum
	{
		ADDRESS = 'A',
		RESET = 'R',
		MAX_POSITION = '1',
		MAX_SPEED = '2',
		MIN_SPEED = '3',
		HUMMER_POSITION = '4',
		DOVODKA_POSITION = '5',
		ECHO = 'E',
		NULL = '0'
	};
}

namespace Modes
{
	enum
	{
		NO = 0,
		STOP = 1,
		OPEN = 2,
		CLOSE = 3,
		DOVODKA = 4,
		HUMMER = 5,
		KONCEVIK = 6,
		BACK = 7,
		WAITING = 8
	};
}

namespace Loading
{
	enum
	{
		NORMAL = 0,
		PROGRAMMING = 1,
		WAITING = 3
	};
}