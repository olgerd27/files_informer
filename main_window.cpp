#include <QFileSystemModel>
#include <QSortFilterProxyModel>
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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_FSmodel;
    delete m_dirsModel;
    delete m_dirsContentsModel;
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
    for (int i = 1; i < ui->m_leftTreeDirs->header()->count(); ++i)
        view->setColumnHidden(i, true); // hide columns

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
            this, SLOT(slotSetLeftViewCurrentIndex(QModelIndex)));
    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotActivatedOnlyDirs(QModelIndex)));
    connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slotUpdateSelection(QItemSelection,QItemSelection)));
//    connect(view, SIGNAL(pressed(QModelIndex)),
//            this, SLOT(slotGetFileInfo(QModelIndex)));
}

void MainWindow::setButtons()
{
//    connect(ui->m_cmdShowSummInfo, SIGNAL(clicked()), SLOT(slotGetFileInfo())); // NOTE: maybe this is not need else
    connect(ui->m_cmdAbout, SIGNAL(clicked()), SLOT(slotAboutApp()));
    connect(ui->m_cmdAboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
}

bool MainWindow::isDir(const QModelIndex &index) const
{
    // TODO: make simple - use the proxy and source model pointer, stored in MainWindow, directly
    bool isdir = false;
    const QSortFilterProxyModel *proxyModel = 0;
    QFileSystemModel *sourceModel = 0;
    if ( !( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) ||
        !( sourceModel = dynamic_cast<QFileSystemModel *>(proxyModel->sourceModel()) ) )
        isdir = false;
    else
        isdir = sourceModel->isDir( proxyModel->mapToSource(index) );
    return isdir;
}

/*
 * MainWindow slots
 */
void MainWindow::slotSetRightViewRootIndex(const QModelIndex &indexTree)
{
    QModelIndex indexSource = m_dirsModel->mapToSource(indexTree);
    QModelIndex indexProxyModel = m_dirsContentsModel->mapFromSource(indexSource);
    ui->m_rightTableInfo->setRootIndex(indexProxyModel);

//    qDebug() << "ROWS4 =" << indexSource.model()->rowCount(indexSource);
//    qDebug() << "ROWS5 =" << indexProxyModel.model()->rowCount(indexProxyModel);
//    qDebug() << "ROWS6 =" << m_dirsContentsModel->rowCount(indexProxyModel);

//    m_FSmodel->resizeLinesQuantity( indexSource.model()->rowCount(indexSource) );
//    m_FSmodel->resizeLinesQuantity(10);

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
    QAbstractItemView *view = 0;
    if (isDir(index) && (view = dynamic_cast<QAbstractItemView *>(sender()) ) )
        view->setRootIndex(index);
//    qDebug() << "ROWS1 =" << view->model()->rowCount(index.parent());
//    qDebug() << "ROWS2 =" << index.model()->rowCount(index.parent());
//    qDebug() << "ROWS3 =" << m_FSmodel->rowCount(index.parent());
//    m_FSmodel->resizeLinesQuantity(10);
}

void MainWindow::slotDirectoryWasLoaded(const QString &dir)
{
//    qDebug() << "path =" << dir << ",  rows =" << m_FSmodel->rowCount(m_FSmodel->index(dir));
    m_FSmodel->resizeLinesQuantity( m_FSmodel->rowCount(m_FSmodel->index(dir)) );
}

void MainWindow::slotUpdateSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
//    qDebug() << "\n***** selection changed *****";
    const QSortFilterProxyModel *proxyModel = 0;
    FileSystemCustomModel *sourceModel = 0;
    QModelIndex index, indexSource;

//    qDebug() << "   selected items:";
    LinesCounter linesCounter;
    QModelIndexList indexes = selected.indexes();
    foreach (index, indexes) {
        if (index.column() == FileSystemCustomModel::rhh_Name) {
            if ( ( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) &&
                ( sourceModel = dynamic_cast<FileSystemCustomModel*>(proxyModel->sourceModel()) ) ) {
                indexSource = proxyModel->mapToSource(index);
                if ( sourceModel->canCalcLines(indexSource) ) {
//                    qDebug() << "row =" << indexSource.row() << ":" << sourceModel->filePath(indexSource);
                    try {
                        linesCounter.setFileName( sourceModel->filePath(indexSource).toStdString() );
                        sourceModel->setLinesQuantity( indexSource.row(), linesCounter.countLines() );
                    }
                    catch(std::exception &ex) {
                        QMessageBox::warning( this, tr("Error count file lines"), tr(ex.what()) );
                    }
//                    qDebug() << "CAN read this item, index =" << indexSource.row() << ", lines =" << sourceModel->getLinesQuantity(indexSource.row());
                }
//                else qDebug() << "CANNOT read this item";
            }
        }
    }

//    qDebug() << "   deselected items:";
    indexes = deselected.indexes();
    foreach (index, indexes) {
        if (index.column() == FileSystemCustomModel::rhh_Name) {
            if ( ( proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) &&
                ( sourceModel = dynamic_cast<FileSystemCustomModel*>(proxyModel->sourceModel()) ) ) {
                indexSource = proxyModel->mapToSource(index);
                if ( sourceModel->canCalcLines(indexSource) ) {
                    sourceModel->setLinesQuantity( indexSource.row(), FileSystemCustomModel::empty_value );
                }
            }
        }
    }
//    qDebug() << "***** selection END *****";
}

void MainWindow::slotGetFileInfo(const QModelIndex &index)
{
    /*
     * Not use the MainWindow::isDir() function, because it recognize only dirs,
     * but dirs there are can to be symbolic link or shortcut on Windows.
     * Using the QFileInfo class for recognition only redeable and not executable files.
     */
    Qt::MouseButtons mouseBtns = QApplication::mouseButtons();
    const QSortFilterProxyModel *proxyModel = 0;
    QFileSystemModel *sourceModel = 0;
    if (mouseBtns & Qt::LeftButton) {
        // get a models
        if ( !(proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model()) ) ||
            !(sourceModel = dynamic_cast<QFileSystemModel *>(proxyModel->sourceModel()) ) )
            return;
        QFileInfo fileInfo = sourceModel->fileInfo( proxyModel->mapToSource(index) );
        if (fileInfo.isFile() && fileInfo.isReadable() && !fileInfo.isExecutable()) {
            bool isSelected = ui->m_rightTableInfo->selectionModel()->isSelected(index);
            if (isSelected)
                qDebug() << fileInfo.absoluteFilePath();
            else
                qDebug() << "is NOT selected";
        }
        //        qDebug() << "col:" << index.column() << "row:" << index.row();
    }
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
