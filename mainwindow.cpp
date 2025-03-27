#include "mainwindow.h"
#include "dialogs/BrightnessContrastDialog.h"
#include "dialogs/ScaleDialog.h"
#include "dialogs/ThresholdDialog.h"
#include "dialogs/BinaryOptionsDialog.h"
#include "dialogs/ConvolveDialog.h"
#include "dialogs/GaussianBlurDialog.h"
#include "dialogs/FilterDialog.h"
#include "dialogs/UnsharpMaskDialog.h"
#include "dialogs/TopHatDialog.h"
#include "processors/MyImage.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),myImage("")
{
    // 设置窗口属性
    setWindowTitle("Label");
    setMinimumSize(1024, 768);// 增大最小尺寸
    setStyleSheet("QMainWindow { background-color: #f8f8f8; }");



    // 设置窗口的初始大小
    resize(1200, 800);

    // 初始化变量
    dirPath = "";
    currentImage = "";
    txtPath = "";
    index = 0;


    // 初始化场景和视图
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::transparent);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);
    view->setStyleSheet("border: 1px solid #ddd;");
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

//     // 初始化按钮
//     QPushButton *newGroupButton = new QPushButton("New", this);
//     QPushButton *clearButton = new QPushButton("Clear", this);
//     QPushButton *loadButton = new QPushButton("Load", this);
//     // 设置按钮的样式表（CSS）
//     QString buttonStyle = R"(
//     QPushButton {
//         background-color: #f0f0f0; /* 浅灰色背景 */
//         border: 1px solid #ddd; /* 浅灰色边框 */
//         border-radius: 5px; /* 圆角 */
//         padding: 5px 10px; /* 内边距 */
//         font-size: 12px; /* 字体大小 */
//         color: #333; /* 字体颜色 */
//     }
//     QPushButton:hover {
//         background-color: #e0e0e0; /* 鼠标悬停时的背景色 */
//     }
//     QPushButton:pressed {
//         background-color: #d0d0d0; /* 按下时的背景色 */
//     }
// )";

//     // 应用样式表到上面按钮
//     newGroupButton->setStyleSheet(buttonStyle);
//     clearButton->setStyleSheet(buttonStyle);
//     loadButton->setStyleSheet(buttonStyle);

//     // 设置按钮的固定宽度（更小的宽度）
//     newGroupButton->setFixedWidth(80);
//     clearButton->setFixedWidth(80);
//     loadButton->setFixedWidth(80);


    // 定义样式表
    QString styleSheet = (
        "QScrollBar:vertical {"
        "    background: #e0e0e0;"
        "    width: 12px;"
        "    border-radius: 6px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #a0a0a0;"
        "    min-height: 40px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar:horizontal {"
        "    background: #e0e0e0;"
        "    height: 12px;"
        "    border-radius: 6px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:horizontal {"
        "    background: #a0a0a0;"
        "    min-width: 40px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none;"
        "    height: 0px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {"
        "    background: none;"
        "    width: 0px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
        "    background: none;"
        "}"
        "QWidget { background-color: #f0f0f0; border-radius: 5px; }"
        "QListWidget { background-color: #f0f0f0; border:none; }"
        "QListWidget::item { padding: 5px;}"
        "QLineEdit { background-color: white; border: 1px solid #ddd; padding: 3px; }"
        "QPushButton { background-color: #f0f0f0;"
        "border: 1px solid #ddd;"
        "border-radius: 5px;"
        " padding: 5px 10px;"
        "font-size: 12px;"
        "color: #333;"
        "}"
        "QPushButton:hover { background-color: #e0e0e0; }"
        "QPushButton:pressed { background-color: #d0d0d0;}"
        );

    // 创建选项卡
    tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::West); // 设置选项卡位置为左侧
    tabWidget->setStyleSheet(
        "QTabWidget::tab-bar {"
        "    background-color: #f8f8f8;" // 设置选项卡栏背景颜色
        "    border: none;" // 移除选项卡栏的边框
        "}"
        "QTabBar::tab {"
        "    background-color: #ffffff;" // 设置选项卡项背景颜色
        "    border: 1px solid #ddd;" // 设置选项卡项边框
        "    border-radius: 5px;" // 设置选项卡项圆角
        "    padding: 5px;" // 设置选项卡项内边距
        "    margin: 3px;" // 设置选项卡项外边距
        "}"
        "QTabBar::tab:selected {"
        "    background-color: #e0e0e0;" // 设置选中选项卡项背景颜色
        "}"

        );

    // 创建不同的功能界面作为选项卡的页面
    QWidget *tab1 = new QWidget(this);
    QWidget *tab2 = new QWidget(this);
    QWidget *tab3 = new QWidget(this);


    // 初始化左侧布局
    QSplitter *leftSplitter1 = new QSplitter(Qt::Vertical, tab1);
    QSplitter *leftSplitter2 = new QSplitter(Qt::Vertical, tab2);
    QSplitter *leftSplitter3 = new QSplitter(Qt::Vertical, tab3);

    //为leftSplitter添加样式表
    leftSplitter1->setStyleSheet(styleSheet);
    leftSplitter2->setStyleSheet(styleSheet);
    leftSplitter3->setStyleSheet(styleSheet);

    // 初始侧列表栏
    listView = new QListWidget(this);
    listView->setStyleSheet("QListWidget { border: none; }"); // 移除边框
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置大小策略为可扩展
    connect(listView, &QListWidget::currentItemChanged, this, &MainWindow::selectFromListView);

    // 初始化标签列表栏
    labelListView = new QListWidget(this);
    // labelListView->setStyleSheet("QListWidget { background-color: #f0f0f0; border: 1px solid #ddd; }");
    labelListLayout = new QVBoxLayout(labelListView);
    labelListLayout->setContentsMargins(10, 10, 10, 10);
    labelListLayout->setSpacing(10);

    QLabel *warnLabel = new QLabel("标注信息展示功能待添加", labelListView);
    labelListLayout->addWidget(warnLabel);

    // 初始化保存路径栏
    saveLocationBar = new QWidget(this);
    saveLocationLayout = new QVBoxLayout(saveLocationBar);

    QLabel *tipLabel = new QLabel("功能待添加", saveLocationBar);
    saveLocationLayout->addWidget(tipLabel);

    // 添加控件到不同的左侧布局
    leftSplitter1->addWidget(listView);
    leftSplitter1->setStretchFactor(0, 1);

    leftSplitter2->addWidget(saveLocationBar);
    leftSplitter2->setStretchFactor(0, 1);


    leftSplitter3->addWidget(labelListView);
    leftSplitter3->setStretchFactor(0, 1);

    // 设置选项卡的标题
    tabWidget->addTab(tab1, tr("功能1"));
    tabWidget->addTab(tab2, tr("功能2"));
    tabWidget->addTab(tab3, tr("功能3"));

    // 设置每个选项卡的布局，使其子部件填充整个选项卡
    QVBoxLayout *tab1Layout = new QVBoxLayout(tab1);
    tab1Layout->addWidget(leftSplitter1);
    tab1Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab1Layout->setSpacing(0); // 移除布局间距
    tab1Layout->setStretchFactor(leftSplitter1, 1);
    tab1->setLayout(tab1Layout);

    QVBoxLayout *tab2Layout = new QVBoxLayout(tab2);
    tab2Layout->addWidget(leftSplitter2);
    tab2Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab2Layout->setSpacing(0); // 移除布局间距
    tab2Layout->setStretchFactor(leftSplitter2, 1);
    tab2->setLayout(tab2Layout);

    QVBoxLayout *tab3Layout = new QVBoxLayout(tab3);
    tab3Layout->addWidget(leftSplitter3);
    tab3Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab3Layout->setSpacing(0); // 移除布局间距
    tab3Layout->setStretchFactor(leftSplitter3, 1);
    tab3->setLayout(tab3Layout);

    // 创建一个竖直排列的工具栏
    QToolBar *rightToolBar = new QToolBar("图片操作工具栏", this);
    rightToolBar->setOrientation(Qt::Vertical); // 设置为竖直排列
    rightToolBar->setIconSize(QSize(28, 28)); // 设置图标大小
    rightToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly); // 仅显示图标
    rightToolBar->setStyleSheet(
        "QToolBar { background-color: #f5f5f5; border: none; spacing: 10px; }" // 增加图标间距
        "QToolButton { padding: 5px; }"
        "QToolButton:hover { background-color: #e0e0e0; }"
        );

    // 预留 5 个按钮并设置图标路径
    QAction *actionIcon1 = new QAction(QIcon(":/IMG/new icons/edit.png"), "操作1", this);
    QAction *actionIcon2 = new QAction(QIcon(":/IMG/new icons/zoom-in.png"), "放大", this);
    connect(actionIcon2, &QAction::triggered, this, &MainWindow::zoomIn);
    QAction *actionIcon3 = new QAction(QIcon(":/IMG/new icons/zoom-out.png"), "缩小", this);
    connect(actionIcon3, &QAction::triggered, this, &MainWindow::zoomOut);
    QAction *actionIcon4 = new QAction(QIcon(":/IMG/new icons/upload.png"), "重置大小", this);
    connect(actionIcon4, &QAction::triggered, this, &MainWindow::resetZoom);
    QAction *actionIcon5 = new QAction(QIcon(":/IMG/new icons/settings.png"), "操作5", this);
    QAction *rectSelectionAction = new QAction(QIcon(":/IMG/new icons/rectangle.png"), "", this); // 文字设置为空
    rectSelectionAction->setToolTip("矩形"); // 设置工具提示
    rightToolBar->addAction(rectSelectionAction);
    connect(rectSelectionAction, &QAction::triggered, this, &MainWindow::onRectSelection);

    QAction *polygonSelectionAction = new QAction(QIcon(":/IMG/new icons/polygon.png"), "", this); // 文字设置为空
    polygonSelectionAction->setToolTip("多边形"); // 设置工具提示
    rightToolBar->addAction(polygonSelectionAction);
    connect(polygonSelectionAction, &QAction::triggered, this, &MainWindow::onPolygonSelection);

    QAction *ovalSelectionAction = new QAction(QIcon(":/IMG/new icons/circle.png"), "", this); // 文字设置为空
    ovalSelectionAction->setToolTip("圆形"); // 设置工具提示
    rightToolBar->addAction(ovalSelectionAction);
    connect(ovalSelectionAction, &QAction::triggered, this, &MainWindow::onOvalSelection);

    QAction *freehandSelectionAction = new QAction(QIcon(":/IMG/new icons/freehand.png"), "", this);
    freehandSelectionAction->setToolTip("抓手");
    rightToolBar->addAction(freehandSelectionAction);
    connect(freehandSelectionAction, &QAction::triggered, this, &MainWindow::onFreehandSelection);

    QAction *lineToolAction = new QAction(QIcon(":/IMG/new icons/straightLine.png"), "", this);
    lineToolAction->setToolTip("直线");
    rightToolBar->addAction(lineToolAction);
    connect(lineToolAction, &QAction::triggered, this, &MainWindow::onLineTool);

    // 将按钮添加到工具栏
    rightToolBar->addAction(actionIcon1);
    rightToolBar->addAction(actionIcon2);
    rightToolBar->addAction(actionIcon3);
    rightToolBar->addAction(actionIcon4);
    rightToolBar->addAction(actionIcon5);

    // 初始化工具栏
    QToolBar *toolbar = addToolBar("工具栏");
    toolbar->setIconSize(QSize(28, 28));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly); // 仅显示图标
    toolbar->setStyleSheet(
        "QToolBar { background-color: #f5f5f5; border: none; spacing: 10px; }" // 增加图标间距
        "QToolButton { padding: 5px; }"
        "QToolButton:hover { background-color: #e0e0e0; }"
        );

    // 添加工具栏按钮
    QAction *openAction = new QAction(QIcon(":/IMG/new icons/png-file.png"), "打开", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    QAction *openFolderAction = new QAction(QIcon(":/IMG/new icons/add-folder.png"), "打开文件夹", this);
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    toolbar->addAction(openFolderAction);

    // Action：上一张
    QAction *action_previous = new QAction(QIcon(":/IMG/new icons/arrowsL.png"), "上一张", this);
    action_previous->setStatusTip("上一张");
    // 设置多个快捷键
    QList<QKeySequence> previousshortcuts;
    previousshortcuts << QKeySequence(Qt::Key_A); // 添加 A 键作为快捷键
    previousshortcuts << QKeySequence(Qt::Key_Left); // 添加左方向键作为快捷键
    action_previous->setShortcuts(previousshortcuts);
    connect(action_previous, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(action_previous);

    // Action：下一张
    QAction *action_next = new QAction(QIcon(":/IMG/new icons/arrowsR.png"), "下一张", this);
    action_next->setStatusTip("下一张");
    // 创建一个快捷键列表
    QList<QKeySequence> nextShortcuts;
    nextShortcuts << QKeySequence(Qt::Key_D); // 添加 D 键作为快捷键
    nextShortcuts << QKeySequence(Qt::Key_Right); // 添加右方向键作为快捷键
    // 设置多个快捷键
    action_next->setShortcuts(nextShortcuts);
    connect(action_next, &QAction::triggered, this, &MainWindow::nextImage);
    toolbar->addAction(action_next);

    // Action：保存
    QAction *action_save = new QAction(QIcon(":/IMG/new icons/floppy-disk.png"), "保存", this);
    action_save->setStatusTip("保存");
    connect(action_save, &QAction::triggered, this, &MainWindow::saveJson);
    toolbar->addAction(action_save);

    // Action：删除文件
    QAction *action_delete = new QAction(QIcon(":/IMG/new icons/trash-bin.png"), "删除文件", this);
    action_delete->setStatusTip("删除文件");
    connect(action_delete, &QAction::triggered, this, &MainWindow::deleteJson);
    toolbar->addAction(action_delete);

    toolbar->addSeparator();

    // 创建顶部菜单栏
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu(tr("文件"));
    // fileMenu->addAction(openAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addAction(action_save);
    fileMenu->addAction(action_delete);

    // 编辑菜单
    QMenu *editMenu = menuBar->addMenu(tr("编辑"));
    editMenu->addAction(openAction);

    // 显示菜单
    QMenu *viewMenu = menuBar->addMenu(tr("显示"));
    viewMenu->addAction(openAction);

    // 帮助菜单
    QMenu *helpMenu = menuBar->addMenu(tr("帮助"));
    helpMenu->addAction(openAction);

    //图像操作菜单
    QMenu *imageMenu = menuBar->addMenu(tr("Image"));
    QMenu *processMenu = menuBar->addMenu(tr("Process"));
    QMenu *analyzeMenu = menuBar->addMenu(tr("Analyze"));

    // "Analyze"子菜单项
    QAction *histogramAction = new QAction(tr("Histogram"), this);
    histogramAction->setShortcut(QKeySequence("Ctrl+H"));
    analyzeMenu->addAction(histogramAction);

    QAction *plotprofileAction = new QAction(tr("Plot Profile"), this);
    plotprofileAction->setShortcut(QKeySequence("Ctrl+K"));
    analyzeMenu->addAction(plotprofileAction);

    // "Image"子菜单项
    // 创建 Type 子菜单
    QMenu *typeMenu = imageMenu->addMenu(tr("Type"));

    // 创建 QAction，并设置标志
    QAction *eightBitAction = typeMenu->addAction(tr("8-bit"));
    eightBitAction->setCheckable(true); // 设置可选中
    eightBitAction->setChecked(false);  // 默认不选中

    QAction *sixteenBitAction = typeMenu->addAction(tr("16-bit"));
    sixteenBitAction->setCheckable(true);
    sixteenBitAction->setChecked(false);

    QAction *thirtyTwoBitAction = typeMenu->addAction(tr("32-bit"));
    thirtyTwoBitAction->setCheckable(true);
    thirtyTwoBitAction->setChecked(false);

    QAction *eightBitColorAction = typeMenu->addAction(tr("8-bit Color"));
    eightBitColorAction->setCheckable(true);
    eightBitColorAction->setChecked(false);

    QAction *rgbColorAction = typeMenu->addAction(tr("RGB Color"));
    rgbColorAction->setCheckable(true);
    rgbColorAction->setChecked(false);

    // 连接信号与槽
    connect(eightBitAction, &QAction::triggered, this, [&]() {
        myImage.convertColorDepth(MyImage::k8BitGrayscale);
        updateImageDisplay();
        // myImage.updateSelectedAction(eightBitAction);
    });

    connect(sixteenBitAction, &QAction::triggered, this, [&]() {
        myImage.convertColorDepth(MyImage::k16BitGrayscale);
        updateImageDisplay();
        // myImage.updateSelectedAction(sixteenBitAction);
    });

    connect(thirtyTwoBitAction, &QAction::triggered, this, [&]() {
        myImage.convertColorDepth(MyImage::k32BitGrayscale);
        updateImageDisplay();
        // myImage.updateSelectedAction(thirtyTwoBitAction);
    });

    connect(eightBitColorAction, &QAction::triggered, this, [&]() {
        myImage.convertColorDepth(MyImage::k8BitColor);
        updateImageDisplay();
        // myImage.updateSelectedAction(eightBitColorAction);
    });

    connect(rgbColorAction, &QAction::triggered, this, [&]() {
        myImage.convertColorDepth(MyImage::kRGBColor);
        updateImageDisplay();
        // myImage.updateSelectedAction(rgbColorAction);
    });

    //adjust 子菜单项
    QMenu *adjustMenu = imageMenu->addMenu(tr("Adjust"));
    QAction *bcAction = adjustMenu->addAction(tr("brightness/contrast"));
    connect(bcAction, &QAction::triggered, this, [this]() {
        BrightnessContrastDialog dialog(this);
        dialog.exec();
    });
    QAction *thresholdAction = adjustMenu->addAction(tr("threshold"));
    connect(thresholdAction, &QAction::triggered, this, [this]() {
        ThresholdDialog dialog(this);
        dialog.exec();
    });



    QAction *showInfoAction = new QAction(tr("Show Info..."), this);
    showInfoAction->setShortcut(QKeySequence("Ctrl+I"));
    imageMenu->addAction(showInfoAction);

    QAction *propertiesAction = new QAction(tr("Properties..."), this);
    propertiesAction->setShortcut(QKeySequence("Ctrl+Shift+P"));
    imageMenu->addAction(propertiesAction);

    QAction *colorAction = new QAction(tr("Color"), this);
    imageMenu->addAction(colorAction);

    QAction *stacksAction = new QAction(tr("Stacks"), this);
    imageMenu->addAction(stacksAction);

    QAction *hyperstacksAction = new QAction(tr("Hyperstacks"), this);
    imageMenu->addAction(hyperstacksAction);

    QAction *cropAction = new QAction(tr("Crop"), this);
    cropAction->setShortcut(QKeySequence("Ctrl+Shift+X"));
    imageMenu->addAction(cropAction);

    QAction *duplicateAction = new QAction(tr("Duplicate..."), this);
    duplicateAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    imageMenu->addAction(duplicateAction);

    QAction *renameAction = new QAction(tr("Rename..."), this);
    imageMenu->addAction(renameAction);

    QAction *scaleAction = new QAction(tr("Scale..."), this);
    scaleAction->setShortcut(QKeySequence("Ctrl+E"));
    imageMenu->addAction(scaleAction);

    // 添加 Transform 菜单
    QMenu *transformMenu = imageMenu->addMenu(tr("Transform"));
    QAction *flipHorizontallyAction = transformMenu->addAction(tr("Flip Horizontally"));
    QAction *flipVerticallyAction = transformMenu->addAction(tr("Flip Vertically"));
    QAction *flipZAction = transformMenu->addAction(tr("Flip Z"));
    QAction *rotate90RightAction = transformMenu->addAction(tr("Rotate 90 Degrees Right"));
    QAction *rotate90LeftAction = transformMenu->addAction(tr("Rotate 90 Degrees Left"));
    QAction *rotateAction = transformMenu->addAction(tr("Rotate..."));
    connect(rotateAction, &QAction::triggered, this, &MainWindow::onRotate);
    QAction *translateAction = transformMenu->addAction(tr("Translate..."));
    QAction *binAction = transformMenu->addAction(tr("Bin..."));
    QAction *imageToResultsAction = transformMenu->addAction(tr("Image to Results"));
    QAction *resultsToImageAction = transformMenu->addAction(tr("Results to Image"));


    QAction *zoomAction = new QAction(tr("Zoom"), this);
    imageMenu->addAction(zoomAction);

    QAction *overlayAction = new QAction(tr("Overlay"), this);
    imageMenu->addAction(overlayAction);

    QAction *lookupTablesAction = new QAction(tr("Lookup Tables"), this);
    imageMenu->addAction(lookupTablesAction);

    // "Process"子菜单项
    QAction *smooothAction = new QAction(tr("Smooth"), this);
    processMenu->addAction(smooothAction);

    QAction *sharpenAction = new QAction(tr("Sharpen"), this);
    processMenu->addAction(sharpenAction);

    // "Process" -> "Binary" 子菜单
    QMenu *binaryMenu = processMenu->addMenu(tr("Binary"));
    QAction *makeBinaryAction = new QAction(tr("Make Binary"), this);
    binaryMenu->addAction(makeBinaryAction);

    QAction *convertToMaskAction = new QAction(tr("Convert to Mask"), this);
    binaryMenu->addAction(convertToMaskAction);

    QAction *erodeAction = new QAction(tr("Erode"), this);
    binaryMenu->addAction(erodeAction);

    QAction *dilateAction = new QAction(tr("Dilate"), this);
    binaryMenu->addAction(dilateAction);

    QAction *openBinaryAction = new QAction(tr("Open"), this);
    binaryMenu->addAction(openBinaryAction);

    QAction *closeAction = new QAction(tr("Close"), this);
    binaryMenu->addAction(closeAction);

    QAction *medianAction = new QAction(tr("Median"), this);
    binaryMenu->addAction(medianAction);


    QAction *outlineAction = new QAction(tr("Outline"), this);
    binaryMenu->addAction(outlineAction);

    QAction *fillHolesAction = new QAction(tr("Fill Holes"), this);
    binaryMenu->addAction(fillHolesAction);

    QAction *skeletonizeAction = new QAction(tr("Skeletonize"), this);
    binaryMenu->addAction(skeletonizeAction);

    QAction *distanceMapAction = new QAction(tr("Distance Map"), this);
    binaryMenu->addAction(distanceMapAction);

    QAction *ultimatePointsAction = new QAction(tr("Ultimate Points"), this);
    binaryMenu->addAction(ultimatePointsAction);

    QAction *watershedAction = new QAction(tr("Watershed"), this);
    binaryMenu->addAction(watershedAction);

    QAction *voronoiAction = new QAction(tr("Voronoi"), this);
    binaryMenu->addAction(voronoiAction);

    QAction *optionsAction = new QAction(tr("Options..."), this);
    binaryMenu->addAction(optionsAction);

    // "Process" -> "Filters" 子菜单
    QMenu *filtersMenu = processMenu->addMenu(tr("Filters"));
    QAction *convolveAction = new QAction(tr("Convolve..."), this);
    filtersMenu->addAction(convolveAction);

    QAction *gaussianBlurAction = new QAction(tr("Gaussian Blur..."), this);
    filtersMenu->addAction(gaussianBlurAction);

    QAction *medianfiltersAction = new QAction(tr("Median..."), this);
    filtersMenu->addAction(medianfiltersAction);

    QAction *meanAction = new QAction(tr("Mean..."), this);
    filtersMenu->addAction(meanAction);

    QAction *minimumAction = new QAction(tr("Minimum..."), this);
    filtersMenu->addAction(minimumAction);

    QAction *maximumAction = new QAction(tr("Maximum..."), this);
    filtersMenu->addAction(maximumAction);

    QAction *unsharpMaskAction = new QAction(tr("Unsharp Mask..."), this);
    filtersMenu->addAction(unsharpMaskAction);

    QAction *varianceAction = new QAction(tr("Variance..."), this);
    filtersMenu->addAction(varianceAction);

    QAction *topHatAction = new QAction(tr("Top Hat..."), this);
    filtersMenu->addAction(topHatAction);





    //连接信号与槽函数
    // 连接 Binary 子菜单项的信号和槽
    connect(makeBinaryAction, &QAction::triggered, this, &MainWindow::onMakeBinary);
    connect(convertToMaskAction, &QAction::triggered, this, &MainWindow::onConvertToMask);
    connect(erodeAction, &QAction::triggered, this, &MainWindow::onErode);
    connect(dilateAction, &QAction::triggered, this, &MainWindow::onDilate);
    connect(openBinaryAction, &QAction::triggered, this, &MainWindow::onOpenBinary);
    connect(closeAction, &QAction::triggered, this, &MainWindow::onClose);
    connect(medianAction, &QAction::triggered, this, &MainWindow::onMedian);
    connect(outlineAction, &QAction::triggered, this, &MainWindow::onOutline);
    connect(fillHolesAction, &QAction::triggered, this, &MainWindow::onFillHoles);
    connect(skeletonizeAction, &QAction::triggered, this, &MainWindow::onSkeletonize);
    connect(distanceMapAction, &QAction::triggered, this, &MainWindow::onDistanceMap);
    connect(ultimatePointsAction, &QAction::triggered, this, &MainWindow::onUltimatePoints);
    connect(watershedAction, &QAction::triggered, this, &MainWindow::onWatershed);
    connect(voronoiAction, &QAction::triggered, this, &MainWindow::onVoronoi);
    connect(optionsAction, &QAction::triggered, this, &MainWindow::onOptions);

    // 连接 Filters 子菜单项的信号和槽
    connect(convolveAction, &QAction::triggered, this, &MainWindow::onConvolve);
    connect(gaussianBlurAction, &QAction::triggered, this, &MainWindow::onGaussianBlur);
    connect(medianfiltersAction, &QAction::triggered, this, &MainWindow::onMedianFilters);
    connect(meanAction, &QAction::triggered, this, &MainWindow::onMean);
    connect(minimumAction, &QAction::triggered, this, &MainWindow::onMinimum);
    connect(maximumAction, &QAction::triggered, this, &MainWindow::onMaximum);
    connect(unsharpMaskAction, &QAction::triggered, this, &MainWindow::onUnsharpMask);
    connect(varianceAction, &QAction::triggered, this, &MainWindow::onVariance);
    connect(topHatAction, &QAction::triggered, this, &MainWindow::onTopHat);



    // 连接信号和槽

    connect(showInfoAction, &QAction::triggered, this, &MainWindow::onShowInfo);
    connect(propertiesAction, &QAction::triggered, this, &MainWindow::onProperties);
    connect(colorAction, &QAction::triggered, this, &MainWindow::onColor);
    connect(stacksAction, &QAction::triggered, this, &MainWindow::onStacks);
    connect(hyperstacksAction, &QAction::triggered, this, &MainWindow::onHyperstacks);
    connect(cropAction, &QAction::triggered, this, &MainWindow::onCrop);
    connect(duplicateAction, &QAction::triggered, this, &MainWindow::onDuplicate);
    connect(renameAction, &QAction::triggered, this, &MainWindow::onRename);
    connect(scaleAction, &QAction::triggered, this, &MainWindow::onScale);
    connect(zoomAction, &QAction::triggered, this, &MainWindow::onZoom);
    connect(overlayAction, &QAction::triggered, this, &MainWindow::onOverlay);
    connect(lookupTablesAction, &QAction::triggered, this, &MainWindow::onLookupTables);

    // 状态栏
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->setStyleSheet("background-color: #f0f0f0;");
    setStatusBar(statusBar);

    // 使用 QSplitter 实现可拖动调整大小的主布局
    QSplitter *centralWidget = new QSplitter(Qt::Horizontal, this);
    centralWidget->addWidget(tabWidget);
    centralWidget->addWidget(rightToolBar); // 添加竖直工具栏
    centralWidget->addWidget(view);
    centralWidget->setSizes({200, 100, 800}); // 调整主视图和右侧布局的大小比例，其中 100 是工具栏的宽度
    setCentralWidget(centralWidget);



    // // 创建可拖动的工具栏
    // QDockWidget *dockWidget = new QDockWidget("工具栏", this);
    // dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea |
    //                             Qt::TopDockWidgetArea |
    //                             Qt::LeftDockWidgetArea |
    //                             Qt::leftDockWidgetArea); // 设置工具栏允许的区域
    // // 添加功能按钮
    // QToolBar *customToolbar = new QToolBar(dockWidget);
    // customToolbar->setIconSize(QSize(28, 28));
    // customToolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // customToolbar->setStyleSheet(
    //     "QToolBar { background-color: #f5f5f5; border: none; spacing: 10px; }" // 增加图标间距
    //     "QToolButton { padding: 5px; }"
    //     "QToolButton:hover { background-color: #e0e0e0; }"
    //     );

    // // 添加按钮
    // QAction *action1 = new QAction(QIcon(":/IMG/new icons/icon1.png"), "功能1", this);
    // connect(action1, &QAction::triggered, this, &MainWindow::function1);
    // customToolbar->addAction(action1);

    // QAction *action2 = new QAction(QIcon(":/IMG/new icons/icon2.png"), "功能2", this);
    // connect(action2, &QAction::triggered, this, &MainWindow::function2);
    // customToolbar->addAction(action2);

    // QAction *action3 = new QAction(QIcon(":/IMG/new icons/icon3.png"), "功能3", this);
    // connect(action3, &QAction::triggered, this, &MainWindow::function3);
    // customToolbar->addAction(action3);

    // QAction *action4 = new QAction(QIcon(":/IMG/new icons/icon4.png"), "功能4", this);
    // connect(action4, &QAction::triggered, this, &MainWindow::function4);
    // customToolbar->addAction(action4);

    // dockWidget->setWidget(customToolbar);
    // addDockWidget(Qt::BottomDockWidgetArea, dockWidget); // 将工具栏添加到主窗口


}

void MainWindow::loadImage(const QString &imagePath)
{
    // 打印当前图片路径
    qDebug() << "Loading image from path:" << imagePath;

    // 检查文件是否存在
    if (!QFile::exists(imagePath)) {
        QMessageBox::warning(this, tr("Error"), tr("File does not exist: %1").arg(imagePath));
        return;
    }

    // 更新 currentImage
    currentImage = imagePath;

    // 更新 myImage 的路径和图像数据
    myImage.updateImage(currentImage.toStdString());

    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image."));
        return;
    }

    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->clear(); // 清除场景中的内容
    scene->addItem(pixmapItem);

    // 先将图片适应窗口大小
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    // 获取当前适应窗口后的缩放比例
    factor = view->transform().m11(); // 获取当前的缩放比例

    // 重置 factor 为当前缩放比例
    view->resetTransform(); // 重置视图变换
    view->scale(factor, factor); // 应用当前的缩放比例
}

void MainWindow::clearScene()
{
    scene->clear();
}

QList<QRectF> MainWindow::saveShapes()
{
    QList<QRectF> shapes;
    // 获取标注方框的坐标
    return shapes;
}

void MainWindow::updateLabelList()
{
    labelListView->clear();
    // 更新标签列表
}



void MainWindow::deleteBox(QGraphicsItem *item)
{
    scene->removeItem(item);
    updateLabelList();
}

bool MainWindow::currentJson()
{
    QString currentJson = dirPath + "/" + currentImage + ".json";
    if (QFile::exists(currentJson))
        return true;
    return false;
}

void MainWindow::showTooltip(const QString &message)
{
    QToolTip::setFont(this->font());
    QToolTip::showText(this->mapToGlobal(QPoint(0, 0)), message, this, QRect(), 1000); // 3秒后自动隐藏
}

void MainWindow::openFile()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
    if (!imagePath.isEmpty()) {
        loadImage(imagePath);
    }
}

void MainWindow::openFolder()
{
    dirPath = QFileDialog::getExistingDirectory(this, "打开目录");

    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg";
    QStringList jsonFilters;
    jsonFilters << "*.json";


    // 获取目录中的图片文件列表
    QStringList imageFiles = QDir(dirPath).entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
    QStringList jsonFiles = QDir(dirPath).entryList(QStringList() << "*.json", QDir::Files);

    // 转换为绝对路径
    QDir dir(dirPath);
    for (const QString &file : imageFiles) {
        folderImages.append(dir.absoluteFilePath(file));
    }

    for (const QString &file : jsonFiles) {
        folderJsons.append(dir.absoluteFilePath(file));
    }


    for (const QString &image : imageFiles)
        listView->addItem(image);

    index = 0;
    currentImage = folderImages[0];
    // 使用 QDir::absoluteFilePath 来生成完整的路径
    QString fullPath = QDir(dirPath).absoluteFilePath(currentImage);
    // 打印完整的图片路径
    qDebug() << "Loading image from path:" << fullPath;
    // 检查文件是否存在
    if (!QFile::exists(fullPath)) {
        QMessageBox::warning(this, tr("警告"), tr("文件不存在: %1").arg(fullPath));
        return;
    }
    loadImage(fullPath);
    listView->setCurrentRow(index);
}

void MainWindow::selectFromListView()
{
    clearScene();
    index = listView->currentRow();
    imageChange();
}

void MainWindow::nextImage()
{
    if (index + 1 >= folderImages.size())
        return;
    index++;
    imageChange();
    listView->setCurrentRow(index);
}

void MainWindow::imageChange()
{
    clearScene();
    currentImage = folderImages[index];
    loadImage(currentImage);
}

void MainWindow::selectSceneImageSaveFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this, "选择保存文件夹");
    if (!folder.isEmpty())
    {
        sceneImageSaveFolder = folder;
        sceneImageSaveFolderEdit->setText(folder); // 显示选择的文件夹路径
    }
}

void MainWindow::saveSceneAsImage()
{
    QRectF sceneRect = scene->sceneRect();
    int width = int(sceneRect.width());
    int height = int(sceneRect.height());

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    scene->render(&painter, QRectF(0, 0, width, height), sceneRect);
    painter.end();

    QString filePath = sceneImageSaveFolder + "/scene_" + QFileInfo(currentImage).baseName() + ".jpg";

    if (image.save(filePath, "JPG", 100))
        showTooltip("图片保存成功！");
    else
        showTooltip("图片保存失败！");
}

void MainWindow::filenameToTxt()
{
    if (txtPath.isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("未选择保存txt文件的路径。"), QMessageBox::Ok);
        return;
    }

    QString filePath = txtPath;
    QString currentFilename = QFileInfo(currentImage).baseName();

    QFile file(filePath);
    QStringList lines;

    if (file.exists())
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "无法打开文件进行读取:" << filePath;
            return;
        }

        QTextStream in(&file);
        lines = in.readAll().split("\n");

        file.close();

        // 检查文件中是否包含当前文件名
        if (lines.contains(currentFilename))
        {
            lines.removeAll(currentFilename);
            showTooltip(QString("已移除 '%1'").arg(currentFilename));
        }
        else
        {
            lines.append(currentFilename);
            showTooltip(QString("已添加 '%1'").arg(currentFilename));
        }
    }
    else
    {
        lines.append(currentFilename);
        showTooltip(QString("文件不存在。新建 '%1'，并添加 '%1'").arg(currentFilename));
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件进行写入:" << filePath;
        return;
    }

    QTextStream out(&file);
    out << lines.join("\n");
    file.close();
}

void MainWindow::selectSaveFilenameLocation()
{
    // 设置文件对话框的选项
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog; // 不使用本地文件对话框
    options |= QFileDialog::DontConfirmOverwrite; // 不确认覆盖

    // 打开保存文件对话框
    txtPath = QFileDialog::getSaveFileName(
        this, // 父窗口
        "保存为 TXT 文件", // 对话框标题
        "", // 默认路径（空字符串表示使用上次的路径）
        "Text Files (*.txt);;All Files (*)", // 文件过滤器
        nullptr, // 没有选择的过滤器，所以传递 nullptr
        options // 对话框选项
        );

    // 如果用户选择了文件且文件名没有以 .txt 结尾，添加后缀
    if (!txtPath.isEmpty())
    {
        if (!txtPath.endsWith(".txt"))
        {
            txtPath += ".txt"; // 添加.txt后缀
        }

        // 将选择的文件路径赋值给 QLineEdit 控件
        sceneImageSaveFolderEdit->setText(txtPath);
    }
}

void MainWindow::toggleTriangleMode(bool checked)
{
    // view->setTriangleMode(checked);
    // if (checked)
    //     triangle_action->setText("停止生成");
    // else
    //     triangle_action->setText("生成三角形");
}

void MainWindow::autoSaveSceneAsImage()
{
    qDebug() << "autoSaveSceneAsImage() method clicked";
    // if (view->triangles)
    //     saveSceneAsImage();
}

void MainWindow::createBox(QRectF rect, const QString& label){
    qDebug() << "createBox() method clicked";
};





void MainWindow::previousImage(){
    // 检查索引是否已经为 0，如果是，则无法再向前移动
    if (index - 1 < 0)
        return;

    // 将索引向前移动
    index--;

    // 调用 imageChange() 函数来更新图片显示
    imageChange();

    // 更新 listView 的当前选中项
    listView->setCurrentRow(index);
    qDebug() << "previousImage() method clicked";
};

// void MainWindow::toggleTriangleMode(bool checked)
// {
//     view->setTriangleMode(checked);
//     if (checked)
//         triangleAction->setText("停止生成");
//     else
//         triangleAction->setText("生成三角形");
// }

void MainWindow::saveJson()
{
    qDebug() << "saveJson() method clicked";
    // LabelFile lf;
    // imageData = LabelFile::loadImageFile(currentImage);
    // QString name = QFileInfo(currentImage).baseName() + ".json";

    // QList<QRectF> shapes = saveShapes();
    // if (shapes.isEmpty())
    // {
    //     QMessageBox msg(this);
    //     msg.setWindowTitle("提示");
    //     msg.setText("当前图片没有标注可保存。");
    //     msg.exec();
    //     return;
    // }

    // lf.save(name, shapes, QFileInfo(currentImage).fileName(), imageData, 640, 640);
}

void MainWindow::deleteJson()
{
    qDebug() << "deleteJson() method clicked";
    // if (currentJson())
    // {
    //     QMessageBox::StandardButton buttonReply = QMessageBox::question(this, "PyQt5 message", "确认删除" + currentJson() + "?",
    //                                                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    //     if (buttonReply == QMessageBox::Yes)
    //     {
    //         QFile file(currentJson());
    //         if (file.remove()) {
    //             qDebug() << "成功删除" << currentJson();
    //         } else {
    //             qDebug() << "删除失败" << currentJson();
    //         }
    //     }
    // }
    // else
    // {
    //     QMessageBox msg(this);
    //     msg.setWindowTitle("提示");
    //     msg.setText("无文件可删除。");
    //     msg.exec();
    // }
}

void MainWindow::onContextMenu(const QPoint &point)
{
    QPointF scenePos = view->mapToScene(point);

    QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

    QMenu contextMenu;
    QAction *actionPrevious = new QAction("上一张", this);
    connect(actionPrevious, &QAction::triggered, this, &MainWindow::previousImage);
    contextMenu.addAction(actionPrevious);

    QAction *actionNext = new QAction("下一张", this);
    connect(actionNext, &QAction::triggered, this, &MainWindow::nextImage);
    contextMenu.addAction(actionNext);

    contextMenu.addSeparator();

    QAction *actionDeleteBox = new QAction("删除方框", this);
    connect(actionDeleteBox, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            deleteBox(clickedItem);
    });
    contextMenu.addAction(actionDeleteBox);

    contextMenu.addSeparator();

    QAction *actionGn = new QAction("G-", this);
    connect(actionGn, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    });
    contextMenu.addAction(actionGn);

    QAction *actionGp = new QAction("G+", this);
    connect(actionGp, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    });
    contextMenu.addAction(actionGp);

    QAction *actionBn = new QAction("B-", this);
    connect(actionBn, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    });
    contextMenu.addAction(actionBn);

    QAction *actionBp = new QAction("B+", this);
    connect(actionBp, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    });
    contextMenu.addAction(actionBp);

    contextMenu.exec(view->mapToGlobal(point));
}

void MainWindow::function1() {
    // 功能1的实现
    qDebug() << "功能1被调用";
}

void MainWindow::function2() {
    // 功能2的实现
    qDebug() << "功能2被调用";
}

void MainWindow::function3() {
    // 功能3的实现
    qDebug() << "功能3被调用";
}

void MainWindow::function4() {
    // 功能4的实现
    qDebug() << "功能4被调用";
}




void MainWindow::onAdjust()
{
    statusBar()->showMessage("Adjust");
}

void MainWindow::onShowInfo()
{
    statusBar()->showMessage("Show Info...");
}

void MainWindow::onProperties()
{
    statusBar()->showMessage("Properties...");
}

void MainWindow::onColor()
{
    statusBar()->showMessage("Color");
}

void MainWindow::onStacks()
{
    statusBar()->showMessage("Stacks");
}

void MainWindow::onHyperstacks()
{
    statusBar()->showMessage("Hyperstacks");
}

void MainWindow::onCrop()
{
    // // 弹出对话框，让用户输入裁剪参数
    // int x = QInputDialog::getInt(this, "Crop", "X:", 0, 0, myImage.getImageWidth());
    // int y = QInputDialog::getInt(this, "Crop", "Y:", 0, 0, myImage.getImageHeight());
    // int width = QInputDialog::getInt(this, "Crop", "Width:", 100, 1, myImage.getImageWidth());
    // int height = QInputDialog::getInt(this, "Crop", "Height:", 100, 1, myImage.getImageHeight());

    // // 调用后端的 crop 函数
    // myImage.crop(x, y, width, height);

    // 可选：更新界面或显示消息
    QMessageBox::information(this, "Crop", "Image cropped successfully!");
    statusBar()->showMessage("Crop");
}

void MainWindow::onDuplicate()
{
    statusBar()->showMessage("Duplicate...");
}

void MainWindow::onRename()
{
    statusBar()->showMessage("Rename...");
}


//弹出scale界面
void MainWindow::onScale() {
    // 创建 ScaleDialog 对话框
    ScaleDialog dialog(this);

    // 连接 ScaleDialog 的 scaleRequested 信号到 MainWindow 的 handleScaleRequest 槽函数
    connect(&dialog, &ScaleDialog::scaleRequested, this, &MainWindow::handleScaleRequest);

    // 显示对话框
    dialog.exec();
}

void MainWindow::onZoom()
{
    statusBar()->showMessage("Zoom");
}

void MainWindow::onOverlay()
{
    statusBar()->showMessage("Overlay");
}

void MainWindow::onLookupTables()
{
    statusBar()->showMessage("Lookup Tables");
}

// 已有的槽函数
void MainWindow::onRectSelection()
{
    statusBar()->showMessage("Rectangular Selection Tool");
}

void MainWindow::onOvalSelection()
{
    statusBar()->showMessage("Oval Selection Tool");
}

void MainWindow::onPolygonSelection()
{
    statusBar()->showMessage("Polygon Selection Tool");
}

void MainWindow::onFreehandSelection()
{
    statusBar()->showMessage("Freehand Selection Tool");
}

void MainWindow::onWandSelection()
{
    statusBar()->showMessage("Wand Selection Tool");
}

void MainWindow::onLineTool()
{
    statusBar()->showMessage("Line Tool");
}

void MainWindow::onTextTool()
{
    statusBar()->showMessage("Text Tool");
}

void MainWindow::onZoomTool()
{
    statusBar()->showMessage("Zoom Tool");
}

void MainWindow::onHandTool()
{
    statusBar()->showMessage("Hand Tool");
}

//Binary子菜单函数
void MainWindow::onMakeBinary()
{
    statusBar()->showMessage("Make Binary");
}

void MainWindow::onConvertToMask()
{
    statusBar()->showMessage("Convert to Mask");
}

void MainWindow::onErode()
{
    statusBar()->showMessage("Erode");
}

void MainWindow::onDilate()
{
    statusBar()->showMessage("Dilate");
}

void MainWindow::onOpenBinary()
{
    statusBar()->showMessage("Open");
}

void MainWindow::onClose()
{
    statusBar()->showMessage("Close");
}

void MainWindow::onMedian()
{
    // 弹出输入对话框
    bool ok;
    QString radiusStr = QInputDialog::getText(this, tr("Input Radius"),
                                              tr("Radius:"), QLineEdit::Normal,
                                              "", &ok);
    if (ok && !radiusStr.isEmpty()) {
        // 用户点击了 OK 并且输入了有效的半径值
        bool conversionOk;
        double radius = radiusStr.toDouble(&conversionOk); // 将输入的字符串转换为 double 类型
        if (conversionOk) {
            // 在这里处理半径值，例如应用到你的图像处理算法中
        } else {
            // 输入的字符串不能转换为 double，可以提示用户重新输入或进行其他处理
        }
    }
    statusBar()->showMessage("Median");
}

void MainWindow::onOutline()
{
    statusBar()->showMessage("Outline");
}

void MainWindow::onFillHoles()
{
    statusBar()->showMessage("Fill Holes");
}

void MainWindow::onSkeletonize()
{
    statusBar()->showMessage("Skeletonize");
}

void MainWindow::onDistanceMap()
{
    statusBar()->showMessage("Distance Map");
}

void MainWindow::onUltimatePoints()
{
    statusBar()->showMessage("Ultimate Points");
}

void MainWindow::onWatershed()
{
    statusBar()->showMessage("Watershed");
}

void MainWindow::onVoronoi()
{
    statusBar()->showMessage("Voronoi");
}

void MainWindow::onOptions()
{
    BinaryOptionsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取对话框中的值并进行处理
        QString iterations = dialog.iterationsLineEdit->text();
        QString count = dialog.countLineEdit->text();
        bool blackBackground = dialog.blackBackgroundCheckBox->isChecked();
        bool padEdges = dialog.padEdgesCheckBox->isChecked();
        QString edmOutput = dialog.edmOutputComboBox->currentText();

        // 在这里处理获取的值
    }
    statusBar()->showMessage("Options");
}





//Filters子菜单槽函数

void MainWindow::onConvolve()
{
    ConvolveDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString kernel = dialog.getKernel();
        bool normalizeKernel = dialog.isNormalizeKernelChecked();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
    }
    statusBar()->showMessage("Convolve");
}

void MainWindow::onGaussianBlur()
{
    GaussianBlurDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        double sigma = dialog.getSigma();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
    }

    statusBar()->showMessage("Gaussian Blur");
}

void MainWindow::onMedianFilters()
{
    FilterDialog dialog("Median...", this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Median - Radius:" << radius << "Preview:" << preview;
    }
}

void MainWindow::onMean()
{
    FilterDialog dialog("Mean...", this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Mean - Radius:" << radius << "Preview:" << preview;
    }
    statusBar()->showMessage("Mean");
}

void MainWindow::onMinimum()
{
    FilterDialog dialog("Minimum...", this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Minimum - Radius:" << radius << "Preview:" << preview;
    }
    statusBar()->showMessage("Minimum");
}

void MainWindow::onMaximum()
{
    FilterDialog dialog("Maximum...", this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Maximum - Radius:" << radius << "Preview:" << preview;
    }
    statusBar()->showMessage("Maximum");
}

void MainWindow::onUnsharpMask()
{
    UnsharpMaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        double maskWeight = dialog.getMaskWeight();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Radius:" << radius << "Mask Weight:" << maskWeight << "Preview:" << preview;}
    statusBar()->showMessage("Unsharp Mask");
}

void MainWindow::onVariance()
{
    FilterDialog dialog("Variance...", this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Variance - Radius:" << radius << "Preview:" << preview;
    }
    statusBar()->showMessage("Variance");
}

void MainWindow::onTopHat()
{
    TopHatDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        double radius = dialog.getRadius();
        bool lightBackground = dialog.isLightBackgroundChecked();
        bool dontSubtract = dialog.isDontSubtractChecked();
        bool preview = dialog.isPreviewChecked();

        // 在这里处理获取的值
        qDebug() << "Radius:" << radius << "Light Background:" << lightBackground
                 << "Don't Subtract:" << dontSubtract << "Preview:" << preview;
    }
    statusBar()->showMessage("Top Hat");
}





//analyze子菜单槽函数

void MainWindow::onHistogram()
{
    statusBar()->showMessage("Histogram");
}
void MainWindow::onPlotProfile()
{
    statusBar()->showMessage("Plot Profile");
}




void MainWindow::onRotate() {
    bool ok;
    // 获取用户输入的角度值，设置默认值为 12.70，范围从 -360 到 360，小数点后保留两位
    double angle = QInputDialog::getDouble(this, tr("Rotate"),
                                           tr("Angle:"), 12.70, -360.0, 360.0, 2, &ok);
    if (ok) {
        // 如果用户点击了 OK 按钮，则输出输入的角度值
        qDebug() << "Rotate by angle:" << angle;
    }
}


void MainWindow::handleScaleRequest(float factor) {
    // 检查 myImage 是否为空
    if (myImage.isEmpty()) { // 假设 MyImage 类有一个 isEmpty() 方法
        QMessageBox::warning(this, "No Image", "No image loaded to scale.");
        return;
    }
    myImage.scale(factor);
    // 更新主界面的图片显示
    updateImageDisplay();

}

void MainWindow::zoomIn()
{
    factor = factor + 0.05; // 放大时，将 factor 增加 5%
    view->resetTransform(); // 重置视图变换
    view->scale(factor, factor); // 根据新的 factor 值进行缩放
}

void MainWindow::zoomOut()
{
    factor = factor - 0.05; // 缩小时，将 factor 减小 5%
    view->resetTransform(); // 重置视图变换
    view->scale(factor, factor); // 根据新的 factor 值进行缩放
}

void MainWindow::resetZoom()
{
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    // 获取当前适应窗口后的缩放比例
    factor = view->transform().m11(); // 获取当前的缩放比例

    // 重置 factor 为当前缩放比例
    view->resetTransform(); // 重置视图变换
    view->scale(factor, factor); // 应用当前的缩放比例
}

void MainWindow::updateImageDisplay() {
    // 将 MyImage 的图像转换为 QPixmap
    cv::Mat image = myImage.getImageMat(); // 假设 MyImage 提供了一个方法来获取当前的 cv::Mat

    // 确保图像是有效的
    if (image.empty()) {
        std::cerr << "Error: Image is empty." << std::endl;
        return;
    }

    // 检查图像尺寸和步长
    if (image.rows <= 0 || image.cols <= 0) {
        std::cerr << "Error: Image dimensions are invalid." << std::endl;
        return;
    }

    QImage qImage;
    QImage::Format format;
    cv::Mat convertedImage;

    // 根据图像通道数和数据类型选择转换方式
    if (image.channels() == 1) {
        // 灰度图
        if (image.depth() == CV_16U) {
            // 16-bit 归一化到 0-255
            image.convertTo(convertedImage, CV_8U, 255.0 / 65535.0);
        } else if (image.depth() == CV_32F) {
            // 32-bit float 归一化到 0-255
            image.convertTo(convertedImage, CV_8U, 255.0 / 1.0);
        } else {
            convertedImage = image;
        }
        format = QImage::Format_Grayscale8;
    } else if (image.channels() == 3) {
        // 彩色图
        if (image.depth() == CV_32F) {
            // 32-bit float 归一化到 0-255
            image.convertTo(convertedImage, CV_8U, 255.0 / 1.0);
        } else {
            convertedImage = image;
        }
        format = QImage::Format_RGB888;
    } else {
        std::cerr << "Error: Unsupported image format." << std::endl;
        return;
    }

    // 将 OpenCV 的图像转换为 QImage
    qImage = QImage(convertedImage.data, convertedImage.cols, convertedImage.rows, convertedImage.step, format);

    // 检查 QImage 是否有效
    if (qImage.isNull()) {
        std::cerr << "Error: QImage is null." << std::endl;
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(qImage);

    // 检查 QPixmap 是否有效
    if (pixmap.isNull()) {
        std::cerr << "Error: QPixmap is null." << std::endl;
        return;
    }

    // 清除场景中的旧内容
    scene->clear();

    // 添加新的 QPixmap 到场景中
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->addItem(pixmapItem);
}
