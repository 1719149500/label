#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

class ScaleDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScaleDialog(QWidget *parent = nullptr);

signals:
    void scaleRequested(float factor);

private slots:
    void accept();

private:
    QLineEdit *ScaleEdit;
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QComboBox *interpolationCombo;
    QCheckBox *fillWithBackgroundColor;
    QCheckBox *averageWhenDownsizing;
    QCheckBox *createNewWindow;
    QLineEdit *titleEdit;
};

#endif // SCALEDIALOG_H
