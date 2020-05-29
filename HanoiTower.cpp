#include <bangtal.h>
#include <stdio.h>

SceneID HanoiScene;
TimerID animationTimer;
ObjectID tower[7];
ObjectID stick1, stick2, stick3;
ObjectID startButton;

int towerFilled[3][7];			//1~3탑에 채워져있는 원판 번호 (빈곳은 0) 위에서부터 (0,0)
int towerX[3] = { 60,460,860 };
int towerY[7];

int count = 0;
int animation1[127];	//2^7 - 1
int animation2[127];
int animation3[127];

char path[256];

ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size) {

	ObjectID object = createObject(name);
	locateObject(object, scene, x, y);
	scaleObject(object, size);

	if (shown == true)
		showObject(object);

	return object;
}


//1~3타워의 채울 수 있는 맨 위 칸 번호
int topNum(int tower) {
	
	int num = 0;

	for (int i = 0; i < 7; i++) {
		if (towerFilled[tower][i] == 0)		//비어있으면 다음칸으로 이동
			num++;
		else 
			break;
	}

	return --num;		//꽉차있으면 -1로 나옴
}

void move(int from, int to) {
	//이동 애니메이션 저장
	animation1[count] = towerFilled[from][topNum(from) + 1] - 1;
	animation2[count] = to;
	animation3[count] = topNum(to);

	towerFilled[to][topNum(to)] = towerFilled[from][topNum(from) + 1];										//towerFilled(from) -> towerFilled(to)
	towerFilled[from][topNum(from) + 1] = 0;																//towerFilled(from) = 0 (빈 곳)

	count++;
}

void moveTower(int n, int from, int to, int temp)    // n개의 원판을 from에서 to로 temp를 이용해서 옮긴다
{
	if (n == 1) 
		move(from, to);

	else {
		moveTower(n - 1, from, temp, to);
		
		move(from, to);

		moveTower(n - 1, temp, to, from);
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startButton) {

		hideObject(startButton);

		moveTower(7, 0, 1, 2);
		printf("%d", count);
		count = 0;
		startTimer(animationTimer);
	}
}

void timerCallback(TimerID timer) {

	locateObject(tower[animation1[count]], HanoiScene, towerX[animation2[count]], towerY[animation3[count]]);

	count++;

	if (count == 127) {
		showMessage("끝");
		return;
	}

	setTimer(animationTimer, 0.1f);
	startTimer(animationTimer);
}


int main() {
	
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	HanoiScene = createScene("하노이 타워", "Images/배경.png");

	animationTimer = createTimer(0.1f);

	startButton = createObject("Images/버튼.png", HanoiScene, 550, 70, true, 2.0f);

	stick1 = createObject("Images/막대.png", HanoiScene, 237, 164, true, 0.53f);
	stick2 = createObject("Images/막대.png", HanoiScene, 637, 164, true, 0.53f);
	stick3 = createObject("Images/막대.png", HanoiScene, 1037, 164, true, 0.53f);

	//167 + 55 * i
	for (int i = 6; i >= 0; i--) {
		towerY[i] = 497 - 55 * i;
		sprintf_s(path, "Images/%d.png", i + 1);
		tower[i] = createObject(path, HanoiScene, 60, 497 - 55 * i, true, 0.6f);
		towerFilled[0][i] = i + 1;				//(0,0) 부터 1~7 채우기
		towerFilled[1][i] = 0;
		towerFilled[2][i] = 0;					//빈곳은 0으로 채우기
	}

	startGame(HanoiScene);

}