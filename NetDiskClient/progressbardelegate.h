#ifndef PROGRESSBARDELEGATE_H
#define PROGRESSBARDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QVector>
#include <QMetaType>

Q_DECLARE_METATYPE(QVector<int>)

// struct DownloadTask {
//     QString fileName;
//     qint64 total = 0;
//     qint64 received = 0;
//     int progress = 0;
//     bool paused = false;
// };

class ProgressBarDelegate : public QStyledItemDelegate
{
      Q_OBJECT
public:
    ProgressBarDelegate();
    explicit ProgressBarDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

signals:
    void pauseToggled(int row, bool paused);
};

#endif // PROGRESSBARDELEGATE_H
