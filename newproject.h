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

private slots:
    void on_toolButton_clicked();

private:
    Ui::NewProject *ui;
    QString movie;
};

#endif // NEWPROJECT_H
