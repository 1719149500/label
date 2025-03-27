#include "BrightnessContrastDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

BrightnessContrastDialog::BrightnessContrastDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi();
}

void BrightnessContrastDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    // Minimum 行
    QLabel *minimumLabel = new QLabel("Minimum:");
    minimumSlider = new QSlider(Qt::Horizontal);
    minimumSlider->setRange(0, 255);
    minimumSlider->setValue(0);
    QLineEdit *minimumEdit = new QLineEdit();

    QHBoxLayout *minimumLayout = new QHBoxLayout();
    minimumLayout->addWidget(minimumSlider,3);
    minimumLayout->addWidget(minimumEdit,1);

    formLayout->addRow(minimumLabel, minimumLayout);

    // Maximum 行
    QLabel *maximumLabel = new QLabel("Maximum:");
    maximumSlider = new QSlider(Qt::Horizontal);
    maximumSlider->setRange(0, 255);
    maximumSlider->setValue(255);
    QLineEdit *maximumEdit = new QLineEdit();

    QHBoxLayout *maximumLayout = new QHBoxLayout();
    maximumLayout->addWidget(maximumSlider,3);
    maximumLayout->addWidget(maximumEdit,1);

    formLayout->addRow(maximumLabel, maximumLayout);

    // Brightness 行
    QLabel *brightnessLabel = new QLabel("Brightness:");
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(-255, 255);
    brightnessSlider->setValue(0);
    QLineEdit *brightnessEdit = new QLineEdit();

    QHBoxLayout *brightnessLayout = new QHBoxLayout();
    brightnessLayout->addWidget(brightnessSlider,3);
    brightnessLayout->addWidget(brightnessEdit,1);

    formLayout->addRow(brightnessLabel, brightnessLayout);

    QLabel *contrastLabel = new QLabel("Contrast:");
    QSlider *contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50);
    QLineEdit *contrastEdit = new QLineEdit("");

    // 创建一个水平布局来包含滑块和文本框
    QHBoxLayout *contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(contrastSlider,3);
    contrastLayout->addWidget(contrastEdit,1);

    // 将标签和水平布局添加到表单布局中
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
}

void BrightnessContrastDialog::onApplyButtonPressed() {
    // Implement apply functionality here
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
    contrastSlider->setValue(50);
}

void BrightnessContrastDialog::onSetButtonPressed() {
    // Implement set functionality here
}
