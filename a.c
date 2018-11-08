#include <string.h>
#include <ncurses.h>
#include <math.h>

#define N 4

void water(h, oh, Kw)
double *h, *oh;
const double Kw;
{
	double x = (- *h - *oh + sqrt((*h + *oh) * (*h + *oh) - *h * *oh * 4 + Kw * 4)) / 2;
	*h += x;
	*oh += x;
}

void acid(ah, a, h, Ka)
double *ah, *a, *h;
const double Ka;
{
	double x = (- *a - *h - Ka + sqrt((*a + *h + Ka) * (*a + *h + Ka) - 4 * *a * *h + 4 * *ah * Ka)) / 2;
	*ah -= x;
	*a += x;
	*h += x;
}

int main(void){
	double c[N] = {};
	double amount = .1, drop = 5e-5, c_drop = .01;
	double add = .01;
	const char name[N][10] = {"H+", "OH-", "CH3COOH", "CH3COO-"};
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	for(;;){
		int in = getch();
		if(in == 10){
			char buf[16];
			move(N + 8, 0);
			echo();
			nodelay(stdscr, FALSE);
			getstr(buf);
			if(!strcmp(buf, "quit")) return endwin();
			else if(!strcmp(buf, "amount")) scanw("%lf", &amount);
			else if(!strcmp(buf, "discard")) amount = 0;
			else if(!strcmp(buf, "conc")) scanw("%lf", &c_drop);
			else if(!strcmp(buf, "water")) c_drop = 0;
			else if(!strcmp(buf, "add")) scanw("%lf", &drop);
			else if(!strcmp(buf, "drop")) drop = 5e-5;
			erase();
			noecho();
			nodelay(stdscr, TRUE);
		}else if('a' <= in && in < 'a' + N){
			for(int i = 0; i < N; ++i) c[i] = (c[i] * amount + (i == in - 'a' ? c_drop * drop : 0)) / (amount + drop);
			amount += drop;
		}

		water(c, c + 1, 1e-14);
		acid(c + 2, c + 3, c, pow(10, -4.76));

		for(int i = 0; i < N; ++i){
			move(i + 2, 0);
			addstr(name[i]);
			move(i + 2, sizeof name[0]);
			printw("%.10f mol/L", c[i]);
		}

		move(0, 0);
		printw("amount = %f mL", amount * 1e+3);

		move(N + 3, 0);
		printw("pH = %5.2f", - log10(c[0]));

		move(N + 5, 0);
		printw("pKw = %f", - log10(c[0] * c[1]));
		move(N + 6, 0);
		printw("pKa(AcOH) = %f", - log10(c[0] * c[3] / c[2]));

		refresh();
	}
}
