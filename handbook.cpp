#include "handbook.h"

// Конструктор класса Handbook. Устанавливает значения по умолчанию для всех полей
Handbook::Handbook()
{
    id = 0;
    universityIndex = 0;
    specialization = "";
    competition = 0;
    tuitionFees = 10000;
    magistracy = false;
    startSubmissionDate = QDate(2024, 1, 1);
    endSubmissionDate = QDate(2024, 1, 2);
}

// Оператор сравнения "не равно"
bool Handbook::operator!=(const Handbook &other) const
{
    return !(*this == other);
}

// Оператор сравнения "равно"
bool Handbook::operator==(const Handbook &other) const
{
    return (universityIndex == other.universityIndex &&
            specialization == other.specialization &&
            competition == other.competition &&
            tuitionFees == other.tuitionFees &&
            magistracy == other.magistracy &&
            startSubmissionDate == other.startSubmissionDate &&
            endSubmissionDate == other.endSubmissionDate);
}

// Оператор сравнения "больше". Учитывает конкуренцию, индекс университета и специализацию
bool Handbook::operator>(const Handbook &other) const
{
    if (competition != other.competition) return competition < other.competition;
    else if (universityIndex != other.universityIndex) return universityIndex > other.universityIndex;
    else return specialization > other.specialization;

}

// Оператор сравнения "меньше". Учитывает конкуренцию, индекс университета и специализацию
bool Handbook::operator<(const Handbook &other) const
{   
    if (competition != other.competition) return competition > other.competition;
    else if (universityIndex != other.universityIndex) return universityIndex < other.universityIndex;
    else return specialization < other.specialization;
}

// Оператор сравнения "больше или равно"
bool Handbook::operator>=(const Handbook &other) const
{
    return !(*this < other);
}

// Оператор сравнения "меньше или равно"
bool Handbook::operator<=(const Handbook &other) const
{
    return !(*this > other);
}

// Оператор присваивания
void Handbook::operator=(const Handbook &other)
{
    id = other.id;
    universityIndex = other.universityIndex;
    specialization = other.specialization;
    competition = other.competition;
    tuitionFees = other.tuitionFees;
    magistracy = other.magistracy;
    startSubmissionDate = other.startSubmissionDate;
    endSubmissionDate = other.endSubmissionDate;
}


// Установить ID записи
void Handbook::setId(uint recordId)
{
    id = recordId;
}

// Устанавливает индекс университета
void Handbook::setUniversityIndex(unsigned int index)
{
    if (index < 2) universityIndex = index;
}

// Устанавливает специализацию, если длина строки не превышает 50 символов
void Handbook::setSpecialization(QString value)
{
    if (value.length() <= 50) specialization = value;
}

// Устанавливает значение конкуренции в пределах от 0 до 15
void Handbook::setCompetition(double value)
{
    if (value >= 0 && value <= 15) competition = value;
}

// Устанавливает стоимость обучения в пределах от 10 000 до 30 000
void Handbook::setTuitionFees(unsigned int value)
{
    if (value >= 10000 && value <= 30000) tuitionFees = value;
}

// Устанавливает наличие магистратуры
void Handbook::setMagistracy(bool value)
{
    magistracy = value;
}

// Устанавливает дату начала подачи заявок, если год — 2024
void Handbook::setStartSubmissionDate(QDate value)
{
    if (value.year() == 2024) startSubmissionDate = value;
}

// Устанавливает дату окончания подачи заявок в пределах 2 января 2024 — 30 января 2025
void Handbook::setEndSubmissionDate(QDate value)
{
    if (value >= QDate(2024, 1, 2) && value <= QDate(2025, 1, 30))
        endSubmissionDate = value;
}


// Возвращает ID записи
uint Handbook::getId() const
{
    return id;
}

// Возвращает индекс университета
uint Handbook::getUniversityIndex() const
{
    return universityIndex;
}

// Возвращает название университета по индексу
QString Handbook::getUniversity(int universityIndex) const
{
    return universityes[universityIndex];
}

// Возвращает специализацию
QString Handbook::getSpecialization() const
{
    return specialization;
}

// Возвращает значение конкуренции
double Handbook::getCompetition() const
{
    return competition;
}

// Возвращает стоимость обучения
unsigned int Handbook::getTuitionFees() const
{
    return tuitionFees;
}

// Возвращает наличие магистратуры
bool Handbook::getMagistracy() const
{
    return magistracy;
}

// Возвращает дату начала подачи заявок
QDate Handbook::getStartSubmissionDate() const
{
    return startSubmissionDate;
}

// Возвращает дату окончания подачи заявок
QDate Handbook::getEndSubmissionDate() const
{
    return endSubmissionDate;
}
