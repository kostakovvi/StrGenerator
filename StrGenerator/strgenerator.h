#ifndef STRGENERATOR_H
#define STRGENERATOR_H

#include <QWidget>
#include <QPushButton>
#include <recordstructure.h>
#include <QLabel>
#include <QLineEdit>

class StrGenerator : public QWidget
{
    Q_OBJECT
private:
    /**
     * @brief Вектор records предназначен для хранения сгенерированных строк
     */
    QVector<RecordStructure> records;
    QPushButton* pGenerateButton;
    QPushButton* pSaveButton;
    QPushButton* pLoadButton;
    QLineEdit* pCountRecordsFromLE;

    QLabel* pCountRecordsFromTable;
    QLabel* pCol1ValueFromTable;
    QLabel* pCol2ValueFromTable;
    QLabel* pCol3ValueFromTable;
    QLabel* pCol4ValueFromTable;

public:
    StrGenerator(QWidget *parent = 0);
    void createForm();
    ~StrGenerator();

public slots:
    void slotGenerateButtonClick();
};

#endif // STRGENERATOR_H
