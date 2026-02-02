#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // устанавливаем первый ВУЗ по умолчанию
    // Отключаем элементы: кнопка удаления и сохранения (нет записей), размер оплаты за обучение, т.к. конкурс изначально равен нулю
    // Отключаем весь функционал полей групБоксом

    resetUi();

    // запрещаем внесение изменений непосредственно в таблицу


    // подключаем кнопки (сохранить, добавить, удалить, заполнить)
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveRecordButton()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addRecordButton()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(removeRecordButton()));
    connect(ui->fillButton, SIGNAL(clicked()), this, SLOT(fillRecordsButton()));
    connect(ui->newFile, SIGNAL(triggered()), this, SLOT(newDatabaseFile()));
    connect(ui->openFile, SIGNAL(triggered()), this, SLOT(openDatabaseFile()));
    connect(ui->saveFile, SIGNAL(triggered()), this, SLOT(saveDatabaseFile()));
    connect(ui->saveAsFile, SIGNAL(triggered()), this, SLOT(saveDatabaseFileAs()));


    // "ловим" изменения записи, конкурса и дат начала и окончания подачи документов
    connect(ui->recordBrowser, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(recordChanged(int, int, int, int)));
    connect(ui->competition, SIGNAL(valueChanged(double)), this, SLOT(competitionChanged(double)));
    connect(ui->startSubmissionDate, SIGNAL(dateChanged(QDate)), this, SLOT(startSubmissionDateChanged(QDate)));
    connect(ui->endSubmissionDate, SIGNAL(dateChanged(QDate)), this, SLOT(endSubmissionDateChanged(QDate)));

}

// заполняем (сохраняем) поля в запись из окна
void MainWindow::fillRecordFields(Handbook * record)
{
    record->setUniversityIndex(getCurrentRadioButton());
    record->setSpecialization(ui->specialization->text());
    record->setCompetition(ui->competition->value());
    record->setTuitionFees(ui->tuitionFees->value());
    record->setMagistracy(ui->magistracy->isChecked());
    record->setStartSubmissionDate(ui->startSubmissionDate->date());
    record->setEndSubmissionDate(ui->endSubmissionDate->date());
}

//
void MainWindow::fillBrowserRow(Handbook record, int row)
{
    // обновляем колонки одной записи
    ui->recordBrowser->setItem(row, 0, new QTableWidgetItem(QString::number(record.getId())));
    ui->recordBrowser->setItem(row, 1, new QTableWidgetItem(record.getSpecialization()));
    ui->recordBrowser->setItem(row, 2, new QTableWidgetItem(record.getUniversity(record.getUniversityIndex())));
    ui->recordBrowser->setItem(row, 3, new QTableWidgetItem(QString::number(record.getCompetition())));
}

void MainWindow::updateUiFields(Handbook record)
{
    // показываем поля текущей записи
    setCurrentRadioButton(record.getUniversityIndex());
    ui->specialization->setText(record.getSpecialization());
    ui->competition->setValue(record.getCompetition());
    ui->tuitionFees->setValue(record.getTuitionFees());
    ui->magistracy->setChecked(record.getMagistracy());
    ui->startSubmissionDate->setDate(record.getStartSubmissionDate());
    ui->endSubmissionDate->setDate(record.getEndSubmissionDate());
}




void MainWindow::removeRecord(int row)
{
    recordChangeDisabled = true;

    uint recordId =  ui->recordBrowser->item(row, 0)->text().toInt();
    handbookRecord.remove(recordId);

    ui->recordBrowser->removeRow(row);

    recordChangeDisabled = false;
}

void MainWindow::addRecord(Handbook * newRecord)
{
    recordChangeDisabled = true;


    // добавляем новую запись в массив с учетом сортировки
    int row = this->handbookRecord.append(*newRecord);

    ui->recordBrowser->insertRow(row);

    // выделяем добавленную запись
    ui->recordBrowser->setCurrentCell(row, 0);
    fillBrowserRow(*newRecord, row);

    // обновляем таблицу и поля

    updateUiFields(*newRecord);

    recordChangeDisabled = false;
}

void MainWindow::updateRow(int row)
{
    uint recordId = ui->recordBrowser->item(row, 0)->text().toInt();
    Handbook * record = new Handbook();


    handbookRecord.record(recordId, *record);


    // обновляем поля
    updateUiFields(*record);
}

void MainWindow::deleteAllRecords()
{
    recordChangeDisabled = true;

    for (int i = ui->recordBrowser->rowCount() - 1; i >= 0 ; i--)
    {
        ui->recordBrowser->removeRow(i);
    }
    handbookRecord.clear();

    recordChangeDisabled = false;
}

void MainWindow::resetUi()
{
    Handbook record;
    updateUiFields(record);

    ui->fillButton->setEnabled(true);
    ui->addButton->setEnabled(true);
    ui->saveButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->groupBox->setEnabled(false);
}

bool MainWindow::canChangeFile()
{
    if (this->handbookRecord.isModified())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Открыть базу данных", "Присутсвуют несохраененные данные, которые могут быть потеряны, вы хотите продолжить?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::insertFilledRow(BrowserRow row)
{
    recordChangeDisabled = true;
    Handbook * record = new Handbook();
    record->setId(row.id);
    record->setSpecialization(row.specialization);
    record->setUniversityIndex(row.universityIndex);
    record->setCompetition(row.competition);
    ui->recordBrowser->insertRow(0);
    fillBrowserRow(*record, 0);
    recordChangeDisabled = false;
}


// Private SLOTS

// сохраняем текущую запись
void MainWindow::saveRecordButton()
{
    Handbook * record = new Handbook();

    uint row = ui->recordBrowser->currentRow();
    uint recordId =  ui->recordBrowser->item(row, 0)->text().toInt();


    fillRecordFields(record);
    record->setId(recordId);





    recordChangeDisabled = true;

    ui->recordBrowser->removeRow(row);

    row = handbookRecord.update(*record);

    ui->recordBrowser->insertRow(row);

    fillBrowserRow(*record, row);

    ui->recordBrowser->setCurrentCell(row, 0);

    recordChangeDisabled = false;


}

void MainWindow::addRecordButton()
{
    // добавляем новую запись
    Handbook * record = new Handbook();
    fillRecordFields(record);
    addRecord(record);

    // обновляем состояние кнопок
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->groupBox->setEnabled(true);


    if (this->handbookRecord.count() > 90)
    {
        ui->fillButton->setEnabled(false);
    }

    if (this->handbookRecord.count() == 100)
    {
        ui->addButton->setEnabled(false);
    }
}

void MainWindow::removeRecordButton()
{
    // удаляем текущую запись

    int row = ui->recordBrowser->currentRow();

    removeRecord(row);

    // переходим на предыдущую/следующую запись
    if (!(row < this->handbookRecord.count() - 1))
    {
        row = this->handbookRecord.count() - 1;
    }

    if (handbookRecord.count() > 0)
    {
        updateRow(row);
        ui->recordBrowser->setCurrentCell(row, 0);
    }

    // обновляем состояние кнопок
    if (this->handbookRecord.count() == 0)
    {
        resetUi();
    }

    if (this->handbookRecord.count() < 91)
    {
        ui->fillButton->setEnabled(true);
    }

    if (this->handbookRecord.count() < 101)
    {
        ui->addButton->setEnabled(true);
    }

}

void MainWindow::fillRecordsButton()
{

    // создаем данные для заполнения
    QString specializations[] = {
        {"Программная инженерия"},
        {"Информатика и вычислительная техника"},
        {"Физика"},
        {"Приборостроение"},
        {"Экономика"},
        {"Реклама и связи с общественностью"},
        {"Менеджмент"},
        {"Бизнес - информатика"},
        {"Прикладная информатика"}
    };
    double competitions[] = {0.0, 0.7, 1.1, 1.2, 4.8, 6.3, 7.7, 9.4, 11.1, 13.6, 15.0};
    QDate startSubmissionDates[] = {
        QDate(2024, 1, 1), QDate(2024, 2, 25), QDate(2024, 3, 8), QDate(2024, 4, 19), QDate(2024, 5, 3),
        QDate(2024, 6, 27), QDate(2024, 7, 15), QDate(2024, 8, 4), QDate(2024, 12, 20), QDate(2024, 12, 31)
    };
    QDate endSubmissionDates[] = {
        QDate(2024, 1, 26), QDate(2024, 3, 6), QDate(2024, 4, 2), QDate(2024, 5, 11), QDate(2024, 6, 1),
        QDate(2024, 7, 9), QDate(2024, 8, 4), QDate(2024, 9, 3), QDate(2025, 1, 12), QDate(2025, 1, 30)
    };


    for (int i = 0; i < 10; i++)
    {
        Handbook * record = new Handbook();

        // задаем случайные данные для записи
        record->setUniversityIndex(rand() % 2);
        record->setSpecialization(specializations[rand() % 9]);
        record->setCompetition(competitions[i]);
        if (competitions[i] > 1)
        {
            record->setTuitionFees(rand() % 20000 + 10000);
        }
        record->setMagistracy(rand() % 2);
        record->setStartSubmissionDate(startSubmissionDates[i]);
        record->setEndSubmissionDate(endSubmissionDates[i]);


        addRecord(record);
    }

    // обновляем состояние кнопок
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->groupBox->setEnabled(true);

    if (this->handbookRecord.count() > 90)
    {
        ui->fillButton->setEnabled(false);
    }

    if (this->handbookRecord.count() == 100)
    {
        ui->addButton->setEnabled(false);
    }
}

void MainWindow::recordChanged(int row, int, int, int)
{
    if (recordChangeDisabled)
    {
        return;
    }
    updateRow(row);
}


int MainWindow::getCurrentRadioButton()
{
    // получаем индекс выбранной радиокнопки
    if (ui->university_1->isChecked()) return 0;
    if (ui->university_2->isChecked()) return 1;
    return 0;
}

void MainWindow::setCurrentRadioButton(int index)
{
    // устанавливаем выбранную радиокнопку по индексу
    if (index == 0) ui->university_1->setChecked(true);
    else if (index == 1) ui->university_2->setChecked(true);

}

void MainWindow::newDatabaseFile()
{
    if (canChangeFile())
    {
        filename = "";
        deleteAllRecords();
        resetUi();
    }
}

void MainWindow::openDatabaseFile()
{
    if (canChangeFile())
    {
        deleteAllRecords();
        QString tmpFilename = QFileDialog::getOpenFileName(this, "Открыть файл...", "", "Файл базы данных (*.hb)");



        if (!tmpFilename.isEmpty())
        {
            filename = tmpFilename;
            bool isFileLoaded = handbookRecord.load(filename);

            if (!isFileLoaded)
            {
                QMessageBox::critical(this, "Ошибка загрузки файла", "Не удалось открыть файл");
            }

            else
            {
                QVector<BrowserRow> browserRow = handbookRecord.records();
                QVectorIterator<BrowserRow> itRow(browserRow);

                itRow.toBack();
                while (itRow.hasPrevious())
                {
                    insertFilledRow(itRow.previous());
                }

                if (handbookRecord.count() > 0)
                {
                    ui->saveButton->setEnabled(true);
                    ui->deleteButton->setEnabled(true);
                    ui->groupBox->setEnabled(true);
                    ui->recordBrowser->setCurrentCell(0, 0);

                    if (this->handbookRecord.count() > 90)
                    {
                        ui->fillButton->setEnabled(false);
                    }

                    if (this->handbookRecord.count() == 100)
                    {
                        ui->addButton->setEnabled(false);
                    }
                }
                else
                {
                    resetUi();
                }
            }
        }
        else
        {
            filename = "";
        }
    }
}

void MainWindow::saveDatabaseFile()
{
    if(filename.isEmpty())
    {
        saveDatabaseFileAs();
    }

    else
    {
        bool isFileSaved = handbookRecord.save(filename);
        if (!isFileSaved)
        {
            QMessageBox::critical(this, "Ошибка сохранения файла", "Не удалось сохранить файл");
        }
    }
}

void MainWindow::saveDatabaseFileAs()
{
    QString tmpFilename = QFileDialog::getSaveFileName(this, "Сохранить как...", "handbook.hb", "Файл базы данных (*.hb)");
    if (!tmpFilename.isEmpty())
    {
        filename = tmpFilename;
        bool isFileSaved = handbookRecord.save(filename);

        if (!isFileSaved)
        {
            QMessageBox::critical(this, "Ошибка сохранения файла", "Не удалось сохранить файл");
        }
    }
}


void MainWindow::competitionChanged(double competitionValue)
{
    // включаем/выключаем поле размера оплаты за обучение
    if (competitionValue <= 1) ui->tuitionFees->setEnabled(false);
    else ui->tuitionFees->setEnabled(true);
}

void MainWindow::endSubmissionDateChanged(QDate endDate)
{
    // устанавливаем дату окончания на 1 день больше, если дата окончания меньше даты начала
    if (endDate <= ui->startSubmissionDate->date())
    {
        ui->endSubmissionDate->setDate(ui->startSubmissionDate->date().addDays(1));
    }
    // устанавливаем дату окончания на 30 дней больше, если дата окончания больше даты начала более, чем на 30 дней
    else if (ui->startSubmissionDate->date().daysTo(endDate) > 30)
    {
        ui->endSubmissionDate->setDate(ui->startSubmissionDate->date().addDays(30));
    }
}

void MainWindow::startSubmissionDateChanged(QDate startDate)
{
    // устанавливаем дату окончания на 1 день больше, если дата окончания меньше даты начала
    if (ui->endSubmissionDate->date() <= startDate)
    {
        ui->endSubmissionDate->setDate(startDate.addDays(1));
    }
    // устанавливаем дату окончания на 30 дней больше, если дата окончания больше даты начала более, чем на 30 дней
    else if (startDate.daysTo(ui->endSubmissionDate->date()) > 30)
    {
        ui->endSubmissionDate->setDate(startDate.addDays(30));
    }
}




MainWindow::~MainWindow()
{
    // освобождаем память и удаляем UI
    delete ui;
}
