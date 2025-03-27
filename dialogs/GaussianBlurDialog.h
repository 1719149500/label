// GaussianBlurDialog.h

#ifndef GAUSSIANBLURDIALOG_H
#define GAUSSIANBLURDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

class GaussianBlurDialog : public QDialog {
    Q_OBJECT

public:
    explicit GaussianBlurDialog(QWidget *parent = nullptr);

    double getSigma() const;
    bool isPreviewChecked() const;

private slots:
    void onOkButtonPressed();
    void onCancelButtonPressed();

private:
    QLineEdit *sigmaLineEdit;
    QCheckBox *previewCheckBox;
};

#endif // GAUSSIANBLURDIALOG_H
