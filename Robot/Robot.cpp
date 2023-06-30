#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include<algorithm>
using namespace std;

int arrData[10][10];			//å¥èâÉfÅ[É^
vector<int *> ptrOutput;		//moved cell's address

//check if the current cell is in any corner, number indicating corner is counted clockwise
int isCorner(int cx, int cy, int x, int y) {			
	if ((cx - 1 < 0) && (cy - 1 < 0)) {
		return 1;				//upper left corner
	}
	else if ((cx - 1 < 0) && (cy + 1 == y)) {
		return 2;				//upper right corner
	}	
	else if ((cx + 1 == x) && (cy + 1 == y)) {
		return 3;				//lower right corner
	}
	else if ((cx + 1 == x) && (cy - 1 < 0)) {
		return 4;				//lower left corner
	}
	else {
		return 0;				//isn't in any corner
	}
}

//check if the current cell is in any border, number indicating border is counted clockwise
int isAtBorder(int cx, int cy, int x, int y) {			
	if ((cx == 0) && (cy != 0)) {
		return 1;				//upper border
	}
	else if ((cx != 0) && (cy == y - 1)) {		
		return 2;				//right border
	}
	else if ((cx == x - 1) && (cy != 0)) {
		return 3;				//lower border
	}
	else if ((cx != 0) && (cy == 0)) {			
		return 4;				//left border
	}
	else {
		return 0;				//isn't in any border
	}
}

bool movedCellCheck(int *n) {
	if (ptrOutput.size() == 0) {
		return false;
	}
	for (int i = 0; i < ptrOutput.size(); i++) {
		if (n == ptrOutput[i]) {
			return true;
		}
	}
	return false;	
}

bool isOutOfValidMove(int cx, int cy, int x, int y) {
	int x1 = cx; int y1 = cy - 1;
	int x2 = cx; int y2 = cy + 1;
	int x3 = cx - 1; int y3 = cy;
	int x4 = cx + 1; int y4 = cy;
	int existedCount = 0;
	vector<int *> checkedCoordinates;
	checkedCoordinates.push_back(&arrData[x1][y1]); 
	checkedCoordinates.push_back(&arrData[x2][y2]); 
	checkedCoordinates.push_back(&arrData[x3][y3]); 
	checkedCoordinates.push_back(&arrData[x4][y4]);
	switch (isCorner(cx, cy, x, y)) {
	case 0:
		switch (isAtBorder(cx, cy, x, y)) {
		case 0:
			break;
		case 1:
			checkedCoordinates.erase(checkedCoordinates.begin()+2);
			break;
		case 2:
			checkedCoordinates.erase(checkedCoordinates.begin()+1);
			break;
		case 3:
			checkedCoordinates.erase(checkedCoordinates.begin()+3);
			break;
		case 4:
			checkedCoordinates.erase(checkedCoordinates.begin());
			break;
		}
	case 1:
		checkedCoordinates.erase(checkedCoordinates.begin() + 1 - 1);
		checkedCoordinates.erase(checkedCoordinates.begin() + 3 - 2);
		break;
	case 2:
		checkedCoordinates.erase(checkedCoordinates.begin() + 2 - 1);
		checkedCoordinates.erase(checkedCoordinates.begin() + 3 - 2);
		break;
	case 3:
		checkedCoordinates.erase(checkedCoordinates.begin() + 2 - 1);
		checkedCoordinates.erase(checkedCoordinates.begin() + 4 - 2);
		break;
	case 4:
		checkedCoordinates.erase(checkedCoordinates.begin() + 1 - 1);
		checkedCoordinates.erase(checkedCoordinates.begin() + 4 - 2);
		break;
	}
	if (size(checkedCoordinates) != 0) {
		for (int i = 0; i < size(checkedCoordinates); i++) {
			if (movedCellCheck(checkedCoordinates[i])) {
				existedCount += 1;
			}
		}
	}
	else {
		return true;
	}
	if (existedCount == size(checkedCoordinates)) {
		return true;
	}
	else {
		return false;
	}
}



int maxNumber(vector<int*> n) {
	for (int i = 0; i < size(n); i++) {
		if (movedCellCheck(n[i])) {
			n.erase(n.begin() + i);
		}
	}
	for (int i = 1; i < size(n); i++) {
		if (*n[0] < *n[i]) {
			n[0] = n[i];
		}
	}
	return *n[0];
}

int main() {
	int x; int y; //x = number of rows, y = number of columns. so x = y in graph and vice versa...
	
	//get data from external file
	ifstream inputData("input.txt");
	inputData >> x >> y;
	
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			inputData >> arrData[i][j];
		}
	}
	inputData.close();

	//Start the robot
	int cx = 0; int cy = 0; //initial coodinate for the robot to run, [MODIFIABLE]
	int x1; int y1; //left cell		(x1; y1)
	int x2; int y2; //right cell	(x2; y2)
	int x3; int y3; //upper cell	(x3; y3)
	int x4; int y4; //lower cell	(x4; y4)
	int i = 0; //counter for output array
	 
	while (isOutOfValidMove(cx, cy, x, y) == false) {		
		int max;
		vector<int *> myVector;					//parameter array to pass to function
		ptrOutput.push_back(&arrData[cx][cy]);		//add the current cell value's address to the output array
		i += 1;
		switch (isCorner(cx, cy, x, y)) {
		case 0:
			switch (isAtBorder(cx, cy, x, y)) {
			case 0:					
				x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
				x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
				x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
				x4 = cx + 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
				max = maxNumber(myVector);
				//Move the robot to the next cell
				if (max == arrData[x1][y1]) {
					cy = cy - 1;
				}
				else if (max == arrData[x2][y2]) {
					cy = cy + 1;
				}
				else if (max == arrData[x3][y3]) {
					cx = cx - 1;
				}
				else if (max == arrData[x4][y4]) {
					cx = cx + 1;
				}
				break;			
			case 1:
				x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
				x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
				x4 = cx + 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
				max = maxNumber(myVector);
				if (max == arrData[x1][y1]) {
					cy = cy - 1;
				}
				else if (max == arrData[x2][y2]) {
					cy = cy + 1;
				}
				else if (max == arrData[x4][y4]) {
					cx = cx + 1;
				}
				break;			
			case 2:
				x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
				x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
				x4 = cx + 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
				max = maxNumber(myVector);
				if (max == arrData[x1][y1]) {
					cy = cy - 1;
				}
				else if (max == arrData[x3][y3]) {
					cx = cx - 1;
				}
				else if (max == arrData[x4][y4]) {
					cx = cx + 1;
				}
				break;
			case 3:
				x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
				x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
				x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
				max = maxNumber(myVector);
				if (max == arrData[x1][y1]) {					
					cy = cy - 1;
				}
				else if (max == arrData[x2][y2]) {
					cy = cy + 1;
				}
				else if (max == arrData[x3][y3]) {
					cx = cx - 1;
				}
				break;
			case 4:
				x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
				x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
				x4 = cx + 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
				max = maxNumber(myVector);
				if (max == arrData[x2][y2]) {
					cy = cy + 1;
				}
				else if (max == arrData[x3][y3]) {
					cx = cx - 1;
				}
				else if (max == arrData[x4][y4]) {
					cx = cx + 1;
				}
				break;
			}
			break;
		case 1:
			x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
			x4 = cx + 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
			max = maxNumber(myVector);
			if (max == arrData[x2][y2]) {
				cy = cy + 1;
			}
			else if (max == arrData[x4][y4]) {
				cx = cx + 1;
			}
			break;
		case 2:
			x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
			x4 = cx - 1; y4 = cy; myVector.push_back(&arrData[x4][y4]);
			max = maxNumber(myVector);
			if (max == arrData[x1][y1]) {
				cy = cy - 1;
			}
			else if (max == arrData[x4][y4]) {
				cx = cx + 1;
			}
			break;
		case 3:
			x1 = cx; y1 = cy - 1; myVector.push_back(&arrData[x1][y1]);
			x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
			max = maxNumber(myVector);
			if (max == arrData[x1][y1]) {
				cy = cy - 1;
			}
			else if (max == arrData[x3][y3]) {
				cx = cx - 1;
			}
			break;
		case 4:
			x2 = cx; y2 = cy + 1; myVector.push_back(&arrData[x2][y2]);
			x3 = cx - 1; y3 = cy; myVector.push_back(&arrData[x3][y3]);
			max = maxNumber(myVector);
			if (max == arrData[x2][y2]) {
				cy = cy + 1;
			}
			else if (max == arrData[x3][y3]) {
				cx = cx - 1;
			}
			break;
		};
		myVector.clear();

	}
	for (int i = 0; i < ptrOutput.size(); i++) {
		cout << *ptrOutput[i] << endl;
	}
	return 0;
}