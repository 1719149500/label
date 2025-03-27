#ifndef IMAGETOOLS_H
#define IMAGETOOLS_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QStatusBar>

class ImageTools : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageTools(QWidget *parent = nullptr);
    ~ImageTools();

private slots:


private:
    void setupToolBar();
    void setupMenuBar(); // 新增的菜单栏设置函数
};

#endif // IMAGETOOLS_H
