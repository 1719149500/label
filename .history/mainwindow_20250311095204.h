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
#include <QFileDialog>
#include <QMessageBox>
#include <QToolTip>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QSplitter>
#include <QGraphicsPixmapItem>
#include <QToolBar>
#include <QKeySequence>
#include <QAction>
#include <QStatusBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


private slots:
    void openFile();
    void openFolder();
    void selectFromListView();
    void selectSceneImageSaveFolder();
    void saveSceneAsImage();
    void filenameToTxt();
    void toggleTriangleMode(bool checked);
    void saveJson();
    void deleteJson();
    void imageChange();
    void nextImage();
    void previousImage();
    void onContextMenu(const QPoint &point);
    void selectSaveFilenameLocation();
    void showTooltip(const QString &message);
    void autoSaveSceneAsImage();
    void createBox(QRectF rect, const QString& label);


private:
    Ui::MainWindow *ui;
    QString dirPath;
    QString currentImage;
    QString txtPath;
    QString folderImages;
    QString folderJsons;
    int index;

    QGraphicsScene *scene;
    QGraphicsView *view;
    QListWidget *listView;
    QListWidget *labelListView;
    QLineEdit *sceneImageSaveFolderEdit;
    QString sceneImageSaveFolder;
    QWidget *saveLocationBar;
    QVBoxLayout *saveLocationLayout;

    void loadImage(const QString &imagePath);
    void clearScene();
    QList<QRectF> saveShapes();
    void updateLabelList();
    void deleteBox(QGraphicsItem *item);
    bool currentJson();
};

#endif // MAINWINDOW_H
