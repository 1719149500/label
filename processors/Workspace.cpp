#include "Workspace.h"
#include <fstream>
#include <nlohmann/json.hpp> // 需要安装 JSON 库
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

Workspace::Workspace(const MyImage& image)
	: image(image), image_path(image.getImagePath()) {}

void Workspace::addShape(const std::string& label, const std::vector<Point>& points, int shape_type) {
	Shape shape(label, shape_type);
	shape.setPoints(points);
	shapes.push_back(shape);
}

void Workspace::importShapes(const std::vector<Shape>& new_shapes) {
	shapes.insert(shapes.end(), new_shapes.begin(), new_shapes.end());
}

bool Workspace::removeShape(size_t index) {
	if (index >= shapes.size()) {
		return false;
	}
	shapes.erase(shapes.begin() + index);
	return true;
}

bool Workspace::updateShape(size_t index, const std::string& label, const std::vector<Point>& points) {
	if (index >= shapes.size()) {
		return false;
	}
	shapes[index].setLabel(label);
	shapes[index].setPoints(points);
	return true;
}

std::string Workspace::getAnnotationFilePath() const {
	fs::path imgPath(image_path);
	return (imgPath.parent_path() / (imgPath.stem().string() + ".json")).string();
}

bool Workspace::loadFromAnnotationFile() {
	std::string annotationPath = getAnnotationFilePath();

	if (!fs::exists(annotationPath)) {
		return false; // 标注文件不存在，返回 false
	}

	std::ifstream file(annotationPath);
	if (!file) {
		return false;
	}

	json j;
	file >> j;

	if (!j.contains("shapes")) {
		return false;
	}

	shapes.clear();
	for (const auto& shapeJson : j["shapes"]) {
		std::string label = shapeJson["label"];
		int shape_type = shapeJson["shape_type"];
		std::vector<Point> points;

		for (const auto& pointJson : shapeJson["points"]) {
			points.emplace_back(pointJson["x"], pointJson["y"]);
		}

		addShape(label, points, shape_type);
	}

	return true;
}

void Workspace::createAnnotationFile() {
	std::string annotationPath = getAnnotationFilePath();
	if (!fs::exists(annotationPath)) {
		saveToAnnotationFile(); // 自动创建新文件
	}
}

bool Workspace::saveToAnnotationFile() const {
	std::string annotationPath = getAnnotationFilePath();
	fs::path dir = fs::path(annotationPath).parent_path();
	if (!fs::exists(dir)) {
		fs::create_directories(dir); // 确保目录存在
	}

	json j;
	j["image_path"] = image_path;
	j["shapes"] = json::array();

	for (const auto& shape : shapes) {
		json shapeJson;
		shapeJson["label"] = shape.getLabel();
		shapeJson["shape_type"] = shape.getShapeType();
		shapeJson["points"] = json::array();

		for (const auto& point : shape.getPoints()) {
			shapeJson["points"].push_back({ {"x", point.x}, {"y", point.y} });
		}

		j["shapes"].push_back(shapeJson);
	}

	std::ofstream file(annotationPath);
	if (!file) {
		return false;
	}
	file << j.dump(4);
	return true;
}

std::string Workspace::getImagePath() const {
	return image_path;
}

int Workspace::getImageWidth() const {
	return image.getWidth();
}

int Workspace::getImageHeight() const {
	return image.getHeight();
}

const std::vector<Shape>& Workspace::getShapes() const {
	return shapes;
}
