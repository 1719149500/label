// TopHatDialog.cpp

#include "TopHatDialog.h"

TopHatDialog::TopHatDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    radiusLineEdit = new QLineEdit();
    radiusLineEdit->setText("2.0");
    lightBackgroundCheckBox = new QCheckBox("Light Background");
    dontSubtractCheckBox = new QCheckBox("Don't subtract (grayscale open)");
    previewCheckBox = new QCheckBox("Preview");

    formLayout->addRow("Radius (pixels):", radiusLineEdit);
    formLayout->addRow(lightBackgroundCheckBox);
    formLayout->addRow(dontSubtractCheckBox);
    formLayout->addRow(previewCheckBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &TopHatDialog::onOkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

double TopHatDialog::getRadius() const {
    bool ok;
    double radius = radiusLineEdit->text().toDouble(&ok);
    return ok ? radius : 0.0;
}

bool TopHatDialog::isLightBackgroundChecked() const {
    return lightBackgroundCheckBox->isChecked();
}

bool TopHatDialog::isDontSubtractChecked() const {
    return dontSubtractCheckBox->isChecked();
}

bool TopHatDialog::isPreviewChecked() const {
    return previewCheckBox->isChecked();
}

void TopHatDialog::onOkButtonPressed() {
    accept();
}

void TopHatDialog::onCancelButtonPressed() {
    reject();  // 关闭对话框，不保存任何更改
}
