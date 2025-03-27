// UnsharpMaskDialog.h

#ifndef UNSHARPMASKDIALOG_H
#define UNSHARPMASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

class UnsharpMaskDialog : public QDialog {
    Q_OBJECT

public:
    explicit UnsharpMaskDialog(QWidget *parent = nullptr);

    double getRadius() const;
    double getMaskWeight() const;
    bool isPreviewChecked() const;

private slots:
    void onOkButtonPressed();
    void onCancelButtonPressed();

private:
    QLineEdit *radiusLineEdit;
    QLineEdit *maskWeightLineEdit;
    QCheckBox *previewCheckBox;
};

#endif // UNSHARPMASKDIALOG_H
