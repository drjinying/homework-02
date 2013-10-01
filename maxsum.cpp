#include <iostream>
#include <fstream>
using namespace std;

#define mode2 "/a"
#define mode3 "/h"
#define mode4 "/v"
int A[33][33];
int Row_num, Clm_num;
int PS[33][33];

int fgint(FILE *file){
	int tmp = 0;
	int isPositive = 1;
	char c;
	while ((c = fgetc(file)) != ',' && c != EOF){
		if (c == '-'){
			isPositive = -1;
		}
		if (c >= '0' && c <= '9'){
			tmp = tmp * 10 + c - '0';
		}
	}
	return tmp * isPositive;
}

void readArray(FILE *file){
	cout << "声明：若输入数字不足，程序自动以0补充" << endl;
	Row_num = fgint(file);
	Clm_num = fgint(file);
	if(Row_num > 32 || Clm_num > 32){
		cerr << "行数或列数过多" << endl;
		exit(1);
	}
	int i, j;
	for (i = 1; i <= Row_num; i++){
		for (j = 1; j <= Clm_num; j++){
			A[i][j] = fgint(file);
		}
	}
}

void cal_PS(){
	int i, j;
	for (i = 0; i <= Row_num; i++){
		PS[i][0] = 0;
	}
	for (j = 0; j <= Clm_num; j++){
		PS[0][j] = 0;
	}
	for (i = 1; i <= Row_num; i++){
		for (j = 1; j <= Clm_num; j++){
			PS[i][j] = PS[i - 1][j] + PS[i][j - 1] - PS[i - 1][j - 1] + A[i][j];
		}
	}
}

int BC(int a, int c, int i){
	return PS[c][i] - PS[a - 1][i] - PS[c][i - 1] + PS[a - 1][i - 1];
}

int MaxSum_mode1(){
	int maximum = -2147483648;
	int Start, All;
	for (int a = 1; a <= Row_num; a++){//起始行
		for (int c = a; c <= Row_num; c++){ //终结行
			Start = BC(a, c, Clm_num);
			All = Start;
			for (int i = Clm_num; i >= 1; i--){
				if(Start < 0)
					Start = 0;
				Start += BC(a, c, i);
				if(Start > All)
					All = Start;
			}
			if(All > maximum)
				maximum = All;
		}
		return maximum;
	}
}

int MaxSum_mode2(){
	int maximum = -2147483648;
	return maximum;
}

int MaxSum_mode3(){
	int maximum = -2147483648;

	return maximum;
}

int MaxSum_mode4(){
	int maximum = -2147483648;
	return maximum;
}

int MaxSum_mode5(){
	int maximum = -2147483648;
	return maximum;
}

int main(int argc, char* argv[]){
	
	FILE * file;

	if ((file = fopen(argv[argc - 1], "r")) == NULL){
		cerr << "File Error, Please Restart The Program." << endl;
		exit(1);
	}

	readArray(file);
	cal_PS();

	if (argc = 1){ //mode1: normal
		cout << MaxSum_mode1() << endl;
		return 0;
	}
	else if (argc = 2){
		if (strcmp(argv[0], mode2) == 0){ // /a, not continues
			cout << MaxSum_mode2() << endl;
			return 0;
		}
		else if (strcmp(argv[0], mode3) == 0){ // /h, horizontal
			cout << MaxSum_mode3() << endl;
			return 0;
		}
		else if (strcmp(argv[0], mode4) == 0){ // /v, vertical
			cout << MaxSum_mode4() << endl;
			return 0;
		}

	}
	else if (argc == 3){ // /h /v
		if ((strcmp(argv[0], mode2) == 0 && strcmp(argv[1], mode3) == 0) || (strcmp(argv[0], mode3) == 0 && strcmp(argv[1], mode2) == 0)){ // /v /h
			cout << MaxSum_mode5() << endl;
			return 0;
		}
	}
	else{
		cerr << "This mode is not supported, input /a, /h, /v, /h /v, or just a file name" << endl;
		exit(1);
	}
}