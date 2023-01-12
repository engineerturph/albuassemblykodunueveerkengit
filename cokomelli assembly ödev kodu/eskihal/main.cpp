#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short *resimdizi_org = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i*N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu){
		case 1:
			Dilation(N*M, filter_size, resimdizi_org);
			resimYaz(dil_resim, resimdizi_org, N, M, Q);
			break;
		case 2:
			Erosion(N*M, filter_size, resimdizi_org);
			resimYaz(ero_resim, resimdizi_org, N, M, Q);
			break;
		case 3:
			system("EXIT");
			break;
		default:
			system("EXIT");
			break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	/*for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			resim_org[i][j] = 255;*/
	__asm {
		; EAX = i
		; EDX = j
		; EBX = max
		; ESI = resim_org
		MOV EAX, filter_size
		SHR EAX, 1; EAX = i
		MOV ECX, n
		SUB ECX, filter_size
		INC ECX
		OUTERLOOP :
		PUSH ECX
			XOR EBX, EBX; max
			XOR EDX, EDX; j
			MOV ECX, filter_size
			SHR ECX, 1
			INC ECX
			INNERLOOP :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			ADD ESI, EDX
			ADD ESI, EDX
			PUSH ECX
			MOV CX,[ESI]
			SHL CX,8
			SHR CX,8
			CMP CX, BX
			POP ECX
			JLE NEXT
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX,CX
			POP ECX
			NEXT:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			SUB ESI, EDX
			SUB ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JLE NEXT2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT2:
		INC EDX
			LOOP INNERLOOP
			SHL BX,8
			MOV ESI,resim_org
			ADD ESI,EAX
			ADD ESI,EAX
			PUSH EAX
			MOV AX,[ESI]
			ADD AX,BX
			MOV [ESI],AX
			POP EAX
			INC EAX
			POP ECX
		DEC ECX
		JNS OUTERLOOP
		MOV ECX,n
		SUB ECX, filter_size
		INC ECX
		MOV EAX, filter_size
		SHR EAX, 1; EAX = i
		ASSIGNLOOP:
		MOV ESI, resim_org
		ADD ESI, EAX
		ADD ESI, EAX
		MOV BX, [ESI]
		SHR BX, 8
		MOV [ESI], BX
		INC EAX
		LOOP ASSIGNLOOP


			; yan cevirme yeri
		MOV ECX, n
		MOV EAX, 0
		YANCEVIRMELOOP:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX

			;ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU:
			INC ECX
			MOV EAX,n
			XOR EDX,EDX
			DIV ECX
			CMP ECX,EAX
			JNE ECXBULMALOOPU

		XOR EDX, EDX
		POP EAX
		PUSH EAX
		DIV ECX
		ADD ESI, EAX
		ADD ESI, EAX
		MOV EAX, EDX
		MUL ECX
		ADD ESI, EAX
		ADD ESI, EAX
		POP EAX
		POP ECX
		XOR EDX, EDX
		MOV DX, [ESI]
		SHL DX, 8
		ADD BX, DX
		MOV ESI, resim_org
		ADD ESI, EAX
		ADD ESI, EAX
		MOV[ESI], BX
		INC EAX
		LOOP YANCEVIRMELOOP
		MOV ECX, n
		MOV EAX, 0
		ASSIGNLOOPYAN :
		MOV ESI, resim_org
		ADD ESI, EAX
		ADD ESI, EAX
		MOV BX, [ESI]
		SHR BX, 8
		MOV[ESI], BX
		INC EAX
		LOOP ASSIGNLOOPYAN

			; EAX = i
			; EDX = j
			; EBX = max
			; ESI = resim_org
			MOV EAX, filter_size
			SHR EAX, 1; EAX = i
			MOV ECX, n
			SUB ECX, filter_size
			INC ECX
			OUTERLOOP_2 :
		PUSH ECX
			XOR EBX, EBX; max
			XOR EDX, EDX; j
			MOV ECX, filter_size
			SHR ECX, 1
			INC ECX
			INNERLOOP_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			ADD ESI, EDX
			ADD ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JLE NEXT_2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			SUB ESI, EDX
			SUB ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JLE NEXT2_2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT2_2 :
		INC EDX
			LOOP INNERLOOP_2
			SHL BX, 8
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			PUSH EAX
			MOV AX, [ESI]
			ADD AX, BX
			MOV[ESI], AX
			POP EAX
			INC EAX
			POP ECX
			DEC ECX
			JNS OUTERLOOP_2
			MOV ECX, n
			SUB ECX, filter_size
			INC ECX
			MOV EAX, filter_size
			SHR EAX, 1; EAX = i
			ASSIGNLOOP_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOP_2

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_2:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_2 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_2
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_2
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_2

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_3:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_3 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_3
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_3
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_3 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_3

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_4:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_4 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_4
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_4
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_4 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_4
	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	__asm {
		; EAX = i
		; EDX = j
		; EBX = min
		; ESI = resim_org
		MOV EAX, filter_size
		SHR EAX, 1; EAX = i
		MOV ECX, n
		SUB ECX, filter_size
		INC ECX
		OUTERLOOP :
		PUSH ECX
			MOV EBX, 256; min
			XOR EDX, EDX; j
			MOV ECX, filter_size
			SHR ECX, 1
			INC ECX
			INNERLOOP :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			ADD ESI, EDX
			ADD ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JGE NEXT
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			SUB ESI, EDX
			SUB ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JGE NEXT2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT2 :
		INC EDX
			LOOP INNERLOOP
			SHL BX, 8
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			PUSH EAX
			MOV AX, [ESI]
			ADD AX, BX
			MOV[ESI], AX
			POP EAX
			INC EAX
			POP ECX
			DEC ECX
			JNS OUTERLOOP
			MOV ECX, n
			SUB ECX, filter_size
			INC ECX
			MOV EAX, filter_size
			SHR EAX, 1; EAX = i
			ASSIGNLOOP :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOP

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN

			; EAX = i
			; EDX = j
			; EBX = min
			; ESI = resim_org
			MOV EAX, filter_size
			SHR EAX, 1; EAX = i
			MOV ECX, n
			SUB ECX, filter_size
			INC ECX
			OUTERLOOP_2 :
		PUSH ECX
			MOV EBX, 256; min
			XOR EDX, EDX; j
			MOV ECX, filter_size
			SHR ECX, 1
			INC ECX
			INNERLOOP_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			ADD ESI, EDX
			ADD ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JGE NEXT_2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			SUB ESI, EDX
			SUB ESI, EDX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			CMP CX, BX
			POP ECX
			JGE NEXT2_2
			XOR EBX, EBX
			PUSH ECX
			MOV CX, [ESI]
			SHL CX, 8
			SHR CX, 8
			MOV BX, CX
			POP ECX
			NEXT2_2 :
		INC EDX
			LOOP INNERLOOP_2
			SHL BX, 8
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			PUSH EAX
			MOV AX, [ESI]
			ADD AX, BX
			MOV[ESI], AX
			POP EAX
			INC EAX
			POP ECX
			DEC ECX
			JNS OUTERLOOP_2
			MOV ECX, n
			SUB ECX, filter_size
			INC ECX
			MOV EAX, filter_size
			SHR EAX, 1; EAX = i
			ASSIGNLOOP_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOP_2

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_2:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_2 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_2
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_2
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_2 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_2

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_3:
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_3 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_3
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_3
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_3 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_3

			; yan cevirme yeri
			MOV ECX, n
			MOV EAX, 0
			YANCEVIRMELOOP_4:
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			XOR EBX, EBX
			MOV BX, [ESI]
			SHL BX, 8
			SHR BX, 8
			MOV ESI, resim_org
			PUSH ECX
			PUSH EAX
			; ECX bulma kismi
			XOR ECX, ECX
			ECXBULMALOOPU_4 :
		INC ECX
			MOV EAX, n
			XOR EDX, EDX
			DIV ECX
			CMP ECX, EAX
			JNE ECXBULMALOOPU_4
			XOR EDX, EDX
			POP EAX
			PUSH EAX
			DIV ECX
			ADD ESI, EAX
			ADD ESI, EAX
			MOV EAX, EDX
			MUL ECX
			ADD ESI, EAX
			ADD ESI, EAX
			POP EAX
			POP ECX
			XOR EDX, EDX
			MOV DX, [ESI]
			SHL DX, 8
			ADD BX, DX
			MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV[ESI], BX
			INC EAX
			LOOP YANCEVIRMELOOP_4
			MOV ECX, n
			MOV EAX, 0
			ASSIGNLOOPYAN_4 :
		MOV ESI, resim_org
			ADD ESI, EAX
			ADD ESI, EAX
			MOV BX, [ESI]
			SHR BX, 8
			MOV[ESI], BX
			INC EAX
			LOOP ASSIGNLOOPYAN_4
	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}
