#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

class QFileSystemModel;
class FilterDirsProxyModel;
class SortFilesDirsProxyModel;
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
    void setDirsViews();
    void setButtons();

    Ui::MainWindow *ui;
    QFileSystemModel *m_FSmodel;
    FilterDirsProxyModel *m_dirsModel;
    SortFilesDirsProxyModel *m_dirsContentsModel;

private slots:
    void slotSetListRootIndex(const QModelIndex &indexTree);
    void slotSetTreeCurrentIndex(const QModelIndex &indexList);
    void slotActivatedOnlyDirs(const QModelIndex &index);
    void slotGetFileInfo();
};

#endif // MAIN_WINDOW_H
