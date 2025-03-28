#ifndef BRIGHTNESSCONTRASTDIALOG_H
#define BRIGHTNESSCONTRASTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLineEdit>

class BrightnessContrastDialog : public QDialog {
    Q_OBJECT

public:
    explicit BrightnessContrastDialog(QWidget *parent = nullptr);
    void setMinimum(int minimum);
    void setMaximum(int maximum);
    void setContrast(double contrast);
    void setBrightness(double brightness);
signals:
    void applyBrightnessContrast(int minimum, int maximum, double contrast, double brightness);

private slots:
    void onApplyButtonPressed();
    void onAutoButtonPressed();
    void onResetButtonPressed();
    void onSetButtonPressed();

    void onMinimumSliderValueChanged(int value);
    void onMaximumSliderValueChanged(int value);
    void onBrightnessSliderValueChanged(int value);
    void onContrastSliderValueChanged(int value);

    void onMinimumEditTextChanged(const QString &text);
    void onMaximumEditTextChanged(const QString &text);
    void onBrightnessEditTextChanged(const QString &text);
    void onContrastEditTextChanged(const QString &text);

private:
    void setupUi();

    QSlider *minimumSlider;
    QSlider *maximumSlider;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;

    QLineEdit *minimumEdit;
    QLineEdit *maximumEdit;
    QLineEdit *brightnessEdit;
    QLineEdit *contrastEdit;
};

#endif // BRIGHTNESSCONTRASTDIALOG_H
