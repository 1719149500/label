#pragma once

#ifndef MY_IMAGE_H
#define MY_IMAGE_H


#include <opencv2/opencv.hpp>
#include "BinaryProcessor.h"
#include "FilterProcessor.h"
#include <QAction>

/* 图像元数据 */
struct ImageMetadata {
    int dataset_size; // 数据集大小
    int position_id; // 位置id
    std::string imagingDevice; // 成像设备
    cv::Size resolution; // 分辨率
    // ...
};





/* 图像类 */
class MyImage {
private:
    // 图像路径
    std::string image_path;
    // 图像元数据
    ImageMetadata image_metadata;
    // 图像转为 OpenCV Mat
    cv::Mat image_mat;
    // 在 MyImage 类中添加成员变量
    QAction* selectedAction;

public:
    BinaryProcessor binary;
    FilterProcessor filter;
    /* 枚举色彩位深，用于 MyImage::convertColorDepth() 函数 */
    enum ColorDepth {
        k8BitGrayscale,
        k16BitGrayscale,
        k32BitGrayscale,
        k8BitColor,
        kRGBColor
    };
    /* 构造函数 */
    MyImage(const std::string& image_path);

    /* 仅用于测试 */
    void show();

    // 更新图像的方法
    void updateImage(const std::string& newImagePath);

    /* 图片输出 */
    void exportImage(std::string outputPath = "temp_image.png");

    /* 图像处理函数 */
    void crop(int x, int y, int width, int height); // 裁剪

    void scale(float factor);
    void scaleByWidth(int width);
    void scaleByHeight(int Height);
    cv::Mat& getImageMat();

    // transform 的一系列功能
    void flipHorizontally(); // 水平翻转
    void flipVertically(); // 垂直翻转
    void rotateNinetyClockwise(); // 顺时针旋转90度
    void rotateNinetyCounterClockwise(); // 逆时针旋转90度
    void rotate(double angle); // 旋转
    void translate(float x_offset, float y_offset); // 偏移

    // type
    void convertColorDepth(ColorDepth color_depth); // 转换为不同位深的灰度图像或是彩色图像
    void updateSelectedAction(QAction* selectedAction);

    // adjust 的一系列功能
    void setBrightnessContrast(int minimum, int maximum, double contrast, double brightness); // 亮度对比度
    void threshold(int minimum, int maximum); // 阈值

    // process 的一系列功能
    void smooth(); //柔化
    void sharpen(); //锐化

    // analyze 的一系列功能
    std::vector<float> histogram();
    std::vector<float> plotProfile(const cv::Mat& mask);
    bool isEmpty() const; // 添加 isEmpty 方法
};


#endif // MY_IMAGE_H
