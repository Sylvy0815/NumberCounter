/**
* @file Student_ID_counter
* @brief 디지털 공학 학번카운터 리포트 제출용 프로그램
* JK Flip-Flop 사용
* @author Hyo-Jun Lee (PNU ITE 201545833)
*/

#define _CRT_SECURE_NO_WARNINGS
#define X 2
#define None 3
#include <stdio.h>

void disp_line(int n)
{
	int x;

	switch (n)
	{
	case 1:
		printf("   %c", 1);
		for (x = 0; x<14; x++)
			printf("---%c", 22);
		printf("---%c\n", 2);
		break;
	case 2:
		printf("\n   %c", 25);
		for (x = 0; x<14; x++)
			printf("---%c", 16);
		printf("---%c\n", 23);
		break;
	case 3:
		printf("\n   %c", 3);
		for (x = 0; x<14; x++)
			printf("---%c", 21);
		printf("---%c\n", 4);
		break;
	}
}

int main(void) {
	int num; int i, j, k, l, temp;
	int numarr[9] = { 0 }; // 2 0 1 5 4 5 8 3 3
	int numofnum[10] = { 0 }; // 1 1 1 2 1 2 0 0 0 1 0 (0개수, 1개수....9개수)
	int present[9][5]; int next[9][5];	// 현재 상태, 다음 상태
	int JK[4][4] = {	// Q(t) Q(t+1)  J  K
		{ 0,0,0,2 },		// 0,	0,		0, X
		{ 0,1,1,2 },		// 0,	1,		1, X
		{ 1,0,2,1 },		// 1,	0,		X, 1
		{ 1,1,2,0 }		// 1,	1,		X, 0
	};
	int Jvalue[9][5] = { 0 };
	int Kvalue[9][5] = { 0 };
	int KarnoughJ[5][2][4][4] = { 0 }; //[E D C B A][E(0,1)][DC(00,01,11,10)][BA]
	int KarnoughK[5][2][4][4] = { 0 };
	//----------------------------- 학번 입력 --------------------------------------
	printf("학번: ");
	scanf("%d", &num);

	// ------------ 입력받은 10진수 학번을 각각 하나의 수로 떼어 냄------------------
	for (i = 8; i >= 0; i--) {
		numarr[i] = num % 10;
		numofnum[num % 10] += 1;
		num = num / 10;
	}

	// ---------------- 현재 상태와 다음 상태 입력을 2진수로 계산 --------------------
	for (i = 0; i < 9; i++) {
		temp = numarr[i];
		for (j = 4; j >= 0; j--) {
			present[i][j] = temp % 2;
			i == 0 ? (next[8][j] = present[i][j]) : (next[i - 1][j] = present[i][j]);
			temp = temp / 2;
		}
	}

	// -------------- 중복된 수에 대한 처리를 위해 한 비트 추가 ----------------------
	for (i = 0; i < 10; i++) {
		temp = 0;
		if (numofnum[i] == 2) {
			for (j = 0; j < 9; j++) {
				if (numarr[j] == i) {
					present[j][0] = temp;
					j == 0 ? (next[8][0] = present[j][0]) : (next[j - 1][0] = present[j][0]);
					temp++;
				}
			}
		}
	}

	// ------------------------- JK Flip-Flop ------------------------------------------
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 5; j++) {
			present[i][j] ? ((Jvalue[i][j] = X) && (Kvalue[i][j] = !next[i][j]))
				: ((Kvalue[i][j] = X) && (Jvalue[i][j] = next[i][j]));
		}
	}

	// ------------------------ Karnough Map initialize---------------------------------
	for (i = 4; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			for (k = 0; k < 4; k++) {
				//00 01 11 10
				for (l = 0; l < 4; l++) {
					KarnoughJ[i][j][k][l] = None;
					KarnoughK[i][j][k][l] = None;
				}
			}
		}
	}

	// ------------------------- Karnough Map -----------------------------------------
	for (i = 0; i < 9; i++){
		int E, DC, BA; //00 01 11 10 --0,1,2,3
		E = present[i][0];
		DC = present[i][1] ? (present[i][2] ? 2 : 3) : (present[i][2] ? 1 : 0);
		BA = present[i][3] ? (present[i][4] ? 2 : 3) : (present[i][4] ? 1 : 0);
		for (j = 0; j < 5; j++) {
			KarnoughJ[j][E][DC][BA] = Jvalue[i][j];
			KarnoughK[j][E][DC][BA] = Kvalue[i][j];
		}
	}


	// -------------------------------- 출력 ------------------------------------------
	printf("%c----%c-------------------%c-------------------%c-------------------------------------------------%c\n", 1, 22, 22, 22, 2);
	printf("|학번|     현재 상태     |     다음 상태     |                   JK Flip-Flop                  |\n");
	printf("%c----%c-------------------%c-------------------%c-------------------------------------------------%c\n", 25, 16, 16, 16, 23);
	printf("| ID | E | D | C | B | A | E | D | C | B | A | JE | KE | JD | KD | JC | KC | JB | KB | JA | KA |\n");
	printf("%c----%c---%c---%c---%c---%c---%c---%c---%c---%c---%c---%c----%c----%c----%c----%c----%c----%c----%c----%c----%c----%c\n"
		, 25, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 23);
	for (i = 0; i < 9; i++) {
		printf("|  ");
		printf("%d |", numarr[i]);
		for (j = 0; j < 5; j++) {
			printf(" %d |", present[i][j]);
		}
		for (j = 0; j < 5; j++) {
			printf(" %d |", next[i][j]);
		}
		for (j = 0; j < 5; j++) {
			if (Jvalue[i][j] == X) printf("  X |");
			else printf("  %d |", Jvalue[i][j]);
			if (Kvalue[i][j] == X) printf("  X |");
			else printf("  %d |", Kvalue[i][j]);
		}
		puts("");
	}
	printf("%c----%c---%c---%c---%c---%c---%c---%c---%c---%c---%c---%c----%c----%c----%c----%c----%c----%c----%c----%c----%c----%c\n"
		, 3, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 4);

	//--------------------------- Karnough Map print --------------------------------------
	for (i = 4; i >= 0; i--) {
		printf("J%c                                                    \t K%c\n", 4-i + 'A', 4-i + 'A');
		printf("%c-------%c-------------------%c%c-------------------%c\t", 1, 22, 22, 22, 2); printf("%c-------%c-------------------%c%c-------------------%c\n", 1, 22, 22, 22, 2);
		printf("|    BA |       E = 0       ||       E = 1       |\t"); printf("|    BA |       E = 0       ||       E = 1       |\n");
		printf("%c-------%c-------------------%c%c-------------------%c\t", 26, 16, 16, 16, 23); printf("%c-------%c-------------------%c%c-------------------%c\n", 26, 16, 16, 16, 23);
		printf("| DC    | 00 | 01 | 11 | 10 || 00 | 01 | 11 | 10 |\t"); printf("| DC    | 00 | 01 | 11 | 10 || 00 | 01 | 11 | 10 |\n");
		printf("%c-------%c-------------------%c%c-------------------%c\t", 26, 16, 16, 16, 23); printf("%c-------%c-------------------%c%c-------------------%c\n", 26, 16, 16, 16, 23);
		
			for (k = 0; k < 4; k++) {
				//00 01 11 10
	// ------------------------ Karnough Map J part -----------------------------------------
				switch (k) {
				case 0:
					printf("|   00  |", k);
					break;
				case 1:
					printf("|   01  |", k);
					break;
				case 2:
					printf("|   11  |");
					break;
				case 3:
					printf("|   10  |");
					break;
				default:
					printf("ERROR");
				}
				for (l = 0; l < 4; l++) {
					temp = KarnoughJ[i][0][k][l];
					switch (temp) {
					case 0:
					case 1:
						printf("  %d |", temp);
						break;
					case X:
						printf("  X |");
						break;
					case None:
						printf("    |");
						break;
					default:
						printf("ERROR");
					}
				}
				printf("|");
				for (l = 0; l < 4; l++) {
					temp = KarnoughJ[i][1][k][l];
					switch (temp) {
					case 0:
					case 1:
						printf("  %d |", temp);
						break;
					case X:
						printf("  X |");
						break;
					case None:
						printf("    |");
						break;
					default:
						printf("ERROR");
					}
				}
				printf("\t");
	// ------------------------ Karnough Map K part -----------------------------------------
				switch (k) {
				case 0:
					printf("|   00  |", k);
					break;
				case 1:
					printf("|   01  |", k);
					break;
				case 2:
					printf("|   11  |");
					break;
				case 3:
					printf("|   10  |");
					break;
				default:
					printf("ERROR");
				}
				for (l = 0; l < 4; l++) {
					temp = KarnoughK[i][0][k][l];
					switch (temp) {
					case 0:
					case 1:
						printf("  %d |", temp);
						break;
					case X:
						printf("  X |");
						break;
					case None:
						printf("    |");
						break;
					default:
						printf("ERROR");
					}
				}
				printf("|");
				for (l = 0; l < 4; l++) {
					temp = KarnoughK[i][1][k][l];
					switch (temp) {
					case 0:
					case 1:
						printf("  %d |", temp);
						break;
					case X:
						printf("  X |");
						break;
					case None:
						printf("    |");
						break;
					default:
						printf("ERROR");
					}
				}
				puts("");
			}
			printf("%c-------%c-------------------%c%c-------------------%c\t", 3, 21, 21, 21, 4); printf("%c-------%c-------------------%c%c-------------------%c\n", 3, 21, 21, 21, 4);
		
	}
	getch();
}