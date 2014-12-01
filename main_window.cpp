#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug> // TODO: delete

#include "main_window.h"
#include "ui_main_window.h"
#include "file_system_custom_model.h"

#ifdef TXP_DBGOUT_QT
#include "modeltest/modeltest.h" // NOTE: for testing
#endif

/*
 * Customs sort filter proxy models
 */
class LeftViewProxyModel : public QSortFilterProxyModel
{
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        QFileSystemModel *model = dynamic_cast<QFileSystemModel*>(sourceModel());
        QModelIndex index = model->index(source_row, 0, source_parent);
        return model->isDir(index) ? true : false;
    }
};

class RightViewProxyModel : public QSortFilterProxyModel
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
    m_FSmodel = new FileSystemCustomModel;
    m_FSmodel->setRootPath(QDir::currentPath());

    m_dirsModel = new LeftViewProxyModel;
    m_dirsModel->setSourceModel(m_FSmodel);
    m_dirsModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_dirsModel->sort(0, Qt::AscendingOrder);

    m_dirsContentsModel = new RightViewProxyModel;
    m_dirsContentsModel->setSourceModel(m_FSmodel);
    m_dirsContentsModel->sort(0, Qt::AscendingOrder);

#ifdef TEST_MODEL
    ModelTest *modelTest = new ModelTest(m_dirsContentsModel, this);
#endif
}

void MainWindow::setWidgets()
{
    ui = new Ui::MainWindow;
    setViews();
    setButtons();
    //    ui->m_rightTableInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    //    resize(minimumSize());
    //    ui->m_rightTableInfo->adjustSize();
}

void MainWindow::setViews()
{
    ui->setupUi(this);
    setLeftView();
    setRightView();
}

void MainWindow::setLeftView()
{
    DirTree *view = ui->m_leftTreeDirs;
    view->setModel(m_dirsModel);
    for(int i = 1; i < ui->m_leftTreeDirs->header()->count(); ++i)
        view->setColumnHidden(i, true); // hide columns

    connect(view, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotSetRightViewRootIndex(QModelIndex)));
}

void MainWindow::setRightView()
{
    QTableView *view = ui->m_rightTableInfo;
    view->setModel(m_dirsContentsModel);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // user cannot resize
    view->horizontalHeader()->resizeSection(0, 280); // user can resize
    //    view->resizeRowsToContents();
    //    view->horizontalHeader()->setSectionResizeMode(view->horizontalHeader()->count() - 1, QHeaderView::ResizeToContents);

    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotSetLeftViewCurrentIndex(QModelIndex)));
    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotActivatedOnlyDirs(QModelIndex)));
}

void MainWindow::setButtons()
{
    connect(ui->m_cmdShowSummInfo, SIGNAL(clicked()), SLOT(slotGetFileInfo()));
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
void MainWindow::slotSetRightViewRootIndex(const QModelIndex &indexTree)
{
    QModelIndex indexSource = m_dirsModel->mapToSource(indexTree);
    QModelIndex indexProxyModel = m_dirsContentsModel->mapFromSource(indexSource);
    ui->m_rightTableInfo->setRootIndex(indexProxyModel);

    // my proba
    //    ui->m_rightTableInfo->model()->insertColumn(2, ui->m_rightTableInfo->model()->index(0, 2, indexProxyModel));
    //    qDebug() << "count =" << ui->m_rightTableInfo->horizontalHeader()->count();

    // from the inet - adding check box in a separated, first column
    //    model->setData(index, Qt::Checked, Qt::CheckStateRole);
    //    model->setData(index, "any string", Qt::DisplayRole);
}

void MainWindow::slotSetLeftViewCurrentIndex(const QModelIndex &indexList)
{
    QModelIndex indexSource = m_dirsContentsModel->mapToSource(indexList);
    QModelIndex indexProxyModel = m_dirsModel->mapFromSource(indexSource);
    ui->m_leftTreeDirs->setCurrentIndex(indexProxyModel);
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
    qDebug() << "summary file info";
}

void MainWindow::slotAboutApp()
{
    QString title = QString("About ") + windowTitle();
    QString text = QString("The <b>") + windowTitle() + "</b> application.<br>"
            "<b>Version 1.0</b> (freeware).<br><br>"
            "The programm is provided \"AS IS\" with no warranty of any kind, "
            "including the warranty of design, merchantability and "
            "fitness for a particular purpose.<br><br>"
            "Copyright: (C) M.O.I., E-mail: olgerd27@gmail.com.<br>"
            "Mykolayiv, Ukraine - 2014.";
    QMessageBox::about(this, tr(title.toStdString().c_str()), tr(text.toStdString().c_str()));
}
