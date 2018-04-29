#pragma once

#include <string>

//-------------------------------------------------------------------------------------------------
class Shape
{
public:
	Shape(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor) :
		location(initialLocation),
		m_outlineColor(initialOutlineColor),
		m_fillColor(initialFillColor)
	{}

	virtual ~Shape() {}

	virtual void DrawOutline() const = 0;
	virtual void DrawFill() const = 0;

	void SetOutlineColor(const std::string &newOutlineColor) { m_outlineColor = newOutlineColor; }
	void SetFillColor(const std::string &newFillColor)	{ m_fillColor = newFillColor; }
	void SetLocation(const int & newLocation) { location = newLocation; }

	const std::string &GetOutlineColor() const { return m_outlineColor; }
	const std::string &GetFillColor() const { return m_fillColor; }
	const int &GetLocation() const { return location; }

	void DrawFilled() //non-virtual function
	{
		//calls overriden implementation functions (virtual) (to invoke the behavior intended for the actual object type)
		DrawOutline();
		DrawFill();
	}

private:
	std::string m_outlineColor;
	std::string m_fillColor;
	int location;
};


//-------------------------------------------------------------------------------------------------
class Rectangle : public Shape
{
public:
	Rectangle(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor,
		double initialHeight,
		double initialWidth) :
		Shape(initialLocation, initialOutlineColor,
			initialFillColor),
		m_height(initialHeight),
		m_width(initialWidth)
	{}

	~Rectangle() override {}

	void SetHeight(double newHeight) { m_height = newHeight; }
	void SetWidth(double newWidth) { m_width = newWidth; }

	double GetHeight() const { return m_height; }
	double GetWidth() const { return m_width; }

	void DrawOutline() const override
	{
		printf("Rectangle::DrawOutline\n");
	}

	void DrawFill() const override
	{
		printf("Rectangle::DrawFill\n");
	}


private:
	double m_height;
	double m_width;
};


//-------------------------------------------------------------------------------------------------
class Circle : public Shape
{
public:
	Circle(const int &initialLocation,
		const std::string &initialOutlineColor,
		const std::string &initialFillColor,
		double initialRadius) :
		Shape(initialLocation, initialOutlineColor,
			initialFillColor),
		m_radius(initialRadius)
	{}

	~Circle() override {}

	void SetRadius(double newRadius) { m_radius = newRadius; }
	double GetRadius() const { return m_radius; }

	void DrawOutline() const override 
	{ 
		printf("Circle::DrawOutline\n");
	}

	void DrawFill() const override 
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

	void drawShapeManyTimes(Shape* shape, int drawCount)
	{
		for (int i = 0; i < drawCount; ++i)
			shape->DrawFilled();
	}
};