#ifndef HANDBOOK_H
#define HANDBOOK_H

#include <QObject>
#include <QDate>
#include <QRadioButton>


typedef struct
{
    uint id;
    QString specialization;
    uint universityIndex;
    double competition;
} BrowserRow;

class Handbook
{
public: 
    Handbook();

    // Объявляем операторы сравнения и присваивания
    bool operator!=(const Handbook &other) const;
    bool operator==(const Handbook &other) const;
    bool operator>(const Handbook &other) const;
    bool operator>=(const Handbook &other) const;
    bool operator<(const Handbook &other) const;
    bool operator<=(const Handbook &other) const;
    void operator=(Handbook const &other);


    // Объявляем методы-сеттеры для установки значений полей
    void setId(uint recordId);
    void setUniversityIndex(uint index);
    void setSpecialization(QString value);
    void setCompetition(double value);
    void setTuitionFees(uint value);
    void setMagistracy(bool value);
    void setStartSubmissionDate(QDate value);
    void setEndSubmissionDate(QDate value);


    // Объявляем методы-геттеры для получения значений полей
    uint getId() const;
    uint getUniversityIndex() const;
    QString getUniversity(int universityIndex) const;
    QString getSpecialization() const;
    double getCompetition() const;
    uint getTuitionFees() const;
    bool getMagistracy() const;
    QDate getStartSubmissionDate() const;
    QDate getEndSubmissionDate() const;


private:
    // Поля для хранения данных записи
    uint id;
    uint universityIndex;
    QString universityes[2] = {"ВолгГТУ", "ВолГУ"};
    QString specialization;
    double competition;
    uint tuitionFees;
    bool magistracy;
    QDate startSubmissionDate;
    QDate endSubmissionDate;
};

#endif // HANDBOOK_H
