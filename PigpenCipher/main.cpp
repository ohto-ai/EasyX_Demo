#include <easyx.h>
#include <string>

namespace thatboy
{
	namespace Pigpen
	{

		enum : UINT
		{
			PIG_NULL = 0
			, LINE_LEFT = 0X0001 // │
			, LINE_TOP = 0X0002 // ─
			, LINE_RIGHT = 0X0004 // │
			, LINE_BOTTON = 0X0008 // ─
			, ARROW_RIGHT = 0X0010 // >
			, ARROW_DOWN = 0X0020 // V
			, ARROW_LEFT = 0X0040 // <
			, ARROW_UP = 0X0080 // ^
			, LINE_DOT = 0X0100 //	·
		};

		constexpr static UINT alpha2Pig[] =
		{
			PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL

			// ABC DEF GHI
			, LINE_BOTTON | LINE_RIGHT				, LINE_BOTTON | LINE_LEFT | LINE_RIGHT				, LINE_BOTTON | LINE_LEFT
			, LINE_BOTTON | LINE_TOP | LINE_RIGHT	, LINE_BOTTON | LINE_TOP | LINE_LEFT | LINE_RIGHT	, LINE_BOTTON | LINE_TOP | LINE_LEFT
			, LINE_TOP | LINE_RIGHT					, LINE_TOP | LINE_LEFT | LINE_RIGHT					, LINE_TOP | LINE_LEFT
			// JKL MNO PQR
			, LINE_DOT | LINE_BOTTON | LINE_RIGHT				, LINE_DOT | LINE_BOTTON | LINE_LEFT | LINE_RIGHT				, LINE_DOT | LINE_BOTTON | LINE_LEFT
			, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_RIGHT	, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_LEFT | LINE_RIGHT	, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_LEFT
			, LINE_DOT | LINE_TOP | LINE_RIGHT					, LINE_DOT | LINE_TOP | LINE_LEFT | LINE_RIGHT					, LINE_DOT | LINE_TOP | LINE_LEFT
			// STUV
			, ARROW_RIGHT | ARROW_DOWN, ARROW_RIGHT | ARROW_LEFT, ARROW_DOWN | ARROW_UP, ARROW_LEFT | ARROW_UP
			// WXYZ
			, LINE_DOT | ARROW_RIGHT | ARROW_DOWN, LINE_DOT | ARROW_RIGHT | ARROW_LEFT, LINE_DOT | ARROW_DOWN | ARROW_UP, LINE_DOT | ARROW_LEFT | ARROW_UP
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL

			// ABC DEF GHI
			, LINE_BOTTON | LINE_RIGHT				, LINE_BOTTON | LINE_LEFT | LINE_RIGHT				, LINE_BOTTON | LINE_LEFT
			, LINE_BOTTON | LINE_TOP | LINE_RIGHT	, LINE_BOTTON | LINE_TOP | LINE_LEFT | LINE_RIGHT	, LINE_BOTTON | LINE_TOP | LINE_LEFT
			, LINE_TOP | LINE_RIGHT					, LINE_TOP | LINE_LEFT | LINE_RIGHT					, LINE_TOP | LINE_LEFT
			// JKL MNO PQR
			, LINE_DOT | LINE_BOTTON | LINE_RIGHT				, LINE_DOT | LINE_BOTTON | LINE_LEFT | LINE_RIGHT				, LINE_DOT | LINE_BOTTON | LINE_LEFT
			, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_RIGHT	, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_LEFT | LINE_RIGHT	, LINE_DOT | LINE_BOTTON | LINE_TOP | LINE_LEFT
			, LINE_DOT | LINE_TOP | LINE_RIGHT					, LINE_DOT | LINE_TOP | LINE_LEFT | LINE_RIGHT					, LINE_DOT | LINE_TOP | LINE_LEFT
			// STUV
			, ARROW_DOWN, ARROW_RIGHT, ARROW_LEFT, ARROW_UP
			// WXYZ
			, LINE_DOT | ARROW_DOWN, LINE_DOT | ARROW_RIGHT, LINE_DOT | ARROW_LEFT, LINE_DOT | ARROW_UP
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL

			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL

			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
			, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL, PIG_NULL
		};

		IMAGE& drawPig(char ch, IMAGE& view, int pigSize, int x = 0, int y = 0, COLORREF foregroundColor = BLACK, COLORREF backgroundColor = WHITE)
		{
			auto lastDevice = GetWorkingImage();
			if (x >= view.getwidth() || y >= view.getheight())
				return view;

			IMAGE pig(pigSize, pigSize);
			SetWorkingImage(&pig);


			setbkcolor(backgroundColor);
			setlinecolor(foregroundColor);
			setfillcolor(foregroundColor);
			setlinestyle(PS_SOLID, pigSize / 16);

			int beginPos = pigSize / 8;
			int endPos = pigSize - beginPos;
			int midPos = pigSize / 2;

			cleardevice();

			if (alpha2Pig[ch] & LINE_LEFT)
				line(beginPos, beginPos, beginPos, endPos);
			if (alpha2Pig[ch] & LINE_TOP)
				line(beginPos, beginPos, endPos, beginPos);
			if (alpha2Pig[ch] & LINE_RIGHT)
				line(endPos, beginPos, endPos, endPos);
			if (alpha2Pig[ch] & LINE_BOTTON)
				line(beginPos, endPos, endPos, endPos);

			if (alpha2Pig[ch] & ARROW_DOWN)
			{
				line(beginPos, beginPos, midPos, endPos);
				line(midPos, endPos, endPos, beginPos);
			}
			else if (alpha2Pig[ch] & ARROW_RIGHT)
			{
				line(beginPos, beginPos, endPos, midPos);
				line(endPos, midPos, beginPos, endPos);
			}
			else if (alpha2Pig[ch] & ARROW_LEFT)
			{
				line(beginPos, midPos, endPos, beginPos);
				line(beginPos, midPos, endPos, endPos);
			}
			else if (alpha2Pig[ch] & ARROW_UP)
			{
				line(beginPos, endPos, midPos, beginPos);
				line(midPos, beginPos, endPos, endPos);
			}

			if (alpha2Pig[ch] & LINE_DOT)
				fillcircle(midPos, midPos, pigSize / 16);

			SetWorkingImage(&view);
			putimage(x, y, &pig);

			SetWorkingImage(lastDevice);

			return view;
		}


		// 
		template<size_t PigSize = 32, COLORREF BackgroundColor = WHITE, COLORREF ForegroundColor = BLACK>
		class String
			: protected IMAGE
		{
			void SetDefault() {}
		public:
			String& assign(const char* lpStr)
			{
				return assign(std::string(lpStr));
			}
			String& assign(const std::string& str)
			{
				Resize(PigSize * str.length(), PigSize);
				for (size_t i = 0; i < str.length(); i++)
					drawPig(str[i], *this, PigSize, i * PigSize);
				return *this;
			}
			String& operator = (String&) = default;
			String& operator = (const char* lpStr) { return assign(lpStr); }
			String& operator = (const std::string& str) { return assign(str); }

			operator const IMAGE* ()
			{
				return this;
			}
		};

	}
}

int main()
{
	initgraph(900, 480);
	using thatboy::Pigpen::String;

	String<80> pig;
	pig = "Hello EasyX";
	putimage(0, 0, pig);
	saveimage(TEXT("pig.bmp"), const_cast<IMAGE*>(static_cast<const IMAGE*>(pig)));
	Sleep(5000);
	return 0;
};