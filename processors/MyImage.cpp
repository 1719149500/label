#include "MyImage.h"

#include <iostream>
#include <string>
#include <vector>
#include <QDebug>
#include <QString>
#include <QFile>
/*  */
MyImage::MyImage(const std::string& image_path)
    : image_path(image_path),
    image_mat(cv::imread(image_path)),
    binary(image_mat),
    filter(image_mat) {}


/*  */
void MyImage::show() {
    cv::imshow("Test result", image_mat);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
}


/*  */


void MyImage::exportImage(std::string outputPath) {
    if (!cv::imwrite(outputPath, image_mat)) {
        std::cout << "Error: Failed to save image.\n";
    }
    else {
        std::cout << "Image saved to " << outputPath << "\n";
    }
}


void MyImage::crop(int x, int y, int width, int height) {
    cv::Rect roi(x, y, width, height);
    image_mat = image_mat(roi);
}

void MyImage::scale(float factor) {
    cv::resize(image_mat, image_mat, cv::Size(), factor, factor, cv::INTER_LINEAR);
}

void MyImage::scaleByWidth(int width) {
    int new_height = static_cast<int>(image_mat.rows * (static_cast<double>(width) / image_mat.cols));
    cv::resize(image_mat, image_mat, cv::Size(width, new_height), 0, 0, cv::INTER_LINEAR);
}

void MyImage::scaleByHeight(int height) {
    int new_width = static_cast<int>(image_mat.cols * (static_cast<double>(height) / image_mat.rows));
    cv::resize(image_mat, image_mat, cv::Size(new_width, height), 0, 0, cv::INTER_LINEAR);
}

void MyImage::flipHorizontally() {
    cv::flip(image_mat, image_mat, 1);
}

void MyImage::flipVertically() {
    cv::flip(image_mat, image_mat, 0);
}

void MyImage::rotateNinetyClockwise() {
    cv::rotate(image_mat, image_mat, cv::ROTATE_90_CLOCKWISE);
}

void MyImage::rotateNinetyCounterClockwise() {
    cv::rotate(image_mat, image_mat, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void MyImage::rotate(double angle) {
    cv::Point2f center_coord((image_mat.cols - 1) / 2.0, (image_mat.rows - 1) / 2.0);
    cv::Mat rotation_matrix = cv::getRotationMatrix2D(center_coord, angle, 1.0);
    cv::warpAffine(image_mat, image_mat, rotation_matrix, image_mat.size());
}

void MyImage::translate(float x_offset, float y_offset) {
    // create the translation matrix using x_offset and y_offset
    float warp_values[] = { 1.0, 0.0, x_offset, 0.0, 1.0, y_offset };
    cv::warpAffine(image_mat, image_mat, cv::Mat(2, 3, CV_32F, warp_values), image_mat.size());
}

void MyImage::convertColorDepth(ColorDepth color_depth) {
    switch (color_depth) {
    case k8BitGrayscale:
        if (image_mat.channels() == 3 || image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2GRAY);
        }
        image_mat.convertTo(image_mat, CV_8U);
        break;

    case k16BitGrayscale:
        if (image_mat.channels() == 3 || image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2GRAY);
        }
        image_mat.convertTo(image_mat, CV_16U);
        break;

    case k32BitGrayscale:
        if (image_mat.channels() == 3 || image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2GRAY);
        }
        image_mat.convertTo(image_mat, CV_32F, 1.0 / 255.0);  //  [0, 1]
        break;

    case k8BitColor:
        if (image_mat.channels() == 1) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_GRAY2BGR);
        }
        image_mat.convertTo(image_mat, CV_8U);
        break;

    case kRGBColor:
        if (image_mat.channels() == 1) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_GRAY2BGR);
        }
        else if (image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGRA2BGR);
        }
        break;
    }
}

/*
* recommended range:
minimum: [0, 255]
maximum: [0, 255]
contrast: [-127, 127]
brightness: [-127, 127]
*/
void MyImage::setBrightnessContrast(int minimum, int maximum, double contrast, double brightness) {
    // �ަ�
    double mid = (minimum + maximum) / 2.0;
    double range = (maximum - minimum) / 2.0 / (contrast > 0 ? contrast : 1);

    // �ަ���
    minimum = static_cast<int>(mid - range);
    maximum = static_cast<int>(mid + range);

    // ����-5000  5000
    minimum = std::max(-5000, std::min(minimum, 5000));
    maximum = std::max(-5000, std::min(maximum, 5000));
    if (minimum >= maximum) {
        std::swap(minimum, maximum); // ��
    }

    cv::Mat adjusted;
    image_mat.convertTo(adjusted, CV_32F); //  float ��

    //
    adjusted = adjusted * (contrast / 127 + 1) - contrast;
    // ��
    adjusted = cv::max(adjusted, minimum);
    adjusted = cv::min(adjusted, maximum);
    //  0-255 ��
    adjusted = (adjusted - minimum) / (maximum - minimum) * 255.0;
    adjusted.convertTo(adjusted, CV_8U); //  8 ��

    //  brightness  LAB  L
    if (brightness != 0) {
        cv::Mat lab;
        cv::cvtColor(adjusted, lab, cv::COLOR_BGR2Lab); //  BGR  LAB
        std::vector<cv::Mat> lab_channels;
        cv::split(lab, lab_channels); //  LAB

        //  L lab_channels[0]�� 0-255
        lab_channels[0].convertTo(lab_channels[0], CV_32F); //  L
        lab_channels[0] += brightness;  //  L  brightness
        lab_channels[0] = cv::max(lab_channels[0], 0); //  L ��
        lab_channels[0] = cv::min(lab_channels[0], 255); //  L
        lab_channels[0].convertTo(lab_channels[0], CV_8U); //  L  8 ��

        cv::merge(lab_channels, lab); //  LAB
        cv::cvtColor(lab, adjusted, cv::COLOR_Lab2BGR); //  LAB  BGR
    }

    adjusted.copyTo(image_mat);
}

void MyImage::threshold(int minimum = 0, int maximum = 255) {
    if (image_mat.channels() == 1) { //
        cv::threshold(image_mat, image_mat, minimum, maximum, cv::THRESH_BINARY);
    }
}

void MyImage::smooth() {
    cv::GaussianBlur(image_mat, image_mat, cv::Size(5, 5), 0);
}

void MyImage::sharpen() {
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
                          0, -1, 0,
                      -1, 5, -1,
                      0, -1, 0);
    cv::filter2D(image_mat, image_mat, image_mat.depth(), kernel);
}

std::vector<float> MyImage::histogram() {
    std::vector<float> histogramData;

    if (image_mat.channels() == 1) {
        int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };
        cv::Mat hist;

        cv::calcHist(&image_mat, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

        //  cv::Mat  std::vector<float>
        histogramData.assign(hist.begin<float>(), hist.end<float>());
    }

    return histogramData;
}


std::vector<float> MyImage::plotProfile(const cv::Mat& mask) {
    std::vector<float> profile;

    if (image_mat.channels() == 1) {
        for (int y = 0; y < image_mat.rows; ++y) {
            for (int x = 0; x < image_mat.cols; ++x) {
                if (mask.at<uchar>(y, x)) {
                    profile.push_back(static_cast<float>(image_mat.at<uchar>(y, x)));
                }
            }
        }
    }

    return profile;
}


void MyImage::updateImage(const std::string& newImagePath) {
    // ��ӡ·��
    qDebug() << "Updating image with path:" << QString::fromStdString(newImagePath);

    // ����·��
    image_path = newImagePath;



    // ����ͼ��
    image_mat = cv::imread(newImagePath);

    // ���ͼ���Ƿ���سɹ�
    if (image_mat.empty()) {
        qDebug() << "Failed to load image from path:" << QString::fromStdString(newImagePath);
    } else {
        qDebug() << "Image loaded successfully from path:" << QString::fromStdString(newImagePath);
    }
}

cv::Mat& MyImage::getImageMat() {
    return image_mat;
}

bool MyImage::isEmpty() const {
    return image_mat.empty();
}

QAction* selectedAction;
void MyImage::updateSelectedAction(QAction* selectedAction) {
    // 如果有当前选中的选项，取消选中
    if (this->selectedAction) {
        this->selectedAction->setChecked(false);
    }

    // 更新当前选中的选项
    this->selectedAction = selectedAction;

    // 设置当前选中的选项为选中状态
    if (this->selectedAction) {
        this->selectedAction->setChecked(true);
    }
}
