// GaussianBlurDialog.cpp

#include "GaussianBlurDialog.h"

GaussianBlurDialog::GaussianBlurDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    sigmaLineEdit = new QLineEdit();
    sigmaLineEdit->setText("2.00");
    previewCheckBox = new QCheckBox("Preview");

    formLayout->addRow("Sigma (Radius):", sigmaLineEdit);
    formLayout->addRow(previewCheckBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &GaussianBlurDialog::onOkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

double GaussianBlurDialog::getSigma() const {
    bool ok;
    double sigma = sigmaLineEdit->text().toDouble(&ok);
    return ok ? sigma : 0.0;
}

bool GaussianBlurDialog::isPreviewChecked() const {
    return previewCheckBox->isChecked();
}

void GaussianBlurDialog::onOkButtonPressed() {
    accept();
}
void GaussianBlurDialog::onCancelButtonPressed(){
    reject();
}
