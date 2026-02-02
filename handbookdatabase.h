#ifndef HANDBOOKDATABASE_H
#define HANDBOOKDATABASE_H

#include <QHash>
#include <QVector>
#include "handbook.h"


class HandbookDatabase : protected QHash<uint, Handbook>
{
public:
    // получить кол-во записей
    int count() const;

    // добавить запись в базу данных; возвращается позиция записи в соответствии с порядком сортировки
    int append(Handbook &record);

    // удалить из базы данных запись с заданным идентификатором
    void remove(uint id);

    // изменить запись в базе данных; возвращается новая позиция записи в соответствии с порядком сортировки
    int update(const Handbook &record);

    //возвращает запись (только для чтения) по заданному идентификатору
    void record(uint id, Handbook &record) const;

    //возвратить вектор записей, которые должны отображается в браузере с учетом сортировки записей
    const QVector<BrowserRow> records() const;

    //сохранить данные в заданный файл
    bool save(QString filename) const;

    //загрузить данные из заданного файла
    bool load(QString filename);

    //уничтожает все данные
    void clear();

    //показывает, имеются ли изменения БД после ее загрузки/сохранения
    bool isModified() const;


private:
    QVector<uint> sortedIds;
    uint currentId = 0;
    mutable bool modified = false;

};
#endif // HANDBOOKDATABASE_H
