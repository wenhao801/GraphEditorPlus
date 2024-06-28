#include "consoletable.h"

ConsoleTable::ConsoleTable(QWidget *parent): QTableWidget(parent) {}

void ConsoleTable::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        QTableWidgetItem *item = itemAt(event->pos());
        if (item != nullptr) {
            editItem(item);
        }
    }
    QTableWidget::mousePressEvent(event);
}
