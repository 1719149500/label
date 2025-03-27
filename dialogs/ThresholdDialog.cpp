#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialog>
#include "ThresholdDialog.h"



ThresholdDialog::ThresholdDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi();
}

void ThresholdDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    QLabel *lowerLabel = new QLabel("Lower:");
    lowerLineEdit = new QLineEdit();
    lowerLineEdit->setText("0");
    formLayout->addRow(lowerLabel, lowerLineEdit);

    QLabel *upperLabel = new QLabel("Upper:");
    upperLineEdit = new QLineEdit();
    upperLineEdit->setText("255");
    formLayout->addRow(upperLabel, upperLineEdit);

    QLabel *modeLabel = new QLabel("Mode:");
    modeComboBox = new QComboBox();
    modeComboBox->addItem("Default");
    formLayout->addRow(modeLabel, modeComboBox);

    QLabel *channelLabel = new QLabel("Channel:");
    channelComboBox = new QComboBox();
    channelComboBox->addItem("Red");
    channelComboBox->addItem("Green");
    channelComboBox->addItem("Blue");
    formLayout->addRow(channelLabel, channelComboBox);

    darkBackgroundCheckBox = new QCheckBox("Dark background");
    stackHistogramCheckBox = new QCheckBox("Stack histogram");
    dontResetRangeCheckBox = new QCheckBox("Don't reset range");
    rawValuesCheckBox = new QCheckBox("Raw values");
    sixteenBitHistogramCheckBox = new QCheckBox("16-bit histogram");

    QHBoxLayout *checkBoxLayout = new QHBoxLayout();
    checkBoxLayout->addWidget(darkBackgroundCheckBox);
    checkBoxLayout->addWidget(stackHistogramCheckBox);
    checkBoxLayout->addWidget(dontResetRangeCheckBox);
    checkBoxLayout->addWidget(rawValuesCheckBox);
    checkBoxLayout->addWidget(sixteenBitHistogramCheckBox);
    formLayout->addRow("Options", checkBoxLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);

    QPushButton *autoButton = new QPushButton("Auto");
    QPushButton *applyButton = new QPushButton("Apply");
    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *setButton = new QPushButton("Set");
    buttonLayout->addWidget(autoButton);
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(setButton);

    connect(autoButton, &QPushButton::clicked, this, &ThresholdDialog::onAutoButtonPressed);
    connect(applyButton, &QPushButton::clicked, this, &ThresholdDialog::onApplyButtonPressed);
    connect(resetButton, &QPushButton::clicked, this, &ThresholdDialog::onResetButtonPressed);
    connect(setButton, &QPushButton::clicked, this, &ThresholdDialog::onSetButtonPressed);
}

void ThresholdDialog::onAutoButtonPressed() {
    // Implement auto functionality here
}

void ThresholdDialog::onApplyButtonPressed() {
    // Implement apply functionality here
    accept();
}

void ThresholdDialog::onResetButtonPressed() {
    // Implement reset functionality here
    lowerLineEdit->setText("0");
    upperLineEdit->setText("255");
    modeComboBox->setCurrentIndex(0);
    channelComboBox->setCurrentIndex(0);
    darkBackgroundCheckBox->setChecked(false);
    stackHistogramCheckBox->setChecked(false);
    dontResetRangeCheckBox->setChecked(true);
    rawValuesCheckBox->setChecked(false);
    sixteenBitHistogramCheckBox->setChecked(false);
}

void ThresholdDialog::onSetButtonPressed() {
    // Implement set functionality here
}
