#include<graphics.h>

void wait_for_char();


int main()
{
	int gd = DETECT, gm = 0;
	initgraph(&gd, &gm,""); // initialize graphics
	circle(300, 300, 150); // graphics function for circle
	getch(); // this is must to hold output!
	wait_for_char();
	//closegraph();
	return 0;
}

void wait_for_char()
{

    //Wait for a key press
    int in = 0;

    while (in == 0) {
        in = getchar();
    }
}
