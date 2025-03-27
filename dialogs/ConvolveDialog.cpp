// ConvolveDialog.cpp

#include "ConvolveDialog.h"

ConvolveDialog::ConvolveDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    kernelTextEdit = new QTextEdit(this);
    kernelTextEdit->setPlainText("[-1 -1 -1 -1 -1]\n[-1 -1 24 -1 -1]\n[-1 -1 -1 -1 -1]\n[-1 -1 -1 -1 -1]\n[-1 -1 -1 -1 -1]");

    QPushButton *openButton = new QPushButton("Open...", this);
    QPushButton *saveButton = new QPushButton("Save...", this);
    normalizeKernelCheckBox = new QCheckBox("Normalize Kernel", this);
    previewCheckBox = new QCheckBox("Preview", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(saveButton);

    QHBoxLayout *checkboxLayout = new QHBoxLayout();
    checkboxLayout->addWidget(normalizeKernelCheckBox);
    checkboxLayout->addWidget(previewCheckBox);

    mainLayout->addWidget(kernelTextEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(checkboxLayout);

    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(okButton);
    bottomLayout->addWidget(cancelButton);
    mainLayout->addLayout(bottomLayout);

    connect(openButton, &QPushButton::clicked, this, &ConvolveDialog::onOpenButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &ConvolveDialog::onSaveButtonClicked);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString ConvolveDialog::getKernel() const {
    return kernelTextEdit->toPlainText();
}

bool ConvolveDialog::isNormalizeKernelChecked() const {
    return normalizeKernelCheckBox->isChecked();
}

bool ConvolveDialog::isPreviewChecked() const {
    return previewCheckBox->isChecked();
}

void ConvolveDialog::onOpenButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Kernel File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            kernelTextEdit->setPlainText(in.readAll());
            file.close();
        }
    }
}

void ConvolveDialog::onSaveButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Kernel File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << kernelTextEdit->toPlainText();
            file.close();
        }
    }
}
