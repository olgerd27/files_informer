#include "checkbox_delegate.h"
#include <QApplication>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    qDebug() << "CheckBoxDelegate::CheckBoxDelegate()";
}

void CheckBoxDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyleOptionButton checkBoxStyle;
    QRect checkBoxRect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkBoxStyle);
    checkBoxStyle.rect = option.rect;
    checkBoxStyle.rect.setLeft(option.rect.x() + option.rect.width() / 2 - checkBoxRect.width() / 2);
    checkBoxStyle.state = QStyle::State_Enabled | QStyle::State_Off;
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyle, painter);
    QItemDelegate::paint(painter, option, index);
}

QWidget * CheckBoxDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &/*option*/,
                                         const QModelIndex &/*index*/) const
{
    qDebug() << "CheckBoxDelegate::createEditor()";
    QCheckBox *checkBox = new QCheckBox(parent);
    checkBox->installEventFilter(const_cast<CheckBoxDelegate*>(this));
    return checkBox;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug() << "CheckBoxDelegate::setEditorData()";
    if( QCheckBox *checkBox = dynamic_cast<QCheckBox*>(editor) )
        checkBox->setCheckState(Qt::Checked);
}

void CheckBoxDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    qDebug() << "CheckBoxDelegate::setModelData()";
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/*index*/) const
{
    qDebug() << "CheckBoxDelegate::updateEditorGeometry()";
    editor->setGeometry(option.rect);
}
