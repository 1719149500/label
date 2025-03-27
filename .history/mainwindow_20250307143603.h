#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void openFile();
    // void openFolder();
    // void selectFromListView();
    // void selectSceneImageSaveFolder();
    // void saveSceneAsImage();
    // void filenameToTxt();
    // void toggleTriangleMode(bool checked);
    // void saveJson();
    // void deleteJson();
    // void nextImage();
    // void previousImage();
    // void onContextMenu(const QPoint &point);

private:
    Ui::MainWindow *ui;
    QString dirPath;
    QString currentImage;
    QString txtPath;
    int index;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QListWidget *listView;
    QListWidget *labelListView;
    QLineEdit *sceneImageSaveFolderEdit;
    QString sceneImageSaveFolder;
    QWidget *saveLocationBar; // 澹版槑 saveLocationBar
    QVBoxLayout *saveLocationLayout; // 澹版槑甯冨眬

    void loadImage(const QString &imagePath);
    void clearScene();
    QList<QRectF> saveShapes();
    void updateLabelList();
    void deleteBox(QGraphicsItem *item);
    bool currentJson();
    void showTooltip(const QString &message);
};

#endif // MAINWINDOW_H
