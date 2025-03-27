// FilterDialog.h

#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

class FilterDialog : public QDialog {
    Q_OBJECT

public:
    explicit FilterDialog(const QString &title, QWidget *parent = nullptr);

    double getRadius() const;
    bool isPreviewChecked() const;

private slots:
    void onOkButtonPressed();
    void onCancelButtonPressed();

private:
    QLineEdit *radiusLineEdit;
    QCheckBox *previewCheckBox;
};

#endif // FILTERDIALOG_H
