// BinaryOptionsDialog.h

#ifndef BINARYOPTIONSDIALOG_H
#define BINARYOPTIONSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

class BinaryOptionsDialog : public QDialog {
    Q_OBJECT

public:
    explicit BinaryOptionsDialog(QWidget *parent = nullptr);

private slots:
    void onOkButtonPressed();
    void onCancelButtonPressed();

public:
    QLineEdit *iterationsLineEdit;
    QLineEdit *countLineEdit;
    QCheckBox *blackBackgroundCheckBox;
    QCheckBox *padEdgesCheckBox;
    QComboBox *edmOutputComboBox;
};

#endif // BINARYOPTIONSDIALOG_H
