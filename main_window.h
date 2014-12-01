#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

class FileSystemCustomModel;
class LeftViewProxyModel;
class RightViewProxyModel;
class QModelIndex;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setModels();
    void setWidgets();
    void setViews();
    void setLeftView();
    void setRightView();
    void setButtons();

    Ui::MainWindow *ui;
    FileSystemCustomModel *m_FSmodel;
    LeftViewProxyModel *m_dirsModel;
    RightViewProxyModel *m_dirsContentsModel;

private slots:
    void slotSetRightViewRootIndex(const QModelIndex &indexTree);
    void slotSetLeftViewCurrentIndex(const QModelIndex &indexList);
    void slotActivatedOnlyDirs(const QModelIndex &index);
    void slotGetFileInfo();
    void slotAboutApp();
};

#endif // MAIN_WINDOW_H
