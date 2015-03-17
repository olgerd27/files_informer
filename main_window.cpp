#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include "file_info_list_model.h"
#include <QMessageBox>
#include <QDebug> // TODO: delete

#include "main_window.h"
#include "ui_main_window.h"
#include "file_system_custom_model.h"
#include "lines_counter.h"

#ifdef TEST_MODEL
#include "modeltest/modeltest.h"
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
        if (!model->isDir(left) && model->isDir(right))
            return false;
        else if (model->isDir(left) && !model->isDir(right))
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
    m_linesCounter = new LinesCounter(f_countLines_STL);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_FSmodel;
    delete m_dirsModel;
    delete m_dirsContentsModel;
    delete m_infoModel;
}

void MainWindow::setModels()
{
    m_FSmodel = new FileSystemCustomModel;
    m_FSmodel->setRootPath("");
    connect(m_FSmodel, SIGNAL(directoryLoaded(QString)), this, SLOT(slotDirectoryWasLoaded(QString)));

    m_dirsModel = new LeftViewProxyModel;
    m_dirsModel->setSourceModel(m_FSmodel);
    m_dirsModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_dirsModel->sort(0, Qt::AscendingOrder);

    m_dirsContentsModel = new RightViewProxyModel;
    m_dirsContentsModel->setSourceModel(m_FSmodel);
    m_dirsContentsModel->sort(0, Qt::AscendingOrder);

    // files info list model
    m_infoModel = new FileInfoListModel;
    connect(this, SIGNAL(sigUpdateFileInfo(long,long)), m_infoModel, SLOT(slotChangeFileInfo(long,long)));
    connect(this, SIGNAL(sigRightViewResetted()), m_infoModel, SLOT(slotClearFileInfo()));

#ifdef TEST_MODEL
    ModelTest *modelTest = new ModelTest(m_dirsContentsModel, this);
#endif
}

void MainWindow::setWidgets()
{
    ui = new Ui::MainWindow;
    setViews();
    setButtons();
}

void MainWindow::setViews()
{
    ui->setupUi(this);
    setLeftView();
    setRightView();
    setBottomView();
}

void MainWindow::setLeftView()
{
    DirTree *view = ui->m_leftTreeDirs;
    view->setModel(m_dirsModel);
    for (int i = 1; i < view->header()->count(); ++i)
        view->setColumnHidden(i, true); // hide columns

    connect(view, SIGNAL(clicked(QModelIndex)),
            ui->m_rightTableInfo, SLOT(clearSelection())); // NOTE: must be the first connection
    connect(view, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotSetRightViewRootIndex(QModelIndex)));
}

void MainWindow::setRightView()
{
    QTableView *view = ui->m_rightTableInfo;
    view->setModel(m_dirsContentsModel);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view->horizontalHeader()->resizeSection(0, 280); // 'Name' section. User can resize
    view->horizontalHeader()->resizeSection(3, 130); // 'Type' section

    connect(view, SIGNAL(activated(QModelIndex)),
            view, SLOT(clearSelection())); // NOTE: must be the first connection
    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotSetLeftViewCurrentIndex(QModelIndex)));
    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotActivatedOnlyDirs(QModelIndex)));
    connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slotUpdateSelection(QItemSelection,QItemSelection)));
}

void MainWindow::setBottomView()
{
    QListView *view = ui->m_bottomListSummary;
    view->setModel(m_infoModel);
}

void MainWindow::setButtons()
{
    setRadioButtons();
    connect(ui->m_cmdAbout, SIGNAL(clicked()), SLOT(slotAboutApp()));
    connect(ui->m_cmdAboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
}

void MainWindow::setRadioButtons()
{
    m_radioGroup = new QButtonGroup;
    m_radioGroup->addButton(ui->m_radioSTLCounter, counter_STL);
    m_radioGroup->addButton(ui->m_radioQtCounter, counter_Qt);
    connect(m_radioGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotSwitchCounter(int)));
}

bool MainWindow::isDir(const QModelIndex &indexProxy) const
{
    const QSortFilterProxyModel *proxyModel = 0;
    QFileSystemModel *sourceModel = 0;
    if ( !( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(indexProxy.model()) ) ||
         !( sourceModel = dynamic_cast<QFileSystemModel*>(proxyModel->sourceModel()) ) )
        return false;
    return sourceModel->isDir( proxyModel->mapToSource(indexProxy) );
}

void MainWindow::resetRightView(const QModelIndex &index)
{
    int rows = index.model()->rowCount(index);
    if (rows != 0) m_FSmodel->resizeLinesQuantity(rows);
    emit sigRightViewResetted();
}

/*
 * MainWindow slots
 */
void MainWindow::slotSetRightViewRootIndex(const QModelIndex &indexTree)
{
    QModelIndex indexSource = m_dirsModel->mapToSource(indexTree);
    QModelIndex indexProxyModel = m_dirsContentsModel->mapFromSource(indexSource);
    ui->m_rightTableInfo->setRootIndex(indexProxyModel);
    resetRightView(indexSource);
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
    QAbstractItemView *view = 0;
    if (isDir(index) && (view = dynamic_cast<QAbstractItemView *>(sender()) ) )
        view->setRootIndex(index);
    resetRightView(index);
}

void MainWindow::slotDirectoryWasLoaded(const QString &dir)
{
    resetRightView( m_FSmodel->index(dir) );
}

void MainWindow::slotUpdateSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
//    qDebug() << "\n***** selection changed *****";
    const QSortFilterProxyModel *proxyModel = 0;
    FileSystemCustomModel *sourceModel = 0;
    QModelIndex index, indexSource;
    QModelIndexList indexes;

//    qDebug() << "   selected items:";
    t_linesQnty lines = 0;
    indexes = selected.indexes();
    foreach (index, indexes) {
        if (index.column() == FileSystemCustomModel::rhh_Name) {
            if ( !( ( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) &&
                    ( sourceModel = dynamic_cast<FileSystemCustomModel*>(proxyModel->sourceModel()) ) ) )
                continue;

            indexSource = proxyModel->mapToSource(index);
            if ( !sourceModel->canCalcLines(indexSource) ) // check can whether count lines of this file
                continue;

            // set the lines quantity value of the selected file
            try {
                m_linesCounter->setFileName( sourceModel->filePath(indexSource).toStdString() );
                lines = m_linesCounter->countLines();
                sourceModel->setLinesQuantity( indexSource.row(), lines );
//                qDebug() << "row =" << indexSource.row() << ":" << sourceModel->filePath(indexSource)
//                         << ", lines =" << sourceModel->getLinesQuantity(indexSource.row());
            }
            catch(std::exception &ex) {
                QMessageBox::warning( this, tr("Error show file lines"), tr(ex.what()) );
            }
            emit sigUpdateFileInfo(sourceModel->size(indexSource), lines);
        }
    }

//    QMessageBox::information(this, "Debug", "Stop");

//    qDebug() << "   deselected items:";
    indexes = deselected.indexes();
    foreach (index, indexes) {
        if (index.column() == FileSystemCustomModel::rhh_Name) {
            if ( !( ( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) &&
                    ( sourceModel = dynamic_cast<FileSystemCustomModel*>(proxyModel->sourceModel()) ) ) )
                continue;

            indexSource = proxyModel->mapToSource(index);
            // checking for deselecting only files, that was selected in past
            if ( !sourceModel->canCalcLines(indexSource) ) // check can whether count lines of this file
                continue;

            /*
             * NOTE: update file info.
             * Must placed before clearing the lines quantity value of the deselected file
             */
            emit sigUpdateFileInfo( sourceModel->size( indexSource ) * (-1),
                                    sourceModel->data( sourceModel->index(indexSource.row(),
                                                                          FileSystemCustomModel::rhh_Lines,
                                                                          indexSource.parent()),
                                                       Qt::DisplayRole).toLongLong() * (-1) );
            // clearing the lines quantity value of the deselected file
            try {
                sourceModel->setLinesQuantity( indexSource.row(), FileSystemCustomModel::empty_value );
            }
            catch(std::exception &ex) {
                QMessageBox::warning( this, tr("Error hide file lines"), tr(ex.what()) );
            }
        }
    }
//    qDebug() << "***** selection END *****";
}

void MainWindow::slotSwitchCounter(int id)
{
    /*
     * Switch between count lines function in a strategy LinesCounter class
     */
    LinesCounter::T_funcLinesCount func = f_countLines_STL;
    switch (id) {
    case counter_STL:
        func = f_countLines_STL;
        break;
    case counter_Qt:
        func = f_countLines_Qt;
        break;
    }
    m_linesCounter->setFuncLinesCount(func);
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
            "Mykolayiv, Ukraine - 2014-2015.";
    QMessageBox::about(this, tr(title.toStdString().c_str()), tr(text.toStdString().c_str()));
}
