#include <iostream>
#include <string>

using namespace std;


int main(int argc, char ** argv) {
	for (int i = 0; i < 256; i++) {
		string color = "\033[48;2;" + to_string(i) + ";" + to_string(i) + ";" + to_string(i) + "m";
	  cout << color << " " << "\033[0m";
	}
	cout << endl;
  return 0;
}


// 2-wide unicode characters for map display
// const wchar_t* map_chars[] = {
//   L"\u3000", // "　"
//   L"\u2B1B", // "⬛"
//   L"\u2B1C", // "⬜"
//   L"\u3267", // "㉧"
//   L"\u26AB", // "⚫"
//   L"\uFF1F"  // "？"
// };
// ANSI control
//   const char* map_chars[] = {
//   "  ", // empty space
//   "\033[48;2;52;63;86m  \033[0m", // wall, rgb(52, 63, 86)
//   "\033[48;2;251;147;0m  \033[0m", // immune wall, rgb(251, 147, 0)
//   "\033[48;2;159;230;160m  \033[0m", // snake head, rgb(159, 230, 160)
//   "\033[48;2;74;169;108m  \033[0m", // snake body, rgb(74, 169, 108)
//   "??"  // import error
// };