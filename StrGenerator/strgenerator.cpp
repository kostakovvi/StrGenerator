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
#include <QProgressDialog>

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

    //Необходимо для корректного срабатывания рандома
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

     //устанавливаем цвет фона
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

     //Описание Layout'ов
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

    setButtonsDisabled(true);

    QProgressDialog *progress = new QProgressDialog("", tr("Отмена"), 0, countRecordsToGenerate-1);
    progress->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    progress->setWindowModality(Qt::WindowModal); //не работает так, как ожидалось
    progress->setWindowTitle(tr("Генерация данных"));
    //progress->setMinimumDuration(8000);
    progress->setMinimumWidth(250);
    progress->setValue(0);
    //progress->show();
    for (int i=0; i < countRecordsToGenerate; i++)
    {        
        RecordStructure rs;
        rs.rndStr = randString(2);        
        rs.rndInt = randInt(10,100);        
        rs.rndDouble = randDouble(10.0, 100.0);        
        rs.rndBool = randBool();        
        records.append(rs);        

        putStructureToModel(*pModel, rs, i);
        if ( (i+1) % 1000 == 0)
        {
            pCountRecordsFromTable->setText(QString::number(records.size()));
            //Позволяет разблокировать графический интерфейс программы в момент проведения интенсивных операций. Работает медленнее(на несколько порядков).
            qApp->processEvents();
        }

        progress->setValue(i);
        if(progress->wasCanceled())
        {
            qDebug() << "Generation Progress cancelled";
            break;
        }
    }
    progress->close();
    delete progress;    
    pCountRecordsFromTable->setText(QString::number(records.size()));
    setButtonsDisabled(false);
    qDebug() << "Generate ended";
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
    qDebug() << "Count records to save: " << records.size();
    if (records.size() <=0)
    {
        QMessageBox::information(0, tr("Информация"), tr("Нет записей для сохранения в файл"), QMessageBox::Ok);
        return;
    }   

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранение"),
                               "./data.xml",
                                tr("XML files (*.xml)"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        setButtonsDisabled(true);
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("data");

        QProgressDialog *progress = new QProgressDialog("", tr("Отмена"), 0, records.size()-1);
        progress->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        progress->setWindowModality(Qt::WindowModal); //не работает так, как ожидалось
        progress->setWindowTitle(tr("Сохранение данных в файл"));
        //progress->setMinimumDuration(8000);
        progress->setMinimumWidth(250);
        progress->setValue(0);
        progress->show();
        for (int i=0; i<records.size(); i++)
        {
            RecordStructure rs = records[i];
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("col1", rs.rndStr);
            xmlWriter.writeAttribute("col2", QString::number(rs.rndInt));
            xmlWriter.writeAttribute("col3", QString::number(rs.rndDouble));
            xmlWriter.writeAttribute("col4", QVariant(rs.rndBool).toString());
            xmlWriter.writeEndElement();

            qApp->processEvents();
            progress->setValue(i);
            if(progress->wasCanceled())
            {
                qDebug() << "Save progress cancelled";
                break;
            }
        }
        xmlWriter.writeEndElement();

        file.close();
        progress->close();
        delete progress;
        setButtonsDisabled(false);
        QMessageBox::information(0, tr("Сообщение"), tr("Данные успешно сохранены в файл"), QMessageBox::Ok);
        return;
    }
}

void StrGenerator::slotLoadButtonClick()
{
    qDebug() << "slotLoadButtonClick started";
    QXmlStreamReader xmlReader;

    QString filename = QFileDialog::getOpenFileName(this,
                                           tr("Открыть"), "./data",
                                           tr("Xml files (*.xml)"));

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(0, tr("Ошибка"), tr("Не удалось открыть файл"), QMessageBox::Ok);
        return;
    }

    pModel->removeRows(0, records.size());
    records.clear();   

    setButtonsDisabled(true);

    QProgressDialog *progress = new QProgressDialog("", tr("Отмена"), 0, 100);
    progress->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    progress->setWindowModality(Qt::WindowModal); //не работает так, как ожидалось
    progress->setWindowTitle(tr("Загрузка данных из файла"));
    //progress->setMinimumDuration(8000);
    progress->setMinimumWidth(250);
    progress->setRange(0,0);
    progress->setValue(0);
    progress->show();
    xmlReader.setDevice(&file);
    xmlReader.readNext();  // Переходит к первому элементу в файле

    int count=0;
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement()) //Проверяем, является ли элемент началом тега
        {
           if (xmlReader.name() == "item")
           {              
               RecordStructure rs;
               foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
               {                  
                   if (attr.name().toString() == "col1")
                   {
                       rs.rndStr = attr.value().toString();
                   }
                   if (attr.name().toString() == "col2")
                   {
                       rs.rndInt = attr.value().toString().toInt();
                   }
                   if (attr.name().toString() == "col3")
                   {
                       rs.rndDouble = attr.value().toString().toDouble();
                   }
                   if (attr.name().toString() == "col4")
                   {
                       rs.rndBool = (QString::compare(attr.value().toString(), "true") == 0) ? true : false;
                   }
               }
               records.append(rs);
               count++;
               putStructureToModel(*pModel, rs, records.size()-1);

               if ( count % 1000 == 0)
               {
                   pCountRecordsFromTable->setText(QString::number(records.size()));
                   qApp->processEvents();
               }

               progress->setValue(records.size()-1);
               if(progress->wasCanceled())
               {
                   qDebug() << "Load progress cancelled";
                   break;
               }
           }
        }
        xmlReader.readNext();
    }
    file.close();    
    if (xmlReader.hasError())
    {
        QMessageBox::critical(0, tr("Ошибка"), tr("Не удалось распарсить содержимое файла"), QMessageBox::Ok);
        return;
    }
    progress->close();
    delete progress;
    //qDebug() << "Records loaded: " << records.size();
    pCountRecordsFromTable->setText(QString::number(records.size()));

    setButtonsDisabled(false);
    QMessageBox::information(0, tr("Сообщение"), tr("Данные успешно загружены из файла"), QMessageBox::Ok);
    return;
}

void StrGenerator::putStructureToModel(QStandardItemModel & model, RecordStructure & rs, int row)
{
    model.setItem(row,0, new QStandardItem(rs.rndStr));
    model.setItem(row,1, new QStandardItem(QString::number(rs.rndInt)));
    model.setItem(row,2, new QStandardItem(QString::number(rs.rndDouble)));
    model.setItem(row,3, new QStandardItem(QVariant(rs.rndBool).toString()));
}

void StrGenerator::setButtonsDisabled(bool b)
{
    pGenerateButton->setDisabled(b);
    pSaveButton->setDisabled(b);
    pLoadButton->setDisabled(b);
}
