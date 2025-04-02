#include <vector>
#include "MyImage.h"
#include "Shape.h"
#include "Workspace.h"
#include "MyModel.h"

class ModelProcessor {
public:
	void processImageWithModel(const std::string& model_path, MyImage& myImage) {
		MyModel model;

		// 1. 运行模型，获取检测到的标注
		std::vector<Shape> detectedShapes = model.runModel(model_path, myImage.getImageMat());

		// 2. 创建 Workspace 并关联图片
		Workspace workspace(myImage);

		// 3. 导入模型生成的标注
		workspace.importShapes(detectedShapes);

		// 4. 保存标注文件
		if (!workspace.saveToAnnotationFile()) {
			std::cerr << "Failed to save annotation file!" << std::endl;
		}
	}
};