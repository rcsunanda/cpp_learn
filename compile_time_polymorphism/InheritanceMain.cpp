//g++ --std=c++11 -ggdb -fsanitize=address InheritanceMain.cpp -o CRTP

#include "InheritanceShapes.h"

int main()
{
	Rectangle* rectangle = new Rectangle(0, "red", "blue", 10, 5);

	ShapeManager shapeMan;
	shapeMan.drawShapeManyTimes(rectangle, 3);

	delete rectangle;

	return 0;
}
