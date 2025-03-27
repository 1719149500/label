#pragma once

// BinaryProcessor.h
#ifndef BINARY_PROCESSOR_H
#define BINARY_PROCESSOR_H

#include <opencv2/opencv.hpp>

enum EdmOutput {
    kOverwrite,
    k8Bit,
    k16Bit,
    k32Bit
};

struct BinaryOptions {
    int iterations = 1; // 1-100
    int count = 1; // 1-8
    bool black_background = true;
    bool pad_edges_when_eroding = false;
    EdmOutput edm_output = kOverwrite;
};

class BinaryProcessor {
private:
    cv::Mat& image_mat;
    BinaryOptions options;
public:
    BinaryProcessor(cv::Mat& img);
    void setOptions(const BinaryOptions& options);

    // binary ��һϵ�й���
    void makeBinary(); // ��ֵ��
    void convertToMask(); // ת��Ϊ mask

    void erode(); // ����
    void dilate(); // ��ʴ
    void open(); // �����㣬�ȸ�ʴ������
    void close(); // �����㣬�������ٸ�ʴ
    void median();

    void outline(); // ��ȡ��Ե
    void fillHoles(); // �
    void skeletonize(); // �Ǽܻ�

    void distanceMap(); // ŷ�Ͼ���ӳ��EDM
    void ultimatePoints(); // ���޸�ʴ��
    void watershed(); // ��ˮ���㷨
    void voronoi(); // άŵͼ
};

#endif // BINARY_PROCESSOR_H