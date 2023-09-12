#include<iostream>
#include<string>
#include<cmath>
#include<cstring>
#include<windows.h>
#include<vector>
#include<conio.h>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

struct EXception {
	int getInfoException = -2;
	int calculateException = -3;
	int chatException = -5;
	int resultException = -100;
	int overrunException = -101;

	int workSucceeded = 1;
} Exception;

struct mathLib {
	float degreesToRadians(float degrees) {
		return degrees * M_PI / 180;
	}
	template<typename T> inline bool isNegative(T num) {
		return num < 0 ? true : false;
	}
	template<typename T> inline double square(T num) {
		return num * num;
	}
	int calculateStronghold(float x1, float x2, float z1, float z2, float r1,
	                        float r2, float &finalx, float &finalz) {
		try {
			float k1 = -tanf(r1),
			      k2 = -tanf(r2);
			if (k1 == k2 || x1 == x2 && z1 == z2) {
				printf("请确认测量时位于不同处!错误码:-100\n");
				system("pause");
				return Exception.resultException; //两点朝向平行 无交点 无解
			}
			finalx = -(((z1 * k1 * k2) - (z2 * k1 * k2) - (x1 * k2) + (x2 * k1)) /
			           (k2 - k1));
			finalz = -(((z1 * k1) - (z2 * k2) - x1 + x2) / (k2 - k1));
			return Exception.workSucceeded;
		} catch (...) {
			printf("计算时出现错误!错误码:-3\n");
			system("pause");
			exit(Exception.calculateException);
		}
	}
	int estimateStronghold(float x, float z, float r, float &finalx,
	                       float &finalz) {
		try {
			float k = -tanf(r),
			      distance = sqrt(square(x) + square(z));
			if (distance < 1728) {
				float delta = sqrtf(abs(powf(1728, 2) - powf(z * k, 2) + (powf(1728,
				                        2) * powf(k, 2)) + (2 * x * z * k) - powf(x, 2))),
				              divisor = powf(k, 2) + 1,
				              z1 = ((z * k * k) - delta - (x * k)) / divisor,
				              z2 = ((z * k * k) + delta - (x * k)) / divisor,
				              x1 = (-k * delta - (z * k) + x) / divisor,
				              x2 = (k * delta - (z * k) + x) / divisor;
				if (0 < r < 90) {
					if (x1 > x && z1 < z) finalx = x1, finalz = z1;
					else finalx = x2, finalz = z2;
				}//South-west -90 -180
				else if (90 < r < 180) {
					if (x1 > x && z1 > z) finalx = x1, finalz = z1;
					else finalx = x2, finalz = z2;
				}//North-west 0 -90
				else if (-90 < r < 0) {
					if (x1 < x && z1 < z) finalx = x1, finalz = z1;
					else finalx = x2, finalz = z2;
				}//South-east 90 180
				else if (-180 < r < -90) {
					if (x1 < x && z1 > z) finalx = x1, finalz = z1;
					else finalx = x2, finalz = z2;
				}//North-east 0 90
				else if (abs(r) == 90) {
					if (x1 > x > x2) finalx = x1, finalz = z1;
					else finalx = x2, finalz = z2;
				}
				return Exception.workSucceeded;
			} else {
				printf("超出范围!请在离原点1728格内测量\n");
				system("pause");
				return Exception.overrunException; //超出1728格
			}
		} catch (...) {
			printf("计算时出现错误!错误码:-3\n");
			system("pause");
			exit(Exception.calculateException);
		}
	}
} math;

struct f3InfoSplit {
	std::vector<std::string> split(std::string str) {
		char* s = new char[str.size() + 1];
		strcpy(s, str.c_str());
		char* p = strtok(s, " ");
		std::vector<std::string> words;
		while (p) {
			words.push_back(p);
			p = strtok(NULL, " ");
		}
		return words;
	}
	std::string splitOverworld(std::string f3Inf) {
		return f3Inf.substr(f3Inf.find("@s") + 3);
	}
	std::string splitNether(std::string f3Inf) {
		return f3Inf.substr(f3Inf.find("@s") + 3);
	}
	int getPlayerData(std::string Inf, float &x, float &z, float &rotation) {
		float INFO[5];
		int i = 0;
		try {
			for (auto tmp : split(Inf)) {
				INFO[i] = std::stof(tmp);
				i++;
			}
			x = INFO[0];
			z = INFO[2];

			rotation = INFO[3] - (int)INFO[3] + (int)INFO[3] % 360;
			//角度处理 超过360取模 南正北负 N ±180 W 90 S ±0 E -90
			if (0 < rotation < 180) rotation -=
				    180; //第二三象限正转化为负 -(180-r) -> r-180
			else if (-180 <= rotation <= 0) rotation += 180; //第一四象限负转化为正 r+180
			else {
				printf("获取角度时出错!错误码:-2");
				system("pause");
				exit(Exception.getInfoException);
			}
			rotation = math.degreesToRadians(rotation);
		} catch (...) {
			printf("获取玩家信息时出现错误!错误码:-2");
			system("pause");
			exit(Exception.getInfoException);
		}
		return Exception.workSucceeded;
	}
} split;

struct CMD {
	void gotoxy(int x, int y) {
		COORD pos = { (short)x, (short)y };
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hOut, pos);
	}
	void hideCursor(bool hide = true) {
		CONSOLE_CURSOR_INFO CursorInfo = {1, 0};
		CursorInfo.bVisible = !hide;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	}
} cmd;

struct {
	int doCalculate() {
		try {
			std::string* a = new std::string[2];
			std::string* b = new std::string[2];
			float* x = new float[3];
			float* z = new float[3];
			float* r = new float[2];

			cmd.hideCursor(true);
			printf("输入第一测量点的f3+c数据：\n");
			std::getline(std::cin, a[0]);
			a[1] = split.splitOverworld(a[0]);
			split.getPlayerData(a[1], x[0], z[0], r[0]);
			printf("输入第二测量点的f3+c数据：\n");
			std::getline(std::cin, b[0]);
			b[1] = split.splitOverworld(b[0]);
			split.getPlayerData(b[1], x[1], z[1], r[1]);
			math.calculateStronghold(x[0], x[1], z[0], z[1], r[0], r[1], x[2], z[2]);
			printf("要塞可能的坐标为：\nx:%.1f\nz:%.1f\n", x[2], z[2]);
			system("pause");
			system("cls");
			delete[] x, z, r, a, b;
			return Exception.workSucceeded;
		} catch (...) {
			printf("对话时出现错误!错误码:-5\n");
			system("pause");
			exit(Exception.chatException);
		}
	}
	int doEstimate() {
		try {
			std::string* a = new std::string[2];
			float* x = new float[2];
			float* z = new float[2];
			float* r = new float[1];

			cmd.hideCursor(true);
			printf("输入测量点的f3+c数据：\n");
			std::getline(std::cin, a[0]);
			a[1] = split.splitOverworld(a[0]);
			split.getPlayerData(a[1], x[0], z[0], r[0]);
			math.estimateStronghold(x[0], z[0], r[0], x[1], x[2]);
			printf("要塞可能的坐标为：\nx:%.1f\nz:%.1f\n", x[1], z[1]);
			system("pause");
			system("cls");
			delete[] x, z, r, a;
			return Exception.workSucceeded;
		} catch (...) {
			printf("对话时出现错误!错误码:-5\n");
			system("pause");
			exit(Exception.chatException);
		}
	}
	int tutorial() {
		Sleep(200);
		printf("测量教程：首先需要使自己处于不动的状态。接着，打开设置，找到Fov，拉至最小；在控制->鼠标设置中找到鼠标灵敏度，拉至最低。接着，回到游戏，投掷末影之眼，并快速将准星对准末影之眼中间的深绿色部分，若要更加精准，请对准偏左侧次深绿和中部深绿的中间部分。对准后，短按F3+c，获取坐标和角度数据，右键复制到命令行中，即可完成一次末影之眼的测量。一般而言，双眼测量的偏差不会超过300格。\n");
		system("pause");
		system("cls");
		return Exception.workSucceeded;
	}
} run;

struct GUI {
	void printMenu() {
		printf("末影之眼-要塞计算器\n");
		printf("-----------菜单-----------\n");
		printf("请选择模式，按Enter键确认：\n");
		printf(">>1.双坐标焦点测算法\n");
		printf("  2.单坐标估算法（测量者需在距原点1728格内的圆内）\n");
		printf("  3.教程\n");
		printf("  4.退出\n");
	}
	void controlMenu() {
		int choice = 1, line = 3, Up = 35, Down = 43;
		char c1, c2, ch;
		do {
			printMenu();
			cmd.gotoxy(0, line);
			printf(">>");
			cmd.gotoxy(0, line);
			printf(">>");
			while ((c1 = getch()) != '\r') {
				c2 = getch();
				if (c1 == -32) {
					ch = c2 - 37;
				}
				if (ch == Up && choice > 1) {
					choice --;
					if (choice == 1) {
						cmd.gotoxy(0, line);
						printf("  ");
						line --;
						cmd.gotoxy(0, line);
						printf(">>");
					}
					if (choice == 2) {
						cmd.gotoxy(0, line);
						printf("  ");
						line --;
						cmd.gotoxy(0, line);
						printf(">>");
					}
					if (choice == 3) {
						cmd.gotoxy(0, line);
						printf("  ");
						line --;
						cmd.gotoxy(0, line);
						printf(">>");
					}
				}

				if (ch == Down && choice < 4) {
					choice ++;
					if (choice == 2) {
						cmd.gotoxy(0, line);
						printf("  ");
						line ++;
						cmd.gotoxy(0, line);
						printf(">>");
					}
					if (choice == 3) {
						cmd.gotoxy(0, line);
						printf("  ");
						line ++;
						cmd.gotoxy(0, line);
						printf(">>");
					}
					if (choice == 4) {
						cmd.gotoxy(0, line);
						printf("  ");
						line ++;
						cmd.gotoxy(0, line);
						printf(">>");
					}
				}
				Sleep(10);
			}
			cmd.gotoxy(0, 6);
			switch (choice) {
				case 1:
					system("cls");
					run.doCalculate();
					break;
				case 2:
					system("cls");
					run.doEstimate();
					break;
				case 3:
					system("cls");
					run.tutorial();
					break;
				case 4:
					exit(1);
				default:
					getch();
					system("cls");
					break;
			}
		} while (choice != 4);
	}
} gui;

int main() {
	cmd.hideCursor(true);
	Sleep(100);
	printf("末影之眼-要塞计算器\n");
	Sleep(300);
	printf("----");
	system("pause");
	Sleep(100);
	system("cls");
	cmd.hideCursor(false);
	gui.controlMenu();
	return 0;
}
