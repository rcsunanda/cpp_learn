#pragma once

#include <string>

//-------------------------------------------------------------------------------------------------
template <typename ShapeType>
class Shape
{
public:
	virtual ~Shape() {}

	void SetOutlineColor(const std::string &newOutlineColor) { m_outlineColor = newOutlineColor; }
	void SetFillColor(const std::string &newFillColor)	{ m_fillColor = newFillColor; }
	void SetLocation(const int & newLocation) { location = newLocation; }

	const std::string &GetOutlineColor() const { return m_outlineColor; }
	const std::string &GetFillColor() const { return m_fillColor; }
	const int &GetLocation() const { return location; }

	//note: this is not virtual or pure virtual. if a derived class did not implement a DrawOutline(), this method will be called (common base class behavior as expected)
	void DrawOutline()
	{
		printf("Shape::DrawOutline\n");
	}

	void DrawFilled() //non-virtual function
	{
		//calls implementation functions (to invoke the behavior intended for the actual ShapeType)
		static_cast<ShapeType*>(this)->DrawOutline();
		static_cast<ShapeType*>(this)->DrawFill();
	}

protected:
	Shape(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor) :
		location(initialLocation),
		m_outlineColor(initialOutlineColor),
		m_fillColor(initialFillColor)
	{}

private:
	std::string m_outlineColor;
	std::string m_fillColor;
	int location;
};


//-------------------------------------------------------------------------------------------------
class Rectangle : public Shape<Rectangle>
{
public:
	Rectangle(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor,
		double initialHeight,
		double initialWidth) :
		Shape<Rectangle>(initialLocation, initialOutlineColor,
			initialFillColor),
		m_height(initialHeight),
		m_width(initialWidth)
	{}

	~Rectangle() override {}

	void SetHeight(double newHeight) { m_height = newHeight; }
	void SetWidth(double newWidth) { m_width = newWidth; }

	double GetHeight() const { return m_height; }
	double GetWidth() const { return m_width; }

	//note: DrawOutline(), DrawFill() implementation functions are not virtual

	void DrawOutline() const
	{
		printf("Rectangle::DrawOutline\n");
	}

	void DrawFill() const
	{
		printf("Rectangle::DrawFill\n");
	}


private:
	double m_height;
	double m_width;
};


//-------------------------------------------------------------------------------------------------
class Circle : public Shape<Circle>
{
public:
	Circle(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor,
		double initialRadius) :
		Shape<Circle>(initialLocation, initialOutlineColor,
			initialFillColor),
		m_radius(initialRadius)
	{}

	~Circle() override {}

	void SetRadius(double newRadius) { m_radius = newRadius; }
	double GetRadius() const { return m_radius; }

	void DrawOutline() const 
	{ 
		printf("Circle::DrawOutline\n");
	}

	void DrawFill() const 
	{
		printf("Circle::DrawFill\n");
	}

private:
	double m_radius;
};


//-------------------------------------------------------------------------------------------------
class ShapeManager
{
public:

	template <typename ShapeType>
	void drawShapeManyTimes(ShapeType* shape, int drawCount)
	{
		for (int i = 0; i < drawCount; ++i)
		{
			shape->DrawFilled();	//function in base class
			
			//can call functions in derived classes too; eg: shape->DrawOutline();

			//if function does not exist in derived class of given ShapeType, will give compiler error
			//eg: shape->GetRadius() will not compile when ShapeType=Rectangle
		}
	}
};