#include <vector>
#include "MyImage.h"
#include "Shape.h"
#include "Workspace.h"
#include "MyModel.h"

class ModelProcessor {
public:
	void processImageWithModel(const std::string& model_path, MyImage& myImage) {
		MyModel model;

		// 1. ����ģ�ͣ���ȡ��⵽�ı�ע
		std::vector<Shape> detectedShapes = model.runModel(model_path, myImage.getImageMat());

		// 2. ���� Workspace ������ͼƬ
		Workspace workspace(myImage);

		// 3. ����ģ�����ɵı�ע
		workspace.importShapes(detectedShapes);

		// 4. �����ע�ļ�
		if (!workspace.saveToAnnotationFile()) {
			std::cerr << "Failed to save annotation file!" << std::endl;
		}
	}
};