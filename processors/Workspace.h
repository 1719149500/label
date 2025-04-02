#pragma once
#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <vector>
#include <string>
#include "Shape.h"
#include "MyImage.h"

class Workspace {
public:
	explicit Workspace(const MyImage& image);

	// 添加标注
	void addShape(const std::string& label, const std::vector<Point>& points, int shape_type);

	// 批量添加标注
	void importShapes(const std::vector<Shape>& new_shapes);

	// 删除标注
	bool removeShape(size_t index);

	// 修改标注
	bool updateShape(size_t index, const std::string& label, const std::vector<Point>& points);

	// 生成标注文件路径（基于图片路径）
	std::string getAnnotationFilePath() const;

	// 读取标注文件
	bool loadFromAnnotationFile();

	// 新建标注文件
	void createAnnotationFile();

	// 保存标注文件
	bool saveToAnnotationFile() const;

	// 获取图像路径
	std::string getImagePath() const;

	// 获取图像尺寸
	int getImageWidth() const;
	int getImageHeight() const;

	// 获取所有标注
	const std::vector<Shape>& getShapes() const;

private:
	const MyImage image;
	const std::string image_path;
	std::vector<Shape> shapes;
};

#endif // WORKSPACE_H
