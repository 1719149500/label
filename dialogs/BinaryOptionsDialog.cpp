#include "BinaryOptionsDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>

BinaryOptionsDialog::BinaryOptionsDialog(QWidget *parent)
    : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    iterationsLineEdit = new QLineEdit();
    iterationsLineEdit->setText("1");
    countLineEdit = new QLineEdit();
    countLineEdit->setText("1");
    blackBackgroundCheckBox = new QCheckBox("Black background");
    padEdgesCheckBox = new QCheckBox("Pad edges when eroding");
    edmOutputComboBox = new QComboBox();
    edmOutputComboBox->addItem("Overwrite");
    edmOutputComboBox->addItem("Append");

    formLayout->addRow("Iterations (1-100):", iterationsLineEdit);
    formLayout->addRow("Count (1-8):", countLineEdit);
    formLayout->addRow(blackBackgroundCheckBox);
    formLayout->addRow(padEdgesCheckBox);
    formLayout->addRow("EDM output:", edmOutputComboBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *helpButton = new QPushButton("Help");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(helpButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &BinaryOptionsDialog::onOkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void BinaryOptionsDialog::onOkButtonPressed() {
    accept();
}
void BinaryOptionsDialog::onCancelButtonPressed() {
    reject();  // 关闭对话框，不保存任何更改
}
