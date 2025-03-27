#include "ScaleDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ScaleDialog::ScaleDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Scale");
    resize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    QLabel *scaleLabel = new QLabel("Scale:");
    ScaleEdit = new QLineEdit();
    formLayout->addRow(scaleLabel, ScaleEdit);

    QLabel *widthLabel = new QLabel("Width (pixels):");
    widthEdit = new QLineEdit();
    formLayout->addRow(widthLabel, widthEdit);

    QLabel *heightLabel = new QLabel("Height (pixels):");
    heightEdit = new QLineEdit();
    formLayout->addRow(heightLabel, heightEdit);

    QLabel *interpolationLabel = new QLabel("Interpolation:");
    interpolationCombo = new QComboBox();
    interpolationCombo->addItem("Bilinear");
    formLayout->addRow(interpolationLabel, interpolationCombo);

    layout->addLayout(formLayout);

    fillWithBackgroundColor = new QCheckBox("Fill with background color");
    layout->addWidget(fillWithBackgroundColor);

    averageWhenDownsizing = new QCheckBox("Average when downsizing");
    layout->addWidget(averageWhenDownsizing);

    createNewWindow = new QCheckBox("Create new window");
    layout->addWidget(createNewWindow);

    QLabel *titleLabel = new QLabel("Title:");
    titleEdit = new QLineEdit();
    layout->addWidget(titleLabel);
    layout->addWidget(titleEdit);

    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &ScaleDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ScaleDialog::accept() {
    // 获取用户输入的缩放因子
    bool ok;
    float factor = ScaleEdit->text().toFloat(&ok);

    if (!ok || factor <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid positive number for Scale.");
        return;
    }

    // 发出信号，通知后端代码
    emit scaleRequested(factor);

    // 关闭对话框
    QDialog::accept();
}
