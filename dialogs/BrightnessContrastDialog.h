#ifndef BRIGHTNESSCONTRASTDIALOG_H
#define BRIGHTNESSCONTRASTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLineEdit>

class BrightnessContrastDialog : public QDialog {
    Q_OBJECT

public:
    explicit BrightnessContrastDialog(QWidget *parent = nullptr);

private slots:
    void onApplyButtonPressed();
    void onAutoButtonPressed();
    void onResetButtonPressed();
    void onSetButtonPressed();

private:
    void setupUi();
    QSlider *minimumSlider;
    QSlider *maximumSlider;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
};

#endif // BRIGHTNESSCONTRASTDIALOG_H
