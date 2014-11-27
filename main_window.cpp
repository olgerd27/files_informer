#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug> // TODO: delete

#include "main_window.h"
#include "ui_main_window.h"

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
public:
    QVariant data(const QModelIndex &index, int role) const
    {
//        if(!index.isValid()) return QVariant();

        // NOTE: There are solution for setting the checking state of the checkboxes in the views
        // There are need to get knows how allow users to change state of the checkboxes
        //        if(role == Qt::CheckStateRole && index.column() == 0)
        //            return Qt::Checked;

        QVariant data;
        QModelIndex sourceModelIndex;
//        if(role == Qt::DisplayRole || role == Qt::DecorationRole) { //for debbuging: uncomment this if there are need to print only "display" and "decoration" item's data
//        if(role == Qt::DisplayRole || role == Qt::DecorationRole || role == Qt::FontRole || role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ihh_Name:
                data = QSortFilterProxyModel::data(index, role); break;
            case ihh_Size:
                data = QSortFilterProxyModel::data(index, role); break;
            case ihh_Type:
                if(role == Qt::DisplayRole)
                    data = QVariant(linesQuantity());
                else if(role == Qt::TextAlignmentRole) { // set text alignment as in "Size" column
                    sourceModelIndex = QSortFilterProxyModel::index(index.row(), ihh_Size, index.parent());
                    data = QSortFilterProxyModel::data(sourceModelIndex, role);
                }
                break;
            case ihh_DataModified: // maybe implement right alignment?
                sourceModelIndex = QSortFilterProxyModel::index(index.row(), ihh_Type, index.parent());
                data = QSortFilterProxyModel::data(sourceModelIndex, role);
                break;
            case ihh_Empty:
                sourceModelIndex = QSortFilterProxyModel::index(index.row(), ihh_DataModified, index.parent());
                data = QSortFilterProxyModel::data(sourceModelIndex, role);
                break;
            }
//            qDebug() << "col:" << index.column() << ", row:" << index.row() << ", role:" << roleName(role) << ", data:" << data;
//        }
        return data;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        QVariant name;
        if(orientation == Qt::Horizontal && (role == Qt::DisplayRole || role == Qt::TextAlignmentRole)) {
            switch(section) {
            case ihh_Name:
                name = QSortFilterProxyModel::headerData(ihh_Name, orientation); break;
            case ihh_Size:
                name = QSortFilterProxyModel::headerData(ihh_Size, orientation); break;
            case ihh_Type:
                name = QVariant("Lines quantity"); break;
            case ihh_DataModified:
                name = QSortFilterProxyModel::headerData(ihh_Type, orientation); break;
            case ihh_Empty:
                name = QSortFilterProxyModel::headerData(ihh_DataModified, orientation); break;
            }
        }
        else
            name = QSortFilterProxyModel::headerData(section, orientation, role);
        return name;
    }

    // TODO: delete this
    QString roleName(int role) const
    {
        QString name;
        switch (role) {
        case Qt::DisplayRole:
            name = "DisplayRole"; break;
        case Qt::DecorationRole:
            name = "DecorationRole"; break;
        case Qt::EditRole:
            name = "EditRole"; break;
        case Qt::ToolTipRole:
            name = "ToolTipRole"; break;
        case Qt::StatusTipRole:
            name = "StatusTipRole"; break;
        case Qt::WhatsThisRole:
            name = "WhatsThisRole"; break;
        case Qt::SizeHintRole:
            name = "SizeHintRole"; break;
        case Qt::FontRole:
            name = "FontRole"; break;
        case Qt::TextAlignmentRole:
            name = "TextAlignmentRole"; break;
        case Qt::BackgroundRole:
            name = "BackgroundRole"; break;
        case Qt::ForegroundRole:
            name = "ForegroundRole"; break;
        case Qt::CheckStateRole:
            name = "CheckStateRole"; break;
        case Qt::InitialSortOrderRole:
            name = "InitialSortOrderRole"; break;
        case Qt::AccessibleTextRole:
            name = "AccessibleTextRole"; break;
        case Qt::AccessibleDescriptionRole:
            name = "AccessibleDescriptionRole"; break;
        case Qt::UserRole:
            name = "UserRole"; break;
        default:
            break;
        }
        return name;
    }

    // WARNING: if implement this function, selection of the rows in table will be losed
    int columnCount(const QModelIndex &parent) const
    {
        return QSortFilterProxyModel::columnCount(parent) + 1;
    }

//    Qt::ItemFlags flags(const QModelIndex &index) const
//    {
//        return QSortFilterProxyModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
//    }

protected:
    enum initHorizHeaderNames
    {
        ihh_Name = 0,
        ihh_Size = 1,
        ihh_Type = 2,
        ihh_DataModified = 3,
        ihh_Empty = 4,
    };

    qlonglong linesQuantity() const
    {
        return 555L;
    }

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

    m_dirsModel = new LeftViewProxyModel;
    m_dirsModel->setSourceModel(m_FSmodel);
    m_dirsModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_dirsModel->sort(0, Qt::AscendingOrder);

    m_dirsContentsModel = new RightViewProxyModel;
    m_dirsContentsModel->setSourceModel(m_FSmodel);
    m_dirsContentsModel->sort(0, Qt::AscendingOrder);
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
            this, SLOT(slotSetListRootIndex(QModelIndex)));
}

void MainWindow::setRightView()
{
    QTableView *view = ui->m_rightTableInfo;
    view->setModel(m_dirsContentsModel);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view->horizontalHeader()->resizeSection(0, 300); //WARNING: when increase section to 300, there are need to make view and main window wider
//    view->resizeRowsToContents();
//    view->horizontalHeader()->setSectionResizeMode(view->horizontalHeader()->count() - 1, QHeaderView::ResizeToContents);

    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotSetTreeCurrentIndex(QModelIndex)));
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
void MainWindow::slotSetListRootIndex(const QModelIndex &indexTree)
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

void MainWindow::slotSetTreeCurrentIndex(const QModelIndex &indexList)
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
