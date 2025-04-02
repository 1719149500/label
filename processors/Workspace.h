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

	// ��ӱ�ע
	void addShape(const std::string& label, const std::vector<Point>& points, int shape_type);

	// ������ӱ�ע
	void importShapes(const std::vector<Shape>& new_shapes);

	// ɾ����ע
	bool removeShape(size_t index);

	// �޸ı�ע
	bool updateShape(size_t index, const std::string& label, const std::vector<Point>& points);

	// ���ɱ�ע�ļ�·��������ͼƬ·����
	std::string getAnnotationFilePath() const;

	// ��ȡ��ע�ļ�
	bool loadFromAnnotationFile();

	// �½���ע�ļ�
	void createAnnotationFile();

	// �����ע�ļ�
	bool saveToAnnotationFile() const;

	// ��ȡͼ��·��
	std::string getImagePath() const;

	// ��ȡͼ��ߴ�
	int getImageWidth() const;
	int getImageHeight() const;

	// ��ȡ���б�ע
	const std::vector<Shape>& getShapes() const;

private:
	const MyImage image;
	const std::string image_path;
	std::vector<Shape> shapes;
};

#endif // WORKSPACE_H
