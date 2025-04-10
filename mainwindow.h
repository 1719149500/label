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
#include <QAction>
#include <QTableWidget>
#include <QIcon>
#include <QStatusBar>
#include <QActionGroup>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QToolTip>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QSplitter>
#include <QGraphicsPixmapItem>
#include <QToolBar>
#include <QKeySequence>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QTabWidget>
#include <QDialog>
#include <QInputDialog>

#include "dialogs/BrightnessContrastDialog.h"
#include "dialogs/ScaleDialog.h"
#include "dialogs/ThresholdDialog.h"
#include "dialogs/BinaryOptionsDialog.h"
#include "dialogs/ConvolveDialog.h"
#include "dialogs/GaussianBlurDialog.h"
#include "dialogs/FilterDialog.h"
#include "dialogs/UnsharpMaskDialog.h"
#include "dialogs/TopHatDialog.h"
#include "processors/Workspace.h"


class ScaleDialog;
class QAction;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    int lastMinimum;
    int lastMaximum;
    double lastContrast;
    double lastBrightness;


public slots:
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
    void function1();
    void function2();
    void function3();
    void function4();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void updateImageDisplay();


    // 图像操作工具栏相关槽函数
    void onRectSelection();
    void onOvalSelection();
    void onPolygonSelection();
    void onFreehandSelection();
    void onWandSelection();
    void onLineTool();
    void onTextTool();
    void onZoomTool();
    void onHandTool();

    // Image 子菜单项槽函数
    //type子菜单

    void onAdjust();
    void onShowInfo();
    void onProperties();
    void onColor();
    void onStacks();
    void onHyperstacks();
    void onCrop();
    void onDuplicate();
    void onRename();
    void onScale();
    void handleScaleRequest(float factor);
    void onZoom();
    void onOverlay();
    void onLookupTables();

    //Transform子菜单
    void onFlipHorizontally();
    void onFlipVertically();
    void onRotate90Right();
    void onRotate90Left();
    void onRotate();
    void onTranslate();

    //Adjust子菜单
     void applyBrightnessContrast(int minimum, int maximum, double contrast, double brightness);

    //Process

    void onSmooth();
    void onSharpen();

    // Binary 子菜单项槽函数
    void onMakeBinary();
    void onConvertToMask();
    void onErode();
    void onDilate();
    void onOpenBinary();
    void onClose();
    void onMedian();
    void onOutline();
    void onFillHoles();
    void onSkeletonize();
    void onDistanceMap();
    void onUltimatePoints();
    void onWatershed();
    void onVoronoi();
    void onOptions();

    // Filters 子菜单项的槽函数
    void onConvolve();
    void onGaussianBlur();
    void onMedianFilters();
    void onMean();
    void onMinimum();
    void onMaximum();
    void onUnsharpMask();
    void onVariance();
    void onTopHat();


    // Analyze 子菜单项的槽函数
    void onHistogram();
    void onPlotProfile();

    //AI功能函数
    void chooseModelPath();  // 声明选择模型路径的方法
    void runAIModel(const std::string& model_path);  // 声明运行 AI 模型的方法


    void loadImage(const QString& imagePath);
    void drawShapes();
    void clearScene();

    void updateLabelList();
    void deleteBox(QGraphicsItem* item);
    bool currentJson();




private:
    Ui::MainWindow *ui;
    QString dirPath;
    QString currentImage;
    QString txtPath;
    QStringList folderImages;
    QStringList folderJsons;

    int index;
    double factor;// 全局图片比例参数

    QAction *eightBitAction;
    QAction *sixteenBitAction;
    QAction *thirtyTwoBitAction;
    QAction *eightBitColorAction;
    QAction *rgbColorAction;
    QAction *selectedAction;
    // 其他成员变量
    QMenu *typeMenu;
    QActionGroup *typeActionGroup;
    // 重置 Type 子菜单中的 QAction 状态的函数
    void resetTypeActions();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QListWidget *listView;
    QListWidget *labelListView;
    QLineEdit *sceneImageSaveFolderEdit;
    QString sceneImageSaveFolder;
    QWidget *saveLocationBar;
    QVBoxLayout *saveLocationLayout;
    QVBoxLayout  *labelListLayout;
    QTableWidget *tableWidget;
    QTabWidget *tabWidget;

    QList<QRectF> saveShapes();



    std::unique_ptr<Workspace> workspace;
    
};

#endif // MAINWINDOW_H
