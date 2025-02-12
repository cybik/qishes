/******************************************************************
 *
 * dwishes.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-02-09, for qgachawishes.
 *
 ******************************************************************/

// You may need to build the project (run Qt uic code generator) to get "ui_DWishes.h" resolved

#include <ui/dwishes.h>
#include "ui_dwishes.h"


DWishes::DWishes(QWidget *parent) :
    QDialog(parent), ui(new Ui::DWishes) {
    ui->setupUi(this);
}

DWishes::~DWishes() {
    delete ui;
}
