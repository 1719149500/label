#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
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
    lastMinimum = 0;
    lastMaximum = 255;
    lastContrast = 0.0;
    lastBrightness = 0.0;

    // 初始化场景和视图
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::transparent);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);
    view->setStyleSheet("border: 1px solid #ddd;");
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

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
    QWidget* tab1 = new QWidget(this);
    QWidget* tab2 = new QWidget(this);
    QWidget* tab3 = new QWidget(this);


    // 初始化左侧布局
    QSplitter* leftSplitter1 = new QSplitter(Qt::Vertical, tab1);
    QSplitter* leftSplitter2 = new QSplitter(Qt::Vertical, tab2);
    QSplitter* leftSplitter3 = new QSplitter(Qt::Vertical, tab3);

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

    QLabel* warnLabel = new QLabel("标注信息展示功能待添加", labelListView);
    labelListLayout->addWidget(warnLabel);

    // 初始化保存路径栏
    saveLocationBar = new QWidget(this);
    saveLocationLayout = new QVBoxLayout(saveLocationBar);

    QLabel* tipLabel = new QLabel("功能待添加", saveLocationBar);
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
    QVBoxLayout* tab1Layout = new QVBoxLayout(tab1);
    tab1Layout->addWidget(leftSplitter1);
    tab1Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab1Layout->setSpacing(0); // 移除布局间距
    tab1Layout->setStretchFactor(leftSplitter1, 1);
    tab1->setLayout(tab1Layout);

    QVBoxLayout* tab2Layout = new QVBoxLayout(tab2);
    tab2Layout->addWidget(leftSplitter2);
    tab2Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab2Layout->setSpacing(0); // 移除布局间距
    tab2Layout->setStretchFactor(leftSplitter2, 1);
    tab2->setLayout(tab2Layout);

    QVBoxLayout* tab3Layout = new QVBoxLayout(tab3);
    tab3Layout->addWidget(leftSplitter3);
    tab3Layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    tab3Layout->setSpacing(0); // 移除布局间距
    tab3Layout->setStretchFactor(leftSplitter3, 1);
    tab3->setLayout(tab3Layout);

    // 创建一个竖直排列的工具栏
    QToolBar* rightToolBar = new QToolBar("图片操作工具栏", this);
    rightToolBar->setOrientation(Qt::Vertical); // 设置为竖直排列
    rightToolBar->setIconSize(QSize(28, 28)); // 设置图标大小
    rightToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly); // 仅显示图标
    rightToolBar->setStyleSheet(
        "QToolBar { background-color: #f5f5f5; border: none; spacing: 10px; }" // 增加图标间距
        "QToolButton { padding: 5px; }"
        "QToolButton:hover { background-color: #e0e0e0; }"
    );

    // 预留 5 个按钮并设置图标路径
    QAction* actionIcon1 = new QAction(QIcon(":/IMG/new icons/edit.png"), "操作1", this);
    QAction* actionIcon2 = new QAction(QIcon(":/IMG/new icons/zoom-in.png"), "放大", this);
    connect(actionIcon2, &QAction::triggered, this, &MainWindow::zoomIn);
    QAction* actionIcon3 = new QAction(QIcon(":/IMG/new icons/zoom-out.png"), "缩小", this);
    connect(actionIcon3, &QAction::triggered, this, &MainWindow::zoomOut);
    QAction* actionIcon4 = new QAction(QIcon(":/IMG/new icons/upload.png"), "重置大小", this);
    connect(actionIcon4, &QAction::triggered, this, &MainWindow::resetZoom);
    QAction* actionIcon5 = new QAction(QIcon(":/IMG/new icons/ai.png"), "AI", this);
    connect(actionIcon5, &QAction::triggered, this, &MainWindow::chooseModelPath);

    QAction* rectSelectionAction = new QAction(QIcon(":/IMG/new icons/rectangle.png"), "", this); // 文字设置为空
    rectSelectionAction->setToolTip("矩形"); // 设置工具提示
    rightToolBar->addAction(rectSelectionAction);
    connect(rectSelectionAction, &QAction::triggered, this, &MainWindow::onRectSelection);

    QAction* polygonSelectionAction = new QAction(QIcon(":/IMG/new icons/polygon.png"), "", this); // 文字设置为空
    polygonSelectionAction->setToolTip("多边形"); // 设置工具提示
    rightToolBar->addAction(polygonSelectionAction);
    connect(polygonSelectionAction, &QAction::triggered, this, &MainWindow::onPolygonSelection);

    QAction* ovalSelectionAction = new QAction(QIcon(":/IMG/new icons/circle.png"), "", this); // 文字设置为空
    ovalSelectionAction->setToolTip("圆形"); // 设置工具提示
    rightToolBar->addAction(ovalSelectionAction);
    connect(ovalSelectionAction, &QAction::triggered, this, &MainWindow::onOvalSelection);

    QAction* freehandSelectionAction = new QAction(QIcon(":/IMG/new icons/freehand.png"), "", this);
    freehandSelectionAction->setToolTip("抓手");
    rightToolBar->addAction(freehandSelectionAction);
    connect(freehandSelectionAction, &QAction::triggered, this, &MainWindow::onFreehandSelection);

    QAction* lineToolAction = new QAction(QIcon(":/IMG/new icons/straightLine.png"), "", this);
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
    QToolBar* toolbar = addToolBar("工具栏");
    toolbar->setIconSize(QSize(28, 28));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly); // 仅显示图标
    toolbar->setStyleSheet(
        "QToolBar { background-color: #f5f5f5; border: none; spacing: 10px; }" // 增加图标间距
        "QToolButton { padding: 5px; }"
        "QToolButton:hover { background-color: #e0e0e0; }"
    );

    // 添加工具栏按钮
    QAction* openAction = new QAction(QIcon(":/IMG/new icons/png-file.png"), "打开", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    QAction* openFolderAction = new QAction(QIcon(":/IMG/new icons/add-folder.png"), "打开文件夹", this);
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    toolbar->addAction(openFolderAction);

    // Action：上一张
    QAction* action_previous = new QAction(QIcon(":/IMG/new icons/arrowsL.png"), "上一张", this);
    action_previous->setStatusTip("上一张");
    // 设置多个快捷键
    QList<QKeySequence> previousshortcuts;
    previousshortcuts << QKeySequence(Qt::Key_A); // 添加 A 键作为快捷键
    previousshortcuts << QKeySequence(Qt::Key_Left); // 添加左方向键作为快捷键
    action_previous->setShortcuts(previousshortcuts);
    connect(action_previous, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(action_previous);

    // Action：下一张
    QAction* action_next = new QAction(QIcon(":/IMG/new icons/arrowsR.png"), "下一张", this);
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
    QAction* action_save = new QAction(QIcon(":/IMG/new icons/floppy-disk.png"), "保存", this);
    action_save->setStatusTip("保存");
    connect(action_save, &QAction::triggered, this, &MainWindow::saveJson);
    toolbar->addAction(action_save);

    // Action：删除文件
    QAction* action_delete = new QAction(QIcon(":/IMG/new icons/trash-bin.png"), "删除文件", this);
    action_delete->setStatusTip("删除文件");
    connect(action_delete, &QAction::triggered, this, &MainWindow::deleteJson);
    toolbar->addAction(action_delete);

    toolbar->addSeparator();

    // 创建顶部菜单栏
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 文件菜单
    QMenu* fileMenu = menuBar->addMenu(tr("文件"));
    // fileMenu->addAction(openAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addAction(action_save);
    fileMenu->addAction(action_delete);

    // 编辑菜单
    QMenu* editMenu = menuBar->addMenu(tr("编辑"));
    editMenu->addAction(openAction);

    // 显示菜单
    QMenu* viewMenu = menuBar->addMenu(tr("显示"));
    viewMenu->addAction(openAction);

    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu(tr("帮助"));
    helpMenu->addAction(openAction);

    //图像操作菜单
    QMenu* imageMenu = menuBar->addMenu(tr("Image"));
    QMenu* processMenu = menuBar->addMenu(tr("Process"));
    QMenu* analyzeMenu = menuBar->addMenu(tr("Analyze"));

    // "Analyze"子菜单项
    QAction* histogramAction = new QAction(tr("Histogram"), this);
    histogramAction->setShortcut(QKeySequence("Ctrl+H"));
    analyzeMenu->addAction(histogramAction);

    QAction* plotprofileAction = new QAction(tr("Plot Profile"), this);
    plotprofileAction->setShortcut(QKeySequence("Ctrl+K"));
    analyzeMenu->addAction(plotprofileAction);

    // "Image"子菜单项
    // 创建 Type 子菜单
    typeMenu = imageMenu->addMenu(tr("Type"));

    // 创建 QActionGroup
    typeActionGroup = new QActionGroup(this);
    typeActionGroup->setExclusive(true); // 设置为互斥模式

    // 创建并初始化 QAction
    eightBitAction = typeMenu->addAction(tr("8-bit"));
    eightBitAction->setCheckable(true);
    eightBitAction->setChecked(false);
    typeActionGroup->addAction(eightBitAction);

    sixteenBitAction = typeMenu->addAction(tr("16-bit"));
    sixteenBitAction->setCheckable(true);
    sixteenBitAction->setChecked(false);
    typeActionGroup->addAction(sixteenBitAction);

    thirtyTwoBitAction = typeMenu->addAction(tr("32-bit"));
    thirtyTwoBitAction->setCheckable(true);
    thirtyTwoBitAction->setChecked(false);
    typeActionGroup->addAction(thirtyTwoBitAction);

    eightBitColorAction = typeMenu->addAction(tr("8-bit Color"));
    eightBitColorAction->setCheckable(true);
    eightBitColorAction->setChecked(false);
    typeActionGroup->addAction(eightBitColorAction);

    rgbColorAction = typeMenu->addAction(tr("RGB Color"));
    rgbColorAction->setCheckable(true);
    rgbColorAction->setChecked(false);
    typeActionGroup->addAction(rgbColorAction);
    // 连接信号与槽
    connect(eightBitAction, &QAction::triggered, this, [&]() {
        workspace->getMyImage().convertColorDepth(MyImage::k8BitGrayscale);
        updateImageDisplay();
        // workspace->getMyImage().updateSelectedAction(eightBitAction);
        });

    connect(sixteenBitAction, &QAction::triggered, this, [&]() {
        workspace->getMyImage().convertColorDepth(MyImage::k16BitGrayscale);
        updateImageDisplay();
        // workspace->getMyImage().updateSelectedAction(sixteenBitAction);
        });

    connect(thirtyTwoBitAction, &QAction::triggered, this, [&]() {
        workspace->getMyImage().convertColorDepth(MyImage::k32BitGrayscale);
        updateImageDisplay();
        // workspace->getMyImage().updateSelectedAction(thirtyTwoBitAction);
        });

    connect(eightBitColorAction, &QAction::triggered, this, [&]() {
        workspace->getMyImage().convertColorDepth(MyImage::k8BitColor);
        updateImageDisplay();
        // workspace->getMyImage().updateSelectedAction(eightBitColorAction);
        });

    connect(rgbColorAction, &QAction::triggered, this, [&]() {
        workspace->getMyImage().convertColorDepth(MyImage::kRGBColor);
        updateImageDisplay();
        // workspace->getMyImage().updateSelectedAction(rgbColorAction);
        });

    //adjust 子菜单项
    QMenu* adjustMenu = imageMenu->addMenu(tr("Adjust(调整)"));
    QAction* bcAction = adjustMenu->addAction(tr("brightness/contrast(亮度/对比度)"));
    connect(bcAction, &QAction::triggered, this, [this]() {
        BrightnessContrastDialog dialog(this);
        dialog.setMinimum(lastMinimum);
        dialog.setMaximum(lastMaximum);
        dialog.setContrast(lastContrast);
        dialog.setBrightness(lastBrightness);
        connect(&dialog, &BrightnessContrastDialog::applyBrightnessContrast, this, &MainWindow::applyBrightnessContrast);
        dialog.exec();
        });
    QAction* thresholdAction = adjustMenu->addAction(tr("threshold(灰度阈值)"));
    connect(thresholdAction, &QAction::triggered, this, [this]() {
        ThresholdDialog dialog(this);
        dialog.exec();
        });



    QAction* showInfoAction = new QAction(tr("Show Info..."), this);
    showInfoAction->setShortcut(QKeySequence("Ctrl+I"));
    imageMenu->addAction(showInfoAction);

    QAction* propertiesAction = new QAction(tr("Properties..."), this);
    propertiesAction->setShortcut(QKeySequence("Ctrl+Shift+P"));
    imageMenu->addAction(propertiesAction);

    QAction* colorAction = new QAction(tr("Color"), this);
    imageMenu->addAction(colorAction);

    QAction* stacksAction = new QAction(tr("Stacks"), this);
    imageMenu->addAction(stacksAction);

    QAction* hyperstacksAction = new QAction(tr("Hyperstacks"), this);
    imageMenu->addAction(hyperstacksAction);

    QAction* cropAction = new QAction(tr("Crop"), this);
    cropAction->setShortcut(QKeySequence("Ctrl+Shift+X"));
    imageMenu->addAction(cropAction);

    QAction* duplicateAction = new QAction(tr("Duplicate..."), this);
    duplicateAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    imageMenu->addAction(duplicateAction);

    QAction* renameAction = new QAction(tr("Rename..."), this);
    imageMenu->addAction(renameAction);

    QAction* scaleAction = new QAction(tr("Scale..."), this);
    scaleAction->setShortcut(QKeySequence("Ctrl+E"));
    imageMenu->addAction(scaleAction);

    // 添加 Transform 菜单
    QMenu* transformMenu = imageMenu->addMenu(tr("Transform"));
    QAction* flipHorizontallyAction = transformMenu->addAction(tr("Flip Horizontally"));
    QAction* flipVerticallyAction = transformMenu->addAction(tr("Flip Vertically"));
    QAction* rotate90RightAction = transformMenu->addAction(tr("Rotate 90 Degrees Right"));
    QAction* rotate90LeftAction = transformMenu->addAction(tr("Rotate 90 Degrees Left"));
    QAction* rotateAction = transformMenu->addAction(tr("Rotate..."));
    QAction* translateAction = transformMenu->addAction(tr("Translate..."));
    // 连接 Transform 菜单项的信号与槽
    connect(flipHorizontallyAction, &QAction::triggered, this, &MainWindow::onFlipHorizontally);
    connect(flipVerticallyAction, &QAction::triggered, this, &MainWindow::onFlipVertically);
    connect(rotate90RightAction, &QAction::triggered, this, &MainWindow::onRotate90Right);
    connect(rotate90LeftAction, &QAction::triggered, this, &MainWindow::onRotate90Left);
    connect(rotateAction, &QAction::triggered, this, &MainWindow::onRotate);
    connect(translateAction, &QAction::triggered, this, &MainWindow::onTranslate);



    QAction* zoomAction = new QAction(tr("Zoom"), this);
    imageMenu->addAction(zoomAction);

    QAction* overlayAction = new QAction(tr("Overlay"), this);
    imageMenu->addAction(overlayAction);

    QAction* lookupTablesAction = new QAction(tr("Lookup Tables"), this);
    imageMenu->addAction(lookupTablesAction);

    // "Process"子菜单项
    QAction* smooothAction = new QAction(tr("Smooth(柔化)"), this);
    processMenu->addAction(smooothAction);
    connect(smooothAction, &QAction::triggered, this, &MainWindow::onSmooth);

    QAction* sharpenAction = new QAction(tr("Sharpen(锐化)"), this);
    processMenu->addAction(sharpenAction);
    connect(sharpenAction, &QAction::triggered, this, &MainWindow::onSharpen);


    // "Process" -> "Binary" 子菜单
    QMenu* binaryMenu = processMenu->addMenu(tr("Binary"));
    QAction* makeBinaryAction = new QAction(tr("Make Binary"), this);
    binaryMenu->addAction(makeBinaryAction);

    QAction* convertToMaskAction = new QAction(tr("Convert to Mask"), this);
    binaryMenu->addAction(convertToMaskAction);

    QAction* erodeAction = new QAction(tr("Erode"), this);
    binaryMenu->addAction(erodeAction);

    QAction* dilateAction = new QAction(tr("Dilate"), this);
    binaryMenu->addAction(dilateAction);

    QAction* openBinaryAction = new QAction(tr("Open"), this);
    binaryMenu->addAction(openBinaryAction);

    QAction* closeAction = new QAction(tr("Close"), this);
    binaryMenu->addAction(closeAction);

    QAction* medianAction = new QAction(tr("Median"), this);
    binaryMenu->addAction(medianAction);


    QAction* outlineAction = new QAction(tr("Outline"), this);
    binaryMenu->addAction(outlineAction);

    QAction* fillHolesAction = new QAction(tr("Fill Holes"), this);
    binaryMenu->addAction(fillHolesAction);

    QAction* skeletonizeAction = new QAction(tr("Skeletonize"), this);
    binaryMenu->addAction(skeletonizeAction);

    QAction* distanceMapAction = new QAction(tr("Distance Map"), this);
    binaryMenu->addAction(distanceMapAction);

    QAction* ultimatePointsAction = new QAction(tr("Ultimate Points"), this);
    binaryMenu->addAction(ultimatePointsAction);

    QAction* watershedAction = new QAction(tr("Watershed"), this);
    binaryMenu->addAction(watershedAction);

    QAction* voronoiAction = new QAction(tr("Voronoi"), this);
    binaryMenu->addAction(voronoiAction);

    QAction* optionsAction = new QAction(tr("Options..."), this);
    binaryMenu->addAction(optionsAction);

    // "Process" -> "Filters" 子菜单
    QMenu* filtersMenu = processMenu->addMenu(tr("Filters"));
    QAction* convolveAction = new QAction(tr("Convolve..."), this);
    filtersMenu->addAction(convolveAction);

    QAction* gaussianBlurAction = new QAction(tr("Gaussian Blur..."), this);
    filtersMenu->addAction(gaussianBlurAction);

    QAction* medianfiltersAction = new QAction(tr("Median..."), this);
    filtersMenu->addAction(medianfiltersAction);

    QAction* meanAction = new QAction(tr("Mean..."), this);
    filtersMenu->addAction(meanAction);

    QAction* minimumAction = new QAction(tr("Minimum..."), this);
    filtersMenu->addAction(minimumAction);

    QAction* maximumAction = new QAction(tr("Maximum..."), this);
    filtersMenu->addAction(maximumAction);

    QAction* unsharpMaskAction = new QAction(tr("Unsharp Mask..."), this);
    filtersMenu->addAction(unsharpMaskAction);

    QAction* varianceAction = new QAction(tr("Variance..."), this);
    filtersMenu->addAction(varianceAction);

    QAction* topHatAction = new QAction(tr("Top Hat..."), this);
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
    QStatusBar* statusBar = new QStatusBar(this);
    statusBar->setStyleSheet("background-color: #f0f0f0;");
    setStatusBar(statusBar);

    // 使用 QSplitter 实现可拖动调整大小的主布局
    QSplitter* centralWidget = new QSplitter(Qt::Horizontal, this);
    centralWidget->addWidget(tabWidget);
    centralWidget->addWidget(rightToolBar); // 添加竖直工具栏
    centralWidget->addWidget(view);
    centralWidget->setSizes({ 200, 100, 800 }); // 调整主视图和右侧布局的大小比例，其中 100 是工具栏的宽度
    setCentralWidget(centralWidget);

}
