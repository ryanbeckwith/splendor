# splendor
A console-based implementation of the physical card game known as Splendor. This program includes a robust implementation of the Vector functionality provided natively by C++ in a class called "Vector". Written as a final project for an introductory course in C++.\
\
### Rules
The rules for the game of Splendor can be found at https://www.ultraboardgames.com/splendor/game-rules.php, and a short video tutorial of the rules can be found at https://www.youtube.com/watch?v=2A0CQ0xsrv0. \
Note that this implementation does not include the "Noble" functionality.
### Commands
1) p3 color1 color2 color3 - Buys three gems of the specified colors and places them into your gem bank. Note that the valid colors are blue, green, white, red, and pink.
2) p2 color - Buys two gems of the specified color, so long as that stack of gems is full.
3) r row column - Reserves a card from the specified row name and column number, placing the card into your hand and giving you a gold gem. Note that the possible rows are m (for Mines), t (for Transport), and v (for Vendors). The possible columns range from 1 to 4, with 1 representing the leftmost column.
4) b row column - Buys a card from the specified row name and column number, automatically applying gems and discounts from your bank. Updates your prestige level accordingly.
5) br position - Buys a card from the specified position in your hand of reserved cards. The possible positions range from 1 to 3, with 1 representing the leftmost hand position.
### Running this program
1) Download and place all files in the same sublevel on your local machine.
2) Navigate to their location and compile all .cpp files using clang++ \*.cpp in a Linux terminal.
3) Run the executable file using ./a.out splendor.data false, ensuring that your terminal window is maximized. If your display does not support resolutions of at least 1080p, unexpected issues may occur.\
Note that an up-to-date C++ compiler is required to run this program. This program was intended to use the compiler Clang, and use of other compilers may result in unexpected behavior.
