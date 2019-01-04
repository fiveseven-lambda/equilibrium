#define _GNU_SOURCE
#include <string.h>
#include <ncurses.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>

#define NAME_LEN 100

struct Weak{
	int step;
	char name[10][NAME_LEN];
	double Ka[10];
	double conc_sum;
	double conc[10];
} weak[1000];
int n;

int main(int argc, char **argv){
	double amount = 100, conc = .01, add = 1;
	double pKw;
	int cursor[2] = {-2, 0};
	double charge;
	_Bool verbose = 0;
	int tab = 0;

	char proton[NAME_LEN], hydroxide[NAME_LEN];
	double h = 1e-7;

	double acid = 0, base = 0;

	initscr();
	cbreak();

	FILE *fp;
	if(argc == 1){
		char filename[100];
		getstr(filename);
		fp = fopen(filename, "r");
	}else{
		fp = fopen(argv[1], "r");
	}
	noecho();
	curs_set(0);
	if(fp == NULL){
		printw("cannot open file: %s", strerror(errno));
		getch();
		return endwin();
	}
	fscanf(fp, "%s%s%lf", proton, hydroxide, &pKw);
	for(char buf[10000]; fgets(buf, sizeof buf, fp) != NULL;){
		buf[strlen(buf) - 1] = '\0';
		for(int i = 0;; ++i){
			if(buf[i] == '\0' || buf[i] == '='){
				int tmp;
				if(sscanf(buf, "%d", &tmp) == 1)
					if(buf[i] == '\0') weak[n++].step = tmp;
					else{
						int len = strlen(buf + i + 1) + 2;
						if(tab < len) tab = len;
						strcpy(weak[n - 1].name[tmp], buf + i + 1);
					}
				else if(buf[i] == '='){
					buf[i] = '\0';
					if(buf[0] == 'p' && buf[1] == 'K'){
						if(sscanf(buf + 3, "%d", &tmp) == 1) --tmp;
						else tmp = 0;
						sscanf(buf + i + 1, "%lf", &weak[n - 1].Ka[tmp]);
						if(buf[2] == 'b') weak[n - 1].Ka[tmp] = 14 - weak[n - 1].Ka[tmp];
					}
				}
				break;
			}
		}
	}
	
	for(struct Weak *i = weak; i < weak + n; ++i) for(int j = 0; j < i->step; ++j) i->Ka[j] = exp10(-i->Ka[j]);

	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	double max = 1e-7, min = 1e-7;

	for(;;){
		int ch = getch();
		switch(ch){
			case 10:
				move(0, 0);
				echo();
				nodelay(stdscr, FALSE);
				curs_set(1);
				char buf[16];
				getstr(buf);
				if(!strcmp(buf, "quit")) return endwin();
				else if(!strcmp(buf, "discard")) amount = 0;
				else if(!strcmp(buf, "amount")) scanw("%lf", &amount);
				else if(!strcmp(buf, "conc")) scanw("%lf", &conc);
				else if(!strcmp(buf, "add")) scanw("%lf", &add);
				erase();
				noecho();
				nodelay(stdscr, TRUE);
				curs_set(0);
				break;
			case KEY_UP:
				if(cursor[0] == -2) cursor[0] = -3;
				else if(cursor[0] == -1) cursor[0] = -2;
				else if(cursor[0] != -3){
					if(cursor[1] == 0)
						if(cursor[0] == 0) cursor[0] = -1;
						else cursor[1] = weak[--cursor[0]].step;
					else --cursor[1];
				}
				break;
			case KEY_DOWN:
				if(cursor[0] == -3) cursor[0] = -2;
				else if(cursor[0] == -2) cursor[0] = -1;
				else if(cursor[0] == -1) cursor[0] = cursor[1] = 0;
				else if(cursor[1] == weak[cursor[0]].step){
					if(cursor[0] != n - 1){
						++cursor[0];
						cursor[1] = 0;
					}
				}else ++cursor[1];
				break;
			case ' ':
				acid = (acid * amount + (cursor[0] == -2) * conc * add) / (amount + add);
				base = (base * amount + ((cursor[0] == -1) + cursor[1]) * conc * add) / (amount + add);
				for(int i = 0; i < n; ++i){
					weak[i].conc_sum = (weak[i].conc_sum * amount + (cursor[0] == i) * conc * add) / (amount + add);
				}
				amount += add;
				max = 1, min = 0;
				break;
			case 'v':
				verbose ^= 1;
				erase();
				break;
		}
		for(int i = 0; i < 100; ++i){
			h = (max + min) / 2;
			charge = h - 1e-14 / h - acid + base;
			for(struct Weak *j = weak; j < weak + n; ++j){
				double tmp[10], sum = 1;
				tmp[0] = 1;
				for(int k = 0; k < j->step; ++k){
					tmp[k + 1] = tmp[k] * j->Ka[k] / h;
					sum += tmp[k + 1];
				}
				j->conc[0] = j->conc_sum / sum;
				for(int k = 1; k <= j->step; ++k){
					charge -= (j->conc[k] = j->conc[0] * tmp[k]) * k;
				}
			}
			if(charge > 0) max = h;
			else min = h;
		}

		int pos = 2;

		move(pos, 0);
		if(cursor[0] == -3) standout();
		printw("amount = %f mL", amount);
		standend();
		move(++pos, 0);
		if(cursor[0] == -2) standout();
		addch(' ');
		printw("%s", proton);
		move(pos, tab);
		printw("%.2e mol/L", h);
		standend();
		move(++pos, 0);
		if(cursor[0] == -1) standout();
		addch(' ');
		printw("%s", hydroxide);
		move(pos, tab);
		printw("%.2e mol/L", 1e-14 / h);
		standend();
		if(verbose){
			move(++pos, 0);
			printw("strong acid %.2e mol/L", acid);
			move(++pos, 0);
			printw("strong base %.2e mol/L", base);
		}
		for(int i = 0; i < n; ++i){
			for(int j = 0; j <= weak[i].step; ++j){
				move(++pos, 0);
				if(cursor[0] == i) standout();
				addch(' ');
				if(cursor[1] != j) standend();
				printw("%s", weak[i].name[j]);
				move(pos, tab);
				printw("%.2e mol/L", weak[i].conc[j]);
				standend();
			}
			if(verbose){
				move(++pos, 0);
				printw("sum %.2e", weak[i].conc_sum);
				for(int j = 0; j < weak[i].step; ++j){
					move(++pos, 0);
					printw("pKa%d = %.2f", j + 1, log10(weak[i].conc[j]) - log10(h) - log10(weak[i].conc[j + 1]));
				}
			}
		}

		pos += 2;
		move(pos, 0);
		printw("ph = %.2f", -log10(h));
		
		if(verbose){
			move(++pos, 0);
			printw("charge = %.10f", charge);
		}

		refresh();
	}
}
