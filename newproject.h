#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>

namespace Ui {
class NewProject;
}

class NewProject : public QDialog
{
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = 0);
    ~NewProject();

    QString getMovie();
    QString getFrequency();

private slots:
    void on_toolButton_clicked();

    void on_buttonBox_accepted();

    void on_freq4_clicked();

    void on_freq6_clicked();

    void on_freq8_clicked();

    void on_freq12_clicked();

    void on_freq24_clicked();

private:
    Ui::NewProject *ui;
    QString movie;
    QString frequency;
};

#endif // NEWPROJECT_H
