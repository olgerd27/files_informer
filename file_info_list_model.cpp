#include "file_info_list_model.h"
#include "files_info_data_impl.h"

FileInfoListModel::FileInfoListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_dataImpl(new FilesInfoDataImpl)
{
}

FileInfoListModel::~FileInfoListModel()
{
    delete m_dataImpl;
}
#include <QDebug>
QVariant FileInfoListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    QVariant data;
    if ( role == Qt::DisplayRole ) {
        switch( index.row() ) {
        case rowFilesQnty:
            data = QString("Files quantity: %1").arg( m_dataImpl->filesQnty() );
            break;
        case rowFilesSize:
            data = QString("Summary size: %1 bytes").arg( m_dataImpl->filesSize() );
            break;
        case rowFilesLines:
            data = QString("Summary lines: %1").arg( m_dataImpl->filesLines() );
            break;
        case rowFilesDensity:
            t_density value = m_dataImpl->filesDensity();
            data = QString("Content density: %1 characters/line").arg( qRound(value * 10) / 10.0 );
            break;
        }
    }
    return data;
}

int FileInfoListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return 4;
}


t_filesQnty FileInfoListModel::filesQnty() const
{
    return m_dataImpl->filesQnty();
}

t_filesSize FileInfoListModel::filesSize() const
{
    return m_dataImpl->filesSize();
}

t_linesQnty FileInfoListModel::filesLines() const
{
    return m_dataImpl->filesLines();
}

t_density FileInfoListModel::filesDensity() const
{
    return m_dataImpl->filesDensity();
}

t_density FileInfoListModel::calcDensity(t_filesSize size, t_linesQnty lines) const
{
    return size * 1.0 / lines;
}

/*
 * Slots
 */
void FileInfoListModel::slotChangeFileInfo(long size, long lines)
{
    int addValue = (lines >= 0) ? 1 : -1; // define adding or subctracting
    m_dataImpl->setFilesQnty( m_dataImpl->filesQnty() + addValue );
    m_dataImpl->setFilesSize( m_dataImpl->filesSize() + size );
    m_dataImpl->setFilesLines( m_dataImpl->filesLines() + lines );
    m_dataImpl->setFilesDensity( calcDensity( m_dataImpl->filesSize(), m_dataImpl->filesLines() ) );
    emit dataChanged( this->index(0), this->index(3) );
}

void FileInfoListModel::slotClearFileInfo()
{
    m_dataImpl->setFilesQnty(0);
    m_dataImpl->setFilesSize(0);
    m_dataImpl->setFilesLines(0);
    m_dataImpl->setFilesDensity(0);
    emit dataChanged( this->index(0), this->index(3) );
}
