

//#include<windows.h>
//#include<string>
//#include<iostream>
#include<stdio.h>
#include<sstream>
#include<D:\Đồ họa máy tính\Final Project\Breakout bricks\Dependencies\freeglut/glut.h>
#include <sys/timeb.h>
#include <sys/utime.h>
#include <time.h>
// hằng số
#define PI 3.142
#define STEPS 40
#define BARX 720/2-80/2 // lấy trọng tâm mà hình lệch sang trái bằng nửa chiều rộng của thanh bar
#define BARY  465
#define BALLX 720 / 2 // chia đôi màn hình => lấy trọng tâm
#define BALLY 465 -15 // cách vị trí của bar 15 đơn vị


using namespace std;
int widthScreen = 720; int heightScreen = 480;//khởi tạo kích thước màn hình 
float barWidth = 80, barX = widthScreen / 2 - barWidth / 2, barY = 465, barheight = 5; // Kích thước của thanh hứng bóng
float ballX = widthScreen / 2, ballY = barY - 15, ballWH = 5, ballVelX = 0.3, ballVelY = 0.3; //vị trí, kích thước và tốc độ ban đầu của bóng của bóng
int brickAmount = 100;
int score = 0, chances = 3, previousScore = 0, highestScore = 0;
bool flag = true, flag2 = true, flagSeconds = false, flagStart = true;
int totalTime = 0, prevTime = 0, countt = 0;
int brickYY = 50;
float Red = 0.0, Green = 0.0, Blue = 0.0;
// hàm trả về miliSeconds
int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int count = tb.millitm + (tb.time & 0xfffff) * 1000;
	return count;
}
// hàm ngủ với tham số và là miliSeconds 
void sleep(int sleeptime) {
	int count = 0;
	int beginsleep = getMilliCount();
	while (getMilliCount() - beginsleep < sleeptime)
	{
		count++;
	}
}
//cấu trúc của viên gạch
struct brick {
	float x;
	float y;
	float width;
	float height;
	bool isAlive = true;
};
brick bricksArray[200]; // khởi tạo mãng có 200 viên gạch

// hàm xác định vị tri và vẽ gạch
void drawBricks(bool isTrue) {

	float brickX = widthScreen / 4.5, brickY = brickYY;

	for (int i = 0; i < brickAmount; i++) {
		if (brickX > 400 + widthScreen / 4.5)
		{// mỗi hàng 10 viên gạch
			brickX = widthScreen / 4.5;// Khởi tạo lại tọa độ x
			brickY += 11;// tăng tọa độ y lên 11 đơn vị
		}
		bricksArray[i].x = brickX;
		bricksArray[i].y = brickY;
		bricksArray[i].width = 40;
		bricksArray[i].height = 10;
		brickX += 41;
	}



	glBegin(GL_QUADS);// bắt đầu vẽ viên gạch
	glColor3f(1.0, 0.5, 1);// tô màu  cho viên gạch

	for (int i = 0; i < brickAmount; i++) {

		if (bricksArray[i].isAlive == true) {// kiễm tra viên gạch đó đã bị ăn hay chưa

			glVertex2f(bricksArray[i].x, bricksArray[i].y);
			glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y);
			glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y + bricksArray[i].height);
			glVertex2f(bricksArray[i].x, bricksArray[i].y + bricksArray[i].height);
		}


	}
	glEnd();
}
// vẽ bóng
void drawBall(GLint x, GLint y, GLint radius)
{
	glColor3f(1.0, 0, 0);
	GLfloat twice = (GLfloat)2.0f * PI;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(x, y);
	for (int i = 0; i <= STEPS; i++)
	{

		glVertex2i(
			(GLint)(x + (radius * cos(i * twice / STEPS)) + 0.5),
			(GLint)(y + (radius * sin(i * twice / STEPS)) + 0.5)
		);
	}
	glEnd();
}
// hàm này in thông số
void print(int a) {
	glRasterPos2f(widthScreen * 5 / 100, heightScreen * 5 / 100);// vị trí print theo (x,y)
	stringstream ss;
	ss << a;
	string s = "Score: " + ss.str();
	int len = s.length();
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
	}
	glRasterPos2f(widthScreen * 25 / 100, heightScreen * 5 / 100);
	stringstream ss2;
	ss2 << chances;
	string chance = "Chances : " + ss2.str();
	for (int i = 0; i < chance.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, chance[i]);
	}
	glRasterPos2f(widthScreen * 50 / 100, heightScreen * 5 / 100);
	stringstream ss3;
	ss3 << previousScore;
	string prevScore = "Previous score: " + ss3.str();
	for (int i = 0; i < prevScore.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, prevScore[i]);
	}
	glRasterPos2f(widthScreen * 70 / 100, heightScreen * 5 / 100);
	stringstream ss4;
	ss4 << highestScore;
	string highScore = "Highest score: " + ss4.str();
	for (int i = 0; i < highScore.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, highScore[i]);
	}
}


void alertStart(bool a) {
	if (a == true) {
		glColor3f(0, 0, 0);
		glRasterPos2f(widthScreen / 2 - 70, heightScreen / 2);
		string ssss = "Click to start new game";
		int lentt = ssss.length();
		for (int i = 0; i < lentt; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ssss[i]);
		}
	}
}

//hàm này in ra màn hình khi thua.
void alert(bool a) {
	if (a == false) {
		glColor3f(0, 0, 0);
		glRasterPos2f(widthScreen / 2 - 45, heightScreen / 2);

		string s = "Game over";
		int len = s.length();
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
		}


		glRasterPos2f(widthScreen / 2 - 45, heightScreen / 2 + 20);
		stringstream ss;
		ss << previousScore;
		string sss = "Your scrore : " + ss.str();
		int lent = sss.length();
		for (int i = 0; i < lent; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, sss[i]);
		}

		glRasterPos2f(widthScreen / 2 - 70, heightScreen / 2 + 40);
		string ssss = "Click to start new game";
		int lentt = ssss.length();
		for (int i = 0; i < lentt; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ssss[i]);
		}
		//trả về vị trí và số lượng ban đầu 
		brickYY = 50;
		brickAmount = 80;
	}

}

void alertWin(bool a) {
	if (a == false) {

		glColor3f(0, 0, 0);
		string s = "CONGRATULATIONS !!";
		int lens = s.length();
		glRasterPos2f(widthScreen / 2 - 70, heightScreen / 2);
		for (int i = 0; i < lens; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
		}

		glRasterPos2f(widthScreen / 2 - 40, heightScreen / 2 + 20);
		stringstream sc;
		sc << previousScore;
		string ss = "Your score : " + sc.str();
		int lenc = ss.length();

		for (int i = 0; i < lenc; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ss[i]);
		}

		glRasterPos2f(widthScreen / 2 - 70, heightScreen / 2 + 40);


		string sss = " Click to start new game.";
		int lenr = sss.length();

		for (int i = 0; i < lenr; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, sss[i]);
		}
	}
}


void Display(void) {

	//lấy 5s
	int timeD = getMilliCount();
	// mỗi 5s sẽ tăng số lượng và vị trí y lên...
	if (!flagSeconds)
	{
		flagSeconds = true;
		prevTime = timeD + 20000;
	}
	if (timeD >= prevTime)
	{
		/*   ballVelX = 0.5;
		   ballVelY = 0.5;*/
		brickYY += 11;
		brickAmount += 10;
		flagSeconds = false;
		if (brickAmount >= 200) flag == false; // số lượng gạch lớn hơn 200 => thua ..(200 là số lượng khởi tạo ban đầu của mảng gạch)
	}
	//
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	//Bar
	glBegin(GL_QUADS);
	glColor3ub(255, 204, 102);
	glVertex2f(barX, barY);
	glVertex2f(barX + barWidth, barY);
	glVertex2f(barX + barWidth, barY + barheight);
	glVertex2f(barX, barY + barheight);
	glEnd();

	//Ball
	drawBall(ballX, ballY, ballWH);

	//sidebar
	glBegin(GL_QUADS);

	glColor3ub(0, 0, 0);

	glVertex2f(0, heightScreen * 7 / 100);
	glVertex2f(widthScreen, heightScreen * 7 / 100);
	glVertex2f(widthScreen, heightScreen * 8 / 100);
	glVertex2f(0, heightScreen * 8 / 100);


	glEnd();

	print(score);
	drawBricks(flagSeconds);

	alertStart(flagStart);
	alert(flag);
	alertWin(flag2);

	glutSwapBuffers();
}

// hàm khởi tạo
void Initial(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glViewport(0, 0, 720, 480); // vị trí bắt đầu x,y = 0, w = 720, h =480
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, widthScreen, heightScreen, 0); // chiều của màn hình

}
// hàm check va chạm giữa bóng và vật thể 
bool checkCollide(float baX, float baY, float baW, float baH, float brX, float brY, float brW, float brH) {


	if (baY + baH < brY)// nếu bóng không chạm gạch theo chiều cao(y) => chưa tới
		return false;
	else if (baY > brY + brH) //nếu bóng không chạm gạch theo chiều cao(y) => bóng phía trên, gạch phía dưới
		return false;
	else if (baX + baW < brX) //nếu bóng không chạm gạch theo chiều ngang(x) = > chưa tới
		return false;
	else if (baX > brX + brW) //nếu bóng không chạm gạch theo chiều ngang(x) = > chưa tới
		return false;
	else
		return true;
}

// hàm reset lại thông số ban đầu
void reset()
{
	ballX = BALLX;
	ballY = BALLY;
	barX = BARX;
	barY = BARY;
}

// hàm xử lý di chuyển của bóng
void moveBall() {
	bool istrue = false;/// kiểm còn bất kỳ viên gạch nào là true không...
	for (int i = 0; i < brickAmount; i++) {

		if (bricksArray[i].isAlive == true) {
			istrue = false;
			break;
		}
		else {
			istrue = true;
		}

	}
	if (istrue) {// full điểm reset lại các biến
		reset();
		ballVelX = 0;
		ballVelY = 0;

		for (int i = 0; i < brickAmount; i++) {

			bricksArray[i].isAlive = true;

		}
		previousScore = score;
		if (highestScore < score) {
			highestScore = score;
		}
		chances = 3;
		score = 0;
		flag2 = false;
		sleep(3000);
		alertWin(flag2);
	}

	else {// trong quá trình chơi
		ballX += ballVelX;// tăng tọa độ x lên x + (kích thước của ball hiện tại) => bay qua lại theo trục x

		// kiểm tra từng viên gạch còn lại, khi x thay đổi có va chạm hay không? => kiểm tra theo trục x
		for (int i = 0; i < brickAmount; i++) {
			if (bricksArray[i].isAlive == true) {

				if (checkCollide(ballX, ballY, ballWH, ballWH, bricksArray[i].x, bricksArray[i].y, bricksArray[i].width, bricksArray[i].height) == true) {
					ballVelX = -ballVelX;// đổi hướng ngược lại  theo trục x
					bricksArray[i].isAlive = false;
					score += 10;
					break;
				}
			}
		}
		// kiểm tra từng viên gạch còn lại, khi y thay đổi có va chạm hay không? => kiểm tra theo trục y
		ballY -= ballVelY;// giảm tọa độ y = y - (kích thước của ball hiện tại) => bay lên xuống theo trục y
		for (int i = 0; i < brickAmount; i++) {
			if (bricksArray[i].isAlive == true) {
				if (checkCollide(ballX, ballY, ballWH, ballWH, bricksArray[i].x, bricksArray[i].y, bricksArray[i].width, bricksArray[i].height) == true) {
					ballVelY = -ballVelY;// đổi hướng ngược lại theo trục y
					bricksArray[i].isAlive = false;// 
					score += 10;
					break;
				}
			}
		}



		// kiểm tra chạm lề
		if (ballX + ballWH < 0) { // chạm phần viền trái của window => đổi hướng ngược lại
			ballVelX = -ballVelX;
		}
		else if (ballX + ballWH > widthScreen) {  // chạm phần viền phải của window => đổi hướng ngược lại
			ballVelX = -ballVelX;
		}
		if (ballY < heightScreen * 8 / 100) { // chạm phần viền trên của window => đổi hướng ngược lại
			ballVelY = -ballVelY;
		}
		else if (ballY + ballWH > 480) {  // chạm phần viền dưới của window => mất một lượt chơi

			if (chances <= 1) {// nếu lượt chơi nhỏ hơn 1
			 /*   float barX = 200, barY = 465, barWidth = 80, barheight = 5;
				float ballX = 235, ballY = 430, ballWH = 10, ballVelX = 0.3, ballVelY = 0.3;*/
				reset();
				ballVelX = 0;
				ballVelY = 0;


				for (int i = 0; i < brickAmount; i++) {// reset trạng thái lại cho những viên gạch bị va chạm trước đó
					if (!bricksArray[i].isAlive)
						bricksArray[i].isAlive = true;

				}
				previousScore = score;// gán điểm hiện tại vào đểm trước
				if (highestScore < score) {// lấy diểm cao nhất điểm cao hơn 
					highestScore = score;
				}
				//reset lại 
				chances = 3;
				score = 0;
				flag = false;
				sleep(3000);
				alert(flag);
			}
			else {
				chances--;
				reset();
				if (ballVelY < 0) { // đổi lại chiều dương => bay lên
					ballVelY = -ballVelY;
				}
				sleep(3000);
			}
		}

		// kiểm tra va chạm với thanh bar
		if (checkCollide(ballX, ballY, ballWH, ballWH, barX, barY, barWidth, barheight) == true) {
			ballVelY = -ballVelY; // đổi chiều y
		}
	}
	glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		barX -= 25;// tốc độ qua trái
		if (barX < 0) { // giới hạn thanh bar khi chạm viền trái
			barX = 0;
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		barX += 25;// tốc độ qua phải
		if (barX + barWidth > widthScreen) { // giới hạn thanh bar khi chạm viền phải
			barX = widthScreen - barWidth;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			flag = true;
			flag2 = true;
			flagStart = false;
			if (ballVelX <= 0 && ballVelY <= 0) { // nếu bóng đứng yên => đặt lại tốc dộ cho ball
				ballVelX = 0.3;
				ballVelY = 0.3;
			}
			glutIdleFunc(moveBall);
		}
		break;
	default:
		break;
	}
}

// Reading part

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(widthScreen, heightScreen);// kích thước màn hình
	glutInitWindowPosition(350, 150);// vị trí xuất hiện màn hình
	glutCreateWindow("Brick Breaker");// Tiêu đề cửa sổ
	glutDisplayFunc(Display);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	Initial();
	glutMainLoop();

}



