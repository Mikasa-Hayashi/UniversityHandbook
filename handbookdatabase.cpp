#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include "handbookdatabase.h"


// получить кол-во записей
int HandbookDatabase::count() const
{
    return QHash::size();
}

// добавить запись в базу данных; возвращается позиция записи в соответствии с порядком сортировки
int HandbookDatabase::append(Handbook &record)
{
    modified = true;

    if (record.getId() == 0)
    {
        record.setId(++currentId);
    }
    QHash::insert(record.getId(), record);

    QVectorIterator<uint> itIds(sortedIds);
    uint index;
    for (index = 0; itIds.hasNext() && QHash::value(itIds.next()) < record; index++);
    sortedIds.insert(index, record.getId());
    return index;
}

// удалить из базы данных запись с заданным идентификатором
void HandbookDatabase::remove(uint id)
{
    modified = true;
    if (id != 0)
    {
        QHash::remove(id);
        sortedIds.removeOne(id);
    }
}

// изменить запись в базе данных; возвращается новая позиция записи в соответствии с порядком сортировки
int HandbookDatabase::update(const Handbook &record)
{
    HandbookDatabase::remove(record.getId());
    return HandbookDatabase::append((Handbook&)record);
}

// возвращает запись (только для чтения) по заданному идентификатору
void HandbookDatabase::record(uint id, Handbook &record) const
{
    QHashIterator<uint, Handbook> i(*this);

    while (i.hasNext() && i.next().key() != id);

    record = i.value();
}

// возвратить вектор записей, которые должны отображается в браузере с учетом сортировки записей
const QVector<BrowserRow> HandbookDatabase::records() const
{
    QVector<BrowserRow> vector;
    QVectorIterator<uint> itIds(sortedIds);

    uint index;

    while (itIds.hasNext())
    {
        index = itIds.next();
        vector << BrowserRow { QHash::value(index).getId(),
                               QHash::value(index).getSpecialization(),
                               QHash::value(index).getUniversityIndex(),
                               QHash::value(index).getCompetition() };
    }
    return vector;
}

// сохранить данные в заданный файл
bool HandbookDatabase::save(QString filename) const
{
    QFile myFile(filename);

    if (myFile.open(QIODevice::WriteOnly))
    {
        QTextStream output(&myFile);
        output << this->currentId << "\n";
        QHashIterator<uint, Handbook> i(*this);

        while (i.hasNext())
        {
            i.next();
            QStringList handbookField = QStringList() << QString::number(i.value().getId())
                                                      << QString::number(i.value().getUniversityIndex())
                                                      << i.value().getSpecialization()
                                                      << QString::number(i.value().getCompetition())
                                                      << QString::number(i.value().getTuitionFees())
                                                      << QString::number(i.value().getMagistracy())
                                                      << i.value().getStartSubmissionDate().toString("yyyy.MM.dd")
                                                      << i.value().getEndSubmissionDate().toString("yyyy.MM.dd");
            output << handbookField.join(";") << "\n";
        }
        modified = false;
        return true;
    }
    return false;
}

// загрузить данные из заданного файла
bool HandbookDatabase::load(QString filename)
{
    QFile myFile(filename);
    if(myFile.open(QIODevice::ReadOnly))
    {
        QTextStream input(&myFile);

        QString line = input.readLine();
        if (line.isEmpty())
        {
            return false;
        }

        currentId = line.toUInt();
        line = input.readLine();

        while(!line.isEmpty())
        {
            QStringList handbookField = line.split(";");
            if (handbookField.length() < 8)
            {
                return false;
            }

            Handbook *record = new Handbook();
            record->setId(handbookField[0].toInt());
            record->setUniversityIndex(handbookField[1].toInt());
            record->setSpecialization(handbookField[2]);
            record->setCompetition(handbookField[3].toDouble());
            record->setTuitionFees(handbookField[4].toUInt());
            record->setMagistracy(handbookField[5].toInt());
            record->setStartSubmissionDate(QDate::fromString(handbookField[6], "yyyy.MM.dd"));
            record->setEndSubmissionDate(QDate::fromString(handbookField[7], "yyyy.MM.dd"));
            this->append(*record);
            line = input.readLine();
        }
        this->modified = false;
        return true;
    }
    return false;
}

// уничтожает все данные
void HandbookDatabase::clear()
{
    QHash::clear();
    sortedIds.clear();
    currentId = 0;
}

// показывает, имеются ли изменения БД после ее загрузки/сохранения
bool HandbookDatabase::isModified() const
{
    return modified;
}
