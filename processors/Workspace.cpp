/// ----------------------- Workspace类 -----------------------
/// 
/// 说明：统一管理与图像相关的 **所有操作**
///      确保与图像的交互都通过Workspace进行。
/// 
///		 一、（重要！）Workspace初始化必须按照以下示例：
/// 
///			// 通过图像路径创建一个 MyImage 的 unique_ptr 对象，放入 workspace 中
///			Workspace workspace(std::make_unique<MyImage>("image.jpg"));
/// 
///		
///		 二、Workspace的功能：
/// 
///			1. 通过 `getMyImage()` 方法，
///				Workspace能够访问并操作MyImage相关功能。
/// 
///					用法示例：  
///					MyImage& image = workspace.getMyImage();
///					image.someMethod();
///		 
///		 
///			2. 自身功能还包括 标注（MyShape）的增删改、JSON文件的读写、
///				以及通过运行Yolo模型自动生成Shapes。
///		 
/// 
/// ----------------------- Workspace类 -----------------------

#include "Workspace.h"
#include <fstream>
#include <nlohmann/json.hpp> // 需要安装 JSON 库
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

<<<<<<< HEAD
Workspace::Workspace(const MyImage& image)
	: image(image), image_path(image.getImagePath()) {}
=======
/// ----------------------- 构造函数 -----------------------
Workspace::Workspace(std::unique_ptr<MyImage> img)
	: image(std::move(img)),
	  image_path(image->getImagePath()),
	  annotation_path((fs::path(image_path).parent_path() /
		  (fs::path(image_path).stem().string() + ".json")).string()) {
	loadFromAnnotationFile();
}
>>>>>>> 299192a (读取旧json框完成)


/// ----------------------- 获取 MyImage 引用 -----------------------
// 获取 MyImage 的引用
MyImage& Workspace::getMyImage() {
	return *image;  // 返回 MyImage 引用
}

// 获取 MyImage 的常量引用
const MyImage& Workspace::getMyImage() const {
	return *image;  // 返回常量引用
}


/// ----------------------- MyShape（标注）的增删改 -----------------------
// 添加标注
void Workspace::addShape(const std::string& label, const std::vector<Point>& points, int shape_type) {
	Shape shape(label, shape_type);
	shape.setPoints(points);
	shapes.push_back(shape);
}

<<<<<<< HEAD
void Workspace::importShapes(const std::vector<Shape>& new_shapes) {
	shapes.insert(shapes.end(), new_shapes.begin(), new_shapes.end());
}

=======
// 删除标注
>>>>>>> 299192a (读取旧json框完成)
bool Workspace::removeShape(size_t index) {
	if (index >= shapes.size()) {
		return false;
	}
	shapes.erase(shapes.begin() + index);
	return true;
}

// 修改标注
bool Workspace::updateShape(size_t index, const std::string& label, const std::vector<Point>& points) {
	if (index >= shapes.size()) {
		return false;
	}
	shapes[index].setLabel(label);
	shapes[index].setPoints(points);
	return true;
}

<<<<<<< HEAD
std::string Workspace::getAnnotationFilePath() const {
	fs::path imgPath(image_path);
	return (imgPath.parent_path() / (imgPath.stem().string() + ".json")).string();
=======
// 批量添加标注
void Workspace::importShapes(const std::vector<MyShape>& new_shapes) {
	shapes.insert(shapes.end(), new_shapes.begin(), new_shapes.end());
>>>>>>> 299192a (读取旧json框完成)
}



/// ----------------------- JSON文件的读写 -----------------------
// 读取标注文件
bool Workspace::loadFromAnnotationFile() {
<<<<<<< HEAD
	std::string annotationPath = getAnnotationFilePath();

	if (!fs::exists(annotationPath)) {
		return false; // ��ע�ļ������ڣ����� false
=======
	if (!fs::exists(annotation_path)) {
		return false; // 标注文件不存在，返回 false
>>>>>>> 299192a (读取旧json框完成)
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
<<<<<<< HEAD
=======

		// 如果有 mask 字段，读取 RLE
		if (shapeJson.contains("mask")) {
			auto maskJson = shapeJson["mask"];
			if (maskJson.contains("format") && maskJson["format"] == "rle") {
				std::vector<int> counts = maskJson["counts"].get<std::vector<int>>();
				shapes.back().setRLE(counts); // 设置 RLE 到刚添加的 shape
			}
		}
>>>>>>> 299192a (读取旧json框完成)
	}

	return true;
}

<<<<<<< HEAD
void Workspace::createAnnotationFile() {
	std::string annotationPath = getAnnotationFilePath();
	if (!fs::exists(annotationPath)) {
		saveToAnnotationFile(); // �Զ��������ļ�
=======
// 新建标注文件
void Workspace::createAnnotationFile() {
	if (!fs::exists(annotation_path)) {
		saveToAnnotationFile(); // 自动创建新文件
>>>>>>> 299192a (读取旧json框完成)
	}
}

// 保存标注文件
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

<<<<<<< HEAD
=======
		// 添加 RLE 掩码支持
		if (shape.hasRLE()) {
			json maskJson;
			maskJson["format"] = "rle";
			maskJson["counts"] = shape.getRLE();
			shapeJson["mask"] = maskJson;
		}

>>>>>>> 299192a (读取旧json框完成)
		j["shapes"].push_back(shapeJson);
	}

	std::ofstream file(annotationPath);
	if (!file) {
		return false;
	}
	file << j.dump(4);
	return true;
}

<<<<<<< HEAD
=======


/// ----------------------- Yolo模型相关 -----------------------
// 初始化YoloModelProcessor
void Workspace::initYoloModelProcessor(const std::string& model_path) {
	yolo_model_processor = std::make_unique<YoloModelProcessor>(model_path);
}

// 执行模型并将结果存于shapes
void Workspace::runYoloOnImage() {
	if (yolo_model_processor) {
		shapes = yolo_model_processor->detectShapes(image->getImageMat());
	}
}


/// ----------------------- get/set -----------------------
// 获取图像路径
>>>>>>> 299192a (读取旧json框完成)
std::string Workspace::getImagePath() const {
	return image_path;
}

<<<<<<< HEAD
int Workspace::getImageWidth() const {
	return image.getWidth();
}

int Workspace::getImageHeight() const {
	return image.getHeight();
}

const std::vector<Shape>& Workspace::getShapes() const {
	return shapes;
}
=======
// 获取标注文件路径
std::string Workspace::getAnnotationPath() const {
	return annotation_path;
}

// 获取图像宽度
int Workspace::getImageWidth() const {
	return image->getWidth();
}

// 获取图像高度
int Workspace::getImageHeight() const {
	return image->getHeight();
}

// 获取所有标注
const std::vector<MyShape>& Workspace::getShapes() const {
	return shapes;
}
>>>>>>> 299192a (读取旧json框完成)
