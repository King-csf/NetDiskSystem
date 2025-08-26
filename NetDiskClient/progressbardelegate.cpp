#include "progressbardelegate.h"

ProgressBarDelegate::ProgressBarDelegate() {}

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 取出进度列表
    QVariant v = index.data();
    QVector<int> progresses = v.value<QVector<int>>();

    if (progresses.size() == 0)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // 检查是否所有进度都是100%（完成状态）
    bool allCompleted = true;
    for (int progress : progresses) {
        if (progress < 100) {
            allCompleted = false;
            break;
        }
    }

    // 如果全部完成，显示单一的完成状态
    if (allCompleted) {
        QRect completeRect = option.rect.adjusted(2, 2, -2, -2);
        
        // 完成状态背景（绿色）
        painter->fillRect(completeRect, QColor("#27ae60"));
        
        // 完成状态文字
        painter->setPen(QColor("white"));
        painter->drawText(completeRect, Qt::AlignCenter, "已完成");
        return;
    }

    // 正常的多线程进度显示
    int barHeight = option.rect.height() / progresses.size();

    for (int i = 0; i < progresses.size(); i++)
    {
        QRect barRect = option.rect.adjusted(2, i * barHeight + 2, -2, -(progresses.size() - i - 1) * barHeight - 2);

        // 背景
        painter->fillRect(barRect, QColor("#e0e0e0"));

        // 前景（蓝色填充）
        int fillWidth = static_cast<int>(barRect.width() * (progresses[i] / 100.0));
        QRect fillRect(barRect.left(), barRect.top(), fillWidth, barRect.height());
        painter->fillRect(fillRect, QColor("#3498db"));

        // 百分比文字
        painter->setPen(QColor("black"));
        painter->drawText(barRect, Qt::AlignCenter, QString("%1%").arg(progresses[i]));
    }
}

