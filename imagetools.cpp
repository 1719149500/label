#include "imagetools.h"
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
ImageTools::ImageTools(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Tools");
    resize(500, 60);

    setupMenuBar();
    setupToolBar();

    // 设置状态栏
    statusBar()->showMessage("Ready");
}

ImageTools::~ImageTools()
{
    // 析构函数实现
}

void ImageTools::setupMenuBar()
{
    // 创建菜单栏
    QMenuBar *menuBar = this->menuBar();

    // 创建菜单项
    QMenu *imageMenu = menuBar->addMenu("Image");
    QMenu *processMenu = menuBar->addMenu("Process");
    QMenu *analyzeMenu = menuBar->addMenu("Analyze");
    // 添加子菜单项

    //"Analyze"子菜单
    QAction *histogramAction = analyzeMenu->addAction("Histogram");
    histogramAction->setShortcut(QKeySequence("Ctrl+H"));
    QAction *plotprofileAction = analyzeMenu->addAction("Plot Profile");
    plotprofileAction->setShortcut(QKeySequence("Ctrl+K"));
    //"Image"子菜单
    QAction *typeAction = imageMenu->addAction("Type");
    QAction *adjustAction = imageMenu->addAction("Adjust");
    QAction *showInfoAction = imageMenu->addAction("Show Info...");
    showInfoAction->setShortcut(QKeySequence("Ctrl+I"));
    QAction *propertiesAction = imageMenu->addAction("Properties...");
    propertiesAction->setShortcut(QKeySequence("Ctrl+Shift+P"));
    QAction *colorAction = imageMenu->addAction("Color");
    QAction *stacksAction = imageMenu->addAction("Stacks");
    QAction *hyperstacksAction = imageMenu->addAction("Hyperstacks");
    QAction *cropAction = imageMenu->addAction("Crop");
    cropAction->setShortcut(QKeySequence("Ctrl+Shift+X"));
    QAction *duplicateAction = imageMenu->addAction("Duplicate...");
    duplicateAction->setShortcut(QKeySequence("Ctrl+Shift+D"));
    QAction *renameAction = imageMenu->addAction("Rename...");
    QAction *scaleAction = imageMenu->addAction("Scale...");
    scaleAction->setShortcut(QKeySequence("Ctrl+E"));
    QAction *transformAction = imageMenu->addAction("Transform");
    QAction *zoomAction = imageMenu->addAction("Zoom");
    QAction *overlayAction = imageMenu->addAction("Overlay");
    QAction *lookupTablesAction = imageMenu->addAction("Lookup Tables");

    //"Process"子菜单
    QAction *smooothAction = processMenu->addAction("Smooth");
    QAction *sharpenAction = processMenu->addAction("Sharpen");

    // 添加 Binary 子菜单及其内容
    QMenu *binaryMenu = processMenu->addMenu("Binary");
    QAction *makeBinaryAction = binaryMenu->addAction("Make Binary");
    QAction *convertToMaskAction = binaryMenu->addAction("Convert to Mask");
    QAction *erodeAction = binaryMenu->addAction("Erode");
    QAction *dilateAction = binaryMenu->addAction("Dilate");
    QAction *openBinaryAction = binaryMenu->addAction("Open");
    QAction *closeAction = binaryMenu->addAction("Close");
    QAction *medianAction = binaryMenu->addAction("Median");
    QAction *outlineAction = binaryMenu->addAction("Outline");
    QAction *fillHolesAction = binaryMenu->addAction("Fill Holes");
    QAction *skeletonizeAction = binaryMenu->addAction("Skeletonize");
    QAction *distanceMapAction = binaryMenu->addAction("Distance Map");
    QAction *ultimatePointsAction = binaryMenu->addAction("Ultimate Points");
    QAction *watershedAction = binaryMenu->addAction("Watershed");
    QAction *voronoiAction = binaryMenu->addAction("Voronoi");
    QAction *optionsAction = binaryMenu->addAction("Options...");

    // 连接 Binary 子菜单项的信号和槽
    connect(makeBinaryAction, &QAction::triggered, this, &ImageTools::onMakeBinary);
    connect(convertToMaskAction, &QAction::triggered, this, &ImageTools::onConvertToMask);
    connect(erodeAction, &QAction::triggered, this, &ImageTools::onErode);
    connect(dilateAction, &QAction::triggered, this, &ImageTools::onDilate);
    connect(openBinaryAction, &QAction::triggered, this, &ImageTools::onOpenBinary);
    connect(closeAction, &QAction::triggered, this, &ImageTools::onClose);
    connect(medianAction, &QAction::triggered, this, &ImageTools::onMedian);
    connect(outlineAction, &QAction::triggered, this, &ImageTools::onOutline);
    connect(fillHolesAction, &QAction::triggered, this, &ImageTools::onFillHoles);
    connect(skeletonizeAction, &QAction::triggered, this, &ImageTools::onSkeletonize);
    connect(distanceMapAction, &QAction::triggered, this, &ImageTools::onDistanceMap);
    connect(ultimatePointsAction, &QAction::triggered, this, &ImageTools::onUltimatePoints);
    connect(watershedAction, &QAction::triggered, this, &ImageTools::onWatershed);
    connect(voronoiAction, &QAction::triggered, this, &ImageTools::onVoronoi);
    connect(optionsAction, &QAction::triggered, this, &ImageTools::onOptions);

    // 添加 Filters 子菜单及其内容
    QMenu *filtersMenu = processMenu->addMenu("Filters");
    QAction *convolveAction = filtersMenu->addAction("Convolve...");
    QAction *gaussianBlurAction = filtersMenu->addAction("Gaussian Blur...");
    QAction *medianfiltersAction = filtersMenu->addAction("Median...");
    QAction *meanAction = filtersMenu->addAction("Mean...");
    QAction *minimumAction = filtersMenu->addAction("Minimum...");
    QAction *maximumAction = filtersMenu->addAction("Maximum...");
    QAction *unsharpMaskAction = filtersMenu->addAction("Unsharp Mask...");
    QAction *varianceAction = filtersMenu->addAction("Variance...");
    QAction *topHatAction = filtersMenu->addAction("Top Hat...");
    QAction *bottomHatAction = filtersMenu->addAction("Bottom Hat...");
    QAction *gaussianBlur3DAction = filtersMenu->addAction("Gaussian Blur 3D...");
    QAction *median3DAction = filtersMenu->addAction("Median 3D...");
    QAction *mean3DAction = filtersMenu->addAction("Mean 3D...");
    QAction *minimum3DAction = filtersMenu->addAction("Minimum 3D...");
    QAction *maximum3DAction = filtersMenu->addAction("Maximum 3D...");
    QAction *variance3DAction = filtersMenu->addAction("Variance 3D...");
    QAction *showCircularMasksAction = filtersMenu->addAction("Show Circular Masks...");

    // 连接 Filters 子菜单项的信号和槽
    connect(convolveAction, &QAction::triggered, this, &ImageTools::onConvolve);
    connect(gaussianBlurAction, &QAction::triggered, this, &ImageTools::onGaussianBlur);
    connect(medianAction, &QAction::triggered, this, &ImageTools::onMedianFilters);
    connect(meanAction, &QAction::triggered, this, &ImageTools::onMean);
    connect(minimumAction, &QAction::triggered, this, &ImageTools::onMinimum);
    connect(maximumAction, &QAction::triggered, this, &ImageTools::onMaximum);
    connect(unsharpMaskAction, &QAction::triggered, this, &ImageTools::onUnsharpMask);
    connect(varianceAction, &QAction::triggered, this, &ImageTools::onVariance);
    connect(topHatAction, &QAction::triggered, this, &ImageTools::onTopHat);
    connect(bottomHatAction, &QAction::triggered, this, &ImageTools::onBottomHat);
    connect(gaussianBlur3DAction, &QAction::triggered, this, &ImageTools::onGaussianBlur3D);
    connect(median3DAction, &QAction::triggered, this, &ImageTools::onMedian3D);
    connect(mean3DAction, &QAction::triggered, this, &ImageTools::onMean3D);
    connect(minimum3DAction, &QAction::triggered, this, &ImageTools::onMinimum3D);
    connect(maximum3DAction, &QAction::triggered, this, &ImageTools::onMaximum3D);
    connect(variance3DAction, &QAction::triggered, this, &ImageTools::onVariance3D);
    connect(showCircularMasksAction, &QAction::triggered, this, &ImageTools::onShowCircularMasks);

    // 连接信号和槽
    connect(typeAction, &QAction::triggered, this, &ImageTools::onType);
    connect(adjustAction, &QAction::triggered, this, &ImageTools::onAdjust);
    connect(showInfoAction, &QAction::triggered, this, &ImageTools::onShowInfo);
    connect(propertiesAction, &QAction::triggered, this, &ImageTools::onProperties);
    connect(colorAction, &QAction::triggered, this, &ImageTools::onColor);
    connect(stacksAction, &QAction::triggered, this, &ImageTools::onStacks);
    connect(hyperstacksAction, &QAction::triggered, this, &ImageTools::onHyperstacks);
    connect(cropAction, &QAction::triggered, this, &ImageTools::onCrop);
    connect(duplicateAction, &QAction::triggered, this, &ImageTools::onDuplicate);
    connect(renameAction, &QAction::triggered, this, &ImageTools::onRename);
    connect(scaleAction, &QAction::triggered, this, &ImageTools::onScale);
    connect(transformAction, &QAction::triggered, this, &ImageTools::onTransform);
    connect(zoomAction, &QAction::triggered, this, &ImageTools::onZoom);
    connect(overlayAction, &QAction::triggered, this, &ImageTools::onOverlay);
    connect(lookupTablesAction, &QAction::triggered, this, &ImageTools::onLookupTables);
}

void ImageTools::setupToolBar()
{
    // 创建工具栏
    QToolBar *toolBar = addToolBar("Tools");
    toolBar->setIconSize(QSize(32, 32));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 添加工具栏按钮
    QAction *rectSelectionAction = new QAction(QIcon(":/IMG/new icons/rectangle.png"), "", this); // 文字设置为空
    rectSelectionAction->setToolTip("矩形"); // 设置工具提示
    toolBar->addAction(rectSelectionAction);
    connect(rectSelectionAction, &QAction::triggered, this, &ImageTools::onRectSelection);

    QAction *polygonSelectionAction = new QAction(QIcon(":/IMG/new icons/polygon.png"), "", this); // 文字设置为空
    polygonSelectionAction->setToolTip("多边形"); // 设置工具提示
    toolBar->addAction(polygonSelectionAction);
    connect(polygonSelectionAction, &QAction::triggered, this, &ImageTools::onPolygonSelection);

    QAction *ovalSelectionAction = new QAction(QIcon(":/IMG/new icons/circle.png"), "", this); // 文字设置为空
    ovalSelectionAction->setToolTip("圆形"); // 设置工具提示
    toolBar->addAction(ovalSelectionAction);
    connect(ovalSelectionAction, &QAction::triggered, this, &ImageTools::onOvalSelection);



    QAction *freehandSelectionAction = new QAction(QIcon(":/IMG/new icons/freehand.png"), "", this);
    freehandSelectionAction->setToolTip("抓手");
    toolBar->addAction(freehandSelectionAction);
    connect(freehandSelectionAction, &QAction::triggered, this, &ImageTools::onFreehandSelection);

    // QAction *wandSelectionAction = new QAction(QIcon(":/IMG/new icons/wand.png"), "Wand Selection", this);
    // toolBar->addAction(wandSelectionAction);
    // connect(wandSelectionAction, &QAction::triggered, this, &ImageTools::onWandSelection);

    QAction *lineToolAction = new QAction(QIcon(":/IMG/new icons/straightLine.png"), "", this);
    lineToolAction->setToolTip("直线");
    toolBar->addAction(lineToolAction);
    connect(lineToolAction, &QAction::triggered, this, &ImageTools::onLineTool);

    // QAction *textToolAction = new QAction(QIcon(":/IMG/new icons/text.png"), "Text Tool", this);
    // toolBar->addAction(textToolAction);
    // connect(textToolAction, &QAction::triggered, this, &ImageTools::onTextTool);

    // QAction *zoomToolAction = new QAction(QIcon(":/IMG/new icons/zoom.png"), "Zoom Tool", this);
    // toolBar->addAction(zoomToolAction);
    // connect(zoomToolAction, &QAction::triggered, this, &ImageTools::onZoomTool);

    // QAction *handToolAction = new QAction(QIcon(":/IMG/new icons/hand.png"), "Hand Tool", this);
    // toolBar->addAction(handToolAction);
    // connect(handToolAction, &QAction::triggered, this, &ImageTools::onHandTool);
}

// 新增的槽函数
void ImageTools::onType()
{
    statusBar()->showMessage("Type");
}

void ImageTools::onAdjust()
{
    statusBar()->showMessage("Adjust");
}

void ImageTools::onShowInfo()
{
    statusBar()->showMessage("Show Info...");
}

void ImageTools::onProperties()
{
    statusBar()->showMessage("Properties...");
}

void ImageTools::onColor()
{
    statusBar()->showMessage("Color");
}

void ImageTools::onStacks()
{
    statusBar()->showMessage("Stacks");
}

void ImageTools::onHyperstacks()
{
    statusBar()->showMessage("Hyperstacks");
}

void ImageTools::onCrop()
{
    statusBar()->showMessage("Crop");
}

void ImageTools::onDuplicate()
{
    statusBar()->showMessage("Duplicate...");
}

void ImageTools::onRename()
{
    statusBar()->showMessage("Rename...");
}

void ImageTools::onScale()
{
    statusBar()->showMessage("Scale...");
}

void ImageTools::onTransform()
{
    statusBar()->showMessage("Transform");
}

void ImageTools::onZoom()
{
    statusBar()->showMessage("Zoom");
}

void ImageTools::onOverlay()
{
    statusBar()->showMessage("Overlay");
}

void ImageTools::onLookupTables()
{
    statusBar()->showMessage("Lookup Tables");
}

// 已有的槽函数
void ImageTools::onRectSelection()
{
    statusBar()->showMessage("Rectangular Selection Tool");
}

void ImageTools::onOvalSelection()
{
    statusBar()->showMessage("Oval Selection Tool");
}

void ImageTools::onPolygonSelection()
{
    statusBar()->showMessage("Polygon Selection Tool");
}

void ImageTools::onFreehandSelection()
{
    statusBar()->showMessage("Freehand Selection Tool");
}

void ImageTools::onWandSelection()
{
    statusBar()->showMessage("Wand Selection Tool");
}

void ImageTools::onLineTool()
{
    statusBar()->showMessage("Line Tool");
}

void ImageTools::onTextTool()
{
    statusBar()->showMessage("Text Tool");
}

void ImageTools::onZoomTool()
{
    statusBar()->showMessage("Zoom Tool");
}

void ImageTools::onHandTool()
{
    statusBar()->showMessage("Hand Tool");
}

//Binary子菜单函数
void ImageTools::onMakeBinary()
{
    statusBar()->showMessage("Make Binary");
}

void ImageTools::onConvertToMask()
{
    statusBar()->showMessage("Convert to Mask");
}

void ImageTools::onErode()
{
    statusBar()->showMessage("Erode");
}

void ImageTools::onDilate()
{
    statusBar()->showMessage("Dilate");
}

void ImageTools::onOpenBinary()
{
    statusBar()->showMessage("Open");
}

void ImageTools::onClose()
{
    statusBar()->showMessage("Close");
}

void ImageTools::onMedian()
{
    statusBar()->showMessage("Median");
}

void ImageTools::onOutline()
{
    statusBar()->showMessage("Outline");
}

void ImageTools::onFillHoles()
{
    statusBar()->showMessage("Fill Holes");
}

void ImageTools::onSkeletonize()
{
    statusBar()->showMessage("Skeletonize");
}

void ImageTools::onDistanceMap()
{
    statusBar()->showMessage("Distance Map");
}

void ImageTools::onUltimatePoints()
{
    statusBar()->showMessage("Ultimate Points");
}

void ImageTools::onWatershed()
{
    statusBar()->showMessage("Watershed");
}

void ImageTools::onVoronoi()
{
    statusBar()->showMessage("Voronoi");
}

void ImageTools::onOptions()
{
    statusBar()->showMessage("Options");
}


//Filters子菜单槽函数

void ImageTools::onConvolve()
{
    statusBar()->showMessage("Convolve");
}

void ImageTools::onGaussianBlur()
{
    statusBar()->showMessage("Gaussian Blur");
}

void ImageTools::onMedianFilters()
{
    statusBar()->showMessage("Median");
}

void ImageTools::onMean()
{
    statusBar()->showMessage("Mean");
}

void ImageTools::onMinimum()
{
    statusBar()->showMessage("Minimum");
}

void ImageTools::onMaximum()
{
    statusBar()->showMessage("Maximum");
}

void ImageTools::onUnsharpMask()
{
    statusBar()->showMessage("Unsharp Mask");
}

void ImageTools::onVariance()
{
    statusBar()->showMessage("Variance");
}

void ImageTools::onTopHat()
{
    statusBar()->showMessage("Top Hat");
}

void ImageTools::onBottomHat()
{
    statusBar()->showMessage("Bottom Hat");
}

void ImageTools::onGaussianBlur3D()
{
    statusBar()->showMessage("Gaussian Blur 3D");
}

void ImageTools::onMedian3D()
{
    statusBar()->showMessage("Median 3D");
}

void ImageTools::onMean3D()
{
    statusBar()->showMessage("Mean 3D");
}

void ImageTools::onMinimum3D()
{
    statusBar()->showMessage("Minimum 3D");
}

void ImageTools::onMaximum3D()
{
    statusBar()->showMessage("Maximum 3D");
}

void ImageTools::onVariance3D()
{
    statusBar()->showMessage("Variance 3D");
}

void ImageTools::onShowCircularMasks()
{
    statusBar()->showMessage("Show Circular Masks");
}


//analyze子菜单槽函数

void ImageTools::onHistogram()
{
    statusBar()->showMessage("Histogram");
}
void ImageTools::onPlotProfile()
{
    statusBar()->showMessage("Plot Profile");
}
