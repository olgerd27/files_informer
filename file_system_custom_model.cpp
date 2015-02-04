#include <QTableView>
#include <QDebug> // TODO: delete
#include <QMessageBox> // TODO; delete

#include <stdexcept>

#include "file_system_custom_model.h"

FileSystemCustomModel::FileSystemCustomModel(QObject *parent)
    : QFileSystemModel(parent)
{
    resizeLinesQuantity(10);
}

QVariant FileSystemCustomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    /* NOTE: There are solution for setting the checking state of the checkboxes in the views
     * There are need to get knows how allow users to change state of the checkboxes
     */
    //        if (role == Qt::CheckStateRole && index.column() == 0)
    //            return Qt::Checked;

    /* NOTE: cannot correct fill background color, because view contents changed dynamically,
     * but this method fill model items statically
     * Want to do the changes rows colors, for example (rotated to 90 grad): | white | red | white | red | white |
     */
//    // fill model background color
//    if (role == Qt::BackgroundRole) {
//        qDebug() << "background role, row =" << index.row();
//        if (index.row() % 2) {
//            qDebug() << "set background";
//            return QColor("#eeeeec");
//        }
//    }

    QVariant data;
    QModelIndex sourceModelIndex;
    switch(index.column()) {
    case ihh_Name:
        if (role == Qt::TextAlignmentRole)
            data = int(Qt::AlignLeft | Qt::AlignVCenter);
        else
            data = QFileSystemModel::data(index, role);
        break;

    case ihh_Size:
        if (role == Qt::TextAlignmentRole)
            data = int(Qt::AlignRight | Qt::AlignVCenter);
//        else if (role == Qt::BackgroundRole)
//            data = QBrush(QColor(234, 150, 174), Qt::Dense3Pattern);
        else
            data = QFileSystemModel::data(index, role);
        break;

    case ihh_Type:
        if (role == Qt::TextAlignmentRole)
            data = int(Qt::AlignRight | Qt::AlignVCenter);
        else if (role == Qt::DisplayRole) {
            t_linesCount value = m_linesQnties.at(index.row());
            if (value != empty_value) data = QVariant(value);
//            qDebug() << "model::data(), row =" << index.row() << ", col =" << index.column() << ", data =" << data;
//            qDebug() << "Quantities:" << m_linesQnties << ", size =" << m_linesQnties.size();
        }
//        else if (role == Qt::BackgroundRole)
//            data = QBrush(QColor(234, 150, 174), Qt::Dense2Pattern);
        break;

    case ihh_Modified:
        if (role == Qt::TextAlignmentRole)
            data = int(Qt::AlignHCenter | Qt::AlignVCenter);
        else if (role == Qt::DisplayRole) {
            sourceModelIndex = QFileSystemModel::index(index.row(), ihh_Type, index.parent());
            data = QFileSystemModel::data(sourceModelIndex, role);
        }
//        else if (role == Qt::BackgroundRole)
//            data = QBrush(QColor(234, 150, 174), Qt::Dense1Pattern);
        break;

    case ihh_Empty:
        if (role == Qt::TextAlignmentRole)
            data = int(Qt::AlignLeft | Qt::AlignVCenter);
//        else if (role == Qt::BackgroundRole)
//            data = QBrush(QColor(234, 150, 174), Qt::SolidPattern);
        else {
            sourceModelIndex = QFileSystemModel::index(index.row(), ihh_Modified, index.parent());
            data = QFileSystemModel::data(sourceModelIndex, role);
        }
        break;
    }
    return data;
}

QVariant FileSystemCustomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant name;
    if (orientation == Qt::Horizontal && (role == Qt::DisplayRole || role == Qt::TextAlignmentRole)) {
        switch(section) {
        case ihh_Name:
            name = QFileSystemModel::headerData(ihh_Name, orientation); break;
        case ihh_Size:
            name = QFileSystemModel::headerData(ihh_Size, orientation); break;
        case ihh_Type:
            name = QVariant("Lines"); break;
        case ihh_Modified:
            name = QFileSystemModel::headerData(ihh_Type, orientation); break;
        case ihh_Empty:
            name = QFileSystemModel::headerData(ihh_Modified, orientation); break;
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

void FileSystemCustomModel::setLinesQuantity(int index, t_linesCount value)
{
    if(index < 0 || index >= m_linesQnties.size())
        throw std::runtime_error( QString("FileSystemCustomModel::setLinesQuantity()\ninvalid index = %1").arg(index).toStdString() );
    m_linesQnties[index] = value;
    qDebug() << "model::setLinesQuantity(): index =" << index << ", data =" << value << ", all data =" << m_linesQnties;
}

t_linesCount FileSystemCustomModel::getLinesQuantity(int index) const
{
    return (index < 0 || index >= m_linesQnties.size())
            ? (t_linesCount)invalid_value
            : m_linesQnties.at(index);
}

void FileSystemCustomModel::resizeLinesQuantity(int size)
{
    if (!m_linesQnties.isEmpty())
        m_linesQnties.clear();
    m_linesQnties.fill(empty_value, size);
    qDebug() << "<!!!>model::resize():" << m_linesQnties << ", size =" << m_linesQnties.size();
}

bool FileSystemCustomModel::canCalcLines(const QModelIndex &indexSource) const
{
    QFileInfo infoFile = fileInfo(indexSource);
    return infoFile.isFile() && infoFile.isReadable() && !infoFile.isExecutable();
}
