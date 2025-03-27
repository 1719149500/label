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

    setWindowTitle("鏁版嵁鏍囨敞");
    setMinimumSize(800, 400);

    // 鍒濆鍖栧彉閲�
    dirPath = "";
    currentImage = "";
    txtPath = "";
    index = 0;

    // 鍒濆鍖栧満鏅拰瑙嗗浘
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);

    // 鍒濆鍖栧彸渚у垪琛ㄦ爮
    listView = new QListWidget(this);
    // connect(listView, &QListWidget::currentItemChanged, this, &MainWindow::selectFromListView);

    // 鍒濆鍖栨爣绛惧垪琛ㄦ爮
    labelListView = new QListWidget(this);

    // 鍒濆鍖栦繚瀛樿矾寰勬爮
    saveLocationBar = new QWidget(this); // 鍒濆鍖� saveLocationBar
    saveLocationLayout = new QVBoxLayout(saveLocationBar); // 鍒濆鍖栧竷灞€

    QLabel *saveLabel = new QLabel("鐢ㄦ潵淇濆瓨褰撳墠瑙嗗浘鍥惧儚鐨勬枃浠跺す:", saveLocationBar);
    sceneImageSaveFolderEdit = new QLineEdit(saveLocationBar);
    sceneImageSaveFolderEdit->setReadOnly(true);

    QPushButton *selectFolderButton = new QPushButton("閫夋嫨鏂囦欢澶�", saveLocationBar);
    // connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::selectSceneImageSaveFolder);

    QPushButton *saveImageButton = new QPushButton("淇濆瓨涓哄浘鐗�", saveLocationBar);
    // connect(saveImageButton, &QPushButton::clicked, this, &MainWindow::saveSceneAsImage);

    saveLocationLayout->addWidget(saveLabel);
    saveLocationLayout->addWidget(sceneImageSaveFolderEdit);
    saveLocationLayout->addWidget(selectFolderButton);
    saveLocationLayout->addWidget(saveImageButton);



    // 甯冨眬
    QSplitter *centralWidget = new QSplitter(Qt::Horizontal, this);
    centralWidget->addWidget(view);

    QSplitter *rightBar = new QSplitter(Qt::Vertical, centralWidget);
    rightBar->addWidget(saveLocationBar);
    rightBar->addWidget(listView);

    centralWidget->addWidget(rightBar);
    centralWidget->setSizes({600, 200});
    setCentralWidget(centralWidget);

    // 鍒濆鍖栧伐鍏锋爮
    QToolBar *toolbar = addToolBar("鎴戠殑宸ュ叿鏍�");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 娣诲姞宸ュ叿鏍忔寜閽�
    QAction *openAction = new QAction(QIcon(":/IMG/icons/document-image.png"), "鎵撳紑", this);
    // connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    QAction *openFolderAction = new QAction(QIcon(":/IMG/icons/folder-open-image.png"), "鎵撳紑鏂囦欢澶�", this);
    // connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    toolbar->addAction(openFolderAction);

    // Action锛氫笂涓€寮�
    QAction *action_previous = new QAction(QIcon(":/IMG/icons/arrow-180.png"), "涓婁竴寮�", this);
    action_previous->setStatusTip("涓婁竴寮�");
    action_previous->setShortcut(QKeySequence(Qt::Key_A));
    // connect(action_previous, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(action_previous);

    // Action锛氫笅涓€寮�
    QAction *action_next = new QAction(QIcon(":/IMG/icons/arrow.png"), "涓嬩竴寮�", this);
    action_next->setStatusTip("涓嬩竴寮�");
    action_next->setShortcut(QKeySequence(Qt::Key_D));
    // connect(action_next, &QAction::triggered, this, &MainWindow::nextImage);
    toolbar->addAction(action_next);

    // Action锛氫繚瀛�
    QAction *action_save = new QAction(QIcon(":/IMG/icons/disk.png"), "淇濆瓨", this);
    action_save->setStatusTip("淇濆瓨");
    // connect(action_save, &QAction::triggered, this, &MainWindow::saveJson);
    toolbar->addAction(action_save);

    // Action锛氬垹闄ゆ枃浠�
    QAction *action_delete = new QAction(QIcon(":/IMG/icons/cross.png"), "鍒犻櫎鏂囦欢", this);
    action_delete->setStatusTip("鍒犻櫎鏂囦欢");
    // connect(action_delete, &QAction::triggered, this, &MainWindow::deleteJson);
    toolbar->addAction(action_delete);

    toolbar->addSeparator();

    // Action锛氬垱寤烘柟妗�
    QAction *action_create_box = new QAction(QIcon(":/IMG/icons/layer--plus.png"), "鍒涘缓鏂规", this);
    action_create_box->setStatusTip("鍒涘缓鏂规");
    connect(action_create_box, &QAction::triggered, this, [this]() {
        // createBox(QRectF(150, 150, 150, 150), "G-");
    });
    toolbar->addAction(action_create_box);

    // Action锛氬垹闄ゆ柟妗�
    QAction *action_delete_box = new QAction(QIcon(":/IMG/icons/layer--minus.png"), "鍒犻櫎鏂规", this);
    action_delete_box->setStatusTip("鍒犻櫎鏂规");
    // connect(action_delete_box, &QAction::triggered, this, &MainWindow::deleteBox);
    toolbar->addAction(action_delete_box);

    // Action锛氬皢褰撳墠鍥剧墖鏂囦欢鍚嶄繚瀛樿嚦txt鏂囦欢涓�
    QAction *action_filename_to_txt = new QAction(QIcon(":/IMG/icons/address-book.png"), "鏂囦欢鍚嶄繚瀛樿嚦txt", this);
    action_filename_to_txt->setStatusTip("鏂囦欢鍚嶄繚瀛樿嚦txt");
    action_filename_to_txt->setShortcut(QKeySequence(Qt::Key_Space));
    // connect(action_filename_to_txt, &QAction::triggered, this, &MainWindow::filenameToTxt);
    toolbar->addAction(action_filename_to_txt);

    // Action: 娣诲姞鎸夐挳锛氭帶鍒剁敓鎴愪笁瑙掑舰
    QAction *triangle_action = new QAction(QIcon(":/IMG/icons/control-090.png"), "鐢熸垚涓夎褰�", this);
    triangle_action->setCheckable(true);  // 璁剧疆涓哄彲鍒囨崲鐘舵€�
    // connect(triangle_action, &QAction::triggered, this, &MainWindow::toggle_triangle_mode);
    toolbar->addAction(triangle_action);

    // 娣诲姞淇濆瓨褰撳墠瑙嗗浘涓哄浘鐗囩殑鎸夐挳
    QAction *save_action = new QAction(QIcon(":/IMG/icons/camera.png"), "淇濆瓨褰撳墠瑙嗗浘涓哄浘鐗�", this);
    // connect(save_action, &QAction::triggered, this, &MainWindow::save_scene_as_image);
    toolbar->addAction(save_action);

    // 鐘舵€佹爮
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
    scene->clear(); // 娓呴櫎鍦烘櫙涓殑鍐呭
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
    // 鑾峰彇鏍囨敞鏂规鐨勫潗鏍�
    return shapes;
}

void MainWindow::updateLabelList()
{
    labelListView->clear();
    // 鏇存柊鏍囩鍒楄〃
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
    QToolTip::showText(this->mapToGlobal(QPoint(0, 0)), message, this, QRect(), 1000); // 3绉掑悗鑷姩闅愯棌
}

// void MainWindow::openFile()
// {
//     QString imagePath = QFileDialog::getOpenFileName(this, "鎵撳紑鍥剧墖", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
//     loadImage(imagePath);
// }

// void MainWindow::openFolder()
// {
//     dirPath = QFileDialog::getExistingDirectory(this, "鎵撳紑鐩綍");

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
//     QString folder = QFileDialog::getExistingDirectory(this, "閫夋嫨淇濆瓨鏂囦欢澶�");
//     if (!folder.isEmpty())
//     {
//         sceneImageSaveFolder = folder;
//         sceneImageSaveFolderEdit->setText(folder); // 鏄剧ず閫夋嫨鐨勬枃浠跺す璺緞
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
//         showTooltip("鍥剧墖淇濆瓨鎴愬姛锛�");
//     else
//         showTooltip("鍥剧墖淇濆瓨澶辫触锛�");
// }

// void MainWindow::filenameToTxt()
// {
//     if (txtPath.isEmpty())
//     {
//         QMessageBox::warning(this, "璀﹀憡", "鏈€夋嫨淇濆瓨txt鏂囦欢鐨勮矾寰勩€�", QMessageBox::Ok);
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
//             showTooltip(QString("宸茬Щ闄� '%1'").arg(currentFilename));
//         }
//         else
//         {
//             lines.append(currentFilename);
//             showTooltip(QString("宸叉坊鍔� '%1'").arg(currentFilename));
//         }

//         file.close();
//     }
//     else
//     {
//         QStringList lines;
//         lines.append(currentFilename);
//         showTooltip(QString("鏂囦欢涓嶅瓨鍦ㄣ€傛柊寤� '%1'锛屽苟娣诲姞 '%1'").arg(currentFilename));
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
//         triangleAction->setText("鍋滄鐢熸垚");
//     else
//         triangleAction->setText("鐢熸垚涓夎褰�");
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
//         msg.setWindowTitle("鎻愮ず");
//         msg.setText("褰撳墠鍥剧墖娌℃湁鏍囨敞鍙繚瀛樸€�");
//         msg.exec();
//         return;
//     }

//     lf.save(name, shapes, QFileInfo(currentImage).fileName(), imageData, 640, 640);
// }

// void MainWindow::deleteJson()
// {
//     if (currentJson())
//     {
//         QMessageBox::StandardButton buttonReply = QMessageBox::question(this, "PyQt5 message", "纭鍒犻櫎" + currentJson() + "?",
//                                                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//         if (buttonReply == QMessageBox::Yes)
//         {
//             QFile::remove(currentJson());
//             qDebug() << "鎴愬姛鍒犻櫎" << currentJson();
//         }
//     }
//     else
//     {
//         QMessageBox msg(this);
//         msg.setWindowTitle("鎻愮ず");
//         msg.setText("鏃犳枃浠跺彲鍒犻櫎銆�");
//         msg.exec();
//     }
// }

// void MainWindow::onContextMenu(const QPoint &point)
// {
//     QPoint scenePos = view->mapToScene(point);

//     QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

//     QMenu contextMenu;
//     QAction *actionPrevious = new QAction("涓婁竴寮�", this);
//     connect(actionPrevious, &QAction::triggered, this, &MainWindow::previousImage);
//     contextMenu.addAction(actionPrevious);

//     QAction *actionNext = new QAction("涓嬩竴寮�", this);
//     connect(actionNext, &QAction::triggered, this, &MainWindow::nextImage);
//     contextMenu.addAction(actionNext);

//     contextMenu.addSeparator();

//     QAction *actionDeleteBox = new QAction("鍒犻櫎鏂规", this);
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
