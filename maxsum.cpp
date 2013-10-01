#include <iostream>
#include <fstream>
using namespace std;

#define mode2 "/a"
#define mode3 "/h"
#define mode4 "/v"
FILE * file;
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

void readArrayR(FILE *file){
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
			A[j][i] = fgint(file);
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

int MaxSum_mode1(int isCalled){
	if(isCalled == 0){
		readArray(file);
		cal_PS();
	}
	int maximum = -2147483648;
	int Start, All;
	for (int a = 1; a <= Row_num; a++){//起始行
		for (int c = a; c <= Row_num; c++){ //终结行
			Start = BC(a, c, Clm_num);
			All = Start;
			for (int i = Clm_num - 1; i >= 1; i--){
				if(Start < 0)
					Start = 0;
				Start += BC(a, c, i);
				if(Start > All)
					All = Start;
			}
			if(All > maximum)
				maximum = All;
		}
	}
	return maximum;
}

int MaxSum_mode2(){ //  /a
	readArray(file);
	cal_PS();
	int maximum = -2147483648;
	return maximum;
}

int MaxSum_mode3(int isCalled){ // /h
	if(isCalled == 0){
		readArray(file);
		cal_PS();
	}
	int MaxSum_noJump = MaxSum_mode1(1); // 不跨越的和最大子数组
	int MaxSum_Jump; //跨越的和最大子数组
	
	int minimum = 2147483647;
	int Start, All;
	int WholeSum = 0;
	int tmpSum = 0;
	for (int a = 1; a <= Row_num; a++){//起始行
		for (int c = a; c <= Row_num; c++){ //终结行
			tmpSum = 0;
			Start = BC(a, c, Clm_num - 1);
			All = Start;
			for (int i = Clm_num - 2; i > 1; i--){
				if(Start > 0)
					Start = 0;
				Start += BC(a, c, i);
				if(Start < All)
					All = Start;
				tmpSum += BC(a, c, i);//累加去除头尾后的 中间元素的和
			}
			if(All <= minimum){
				int newSum = tmpSum + BC(a, c, 1) + BC(a, c, Clm_num) + BC(a, c, Clm_num - 1);
				if (newSum - All > WholeSum - minimum){
					minimum = All;
					WholeSum = newSum; //当找到更小的 中间元素的和，重新算a,c两行之间全部元素的和
				}
			}
		}
	}
	MaxSum_Jump = WholeSum - minimum; //有跨越情况，子数组分头尾两半。两半的和等于全部元素和 减 中间踢出去的元素和的小于零的最小值
	return MaxSum_noJump > MaxSum_Jump ? MaxSum_noJump : MaxSum_Jump;
}

void swap(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
int MaxSum_mode4(){ // /v
	readArrayR(file);
	cal_PS();
	swap(&Clm_num, &Row_num);
	return MaxSum_mode3(1);
}

int MaxSum_mode5(){ // /h /v
	readArray(file);
	cal_PS();
	int maximum = -2147483648;
	return maximum;
}

int main(int argc, char* argv[]){
	
	if ((file = fopen(argv[argc - 1], "r")) == NULL){
		cerr << "File Error, Please Restart The Program." << endl;
		exit(1);
	}

	if (argc == 2){ //mode1: normal
		cout << "Mode: Normal" << endl;
		cout << MaxSum_mode1(0) << endl;
		return 0;
	}
	else if (argc == 3){
		if (strcmp(argv[1], mode2) == 0){ // /a, not continues
			cout << "Mode: /a" << endl;
			cout << MaxSum_mode2() << endl;
			return 0;
		}
		else if (strcmp(argv[1], mode3) == 0){ // /h, horizontal
			cout << "Mode: /h" << endl;
			cout << MaxSum_mode3(0) << endl;
			return 0;
		}
		else if (strcmp(argv[1], mode4) == 0){ // /v, vertical
			cout << "Mode: /v" << endl;
			cout << MaxSum_mode4() << endl;
			return 0;
		}

	}
	else if (argc == 4){ // /h /v
		if ((strcmp(argv[1], mode2) == 0 && strcmp(argv[2], mode3) == 0) || (strcmp(argv[1], mode3) == 0 && strcmp(argv[2], mode2) == 0)){ // /v /h
			cout << "Mode: /h /v" << endl;
			cout << MaxSum_mode5() << endl;
			return 0;
		}
	}
	else{
		cerr << "This mode is not supported, input /a, /h, /v, /h /v, or just a file name" << endl;
		exit(1);
	}
}