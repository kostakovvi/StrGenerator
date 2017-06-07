#include "strgenerator.h"
#include <QDebug>
#include <QBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <cmath>
#include <QtXml>
#include <QFile>
#include <QFileDialog>
#include <QDir>

/**
 * @brief Глобальная переменная countRecordsToGenerate предназначена для хранения числа записей при нажатии кнопки генерации
 */
int countRecordsToGenerate=0;

/**
 * @brief Функция randString генерирует случайную строку
 * @param Параметр len задает длину требуемой строки
 * @return Сгенерированную строку
 */
QString randString(int len)
{
    QString str;
    str.resize(len);
    for (int s = 0; s < len ; ++s)
        str[s] = QChar('a' + char(qrand() % ('z' - 'a')));

    return str;
}

/**
 * @brief Функция randBool генерирует случайное булево значение
 * @return Сгенерированное булево значение
 */
bool randBool()
{
  return qrand() % 2 == 1;
}

/**
 * @brief Функция randInt генерирует случайное число типа int
 * @param Параметр min задает минимальное число
 * @param Параметр max задает максимальное число
 * @return Сгенерированное число
 */
int randInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}

/**
 * @brief Функция randDouble генерирует случайное число типа double
 * @param Параметр min задает минимальное число
 * @param Параметр max задает максимальное число
 * @return Сгенерированное число
 */
double randDouble(double min, double max)
{
    //с округлением до 3 знака после запятой
    return round((((double)qrand()*(max - min))/(double)RAND_MAX + min)*1000)/1000.;
    //return (((double)qrand()*(max - min))/(double)RAND_MAX + min);
}

StrGenerator::StrGenerator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "StrGenerator constructor started";
    createForm();   

    // Необходимо для корректного срабатывания рандома
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    connect(pGenerateButton, SIGNAL(clicked()), SLOT(slotGenerateButtonClick()));
    connect(pRecordsTableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slotOnTableClick(const QModelIndex&)));
    connect(pSaveButton, SIGNAL(clicked()), SLOT(slotSaveButtonClick()));
    connect(pLoadButton, SIGNAL(clicked()), SLOT(slotLoadButtonClick()));
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

     pRecordsTableView = new QTableView;
     pRecordsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
     pRecordsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
     pRecordsTableView->verticalHeader()->hide();
     pRecordsTableView->horizontalHeader()->setHighlightSections(false);

     /**
      * @brief Переменная countCol задает количество колонок
      */
     int countCol = 4;

     pModel = new QStandardItemModel(countRecordsToGenerate,countCol,this);
     pModel->setHorizontalHeaderItem(0, new QStandardItem(QString(tr("Колонка1"))));
     pModel->setHorizontalHeaderItem(1, new QStandardItem(QString(tr("Колонка2"))));
     pModel->setHorizontalHeaderItem(2, new QStandardItem(QString(tr("Колонка3"))));
     pModel->setHorizontalHeaderItem(3, new QStandardItem(QString(tr("Колонка4"))));

     //QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
     //model->setItem(0,0,firstRow);

     pRecordsTableView->setModel(pModel);

     for (int col=0; col<countCol; col++)
     {
        pRecordsTableView->setColumnWidth(col,70);
     }

     QGroupBox* gbxRecords = new QGroupBox(tr("&Записи"));
     gbxRecords->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

     pCountRecordsFromLE = new QLineEdit("0");
     pCountRecordsFromLE->setMaximumSize(95,20);
     pCountRecordsFromLE->setAlignment(Qt::AlignRight);
     //ограничение ввода не более 11000000
     pCountRecordsFromLE->setValidator(new QIntValidator(0, 11000000, this));
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

     hbxLayout->addWidget(pRecordsTableView);
     hbxLayout->addLayout(vbxLayout);

     setLayout(hbxLayout);
}

StrGenerator::~StrGenerator()
{

}

void StrGenerator::slotGenerateButtonClick()
{
    qDebug() << "slotGenerateButtonClick started";
    countRecordsToGenerate = pCountRecordsFromLE->text().toInt();
    qDebug() << "countRecordsToGenerate" << countRecordsToGenerate;
    if (countRecordsToGenerate <=0)
    {
        QMessageBox::critical(0, tr("Ошибка"), tr("Введите положительное число"), QMessageBox::Ok);
        return;
    }

    pModel->removeRows(0, records.size());
    records.clear();    

    for (int i=0; i < countRecordsToGenerate; i++)
    {
        //qDebug() << "Generate the structure element " << i+1;
        RecordStructure rs;
        rs.rndStr = randString(2);
        //qDebug() << rs.rndStr;
        rs.rndInt = randInt(10,100);
        //qDebug() << rs.rndInt;
        rs.rndDouble = randDouble(10.0, 100.0);
        //qDebug() << rs.rndDouble;
        rs.rndBool = randBool();
        //qDebug() << rs.rndBool;
        records.append(rs);
        //QStandardItem* item =  new QStandardItem(rs.rndStr);

        pModel->setItem(i,0, new QStandardItem(rs.rndStr));
        pModel->setItem(i,1, new QStandardItem(QString::number(rs.rndInt)));
        pModel->setItem(i,2, new QStandardItem(QString::number(rs.rndDouble)));
        pModel->setItem(i,3, new QStandardItem(QVariant(rs.rndBool).toString()));
    }
    qDebug() << "Generate ended";
    qDebug() << "records.size(): " << records.size();
    pCountRecordsFromTable->setText(QString::number(records.size()));
}

void StrGenerator::slotOnTableClick(const QModelIndex &index)
{
    if (index.isValid())
    {
        int row = index.row();
        pCol1ValueFromTable->setText(index.sibling(row, 0).data().toString());
        pCol2ValueFromTable->setText(index.sibling(row, 1).data().toString());
        pCol3ValueFromTable->setText(index.sibling(row, 2).data().toString());
        pCol4ValueFromTable->setText(index.sibling(row, 3).data().toString());
        qDebug() << "Current row clicked: " << index.row()+1;
    }
}

void StrGenerator::slotSaveButtonClick()
{
    qDebug() << "slotSaveButtonClick started";
    qDebug() << "Records to save: " << records.size();
    if (records.size() <=0)
    {
        QMessageBox::information(0, tr("Информация"), tr("Нет записей для сохранения в файл"), QMessageBox::Ok);
        return;
    }   

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранение"),
                               "./data.xml",
                                tr("XML documents (*.xml)"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("data");
        for (int i=0; i<records.size(); i++)
        {
            RecordStructure rs = records[i];
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("col1", rs.rndStr);
            xmlWriter.writeAttribute("col2", QString::number(rs.rndInt));
            xmlWriter.writeAttribute("col3", QString::number(rs.rndDouble));
            xmlWriter.writeAttribute("col4", QVariant(rs.rndBool).toString());
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        //QTextStream(&file) << doc.toString();
        file.close();
        QMessageBox::information(0, tr("Сообщение"), tr("Данные успешно сохранены в файл"), QMessageBox::Ok);
        return;
    }
}

void StrGenerator::slotLoadButtonClick()
{
    qDebug() << "slotLoadButtonClick started";
}
