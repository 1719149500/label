// Shape.cpp
#include "Shape.h"

Shape::Shape(const std::string& label, int shape_type)
	: label(label), shape_type(shape_type) {}

void Shape::addPoint(double x, double y) {
	saveHistory();
	points.emplace_back(x, y);
}

void Shape::setPoints(const std::vector<Point>& new_points) {
	saveHistory();
	points = new_points;
}

const std::string& Shape::getLabel() const {
	return label;
}

void Shape::setLabel(const std::string& new_label) {
	label = new_label;
}

int Shape::getShapeType() const {
	return shape_type;
}

void Shape::setShapeType(int type) {
	shape_type = type;
}

const std::vector<Point>& Shape::getPoints() const {
	return points;
}

void Shape::saveHistory() {
	history.push_back(points);
}

void Shape::undoLastChange() {
	if (!history.empty()) {
		points = history.back();
		history.pop_back();
	}
}
