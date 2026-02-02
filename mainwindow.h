#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QTableWidget>

#include "handbook.h"
#include "handbookdatabase.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    HandbookDatabase handbookRecord;
    QFile * currentFile = NULL;
    QString filename = "";

    unsigned int recordCount = 0;

    bool recordChangeDisabled = false;


    // Создаем сеттер и геттер радиокнопок
    int getCurrentRadioButton();
    void setCurrentRadioButton(int);

    // Создаем методы для работы с данными записи
    void fillRecordFields(Handbook * record);
    void fillBrowserRow(Handbook record, int row);
    void updateUiFields(Handbook record);

    void removeRecord(int removeIndex);
    void addRecord(Handbook * newRecord);


    void updateRow(int row);

    void deleteAllRecords();

    void resetUi();

    bool canChangeFile();

    void insertFilledRow(BrowserRow row);


private slots:
    // Создаем методы-слоты для работы с записями
    void saveRecordButton();
    void addRecordButton();
    void removeRecordButton();
    void fillRecordsButton();
    void recordChanged(int currentRow, int, int, int);
    void newDatabaseFile();
    void openDatabaseFile();
    void saveDatabaseFile();
    void saveDatabaseFileAs();

    // Создаем методы-слоты для обработки изменений данных
    void competitionChanged(double);
    void endSubmissionDateChanged(QDate);
    void startSubmissionDateChanged(QDate);
};



#endif // MAINWINDOW_H
