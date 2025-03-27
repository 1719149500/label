// FilterDialog.cpp

#include "FilterDialog.h"

FilterDialog::FilterDialog(const QString &title, QWidget *parent) : QDialog(parent) {
    setWindowTitle(title);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    radiusLineEdit = new QLineEdit();
    radiusLineEdit->setText("2.0");
    previewCheckBox = new QCheckBox("Preview");

    formLayout->addRow("Radius (pixels):", radiusLineEdit);
    formLayout->addRow(previewCheckBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &FilterDialog::onOkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

double FilterDialog::getRadius() const {
    bool ok;
    double radius = radiusLineEdit->text().toDouble(&ok);
    return ok ? radius : 0.0;
}

bool FilterDialog::isPreviewChecked() const {
    return previewCheckBox->isChecked();
}

void FilterDialog::onOkButtonPressed() {
    accept();
}

void FilterDialog::onCancelButtonPressed() {
    reject();  // 关闭对话框，不保存任何更改
}
