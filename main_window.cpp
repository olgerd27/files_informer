#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug> // TODO: delete

#include "main_window.h"
#include "ui_main_window.h"

/*
 * Customs sort filter proxy models
 */
class FilterDirsProxyModel : public QSortFilterProxyModel
{
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        QFileSystemModel *model = dynamic_cast<QFileSystemModel*>(sourceModel());
        QModelIndex index = model->index(source_row, 0, source_parent);
        return model->isDir(index) ? true : false;
    }
};

class SortFilesDirsProxyModel : public QSortFilterProxyModel
{
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const
    {
        QFileSystemModel *model = dynamic_cast<QFileSystemModel*>(sourceModel());
        if(!model->isDir(left) && model->isDir(right))
            return false;
        else if(model->isDir(left) && !model->isDir(right))
            return true;
        else
            return left.data().toString().toLower() < right.data().toString().toLower(); // for files and dirs
    }
};

/*
 * MainWindow methods
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setModels();
    setWidgets();
    setWindowIcon(QIcon(":/images/window_icon.png"));
}

void MainWindow::setModels()
{
    m_FSmodel = new QFileSystemModel;
    m_FSmodel->setRootPath(QDir::currentPath());

    m_dirsModel = new FilterDirsProxyModel;
    m_dirsModel->setSourceModel(m_FSmodel);
    m_dirsModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_dirsModel->sort(0, Qt::AscendingOrder);

    m_dirsContentsModel = new SortFilesDirsProxyModel;
    m_dirsContentsModel->setSourceModel(m_FSmodel);
    m_dirsContentsModel->sort(0, Qt::AscendingOrder);
}

void MainWindow::setWidgets()
{
    ui = new Ui::MainWindow;
    setDirsViews();
    setButtons();
}

void MainWindow::setDirsViews()
{
    ui->setupUi(this);

    ui->m_dirsTree->setModel(m_dirsModel);
    ui->m_dirsContentsList->setModel(m_dirsContentsModel);

    // hide columns
    for(int i = 1; i < ui->m_dirsTree->header()->count(); ++i)
        ui->m_dirsTree->setColumnHidden(i, true);

    connect(ui->m_dirsTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotSetListRootIndex(QModelIndex)));
    connect(ui->m_dirsContentsList, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotSetTreeCurrentIndex(QModelIndex)));
    connect(ui->m_dirsContentsList, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotActivatedOnlyDirs(QModelIndex)));
}

void MainWindow::setButtons()
{
    connect(ui->m_cmdShowInfo, SIGNAL(clicked()), SLOT(slotGetFileInfo()));
    connect(ui->m_cmdAbout, SIGNAL(clicked()), SLOT(slotAboutApp()));
    connect(ui->m_cmdAboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_FSmodel;
    delete m_dirsModel;
    delete m_dirsContentsModel;
}

/*
 * MainWindow slots
 */
void MainWindow::slotSetListRootIndex(const QModelIndex &indexTree)
{
    QModelIndex indexSource = m_dirsModel->mapToSource(indexTree);
    QModelIndex indexProxyModel = m_dirsContentsModel->mapFromSource(indexSource);
    ui->m_dirsContentsList->setRootIndex(indexProxyModel);
}

void MainWindow::slotSetTreeCurrentIndex(const QModelIndex &indexList)
{
    QModelIndex indexSource = m_dirsContentsModel->mapToSource(indexList);
    QModelIndex indexProxyModel = m_dirsModel->mapFromSource(indexSource);
    ui->m_dirsTree->setCurrentIndex(indexProxyModel);
}

/*
 * This slot uses for setting root index in a current view only if a dir was activated.
 * If there are trying to activate a file, nothing will be done.
 */
void MainWindow::slotActivatedOnlyDirs(const QModelIndex &index)
{
    const QSortFilterProxyModel *model = dynamic_cast<const QSortFilterProxyModel*>(index.model());
    QModelIndex indexSource = model->mapToSource(index);
    if( QFileSystemModel *sourceModel = dynamic_cast<QFileSystemModel *>(model->sourceModel()) )
        if(sourceModel->isDir(indexSource))
            if( QAbstractItemView *view = dynamic_cast<QAbstractItemView *>(sender()) )
                view->setRootIndex(index);
}

void MainWindow::slotGetFileInfo()
{
    qDebug() << "file info";
}

void MainWindow::slotAboutApp()
{
    QString title = QString("About ") + windowTitle();
    QString text = QString("The <b>") + windowTitle() + "</b> application.<br>"
                   "<b>Version 1.0</b> (freeware).<br><br>"
                   "The programm is provided \"AS IS\" with no warranty of any kind,"
                   "including the warranty of defign, merchantability and"
                   "fitness for a particular purpose.<br><br>"
                   "Copyright: (C) M.O.I., e-mail: olgerd27@gmail.com.<br>"
                   "Mykolayiv, Ukraine - 2014.";
    QMessageBox::about(this, tr(title.toStdString().c_str()), tr(text.toStdString().c_str()));
}
