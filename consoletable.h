#ifndef CONSOLETABLE_H
#define CONSOLETABLE_H

#include <QTableWidget>
#include <QMouseEvent>

class ConsoleTable : public QTableWidget
{
public:
    ConsoleTable(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

};

#endif // CONSOLETABLE_H
