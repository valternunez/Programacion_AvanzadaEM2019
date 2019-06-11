#include "header.h"

int main(int argc, char* argv[]) {
	CarPart arr[5];
	FILE *file;
	
	arr[0].id = 0;
	strcpy(arr[0].name, "part 0");
	arr[0].quantity = 5;
	
	arr[1].id = 1;
	strcpy(arr[1].name, "part 1");
	arr[1].quantity = 5;
	
	arr[2].id = 2;
	strcpy(arr[2].name, "part 2");
	arr[2].quantity = 5;
	
	arr[3].id = 3;
	strcpy(arr[3].name, "part 3");
	arr[3].quantity = 5;
	
	arr[4].id = 4;
	strcpy(arr[4].name, "part 4");
	arr[4].quantity = 5;
	
	file = fopen("inventory.dat", "wb+");
	fwrite(arr, sizeof(CarPart), 5, file);
	fclose(file);
	
	return 0;
}
