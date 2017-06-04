#ifndef RECORDSTRUCTURE_H
#define RECORDSTRUCTURE_H

#include <QString>

/**
 * @brief Структура RecordStructure предназначена для хранения сгенерированной записи,
 * включает в себя случайную строку, число типа int, типа double и случайное значение типа bool
 */
struct RecordStructure
{
    QString rndStr;
    int rndInt;
    double rndDouble;
    bool rndBool;
};

#endif // RECORDSTRUCTURE_H
