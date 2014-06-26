#include <QItemDelegate>
#include <QPainter>
#include <QDebug> // TODO: delete
#include "dir_tree.h"

/*
 * Delegate for highlight a lines in a view
 */
class HighlightDelegate : public QItemDelegate
{
public:
    HighlightDelegate(QObject *parent = 0)
        : QItemDelegate(parent)
    {
    }

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex          &index) const {
        if(option.state & QStyle::State_MouseOver) {
            QRect rect = option.rect;
            QLinearGradient gradient(0, 0, rect.width(), rect.height());
            gradient.setColorAt(0, Qt::lightGray);
            gradient.setColorAt(1, Qt::lightGray);
            // painter paint a rectangle with setted gradient
            painter->setBrush(gradient);
            painter->setPen(Qt::NoPen);
            painter->drawRect(rect);
        }
        QItemDelegate::paint(painter, option, index); // paint part of the base class
    }
};

/*
 * DirTree methods
 */
DirTree::DirTree(QWidget *parent)
    : QTreeView(parent)
{
    setItemDelegate(new HighlightDelegate(this));
    viewport()->setAttribute(Qt::WA_Hover);
}
