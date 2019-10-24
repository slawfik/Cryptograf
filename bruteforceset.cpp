#include "bruteforceset.h"
#include "ui_bruteforceset.h"
#include <QList>

bruteForceSet::bruteForceSet(int passLen,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bruteForceSet)
{
    ui->setupUi(this);
    dlzka_hesla = passLen;
    generate_confStructure();
}

bruteForceSet::~bruteForceSet()
{
    delete ui;
}

void bruteForceSet::generate_confStructure()
{
    QString structure = "";
    for (int i =0;i<dlzka_hesla;i++) {
        structure += QString::number(i) +tr("--> { A,B,C }\n");
    }
    ui->textEdit_pass->setPlainText(structure);
}

QList<QList<QChar> *> *bruteForceSet::getList() const
{
    return list;
}

void bruteForceSet::on_buttonBox_accepted()
{
    QString read_conf;
    QList<QString> list_riadky;
    int znaky =0,j;

    list = new (QList<QList<QChar>*>);
    for (int i = 0;i<dlzka_hesla;i++) {
        list->push_back(new (QList<QChar>));
    }

    read_conf = ui->textEdit_pass->toPlainText();
    int riadky = read_conf.count('\n');
    list_riadky = read_conf.split('\n');

    if(riadky != dlzka_hesla)
        return;
    for (int i = 0;i<riadky;i++) {
        znaky = list_riadky[i].count(',');
        j = 0;
        while(j<=znaky){
            if(j==0)
                list->operator[](i)->push_back(list_riadky[i].split(',')[j].back());
            else
                list->operator[](i)->push_back(list_riadky[i].split(',')[j].operator[](0));
            j++;
        }
    }
    this->done(1);
}

void bruteForceSet::on_buttonBox_rejected()
{
    this->done(0);
}
