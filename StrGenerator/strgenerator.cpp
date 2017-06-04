#include "strgenerator.h"
#include <QDebug>
#include <QBoxLayout>
#include <QGroupBox>
#include <QTableView>

StrGenerator::StrGenerator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "StrGenerator constructor started";
    createForm();
}

void StrGenerator::createForm()
{
     qDebug() << "StrGenerator createForm started";
     setWindowTitle(tr("Тестовое задание"));
     resize(595,280);
     setMinimumSize(595,280);

     QPalette Pal(palette());

     // устанавливаем цвет фона
     Pal.setColor(QPalette::Background, Qt::lightGray);
     setAutoFillBackground(true);
     setPalette(Pal);

     QTableView* recordsTableView = new QTableView;

     QGroupBox* gbxRecords = new QGroupBox(tr("&Записи"));
     gbxRecords->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     pCountRecordsFromLE = new QLineEdit("0");
     pCountRecordsFromLE->setMaximumSize(95,20);
     pCountRecordsFromLE->setAlignment(Qt::AlignRight);
     pGenerateButton = new QPushButton(tr("С&генерировать"));
     pSaveButton = new QPushButton(tr("&Сохранить"));
     pLoadButton = new QPushButton(tr("&Загрузить"));
     pCountRecordsFromTable = new QLabel;
     pCountRecordsFromTable->setText(pCountRecordsFromLE->text());
     pCol1ValueFromTable = new QLabel;
     pCol2ValueFromTable = new QLabel;
     pCol3ValueFromTable = new QLabel;
     pCol4ValueFromTable = new QLabel;

     QLabel* recordsInfoLbl = new QLabel(tr("Всего записей:"));
     recordsInfoLbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
     recordsInfoLbl->setStyleSheet("font-weight: bold;");

     QLabel* currentRecordLbl = new QLabel(tr("Текущая запись:"));
     currentRecordLbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
     currentRecordLbl->setStyleSheet("font-weight: bold;");

     QLabel* col1Lbl = new QLabel(tr("Колонка 1:"));
     col1Lbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     QLabel* col2Lbl = new QLabel(tr("Колонка 2:"));
     col2Lbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     QLabel* col3Lbl = new QLabel(tr("Колонка 3:"));
     col3Lbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     QLabel* col4Lbl = new QLabel(tr("Колонка 4:"));
     col4Lbl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     //Layout Setup
     QHBoxLayout* hbxLayout = new QHBoxLayout;
     QVBoxLayout* vbxLayout = new QVBoxLayout;
     QGridLayout* gridRecordLayout = new QGridLayout;
     QGridLayout* gridStatLayout = new QGridLayout;

     gridRecordLayout->addWidget(pCountRecordsFromLE,0,0);
     gridRecordLayout->addWidget(pGenerateButton,0,1);
     gridRecordLayout->addWidget(pSaveButton,1,0);
     gridRecordLayout->addWidget(pLoadButton,1,1);

     gbxRecords->setLayout(gridRecordLayout);

     gridStatLayout->addWidget(recordsInfoLbl,0,0,1,1,Qt::AlignRight);
     gridStatLayout->addWidget(pCountRecordsFromTable,0,1);
     gridStatLayout->addWidget(currentRecordLbl,1,0,1,1,Qt::AlignRight);
     gridStatLayout->addWidget(col1Lbl,2,0,1,1,Qt::AlignRight);
     gridStatLayout->addWidget(pCol1ValueFromTable,2,1);
     gridStatLayout->addWidget(col2Lbl,3,0,1,1,Qt::AlignRight);
     gridStatLayout->addWidget(pCol2ValueFromTable,3,1);
     gridStatLayout->addWidget(col3Lbl,4,0,1,1,Qt::AlignRight);
      gridStatLayout->addWidget(pCol3ValueFromTable,4,1);
     gridStatLayout->addWidget(col4Lbl,5,0,1,1,Qt::AlignRight);
      gridStatLayout->addWidget(pCol4ValueFromTable,5,1);

     vbxLayout->addWidget(gbxRecords);
     vbxLayout->addLayout(gridStatLayout);
     vbxLayout->setAlignment(Qt::AlignTop);


     hbxLayout->addWidget(recordsTableView);
     hbxLayout->addLayout(vbxLayout);

     setLayout(hbxLayout);
}

StrGenerator::~StrGenerator()
{

}
