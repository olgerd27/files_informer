#ifndef FILE_SYSTEM_CUSTOM_MODEL_H
#define FILE_SYSTEM_CUSTOM_MODEL_H

#include <QFileSystemModel>
#include <QVector>
#include "lines_counter.h"

/*
 * Custom source model methods
 * First of all I have tried to make custom proxy model for the right view (insert new column), but this attempt
 * was not successful - when reimplement columnCount() method (that increment the column quantity), then added column
 * becomes not enabled, lossed the ability of table to select rows by user, data() method even not receive the model index
 * for the new inserted column, and others issues.
 * Solution for this issues with the proxy model was not found. And then I was decided to make a custom source model.
 * All problems with the right view, that was when the custom proxy model was aplyied, was disappeared.
 * Solution with the source model was applied, although inserting the column into a source model is worse decision as
 * into a proxy model, because of the model/view conception.
 */
class FileSystemCustomModel : public QFileSystemModel
{
    Q_OBJECT
public:
    // table columns structure after adding new column "Lines" (from left to right)
    enum resHorizHeaderNames
    {
        rhh_Name = 0,
        rhh_Size,
        rhh_Lines,
        rhh_Type,
        rhh_Modified
    };

    enum
    {
        empty_value = -5,
        invalid_value = -4
    };

    explicit FileSystemCustomModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &parent) const;

    void setLinesQuantity(int index, t_linesCount value);
    t_linesCount getLinesQuantity(int index) const;

    void resizeLinesQuantity(int size);
    bool canCalcLines(const QModelIndex &indexSource) const;

private:
    // initial table columns structure (from left to right)
    enum initHorizHeaderNames
    {
        ihh_Name = 0,
        ihh_Size,
        ihh_Type,
        ihh_Modified,
        ihh_Empty,
    };

    QVector<t_linesCount> m_linesQnties;
};

#endif // FILE_SYSTEM_CUSTOM_MODEL_H
