#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int main(){
	clock_t start = clock();
	int minX = 22, maxX = 27, minY = -10, maxY = -5, result[3] {0}, maxHeight, posX, posY, velX, velY;;

	PRINT(minY * (minY + 1)/2);

	for(int y = -99; y < 201; y++){
		for(int x = 0; x < 231; x++){
			velX = x, velY = y, maxHeight = 0, posX = 0, posY = 0;
			while(posX < maxX && posY > minY){
				posX += velX;
				posY += velY--;

				if(velX > 0)
					velX--;
				else if(velX < 0)
					velX++;
				if(posY > maxHeight)
					maxHeight = posY;

				if(posX >= minX && posX <= maxX && posY >= minY && posY <= maxY){
					if(maxHeight > result[0]){
						result[0] = maxHeight, result[1] = x, result[2] = y;
					}
					break;
				}
			}
		}
	}
	clock_t end = clock();

	printf("Time for program: %.3fms\n", (((double)end - start)/CLOCKS_PER_SEC) * 1000);
	printf("Result:\n\nHeight: %i (%i,%i)\n", result[0], result[1], result[2]);

}