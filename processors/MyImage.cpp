#include "MyImage.h"

#include <iostream>
#include <string>
#include <vector>

/* 构造函数 */
MyImage::MyImage(const std::string& image_path)
    : image_path(image_path),
    image_mat(cv::imread(image_path)),
    binary(image_mat),
    filter(image_mat),
    image_width(image_mat.rows),
    image_height(image_mat.cols)
{}


/* 仅用于测试 */
void MyImage::show() const{
    cv::imshow("Test result", image_mat);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
}


/* 函数的具体实现 */
std::string MyImage::getImagePath() const{
    return image_path;
};

int MyImage::getWidth() const {
    return image_width;
}

int MyImage::getHeight() const{
    return image_height;
}



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
        if (image_mat.depth() == CV_16U) {
            image_mat.convertTo(image_mat, CV_8U, 1.0 / 256.0);
        }
        else if (image_mat.depth() == CV_32F) {
            image_mat.convertTo(image_mat, CV_8U, 255.0);
        }
        break;

    case k16BitGrayscale:
        if (image_mat.channels() == 3 || image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2GRAY);
        }
        if (image_mat.depth() == CV_8U) {
            image_mat.convertTo(image_mat, CV_16U, 256.0);
        }
        else {
            image_mat.convertTo(image_mat, CV_16U);
        }
        break;

    case k32BitGrayscale:
        if (image_mat.channels() == 3 || image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2GRAY);
        }
        image_mat.convertTo(image_mat, CV_32F, 1.0 / ((image_mat.depth() == CV_16U) ? 65535.0 : 255.0));
        break;

    case k8BitColor:
        if (image_mat.channels() == 1) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_GRAY2BGR);
        }
        if (image_mat.depth() == CV_32F) {
            image_mat.convertTo(image_mat, CV_8U, 255.0);
        }
        else if (image_mat.depth() == CV_16U) {
            image_mat.convertTo(image_mat, CV_8U, 1.0 / 256.0);
        }
        break;

    case kRGBColor:
        if (image_mat.channels() == 1) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_GRAY2BGR);
        }
        else if (image_mat.channels() == 4) {
            cv::cvtColor(image_mat, image_mat, cv::COLOR_BGRA2BGR);
        }
        if (image_mat.depth() == CV_32F) {
            image_mat.convertTo(image_mat, CV_8U, 255.0);
        }
        else if (image_mat.depth() == CV_16U) {
            image_mat.convertTo(image_mat, CV_8U, 1.0 / 256.0);
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
    // 计算中间值和范围，用于调整对比度
    double mid = (minimum + maximum) / 2.0;
    double range = (maximum - minimum) / 2.0 / (contrast > 0 ? contrast : 1);

    // 根据中间值和范围重新计算最小值和最大值
    minimum = static_cast<int>(mid - range);
    maximum = static_cast<int>(mid + range);

    // 限制最小值和最大值在合理范围内（-5000 到 5000）
    minimum = std::max(-5000, std::min(minimum, 5000));
    maximum = std::max(-5000, std::min(maximum, 5000));
    if (minimum >= maximum) {
        std::swap(minimum, maximum); // 如果最小值大于最大值，交换两者
    }

    cv::Mat adjusted;
    image_mat.convertTo(adjusted, CV_32F); // 转换为 float 进行计算

    // 应用对比度调整公式
    adjusted = adjusted * (contrast / 127 + 1) - contrast;
    // 限制像素值在最小值和最大值之间
    adjusted = cv::max(adjusted, minimum);
    adjusted = cv::min(adjusted, maximum);
    // 将像素值映射到 0-255 范围
    adjusted = (adjusted - minimum) / (maximum - minimum) * 255.0;
    adjusted.convertTo(adjusted, CV_8U); // 将图像转换回 8 位无符号整型

    // 针对 brightness 部分，避免颜色偏移：转换到 LAB 颜色空间，仅修改 L 通道
    if (brightness != 0) {
        cv::Mat lab;
        cv::cvtColor(adjusted, lab, cv::COLOR_BGR2Lab); // 将图像从 BGR 颜色空间转换到 LAB 颜色空间
        std::vector<cv::Mat> lab_channels;
        cv::split(lab, lab_channels); // 分离 LAB 通道

        // 调整 L 通道（lab_channels[0]），其范围为 0-255
        lab_channels[0].convertTo(lab_channels[0], CV_32F); // 将 L 通道转换为浮点型
        lab_channels[0] += brightness;  // 仅在 L 通道上加上 brightness 偏移
        lab_channels[0] = cv::max(lab_channels[0], 0); // 限制 L 通道最小值
        lab_channels[0] = cv::min(lab_channels[0], 255); // 限制 L 通道最大值
        lab_channels[0].convertTo(lab_channels[0], CV_8U); // 将 L 通道转换回 8 位无符号整型

        cv::merge(lab_channels, lab); // 合并 LAB 通道
        cv::cvtColor(lab, adjusted, cv::COLOR_Lab2BGR); // 将图像从 LAB 颜色空间转换回 BGR 颜色空间
    }

    adjusted.copyTo(image_mat);
}

void MyImage::threshold(int minimum = 0, int maximum = 255) {
    if (image_mat.channels() == 1) { // 仅在灰度图像时执行
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

        // 转换 cv::Mat 为 std::vector<float>
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
    // ???·??
    qDebug() << "Updating image with path:" << QString::fromStdString(newImagePath);

    // ????·??
    image_path = newImagePath;



    // ???????
    image_mat = cv::imread(newImagePath);

    // ??????????????
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

