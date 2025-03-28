#include "BrightnessContrastDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIntValidator>  // 用于输入框的值验证
#include <QMessageBox>    // 用于显示提示信息
#include "../mainwindow.h"

BrightnessContrastDialog::BrightnessContrastDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi();
    // 暂时断开信号和槽
    disconnect(minimumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMinimumSliderValueChanged);
    disconnect(minimumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMinimumEditTextChanged);

    disconnect(maximumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMaximumSliderValueChanged);
    disconnect(maximumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMaximumEditTextChanged);

    disconnect(brightnessSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onBrightnessSliderValueChanged);
    disconnect(brightnessEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onBrightnessEditTextChanged);

    disconnect(contrastSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onContrastSliderValueChanged);
    disconnect(contrastEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onContrastEditTextChanged);

    // 重新连接信号和槽
    connect(minimumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMinimumSliderValueChanged);
    connect(minimumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMinimumEditTextChanged);

    connect(maximumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMaximumSliderValueChanged);
    connect(maximumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMaximumEditTextChanged);

    connect(brightnessSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onBrightnessSliderValueChanged);
    connect(brightnessEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onBrightnessEditTextChanged);

    connect(contrastSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onContrastSliderValueChanged);
    connect(contrastEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onContrastEditTextChanged);
}

void BrightnessContrastDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    // Minimum 行
    QLabel *minimumLabel = new QLabel("Minimum(像素值最小值):");
    minimumSlider = new QSlider(Qt::Horizontal);
    minimumSlider->setRange(0, 255);
    minimumSlider->setValue(0);
    minimumEdit = new QLineEdit();  // 将minimumEdit定义为类成员变量
    minimumEdit->setValidator(new QIntValidator(0, 255, this));  // 设置输入范围验证

    QHBoxLayout *minimumLayout = new QHBoxLayout();
    minimumLayout->addWidget(minimumSlider, 3);
    minimumLayout->addWidget(minimumEdit, 1);

    formLayout->addRow(minimumLabel, minimumLayout);

    // Maximum 行
    QLabel *maximumLabel = new QLabel("Maximum(像素值最大值):");
    maximumSlider = new QSlider(Qt::Horizontal);
    maximumSlider->setRange(0, 255);
    maximumSlider->setValue(255);
    maximumEdit = new QLineEdit();  // 将maximumEdit定义为类成员变量
    maximumEdit->setValidator(new QIntValidator(0, 255, this));  // 设置输入范围验证

    QHBoxLayout *maximumLayout = new QHBoxLayout();
    maximumLayout->addWidget(maximumSlider, 3);
    maximumLayout->addWidget(maximumEdit, 1);

    formLayout->addRow(maximumLabel, maximumLayout);

    // Brightness 行
    QLabel *brightnessLabel = new QLabel("Brightness(亮度):");
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(-255, 255);
    brightnessSlider->setValue(0);
    brightnessEdit = new QLineEdit();  // 将brightnessEdit定义为类成员变量
    brightnessEdit->setValidator(new QIntValidator(-255, 255, this));  // 设置输入范围验证

    QHBoxLayout *brightnessLayout = new QHBoxLayout();
    brightnessLayout->addWidget(brightnessSlider, 3);
    brightnessLayout->addWidget(brightnessEdit, 1);

    formLayout->addRow(brightnessLabel, brightnessLayout);

    QLabel *contrastLabel = new QLabel("Contrast(对比度):");
    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(-50, 100);
    contrastSlider->setValue(0);
    contrastEdit = new QLineEdit();  // 将contrastEdit定义为类成员变量
    contrastEdit->setValidator(new QIntValidator(-50, 100, this));  // 设置输入范围验证

    QHBoxLayout *contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(contrastSlider, 3);
    contrastLayout->addWidget(contrastEdit, 1);

    formLayout->addRow(contrastLabel, contrastLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);

    QPushButton *autoButton = new QPushButton("Auto");
    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *setButton = new QPushButton("Set");
    QPushButton *applyButton = new QPushButton("Apply");
    buttonLayout->addWidget(autoButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(setButton);
    buttonLayout->addWidget(applyButton);

    connect(autoButton, &QPushButton::clicked, this, &BrightnessContrastDialog::onAutoButtonPressed);
    connect(resetButton, &QPushButton::clicked, this, &BrightnessContrastDialog::onResetButtonPressed);
    connect(setButton, &QPushButton::clicked, this, &BrightnessContrastDialog::onSetButtonPressed);
    connect(applyButton, &QPushButton::clicked, this, &BrightnessContrastDialog::onApplyButtonPressed);

    // 连接滑块和输入框的信号与槽
    connect(minimumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMinimumSliderValueChanged);
    connect(minimumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMinimumEditTextChanged);

    connect(maximumSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onMaximumSliderValueChanged);
    connect(maximumEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onMaximumEditTextChanged);

    connect(brightnessSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onBrightnessSliderValueChanged);
    connect(brightnessEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onBrightnessEditTextChanged);

    connect(contrastSlider, &QSlider::valueChanged, this, &BrightnessContrastDialog::onContrastSliderValueChanged);
    connect(contrastEdit, &QLineEdit::textChanged, this, &BrightnessContrastDialog::onContrastEditTextChanged);

    // 初始化输入框的值
    onMinimumSliderValueChanged(minimumSlider->value());
    onMaximumSliderValueChanged(maximumSlider->value());
    onBrightnessSliderValueChanged(brightnessSlider->value());
    onContrastSliderValueChanged(contrastSlider->value());
}

void BrightnessContrastDialog::onApplyButtonPressed() {
    int minimum = minimumSlider->value();
    int maximum = maximumSlider->value();
    double brightness = brightnessSlider->value();
    double contrast = contrastSlider->value() / 50.0; // 将对比度滑块的值转换为比例

    emit applyBrightnessContrast(minimum, maximum, contrast, brightness);
    // 保存参数
    MainWindow *mainwindow = qobject_cast<MainWindow *>(parent());
    if (mainwindow) {
        mainwindow->lastMinimum = minimum;
        mainwindow->lastMaximum = maximum;
        mainwindow->lastContrast = contrast;
        mainwindow->lastBrightness = brightness;
    }
    accept();
}

void BrightnessContrastDialog::onAutoButtonPressed() {
    // Implement auto functionality here
}

void BrightnessContrastDialog::onResetButtonPressed() {
    // Implement reset functionality here
    minimumSlider->setValue(0);
    maximumSlider->setValue(255);
    brightnessSlider->setValue(0);
    contrastSlider->setValue(0);
}

void BrightnessContrastDialog::onSetButtonPressed() {
    // Implement set functionality here
}

void BrightnessContrastDialog::onMinimumSliderValueChanged(int value) {
    minimumEdit->setText(QString::number(value));
}

void BrightnessContrastDialog::onMinimumEditTextChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        // 修正值范围
        value = qBound(minimumSlider->minimum(), value, minimumSlider->maximum());
        minimumSlider->setValue(value);
        minimumEdit->setText(QString::number(value)); // 确保输入框显示修正后的值
    } else {
        // 输入非法字符，清除输入框内容或恢复到上一个合法值
        minimumEdit->setText(QString::number(minimumSlider->value()));

    }
}

void BrightnessContrastDialog::onMaximumSliderValueChanged(int value) {
    maximumEdit->setText(QString::number(value));
}

void BrightnessContrastDialog::onMaximumEditTextChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        // 修正值范围
        value = qBound(maximumSlider->minimum(), value, maximumSlider->maximum());
        maximumSlider->setValue(value);
        maximumEdit->setText(QString::number(value)); // 确保输入框显示修正后的值
    } else {
        // 输入非法字符，清除输入框内容或恢复到上一个合法值
        maximumEdit->setText(QString::number(maximumSlider->value()));

    }
}

void BrightnessContrastDialog::onBrightnessSliderValueChanged(int value) {
    brightnessEdit->setText(QString::number(value));
}

void BrightnessContrastDialog::onBrightnessEditTextChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        // 修正值范围
        value = qBound(brightnessSlider->minimum(), value, brightnessSlider->maximum());
        brightnessSlider->setValue(value);
        brightnessEdit->setText(QString::number(value)); // 确保输入框显示修正后的值
    } else {
        // 输入非法字符，清除输入框内容或恢复到上一个合法值
        brightnessEdit->setText(QString::number(brightnessSlider->value()));

    }
}

void BrightnessContrastDialog::onContrastSliderValueChanged(int value) {
    contrastEdit->setText(QString::number(value));
}

void BrightnessContrastDialog::onContrastEditTextChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        // 修正值范围
        value = qBound(contrastSlider->minimum(), value, contrastSlider->maximum());
        contrastSlider->setValue(value);
        contrastEdit->setText(QString::number(value)); // 确保输入框显示修正后的值
    } else {
        // 输入非法字符，清除输入框内容或恢复到上一个合法值
        contrastEdit->setText(QString::number(contrastSlider->value()));

    }
}

void BrightnessContrastDialog::setMinimum(int minimum) {
    minimumSlider->setValue(minimum);
    minimumEdit->setText(QString::number(minimum));
}

void BrightnessContrastDialog::setMaximum(int maximum) {
    maximumSlider->setValue(maximum);
    maximumEdit->setText(QString::number(maximum));
}

void BrightnessContrastDialog::setContrast(double contrast) {
    int sliderValue = static_cast<int>(contrast * 50); // 将对比度比例转换为滑块的值
    contrastSlider->setValue(sliderValue);
    contrastEdit->setText(QString::number(contrast));
}

void BrightnessContrastDialog::setBrightness(double brightness) {
    brightnessSlider->setValue(brightness);
    brightnessEdit->setText(QString::number(brightness));
}
