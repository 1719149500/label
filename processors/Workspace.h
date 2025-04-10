/// ----------------------- Workspace类 -----------------------
/// 
/// 说明：统一管理与图像相关的 **所有操作**
///      确保与图像的交互都通过Workspace进行。
/// 
///		 一、（重要！）Workspace初始化必须按照以下示例：
/// 
///			// 通过图像路径创建一个 MyImage 的 unique_ptr 对象，放入 workspace 中
///         std::unique_ptr<Workspace> workspace;
///			workspace = std::make_unique<Workspace>(std::make_unique<MyImage>(path));
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


#pragma once
#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <vector>
#include <string>
#include "Shape.h"
#include "MyImage.h"
#include "YoloModelProcessor.h"

<<<<<<< HEAD
=======

>>>>>>> 299192a (读取旧json框完成)
class Workspace {
private:
	std::unique_ptr<MyImage> image;
	std::vector<MyShape> shapes;

	const std::string image_path;
	const std::string annotation_path;
	
	std::unique_ptr<YoloModelProcessor> yolo_model_processor;

public:
	Workspace(std::unique_ptr<MyImage> img);

	/// ----------------------- 获取 MyImage 引用 -----------------------
	// 获取 MyImage 的引用
	MyImage& getMyImage();

	// 获取 MyImage 的常量引用
	const MyImage& getMyImage() const;


	/// ----------------------- MyShape（标注）的增删改 -----------------------
	// 添加标注
	void addShape(const std::string& label, const std::vector<Point>& points, int shape_type);

<<<<<<< HEAD
	// �������ӱ�ע
	void importShapes(const std::vector<Shape>& new_shapes);

	// ɾ����ע
=======
	// 删除标注
>>>>>>> 299192a (读取旧json框完成)
	bool removeShape(size_t index);

	// 修改标注
	bool updateShape(size_t index, const std::string& label, const std::vector<Point>& points);

<<<<<<< HEAD
	// ���ɱ�ע�ļ�·��������ͼƬ·����
	std::string getAnnotationFilePath() const;
=======
	// 批量添加标注
	void importShapes(const std::vector<MyShape>& new_shapes);

>>>>>>> 299192a (读取旧json框完成)

	/// ----------------------- JSON文件的读写 -----------------------
	// 读取标注文件
	bool loadFromAnnotationFile();

	// 新建标注文件
	void createAnnotationFile();

	// 保存标注文件
	bool saveToAnnotationFile() const;


	/// ----------------------- Yolo模型相关 -----------------------
	// 初始化YoloModelProcessor
	void initYoloModelProcessor(const std::string& model_path);

	// 执行模型并将结果存于shapes
	void runYoloOnImage();



	/// ----------------------- get/set -----------------------
	// 获取图像路径
	std::string getImagePath() const;
	// 获取标注文件路径
	std::string getAnnotationPath() const;

	// 获取图像尺寸
	int getImageWidth() const;
	int getImageHeight() const;

<<<<<<< HEAD
	// ��ȡ���б�ע
	const std::vector<Shape>& getShapes() const;

private:
	const MyImage image;
	const std::string image_path;
	std::vector<Shape> shapes;
=======
	// 获取所有标注
	const std::vector<MyShape>& getShapes() const;
>>>>>>> 299192a (读取旧json框完成)
};

#endif // WORKSPACE_H
