#include "mainwindow.h"

void MainWindow::loadImage(const QString& imagePath)
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

    // 更新 workspace->getMyImage() 的路径和图像数据
    /*workspace->getMyImage().updateImage(currentImage.toStdString());*/
    workspace = std::make_unique<Workspace>(std::make_unique<MyImage>(currentImage.toStdString()));

    // 加载标注文件
    if (!workspace->loadFromAnnotationFile()) {
        qDebug() << "No annotation file found for the image.";
    }



    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image."));
        return;
    }

    scene->clear(); // 清除场景中的内容

    // 添加图片到场景中
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->addItem(pixmapItem);

    // 先将图片适应窗口大小
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    // 获取当前适应窗口后的缩放比例
    factor = view->transform().m11(); // 获取当前的缩放比例

    // 重置 factor 为当前缩放比例
    view->resetTransform(); // 重置视图变换
    view->scale(factor, factor); // 应用当前的缩放比例

    // 绘制点或矩形
    drawShapes();
}

void MainWindow::drawShapes()
{

    // 获取当前图片的标注信息
    const std::vector<MyShape>& shapes = workspace->getShapes();

    // 遍历标注信息并绘制
    for (const auto& shape : shapes) {
        const std::vector<Point>& points = shape.getPoints();
        if (points.size() >= 2) {
            // 获取两个点的坐标
            double x1 = points[0].x;
            double y1 = points[0].y;
            double x2 = points[1].x;
            double y2 = points[1].y;

            // 计算矩形的左上角和右下角坐标
            double left = std::min(x1, x2);
            double top = std::min(y1, y2);
            double right = std::max(x1, x2);
            double bottom = std::max(y1, y2);

            // 绘制矩形
            QGraphicsRectItem* rectItem = scene->addRect(
                QRectF(left, top, right - left, bottom - top),
                QPen(Qt::red, 2), QBrush(Qt::transparent));
            rectItem->setFlag(QGraphicsItem::ItemIsSelectable);
        }
    }   
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



void MainWindow::deleteBox(QGraphicsItem* item)
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

void MainWindow::showTooltip(const QString& message)
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
    // 弹出文件夹选择对话框
    dirPath = QFileDialog::getExistingDirectory(this, "打开目录");

    // 检查用户是否取消了选择文件夹
    if (dirPath.isEmpty()) {
        return; // 用户取消选择，直接返回
    }

    // 确保路径存在
    if (!QDir(dirPath).exists()) {
        QMessageBox::warning(this, tr("错误"), tr("指定的文件夹不存在: %1").arg(dirPath));
        return; // 文件夹不存在，返回
    }

    // 获取目录中的图片文件列表
    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg";
    QDir dir(dirPath);
    QStringList imageFiles = dir.entryList(imageFilters, QDir::Files | QDir::NoDotAndDotDot);

    // 清空之前的图片列表
    folderImages.clear();
    listView->clear(); // 清空 listView

    // 转换为绝对路径并添加到列表中
    for (const QString& file : imageFiles) {
        folderImages.append(dir.absoluteFilePath(file));
    }

    // 更新 listView
    for (const QString& image : imageFiles) {
        listView->addItem(image);
    }

    // 检查是否找到图片文件
    if (!folderImages.isEmpty()) {
        index = 0; // 重置索引
        currentImage = folderImages[0];
        QString fullPath = QDir(dirPath).absoluteFilePath(currentImage);
        qDebug() << "Loading image from path:" << fullPath;
        if (!QFile::exists(fullPath)) {
            QMessageBox::warning(this, tr("警告"), tr("文件不存在: %1").arg(fullPath));
            return;
        }
        loadImage(fullPath);
        listView->setCurrentRow(index);
    }
    else {
        // 如果没有找到图片文件，弹出提示信息
        QMessageBox::information(this, tr("提示"), tr("文件夹需要包含PNG或者JPG文件。"));
    }
}

void MainWindow::selectFromListView()
{
    clearScene();
    index = listView->currentRow();
    if (index >= 0 && index < folderImages.size()) {
        imageChange();
    }
    else {
        qDebug() << "Invalid index:" << index;
    }
}

void MainWindow::previousImage() {
    if (index > 0) {
        index--;
        imageChange();
        listView->setCurrentRow(index);
    }
    else {
        qDebug() << "Already at the first image";
    }
};

void MainWindow::nextImage()
{
    if (index + 1 < folderImages.size()) {
        index++;
        imageChange();
        listView->setCurrentRow(index);
    }
    else {
        qDebug() << "Already at the last image";
    }


}

void MainWindow::imageChange()
{
    clearScene();
    if (index >= 0 && index < folderImages.size()) {
        currentImage = folderImages[index];
        loadImage(currentImage);
        resetTypeActions(); // 调用重置函数
    }
    else {
        qDebug() << "Invalid index:" << index;
    }
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

void MainWindow::createBox(QRectF rect, const QString& label) {
    qDebug() << "createBox() method clicked";
};









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

void MainWindow::onContextMenu(const QPoint& point)
{
    QPointF scenePos = view->mapToScene(point);

    QGraphicsItem* clickedItem = scene->itemAt(scenePos, QTransform());

    QMenu contextMenu;
    QAction* actionPrevious = new QAction("上一张", this);
    connect(actionPrevious, &QAction::triggered, this, &MainWindow::previousImage);
    contextMenu.addAction(actionPrevious);

    QAction* actionNext = new QAction("下一张", this);
    connect(actionNext, &QAction::triggered, this, &MainWindow::nextImage);
    contextMenu.addAction(actionNext);

    contextMenu.addSeparator();

    QAction* actionDeleteBox = new QAction("删除方框", this);
    connect(actionDeleteBox, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            deleteBox(clickedItem);
        });
    contextMenu.addAction(actionDeleteBox);

    contextMenu.addSeparator();

    QAction* actionGn = new QAction("G-", this);
    connect(actionGn, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
        });
    contextMenu.addAction(actionGn);

    QAction* actionGp = new QAction("G+", this);
    connect(actionGp, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
        });
    contextMenu.addAction(actionGp);

    QAction* actionBn = new QAction("B-", this);
    connect(actionBn, &QAction::triggered, this, [this, clickedItem]() {
        if (clickedItem)
            clickedItem->setFlag(QGraphicsItem::ItemIsMovable, false);
        });
    contextMenu.addAction(actionBn);

    QAction* actionBp = new QAction("B+", this);
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
    // int x = QInputDialog::getInt(this, "Crop", "X:", 0, 0, workspace->getMyImage().getImageWidth());
    // int y = QInputDialog::getInt(this, "Crop", "Y:", 0, 0, workspace->getMyImage().getImageHeight());
    // int width = QInputDialog::getInt(this, "Crop", "Width:", 100, 1, workspace->getMyImage().getImageWidth());
    // int height = QInputDialog::getInt(this, "Crop", "Height:", 100, 1, workspace->getMyImage().getImageHeight());

    // // 调用后端的 crop 函数
    // workspace->getMyImage().crop(x, y, width, height);

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
        }
        else {
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
        qDebug() << "Radius:" << radius << "Mask Weight:" << maskWeight << "Preview:" << preview;
    }
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







void MainWindow::handleScaleRequest(float factor) {
    // 检查 workspace->getMyImage() 是否为空
    if (workspace->getMyImage().isEmpty()) { // 假设 MyImage 类有一个 isEmpty() 方法
        QMessageBox::warning(this, "No Image", "No image loaded to scale.");
        return;
    }
    workspace->getMyImage().scale(factor);
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
    cv::Mat image = workspace->getMyImage().getImageMat(); // 获取当前的 cv::Mat

    // 确保图像是有效的
    if (image.empty()) {
        QMessageBox::warning(this, "Error", "No image loaded.");
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
        }
        else if (image.depth() == CV_32F) {
            // 32-bit float 归一化到 0-255
            image.convertTo(convertedImage, CV_8U, 255.0);
        }
        else {
            convertedImage = image;
        }
        format = QImage::Format_Grayscale8;
    }
    else if (image.channels() == 3) {
        // 彩色图
        if (image.depth() == CV_32F) {
            // 32-bit float 归一化到 0-255
            image.convertTo(convertedImage, CV_8U, 255.0);
        }
        else {
            convertedImage = image;
        }
        format = QImage::Format_BGR888; // 使用BGR格式
    }
    else {
        std::cerr << "Error: Unsupported image format." << std::endl;
        return;
    }

    // 确保转换后的图像数据是连续的
    if (!convertedImage.isContinuous()) {
        convertedImage = convertedImage.clone();
    }

    // 将 OpenCV 的图像转换为 QImage
    qImage = QImage((uchar*)convertedImage.data, convertedImage.cols, convertedImage.rows, convertedImage.step, format);

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
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->addItem(pixmapItem);
}



//type
void MainWindow::resetTypeActions() {
    eightBitAction->setChecked(false);
    sixteenBitAction->setChecked(false);
    thirtyTwoBitAction->setChecked(false);
    eightBitColorAction->setChecked(false);
    rgbColorAction->setChecked(false);
}


//Transform功能

// 水平翻转
void MainWindow::onFlipHorizontally() {
    workspace->getMyImage().flipHorizontally(); // 调用 MyImage 的水平翻转方法
    updateImageDisplay(); // 更新图像显示
}

// 垂直翻转
void MainWindow::onFlipVertically() {
    workspace->getMyImage().flipVertically(); // 调用 MyImage 的垂直翻转方法
    updateImageDisplay(); // 更新图像显示
}

// 顺时针旋转90度
void MainWindow::onRotate90Right() {
    workspace->getMyImage().rotateNinetyClockwise(); // 调用 MyImage 的顺时针旋转90度方法
    updateImageDisplay(); // 更新图像显示
}

// 逆时针旋转90度
void MainWindow::onRotate90Left() {
    workspace->getMyImage().rotateNinetyCounterClockwise(); // 调用 MyImage 的逆时针旋转90度方法
    updateImageDisplay(); // 更新图像显示
}

// 自定义旋转角度
void MainWindow::onRotate() {
    bool ok;
    double angle = QInputDialog::getDouble(this, tr("Rotate"),
        tr("Angle:"), 0.0, -360.0, 360.0, 2, &ok);
    if (ok) {
        workspace->getMyImage().rotate(angle); // 调用 MyImage 的旋转方法
        updateImageDisplay(); // 更新图像显示
    }
}

// 平移
void MainWindow::onTranslate() {
    bool ok;
    double x_offset = QInputDialog::getDouble(this, tr("Translate"),
        tr("X Offset:"), 0.0, -1000.0, 1000.0, 2, &ok);
    if (!ok) return;

    double y_offset = QInputDialog::getDouble(this, tr("Translate"),
        tr("Y Offset:"), 0.0, -1000.0, 1000.0, 2, &ok);
    if (ok) {
        workspace->getMyImage().translate(x_offset, y_offset); // 调用 MyImage 的平移方法
        updateImageDisplay(); // 更新图像显示
    }
}


//Adjust
void MainWindow::applyBrightnessContrast(int minimum, int maximum, double contrast, double brightness) {
    workspace->getMyImage().setBrightnessContrast(minimum, maximum, contrast, brightness);
    // 更新图像显示
    updateImageDisplay();
}


//Process
//smooth柔化
void MainWindow::onSmooth() {
    workspace->getMyImage().smooth();
    updateImageDisplay();
}

void MainWindow::onSharpen() {
    workspace->getMyImage().sharpen();
    updateImageDisplay();
}

//AI
void MainWindow::chooseModelPath() {
    // 弹出文件选择对话框，让用户选择模型文件
    QString modelFilePath = QFileDialog::getOpenFileName(this, "选择模型文件", QDir::homePath(), "Model Files (*.torchscript)");

    // 检查用户是否取消了选择
    if (modelFilePath.isEmpty()) {
        QMessageBox::warning(this, "操作取消", "未选择模型文件。");
        return;
    }

    // 调用方法处理图像并生成 JSON 文件
    runAIModel(modelFilePath.toStdString());
}

void MainWindow::runAIModel(const std::string& model_path) {
    workspace->initYoloModelProcessor(model_path);
    workspace->runYoloOnImage();
    workspace->saveToAnnotationFile();
}

