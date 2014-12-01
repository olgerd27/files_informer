#ifndef FILE_SYSTEM_CUSTOM_MODEL_H
#define FILE_SYSTEM_CUSTOM_MODEL_H

#include <QFileSystemModel>

/*
 * Custom source model methods
 * First of all I have tried to make custom proxy model for the right view (insert new column), but this attempt
 * was not successful - when reimplement columnCount() method (that increment the column quantity), then added column
 * becomes not enabled, lossed the ability of table to select rows by user, data() method even not receive the model index
 * for the new inserted column, and others issues.
 * Solution for this issues with the proxy model was not found. And then there was decided to make custom source model.
 * All problems with the right view, that was when the custom proxy model aplyied (described above), was disappeared.
 * And this solution was applied, although inserting the column into a source model is worse decision as into a proxy model,
 * because of the model/view conception.
 */
class FileSystemCustomModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FileSystemCustomModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &parent) const;

private:
    enum initHorizHeaderNames
    {
        ihh_Name = 0,
        ihh_Size = 1,
        ihh_Type = 2,
        ihh_DataModified = 3,
        ihh_Empty = 4,
    };

    qlonglong linesQuantity() const;
};

#endif // FILE_SYSTEM_CUSTOM_MODEL_H
