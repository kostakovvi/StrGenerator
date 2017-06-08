#ifndef STRGENERATOR_H
#define STRGENERATOR_H

#include <QWidget>
#include <QPushButton>
#include <recordstructure.h>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndex>

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

    QTableView* pRecordsTableView;
    QStandardItemModel* pModel;

    /**
     * @brief Функция putStructureToModel осуществляет размещение элементов структуры в модель
     * для отображения в QTableView
     */
    void putStructureToModel(QStandardItemModel&, RecordStructure&, int);

public:
    StrGenerator(QWidget *parent = 0);
    /**
     * @brief Функция createForm производит заполнение формы
     */
    void createForm();
    /**
     * @brief Функция setButtonsDisabled устанавливает запрет или разрешение нажатия кнопок
     * генерации, сохранения и загрузки
     */
    void setButtonsDisabled(bool);
    ~StrGenerator();

public slots:
    void slotGenerateButtonClick();
    void slotOnTableClick(const QModelIndex& );
    void slotSaveButtonClick();
    void slotLoadButtonClick();
};

#endif // STRGENERATOR_H
