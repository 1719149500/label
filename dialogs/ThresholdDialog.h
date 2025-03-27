#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>

class ThresholdDialog : public QDialog {
    Q_OBJECT

public:
    explicit ThresholdDialog(QWidget *parent = nullptr);

private slots:
    void onAutoButtonPressed();
    void onApplyButtonPressed();
    void onResetButtonPressed();
    void onSetButtonPressed();

private:
    void setupUi();
    QLineEdit *lowerLineEdit;
    QLineEdit *upperLineEdit;
    QComboBox *channelComboBox;
    QComboBox *modeComboBox;
    QCheckBox *darkBackgroundCheckBox;
    QCheckBox *stackHistogramCheckBox;
    QCheckBox *dontResetRangeCheckBox;
    QCheckBox *rawValuesCheckBox;
    QCheckBox *sixteenBitHistogramCheckBox;
};

#endif // THRESHOLDDIALOG_H
