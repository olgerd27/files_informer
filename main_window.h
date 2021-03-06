#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

class FileSystemCustomModel;
class LeftViewProxyModel;
class RightViewProxyModel;
class FileInfoListModel;
class LinesCounter;
class QModelIndex;
class QItemSelection;
class QButtonGroup;

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
    enum countFunctions {
        counter_STL,
        counter_Qt
    };

    void setModels();
    void setWidgets();
    void setViews();
    void setLeftView();
    void setRightView();
    void setBottomView();
    void setButtons();
    void setRadioButtons();

    bool isDir(const QModelIndex &indexProxy) const;
    void resetRightView(const QModelIndex &index);

    Ui::MainWindow *ui;
    FileSystemCustomModel *m_FSmodel;
    LeftViewProxyModel *m_dirsModel;
    RightViewProxyModel *m_dirsContentsModel;
    FileInfoListModel *m_infoModel;
    LinesCounter *m_linesCounter;
    QButtonGroup *m_radioGroup;

private slots:
    void slotSetRightViewRootIndex(const QModelIndex &indexTree);
    void slotSetLeftViewCurrentIndex(const QModelIndex &indexList);
    void slotActivatedOnlyDirs(const QModelIndex &index);
    void slotDirectoryWasLoaded(const QString &dir);
    void slotUpdateSelection(const QItemSelection &selected, const QItemSelection &deselected);
    void slotSwitchCounter(int id);
    void slotAboutApp();

signals:
    void sigUpdateFileInfo(long size, long lines);
    void sigRightViewResetted();
};

#endif // MAIN_WINDOW_H
