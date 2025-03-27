// ConvolveDialog.h

#ifndef CONVOLVEDIALOG_H
#define CONVOLVEDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

class ConvolveDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConvolveDialog(QWidget *parent = nullptr);

    QString getKernel() const;
    bool isNormalizeKernelChecked() const;
    bool isPreviewChecked() const;

private slots:
    void onOpenButtonClicked();
    void onSaveButtonClicked();

private:
    QTextEdit *kernelTextEdit;
    QCheckBox *normalizeKernelCheckBox;
    QCheckBox *previewCheckBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // CONVOLVEDIALOG_H
