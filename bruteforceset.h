#ifndef BRUTEFORCESET_H
#define BRUTEFORCESET_H

#include <QDialog>
#include <QList>

namespace Ui {
class bruteForceSet;
}

class bruteForceSet : public QDialog
{
    Q_OBJECT

public:
    explicit bruteForceSet(int passLen,QWidget *parent = nullptr);
    ~bruteForceSet();

    QList<QList<QChar> *> *getList() const;

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    QList<QList<QChar>*>* list;
    int dlzka_hesla;
    Ui::bruteForceSet *ui;

    void generate_confStructure();
};

#endif // BRUTEFORCESET_H
