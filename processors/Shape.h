#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <string>

class Point {
public:
	double x, y;
	Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Shape {
public:
	Shape(const std::string& label, int shape_type);

	void addPoint(double x, double y);
	void setPoints(const std::vector<Point>& points);

	const std::string& getLabel() const;
	void setLabel(const std::string& label);

	int getShapeType() const;
	void setShapeType(int type);

	const std::vector<Point>& getPoints() const;

	// 历史记录相关方法
	void saveHistory();
	void undoLastChange();

private:
	std::string label;
	int shape_type;
	std::vector<Point> points;
	std::vector<std::vector<Point>> history; // 存储历史记录
};

#endif // SHAPE_H
