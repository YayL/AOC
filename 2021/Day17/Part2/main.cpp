#include <bits/stdc++.h>
#define PRINT(x) std::cout << x << std::endl;

int main(){
	clock_t start = clock();
	int minX = 201, maxX = 230, minY = -99, maxY = -65, count = 0, posX, posY, velX, velY;

	for(int y = -99; y < 201; y++){
		for(int x = 0; x < 231; x++){
			velX = x, velY = y, posX = 0, posY = 0;
			while(posX < maxX && posY > minY){
				posX += velX;
				posY += velY--;
				if(velX > 0)
					velX--;
				else if(velX < 0)
					velX++;
				if(posX >= minX && posX <= maxX && posY >= minY && posY <= maxY){
					count++;
					break;
				}
			}
		}
	}
	clock_t end = clock();
	printf("Time for program: %.3fms\n", (((double)end - start)/CLOCKS_PER_SEC) * 1000);
	PRINT("Result:\n\n" << count);
}