#ifndef FILE_INFO_LIST_MODEL_H
#define FILE_INFO_LIST_MODEL_H

#include <QAbstractListModel>
#include "common_defines.h"

class FilesInfoDataImpl;

class FileInfoListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileInfoListModel(QObject *parent = 0);
    ~FileInfoListModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;


    t_filesQnty filesQnty() const;
    t_filesSize filesSize() const;
    t_linesQnty filesLines() const;
    t_density filesDensity() const;

public slots:
    void slotChangeFileInfo(long size, long lines);
    void slotClearFileInfo();

private:
    enum rowsNumbers {
        rowFilesQnty = 0,
        rowFilesSize,
        rowFilesLines,
        rowFilesDensity
    };

    QString bytesToSuitUnits(t_filesSize size) const;
    t_density calcDensity(t_filesSize size, t_linesQnty lines) const;

    FilesInfoDataImpl *m_dataImpl;
};

#endif // FILE_INFO_LIST_MODEL_H
