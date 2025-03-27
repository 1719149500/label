// TopHatDialog.h

#ifndef TOPHATDIALOG_H
#define TOPHATDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

class TopHatDialog : public QDialog {
    Q_OBJECT

public:
    explicit TopHatDialog(QWidget *parent = nullptr);

    double getRadius() const;
    bool isLightBackgroundChecked() const;
    bool isDontSubtractChecked() const;
    bool isPreviewChecked() const;

private slots:
    void onOkButtonPressed();
    void onCancelButtonPressed();

private:
    QLineEdit *radiusLineEdit;
    QCheckBox *lightBackgroundCheckBox;
    QCheckBox *dontSubtractCheckBox;
    QCheckBox *previewCheckBox;
};

#endif // TOPHATDIALOG_H
