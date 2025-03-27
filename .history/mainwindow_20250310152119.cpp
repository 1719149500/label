#include "mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{


    setWindowTitle("数据标注");
    setMinimumSize(800, 600);

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
    // view->setStyleSheet("background: transparent;");
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio); // 自动调整视图以适应内容
    // 初始化右侧列表栏
    listView = new QListWidget(this);
    connect(listView, &QListWidget::currentItemChanged, this, &MainWindow::selectFromListView);

    // 初始化标签列表栏
    labelListView = new QListWidget(this);

    // 初始化保存路径栏
    saveLocationBar = new QWidget(this); // 初始化 saveLocationBar
    saveLocationLayout = new QVBoxLayout(saveLocationBar); // 初始化布局

    QLabel *saveLabel = new QLabel("用来保存当前视图图像的文件夹:", saveLocationBar);
    sceneImageSaveFolderEdit = new QLineEdit(saveLocationBar);
    sceneImageSaveFolderEdit->setReadOnly(true);

    QPushButton *selectFolderButton = new QPushButton("选择文件夹", saveLocationBar);
    connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::selectSceneImageSaveFolder);

    QPushButton *saveImageButton = new QPushButton("保存为图片", saveLocationBar);
    connect(saveImageButton, &QPushButton::clicked, this, &MainWindow::saveSceneAsImage);

    QPushButton *selectSaveFilenameButton = new QPushButton("保存文件名到TXT", saveLocationBar);
    connect(selectSaveFilenameButton, &QPushButton::clicked, this, &MainWindow::selectSaveFilenameLocation);

    saveLocationLayout->addWidget(saveLabel);
    saveLocationLayout->addWidget(sceneImageSaveFolderEdit);
    saveLocationLayout->addWidget(selectFolderButton);
    saveLocationLayout->addWidget(saveImageButton);
    saveLocationLayout->addWidget(selectSaveFilenameButton);

    // 布局
    QSplitter *centralWidget = new QSplitter(Qt::Horizontal, this);
    centralWidget->addWidget(view);

    QSplitter *rightBar = new QSplitter(Qt::Vertical, centralWidget);
    rightBar->addWidget(saveLocationBar);
    rightBar->addWidget(listView);

    centralWidget->addWidget(rightBar);
    centralWidget->setSizes({500, 200});
    setCentralWidget(centralWidget);

    // 初始化工具栏
    QToolBar *toolbar = addToolBar("我的工具栏");
    toolbar->setIconSize(QSize(28,28));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 添加工具栏按钮
    QAction *openAction = new QAction(QIcon(":/IMG/icons/document-image.png"), "打开", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    QAction *openFolderAction = new QAction(QIcon(":/IMG/icons/folder-open-image.png"), "打开文件夹", this);
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    toolbar->addAction(openFolderAction);

    // Action：上一张
    QAction *action_previous = new QAction(QIcon(":/IMG/icons/arrow-180.png"), "上一张", this);
    action_previous->setStatusTip("上一张");
    action_previous->setShortcut(QKeySequence(Qt::Key_A));
    connect(action_previous, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(action_previous);

    // Action：下一张
    QAction *action_next = new QAction(QIcon(":/IMG/icons/arrow.png"), "下一张", this);
    action_next->setStatusTip("下一张");
    action_next->setShortcut(QKeySequence(Qt::Key_D));
    connect(action_next, &QAction::triggered, this, &MainWindow::nextImage);
    toolbar->addAction(action_next);

    // Action：保存
    QAction *action_save = new QAction(QIcon(":/IMG/icons/disk.png"), "保存", this);
    action_save->setStatusTip("保存");
    connect(action_save, &QAction::triggered, this, &MainWindow::saveJson);
    toolbar->addAction(action_save);

    // Action：删除文件
    QAction *action_delete = new QAction(QIcon(":/IMG/icons/cross.png"), "删除文件", this);
    action_delete->setStatusTip("删除文件");
    connect(action_delete, &QAction::triggered, this, &MainWindow::deleteJson);
    toolbar->addAction(action_delete);

    toolbar->addSeparator();

    // Action：创建方框
    QAction *action_create_box = new QAction(QIcon(":/IMG/icons/layer--plus.png"), "创建方框", this);
    action_create_box->setStatusTip("创建方框");
    connect(action_create_box, &QAction::triggered, this, [this]() {
        createBox(QRectF(150, 150, 150, 150), "G-");
    });
    toolbar->addAction(action_create_box);

    // Action：删除方框
    QAction *action_delete_box = new QAction(QIcon(":/IMG/icons/layer--minus.png"), "删除方框", this);
    action_delete_box->setStatusTip("删除方框");
    // connect(action_delete_box, &QAction::triggered, this, &MainWindow::deleteBox);
    toolbar->addAction(action_delete_box);

    // Action：将当前图片文件名保存至txt文件中
    QAction *action_filename_to_txt = new QAction(QIcon(":/IMG/icons/address-book.png"), "文件名保存至txt", this);
    action_filename_to_txt->setStatusTip("文件名保存至txt");
    action_filename_to_txt->setShortcut(QKeySequence(Qt::Key_Space));
    connect(action_filename_to_txt, &QAction::triggered, this, &MainWindow::filenameToTxt);
    toolbar->addAction(action_filename_to_txt);

    // Action: 添加按钮：控制生成三角形
    QAction *triangle_action = new QAction(QIcon(":/IMG/icons/control-090.png"), "生成三角形", this);
    triangle_action->setCheckable(true);  // 设置为可切换状态
    connect(triangle_action, &QAction::triggered, this, &MainWindow::toggleTriangleMode);
    toolbar->addAction(triangle_action);

    // 添加保存当前视图为图片的按钮
    QAction *save_action = new QAction(QIcon(":/IMG/icons/camera.png"), "保存当前视图为图片", this);
    connect(save_action, &QAction::triggered, this, &MainWindow::saveSceneAsImage);
    toolbar->addAction(save_action);


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


    // 状态栏
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->clear(); // 清除场景中的内容
    scene->addItem(pixmapItem);
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
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
    loadImage(imagePath);
}

void MainWindow::openFolder()
{
    dirPath = QFileDialog::getExistingDirectory(this, "打开目录");

    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg";
    QStringList jsonFilters;
    jsonFilters << "*.json";

    folderImages = dirPath + "/" + imageFilters.join(" ");
    folderJsons = dirPath + "/" + jsonFilters.join(" ");

    QStringList imageFiles = QDir(dirPath).entryList(QStringList() << "*.png" << "*.jpg");
    QStringList jsonFiles = QDir(dirPath).entryList(QStringList() << "*.json");

    for (const QString &image : imageFiles)
        listView->addItem(image);

    index = 0;
    currentImage = imageFiles[0];
    loadImage(currentImage);
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
    autoSaveSceneAsImage();
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
    // 这里是方法的实现
    // 如果您还没有实现细节，可以先简单地打印一条消息或返回
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
