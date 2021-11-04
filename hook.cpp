#include <Windows.h>
#include <stdlib.h>
#include <time.h>

const unsigned int range1_lo = 0x300;
const unsigned int range1_hi = 0x367;
const unsigned int range1_size = range1_hi - range1_lo + 1;
const unsigned int range2_lo = 0x483;
const unsigned int range2_hi = 0x489;
const unsigned int range2_size = range2_hi - range2_lo + 1;
const unsigned max_distortion_counter = 100;
const unsigned max_distortion_probability = 75;
const unsigned distortion_cooldown = 60;

unsigned int distortion_probability = 0;
clock_t last_character_time;

extern "C" __declspec(dllexport) LRESULT CALLBACK hook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code != HC_ACTION) return CallNextHookEx(NULL, code, wParam, lParam);

	MSG *message = (MSG*)lParam;
	if (message->message == WM_CHAR
	&& !(message->wParam <= 0x20) && !(message->wParam >= 0x7F && message->wParam <= 0xA0) && !(message->wParam == 0xAD)
	&& !(message->wParam >= range1_lo && message->wParam <= range1_hi) && !(message->wParam >= range2_lo && message->wParam <= range2_hi))
	{
		clock_t character_time = clock();
		if ((character_time - last_character_time) / CLOCKS_PER_SEC > distortion_cooldown) distortion_probability = 0;
		last_character_time = character_time;

		if (message->wParam == '!' && rand() % max_distortion_counter < distortion_probability) message->wParam = 0xA1; 
		if (message->wParam == '?' && rand() % max_distortion_counter < distortion_probability) message->wParam = 0xBF;
		for (size_t i = 0; i < 5 && rand() % max_distortion_counter < distortion_probability; i++)
		{
			unsigned int symbol = rand() % (range1_size + range2_size);
			if (symbol < range1_size) symbol = symbol + range1_lo;
			else symbol = symbol - range1_size + range2_lo;
			PostMessageW(message->hwnd, WM_CHAR, symbol, 0);
		}

		if (distortion_probability < max_distortion_probability) distortion_probability++;
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}