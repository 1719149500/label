#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("??????");
    setMinimumSize(800, 400);

    // ????????
    dirPath = "";
    currentImage = "";
    txtPath = "";
    index = 0;

    // ????????????
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);

    // ????????????
    listView = new QListWidget(this);
    // connect(listView, &QListWidget::currentItemChanged, this, &MainWindow::selectFromListView);

    // ????????????
    labelListView = new QListWidget(this);

    // ????????????
    saveLocationBar = new QWidget(this); // ????? saveLocationBar
    saveLocationLayout = new QVBoxLayout(saveLocationBar); // ????????

    QLabel *saveLabel = new QLabel("?????????????????????:", saveLocationBar);
    sceneImageSaveFolderEdit = new QLineEdit(saveLocationBar);
    sceneImageSaveFolderEdit->setReadOnly(true);

    QPushButton *selectFolderButton = new QPushButton("????????", saveLocationBar);
    // connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::selectSceneImageSaveFolder);

    QPushButton *saveImageButton = new QPushButton("????????", saveLocationBar);
    // connect(saveImageButton, &QPushButton::clicked, this, &MainWindow::saveSceneAsImage);

    saveLocationLayout->addWidget(saveLabel);
    saveLocationLayout->addWidget(sceneImageSaveFolderEdit);
    saveLocationLayout->addWidget(selectFolderButton);
    saveLocationLayout->addWidget(saveImageButton);



    // ???
    QSplitter *centralWidget = new QSplitter(Qt::Horizontal, this);
    centralWidget->addWidget(view);

    QSplitter *rightBar = new QSplitter(Qt::Vertical, centralWidget);
    rightBar->addWidget(saveLocationBar);
    rightBar->addWidget(listView);

    centralWidget->addWidget(rightBar);
    centralWidget->setSizes({600, 200});
    setCentralWidget(centralWidget);

    // ?????????
    QToolBar *toolbar = addToolBar("????????");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // ???????????
    QAction *openAction = new QAction(QIcon(":/IMG/icons/document-image.png"), "???", this);
    // connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    QAction *openFolderAction = new QAction(QIcon(":/IMG/icons/folder-open-image.png"), "????????", this);
    // connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    toolbar->addAction(openFolderAction);

    // Action???????
    QAction *action_previous = new QAction(QIcon(":/IMG/icons/arrow-180.png"), "?????", this);
    action_previous->setStatusTip("?????");
    action_previous->setShortcut(QKeySequence(Qt::Key_A));
    // connect(action_previous, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(action_previous);

    // Action???????
    QAction *action_next = new QAction(QIcon(":/IMG/icons/arrow.png"), "?????", this);
    action_next->setStatusTip("?????");
    action_next->setShortcut(QKeySequence(Qt::Key_D));
    // connect(action_next, &QAction::triggered, this, &MainWindow::nextImage);
    toolbar->addAction(action_next);

    // Action?????
    QAction *action_save = new QAction(QIcon(":/IMG/icons/disk.png"), "???", this);
    action_save->setStatusTip("???");
    // connect(action_save, &QAction::triggered, this, &MainWindow::saveJson);
    toolbar->addAction(action_save);

    // Action????????
    QAction *action_delete = new QAction(QIcon(":/IMG/icons/cross.png"), "??????", this);
    action_delete->setStatusTip("??????");
    // connect(action_delete, &QAction::triggered, this, &MainWindow::deleteJson);
    toolbar->addAction(action_delete);

    toolbar->addSeparator();

    // Action????????
    QAction *action_create_box = new QAction(QIcon(":/IMG/icons/layer--plus.png"), "??????", this);
    action_create_box->setStatusTip("??????");
    connect(action_create_box, &QAction::triggered, this, [this]() {
        // createBox(QRectF(150, 150, 150, 150), "G-");
    });
    toolbar->addAction(action_create_box);

    // Action????????
    QAction *action_delete_box = new QAction(QIcon(":/IMG/icons/layer--minus.png"), "??????", this);
    action_delete_box->setStatusTip("??????");
    // connect(action_delete_box, &QAction::triggered, this, &MainWindow::deleteBox);
    toolbar->addAction(action_delete_box);

    // Action??????????????????txt?????
    QAction *action_filename_to_txt = new QAction(QIcon(":/IMG/icons/address-book.png"), "?????????txt", this);
    action_filename_to_txt->setStatusTip("?????????txt");
    action_filename_to_txt->setShortcut(QKeySequence(Qt::Key_Space));
    // connect(action_filename_to_txt, &QAction::triggered, this, &MainWindow::filenameToTxt);
    toolbar->addAction(action_filename_to_txt);

    // Action: ??????????????????
    QAction *triangle_action = new QAction(QIcon(":/IMG/icons/control-090.png"), "????????", this);
    triangle_action->setCheckable(true);  // ?????????????
    // connect(triangle_action, &QAction::triggered, this, &MainWindow::toggle_triangle_mode);
    toolbar->addAction(triangle_action);

    // ?????????????????????
    QAction *save_action = new QAction(QIcon(":/IMG/icons/camera.png"), "??????????????", this);
    // connect(save_action, &QAction::triggered, this, &MainWindow::save_scene_as_image);
    toolbar->addAction(save_action);

    // ?????
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
    scene->clear(); // ????????????
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
    // ??????????????
    return shapes;
}

void MainWindow::updateLabelList()
{
    labelListView->clear();
    // ?????????
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
    QToolTip::showText(this->mapToGlobal(QPoint(0, 0)), message, this, QRect(), 1000); // 3?????????
}

// void MainWindow::openFile()
// {
//     QString imagePath = QFileDialog::getOpenFileName(this, "??????", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
//     loadImage(imagePath);
// }

// void MainWindow::openFolder()
// {
//     dirPath = QFileDialog::getExistingDirectory(this, "??????");

//     QStringList imageFilters;
//     imageFilters << "*.png" << "*.jpg";
//     QStringList jsonFilters;
//     jsonFilters << "*.json";

//     folderImages = dirPath + "/" + imageFilters.join(" ");
//     folderJsons = dirPath + "/" + jsonFilters.join(" ");

//     QStringList imageFiles = QDir(dirPath).entryList(QStringList() << "*.png" << "*.jpg");
//     QStringList jsonFiles = QDir(dirPath).entryList(QStringList() << "*.json");

//     for (const QString &image : imageFiles)
//         listView->addItem(image);

//     index = 0;
//     currentImage = imageFiles[0];
//     loadImage(currentImage);
//     listView->setCurrentRow(index);
// }

// void MainWindow::selectFromListView()
// {
//     clearScene();
//     index = listView->currentRow();
//     // imageChange();
// }

// void MainWindow::nextImage()
// {
// //     if (index + 1 >= folderImages.size())
// //         return;
// //     index++;
// //     imageChange();
// //     listView->setCurrentRow(index);
// }

// void MainWindow::previousImage()
// {
//     if (index - 1 < 0)
//         return;
//     index--;
//     imageChange();
//     listView->setCurrentRow(index);
// }

// void MainWindow::imageChange()
// {
//     autoSaveSceneAsImage();
//     clearScene();
//     currentImage = folderImages[index];
//     loadImage(currentImage);
// }

// void MainWindow::selectSceneImageSaveFolder()
// {
//     QString folder = QFileDialog::getExistingDirectory(this, "???????????");
//     if (!folder.isEmpty())
//     {
//         sceneImageSaveFolder = folder;
//         sceneImageSaveFolderEdit->setText(folder); // ???????????????
//     }
// }

// void MainWindow::saveSceneAsImage()
// {
//     QRectF sceneRect = scene->sceneRect();
//     int width = int(sceneRect.width());
//     int height = int(sceneRect.height());

//     QImage image(width, height, QImage::Format_ARGB32);
//     image.fill(Qt::white);

//     QPainter painter(&image);
//     scene->render(&painter, QRectF(0, 0, width, height), sceneRect);
//     painter.end();

//     QString filePath = sceneImageSaveFolder + "/scene_" + QFileInfo(currentImage).baseName() + ".jpg";

//     if (image.save(filePath, "JPG", 100))
//         showTooltip("???????????");
//     else
//         showTooltip("???????????");
// }

// void MainWindow::filenameToTxt()
// {
//     if (txtPath.isEmpty())
//     {
//         QMessageBox::warning(this, "???", "????????txt??????????", QMessageBox::Ok);
//         return;
//     }

//     QString filePath = txtPath;
//     QString currentFilename = QFileInfo(currentImage).baseName();

//     QFile file(filePath);
//     if (file.exists())
//     {
//         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//             return;

//         QTextStream in(&file);
//         QStringList lines = in.readAll().split("\n");

//         if (lines.contains(currentFilename))
//         {
//             lines.removeAll(currentFilename);
//             showTooltip(QString("????? '%1'").arg(currentFilename));
//         }
//         else
//         {
//             lines.append(currentFilename);
//             showTooltip(QString("????? '%1'").arg(currentFilename));
//         }

//         file.close();
//     }
//     else
//     {
//         QStringList lines;
//         lines.append(currentFilename);
//         showTooltip(QString("????????????? '%1'?????? '%1'").arg(currentFilename));
//     }

//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//         return;

//     QTextStream out(&file);
//     out << lines.join("\n");
// }

// void MainWindow::toggleTriangleMode(bool checked)
// {
//     view->setTriangleMode(checked);
//     if (checked)
//         triangleAction->setText("??????");
//     else
//         triangleAction->setText("????????");
// }

// void MainWindow::saveJson()
// {
//     LabelFile lf;
//     imageData = LabelFile::loadImageFile(currentImage);
//     QString name = QFileInfo(currentImage).baseName() + ".json";

//     QList<QRectF> shapes = saveShapes();
//     if (shapes.isEmpty())
//     {
//         QMessageBox msg(this);
//         msg.setWindowTitle("???");
//         msg.setText("???????????????????");
//         msg.exec();
//         return;
//     }

//     lf.save(name, shapes, QFileInfo(currentImage).fileName(), imageData, 640, 640);
// }

// void MainWindow::deleteJson()
// {
//     if (currentJson())
//     {
//         QMessageBox::StandardButton buttonReply = QMessageBox::question(this, "PyQt5 message", "??????" + currentJson() + "?",
//                                                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//         if (buttonReply == QMessageBox::Yes)
//         {
//             QFile::remove(currentJson());
//             qDebug() << "??????" << currentJson();
//         }
//     }
//     else
//     {
//         QMessageBox msg(this);
//         msg.setWindowTitle("???");
//         msg.setText("???????????");
//         msg.exec();
//     }
// }

// void MainWindow::onContextMenu(const QPoint &point)
// {
//     QPoint scenePos = view->mapToScene(point);

//     QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

//     QMenu contextMenu;
//     QAction *actionPrevious = new QAction("?????", this);
//     connect(actionPrevious, &QAction::triggered, this, &MainWindow::previousImage);
//     contextMenu.addAction(actionPrevious);

//     QAction *actionNext = new QAction("?????", this);
//     connect(actionNext, &QAction::triggered, this, &MainWindow::nextImage);
//     contextMenu.addAction(actionNext);

//     contextMenu.addSeparator();

//     QAction *actionDeleteBox = new QAction("??????", this);
//     connect(actionDeleteBox, &QAction::triggered, this, [this, clickedItem]() {
//         if (clickedItem)
//             deleteBox(clickedItem);
//     });
//     contextMenu.addAction(actionDeleteBox);

//     contextMenu.addSeparator();

//     QAction *actionGn = new QAction("G-", this);
//     connect(actionGn, &QAction::triggered, this, [this, clickedItem]() {
//         if (clickedItem)
//             clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
//     });
//     contextMenu.addAction(actionGn);

//     QAction *actionGp = new QAction("G+", this);
//     connect(actionGp, &QAction::triggered, this, [this, clickedItem]() {
//         if (clickedItem)
//             clickedItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
//     });
//     contextMenu.addAction(actionGp);

//     QAction *actionBn = new QAction("B-", this);
//     connect(actionBn, &QAction::triggered, this, [this, clickedItem]() {
//         if (clickedItem)
//             clickedItem->setFlag(QGraphicsItem::ItemIsMovable, false);
//     });
//     contextMenu.addAction(actionBn);

//     QAction *actionBp = new QAction("B+", this);
//     connect(actionBp, &QAction::triggered, this, [this, clickedItem]() {
//         if (clickedItem)
//             clickedItem->setFlag(QGraphicsItem::ItemIsMovable, true);
//     });
//     contextMenu.addAction(actionBp);

//     contextMenu.exec(view->mapToGlobal(point));
// }
