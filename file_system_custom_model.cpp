#include "file_system_custom_model.h"

FileSystemCustomModel::FileSystemCustomModel(QObject *parent)
    : QFileSystemModel(parent)
{
}

QVariant FileSystemCustomModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    // NOTE: There are solution for setting the checking state of the checkboxes in the views
    // There are need to get knows how allow users to change state of the checkboxes
    //        if(role == Qt::CheckStateRole && index.column() == 0)
    //            return Qt::Checked;

    QVariant data;
    QModelIndex sourceModelIndex;
    switch(index.column()) {
    case ihh_Name:
        data = QFileSystemModel::data(index, role); break;
    case ihh_Size:
        data = QFileSystemModel::data(index, role); break;
    case ihh_Type:
        if(role == Qt::DisplayRole)
            data = QVariant(linesQuantity());
        else if(role == Qt::TextAlignmentRole) { // set text alignment as in "Size" column
            sourceModelIndex = QFileSystemModel::index(index.row(), ihh_Size, index.parent());
            data = QFileSystemModel::data(sourceModelIndex, role);
        }
        break;
    case ihh_DataModified:
        if(role == Qt::DisplayRole) {
            sourceModelIndex = QFileSystemModel::index(index.row(), ihh_Type, index.parent());
            data = QFileSystemModel::data(sourceModelIndex, role);
        }
        else if(role == Qt::TextAlignmentRole) {
            sourceModelIndex = QFileSystemModel::index(index.row(), ihh_Size, index.parent());
            data = QFileSystemModel::data(sourceModelIndex, role);
        }
        break;
    case ihh_Empty:
        sourceModelIndex = QFileSystemModel::index(index.row(), ihh_DataModified, index.parent());
        data = QFileSystemModel::data(sourceModelIndex, role);
        break;
    }
    return data;
}

QVariant FileSystemCustomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant name;
    if(orientation == Qt::Horizontal && (role == Qt::DisplayRole || role == Qt::TextAlignmentRole)) {
        switch(section) {
        case ihh_Name:
            name = QFileSystemModel::headerData(ihh_Name, orientation); break;
        case ihh_Size:
            name = QFileSystemModel::headerData(ihh_Size, orientation); break;
        case ihh_Type:
            name = QVariant("Lines"); break;
        case ihh_DataModified:
            name = QFileSystemModel::headerData(ihh_Type, orientation); break;
        case ihh_Empty:
            name = QFileSystemModel::headerData(ihh_DataModified, orientation); break;
        }
    }
//        else
//            name = QFileSystemModel::headerData(section, orientation, role);
    return name;
}

int FileSystemCustomModel::columnCount(const QModelIndex &parent) const
{
return QFileSystemModel::columnCount(parent) + 1;
}

qlonglong FileSystemCustomModel::linesQuantity() const
{
    return 555L;
}
