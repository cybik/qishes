/******************************************************************
 *
 * dwishes.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-02-09, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class DWishes; }
QT_END_NAMESPACE

class DWishes : public QDialog {
Q_OBJECT

public:
    explicit DWishes(QWidget *parent = nullptr);
    ~DWishes() override;

private:
    Ui::DWishes *ui;
};
