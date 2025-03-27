#pragma once

#ifndef MY_IMAGE_H
#define MY_IMAGE_H

 
#include <opencv2/opencv.hpp>
#include "BinaryProcessor.h"
#include "FilterProcessor.h"
#include <QAction>

/* ͼ��Ԫ���� */
struct ImageMetadata {
    int dataset_size; // ���ݼ���С
    int position_id; // λ��id
    std::string imagingDevice; // �����豸
    cv::Size resolution; // �ֱ���
    // ...
};





/* ͼ���� */
class MyImage {
private:
    // ͼ��·��
    std::string image_path;
    // ͼ��Ԫ����
    ImageMetadata image_metadata;
    // ͼ��תΪ OpenCV Mat
    cv::Mat image_mat;
    // �� MyImage ������ӳ�Ա����
    QAction* selectedAction;

public:
    BinaryProcessor binary;
    FilterProcessor filter;

    /* ö��ɫ��λ����� convertColorDepth() ���� */
    enum ColorDepth { // ��ö�ٶ���Ϊ���ڲ�ö��
        k8BitGrayscale,
        k16BitGrayscale,
        k32BitGrayscale,
        k8BitColor,
        kRGBColor
    };
    /* ���캯�� */
    MyImage(const std::string& image_path);

    /* �����ڲ��� */
    void show();

    // ����ͼ��ķ���
    void updateImage(const std::string& newImagePath);

    /* ͼƬ��� */
    void exportImage(std::string outputPath = "temp_image.png");

    /* ͼ������ */
    void crop(int x, int y, int width, int height); // �ü�

    void scale(float factor);
    void scaleByWidth(int width); 
    void scaleByHeight(int Height);
    cv::Mat& getImageMat();

    // transform ��һϵ�й���
    void flipHorizontally(); // ˮƽ��ת
    void flipVertically(); // ��ֱ��ת
    void rotateNinetyClockwise(); // ˳ʱ����ת90��
    void rotateNinetyCounterClockwise(); // ��ʱ����ת90��
    void rotate(double angle); // ��ת
    void translate(float x_offset, float y_offset); // ƫ��

    // type
    void convertColorDepth(ColorDepth color_depth); // ת��Ϊ��ͬλ��ĻҶ�ͼ����ǲ�ɫͼ��
    void updateSelectedAction(QAction* selectedAction);

    // adjust ��һϵ�й���
    void setBrightnessContrast(int minimum, int maximum, double contrast, double brightness); // ���ȶԱȶ�
    void threshold(int minimum, int maximum); // ��ֵ

    // process ��һϵ�й���
    void smooth(); //�ữ
    void sharpen(); //��

    // analyze ��һϵ�й���
    std::vector<float> histogram();
    std::vector<float> plotProfile(const cv::Mat& mask);
    bool isEmpty() const; // ��� isEmpty ����
};


#endif // MY_IMAGE_H
