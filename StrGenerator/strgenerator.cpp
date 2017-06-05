#include "strgenerator.h"
#include <QDebug>
#include <QBoxLayout>
#include <QGroupBox>
#include <QTableView>
#include <QMessageBox>

/**
 * @brief Глобальная переменная countRecordsToGenerate предназначена для хранения числа записей при нажатии кнопки генерации
 */
int countRecordsToGenerate;

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
    return ((double)qrand()*(max - min))/(double)RAND_MAX + min;
}

StrGenerator::StrGenerator(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "StrGenerator constructor started";
    createForm();

    connect(pGenerateButton, SIGNAL(clicked()), SLOT(slotGenerateButtonClick()));
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

     hbxLayout->addWidget(recordsTableView);
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
    }
    qDebug() << "records.size(): " << records.size();
    pCountRecordsFromTable->setText(QString::number(records.size()));

}
