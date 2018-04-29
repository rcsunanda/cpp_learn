//g++ --std=c++11 -ggdb -fsanitize=address CRTPMain.cpp -o CRTP

//CRTP = Curiously recurring template pattern

#include "CRTPShapes.h"

int main()
{
	Rectangle* rectangle = new Rectangle(0, "red", "blue", 10, 5);

	ShapeManager shapeMan;
	shapeMan.drawShapeManyTimes(rectangle, 3);

	delete rectangle;

	return 0;
}
