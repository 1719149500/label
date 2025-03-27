// UnsharpMaskDialog.cpp

#include "UnsharpMaskDialog.h"

UnsharpMaskDialog::UnsharpMaskDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    radiusLineEdit = new QLineEdit();
    radiusLineEdit->setText("1.0");
    maskWeightLineEdit = new QLineEdit();
    maskWeightLineEdit->setText("0.60");
    previewCheckBox = new QCheckBox("Preview");

    formLayout->addRow("Radius (Sigma):", radiusLineEdit);
    formLayout->addRow("Mask Weight (0.1-0.9):", maskWeightLineEdit);
    formLayout->addRow(previewCheckBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &UnsharpMaskDialog::onOkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

double UnsharpMaskDialog::getRadius() const {
    bool ok;
    double radius = radiusLineEdit->text().toDouble(&ok);
    return ok ? radius : 0.0;
}

double UnsharpMaskDialog::getMaskWeight() const {
    bool ok;
    double weight = maskWeightLineEdit->text().toDouble(&ok);
    return ok ? weight : 0.0;
}

bool UnsharpMaskDialog::isPreviewChecked() const {
    return previewCheckBox->isChecked();
}

void UnsharpMaskDialog::onOkButtonPressed() {
    accept();
}
void UnsharpMaskDialog::onCancelButtonPressed() {
    reject();  // 关闭对话框，不保存任何更改
}
