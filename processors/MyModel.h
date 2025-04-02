#pragma once
#ifndef MY_MODEL_H
#define MY_MODEL_H

#include "Shape.h"
#include <opencv2/opencv.hpp>
#include <io.h>

//应该是libtorch的slots字符和Qt的slots冲突了，用下面的代码替代#include <torch/script.h>
#undef slots
#include <torch/script.h>
#define slots Q_SLOTS

class MyModel {
public:
	float generate_scale(cv::Mat& image, const std::vector<int>& target_size);
	float letterbox(cv::Mat& input_image, cv::Mat& output_image, const std::vector<int>& target_size);
	torch::Tensor xyxy2xywh(const torch::Tensor& x);
	torch::Tensor xywh2xyxy(const torch::Tensor& x);
	torch::Tensor nms(const torch::Tensor& bboxes, const torch::Tensor& scores, float iou_threshold);
	torch::Tensor non_max_suppression(torch::Tensor& prediction, float conf_thres = 0.25, float iou_thres = 0.45, int max_det = 300);
	torch::Tensor clip_boxes(torch::Tensor& boxes, const std::vector<int>& shape);
	torch::Tensor scale_boxes(const std::vector<int>& img1_shape, torch::Tensor& boxes, const std::vector<int>& img0_shape);

	std::vector<Shape> runModel(const std::string& model_path, cv::Mat image);
};

#endif //MY_MODEL_H