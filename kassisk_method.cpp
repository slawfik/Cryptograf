#include "kassisk_method.h"
#include <QTextStream>
#include <QDebug>
#include <string>
#include <math.h>

QString kassisk_method::getSifrovany_text() const
{
    QString string;
    /*for(int i = 0;i < sifrovany_text.length();i++)
        string.append(QChar(sifrovany_text.operator[](i)+'A'));*/
    return read_allText;
}

long long kassisk_method::getPoced_desifrovanych() const
{
    return poced_desifrovanych;
}

bool kassisk_method::init_outputFstream()
{
    output.open(QFile::WriteOnly);
    if(!output.isOpen())    {
        qDebug() << "Problém s otvorením výstupného súboru";
        return false;
    }
    return true;
}

void kassisk_method::calc_nasobky_dlzky_hesla()
{
    int pom;
    for(int poz2 = 0;poz2 < sifrovany_text.length()-3;poz2++)   {
        for(int poz = poz2+1;poz < sifrovany_text.length()-3;poz++)   {
            if(sifrovany_text[poz2] == sifrovany_text[poz] && sifrovany_text[poz2+1] == sifrovany_text[poz+1]
                    && sifrovany_text[poz2+2] == sifrovany_text[poz+2]){
                pom=poz-poz2;
                if(pom < 50){
                    dlzky->append(pom);
                }
            }

        }
    }
}

kassisk_method::kassisk_method(QString pa_sifrovany_test,QString pathFor_bruteForce_oFile, int pa_krok)
{
    dlzky = new QList<int>;
    read_allText = pa_sifrovany_test;
    set_sifrovanyText();    //premaze neplatne znaky
    krok = pa_krok;
    output.setFileName(pathFor_bruteForce_oFile);

    calc_nasobky_dlzky_hesla();
}

kassisk_method::kassisk_method(QFile& file, int pa_krok)
{
    dlzky = new QList<int>;
    krok = pa_krok;

    output.setFileName(file.fileName().split('.')[0]+"_bruteForce.txt");

    file.open(QFile::ReadOnly);
    if(file.isOpen())  {
        QTextStream in(&file);
        read_allText = in.readAll();
        set_sifrovanyText();    //premaze neplatne znaky
        in.flush();
        file.close();
        calc_nasobky_dlzky_hesla();
    }
}

/*vracia string znakov 65-91*/
QString kassisk_method::rozdel_na_casti(int dlzka_hesla, int part)
{
    QString ret("");
    if(part < 1 || part > dlzka_hesla)
        return QString("Zle parametre pre funkciu \"rozdel_na_casti\"");

    for(int i = 0;i<sifrovany_text.length();i++)    {
        if(i%dlzka_hesla == part-1) {
            ret.append(sifrovany_text.operator[](i)+'A');
        }
    }
    return ret;
}

QString kassisk_method::toString_nasobkyHesla()
{
    QString ret = "Kassiského metóda(3) nasobky hesla: ";
    for(int i = 0;i<dlzky->length();i++)    {
        if(!(i%10))
            ret += '\n';
        ret += QString::number(dlzky->operator[](i)) + " ,";
    }
    ret += '\n';
    return ret;
}

void kassisk_method::cezar(int *heslo, int dlzka_hesla)
{
    QString S_heslo = "PASSWD: ";
    QString ret;
    int posun =0;
    for(int i=0;i<read_allText.length();i++) {
        if(read_allText[i] >= 65 &&  read_allText[i] <= 91)   {
            read_allText[i] = (((sifrovany_text.operator[](posun) +26 - heslo[posun%dlzka_hesla])%26)+'A');
            posun++;
        }
        //ret[i] = (((sifrovany_text.operator[](i) +26 - heslo[i%dlzka_hesla])%26)+'A');

    }
    for(int i=0;i<dlzka_hesla;i++)  {
        S_heslo.append(heslo[i]+'A');
    }
    size_t a = read_allText.toStdString().find('W');
    size_t b = read_allText.toStdString().find('Q');
    if(a == std::string::npos && b == std::string::npos){
        output.write(S_heslo.toLatin1()+'\n'+read_allText.toLatin1()+'\n');
    }
}

void kassisk_method::set_sifrovanyText()
{
    int znak =0;
    sifrovany_text.clear();
    for(int i = 0;i<read_allText.size();i++)    {
        znak = read_allText[i].unicode() - 'A';
        if(znak >= 0 && znak <= 26)
            sifrovany_text.append(znak);
    }
}

void kassisk_method::swap(int row, int destRow)
{
    if(destRow == row)  {
        return;
    }
    int *pom = pocty[row];
    for(int i =2;i>=destRow;i--) {
        pocty[i+1] = pocty[i];
    }
    pocty[destRow] = pom;
}

void kassisk_method::zistiPravdepodobnostiZnakov(QString part,QList<double>&  pravd_part)
{
    int pom;
    double part_sif_txt_len = part.size();
    QString::iterator it;

    for(int i = 65;i<92;i++){
        pom = part.count(QChar(i));
        pravd_part.push_back(static_cast<double>(pom)/part_sif_txt_len);
    }
}

double kassisk_method::vzdialenost_vektorov(bool use_svk_pravd,QList<double>&  pravd_part)
{
    double sum = 0;
    if(use_svk_pravd){
        for(int i = 0;i<pravd_part.size();i++){
            sum += pow(pravd_part[i]- svk_pravdepodobnosti.find(QChar(i+'A')).value(),2);
        }
        return sqrt(sum);
    } else {
        for(int i = 0;i<pravd_part.size();i++){
            sum += pow(pravd_part[i]- en_pravdepodobnosti.find(QChar(i+'A')).value(),2);
        }
        return sqrt(sum);
    }
}

QString kassisk_method::part_cezar(int *heslo, int dlzka_hesla, QString part)
{
    int znak;
    QString ret;
    for(int i=0;i<part.length();i++) {
        znak = part[i].unicode()-'A';
        ret[i] = ((znak+26 - heslo[i%dlzka_hesla])%26)+'A';
    }
    return ret;
}

QString kassisk_method::sum_4_najpocetnejsieZnaky(QString part_sifrovanehoTextu)
{
    QString freqPrav;
    int pocet;
/*    QList<std::tuple<int,int>> pocty = {std::tuple<int,int>(0,0), std::tuple<int,int>(0,0),
                                       std::tuple<int,int>(0,0), std::tuple<int,int>(0,0)} ;
*/
    for(int i=0;i<4;i++)    {
        pocty[i] = new int[2];
        memset(pocty[i],0,sizeof (int)*2);
    }
    QList<int> list;

    for (char i = 65;i<91;i++)   {
        pocet = part_sifrovanehoTextu.count(QChar(i));
        for(int j = 0;j<4;j++) {
            if(pocet > pocty[j][0]){
                swap(3,j);
                pocty[j][0] = pocet;
                pocty[j][1] = i;
                break;
            } else {
                continue;
            }
        }
    }
    freqPrav += QString(QChar(pocty[0][1])+QString("->") + QString::number(pocty[0][0]) + QString(" ,"));
    freqPrav += QString(QChar(pocty[1][1])+QString("->") + QString::number(pocty[1][0]) + QString(" ,"));
    freqPrav += QString(QChar(pocty[2][1])+QString("->") + QString::number(pocty[2][0]) + QString(" ,"));
    freqPrav += QString(QChar(pocty[3][1])+QString("->") + QString::number(pocty[3][0]));

    for(int i=0;i<4;i++)    {
        delete pocty[i];
    }
    return freqPrav;
}

QString kassisk_method::cezar_ret(int *heslo, int dlzka_hesla)
{
    int pop;
    int ps = 0;
    QString ret = read_allText;
    for(int i=0;i<ret.length();i++) {
        pop=ret.operator[](i).unicode()-'A';
        if(pop >= 0 && pop <= 26)   {
            ret[i] = (((pop+26 - heslo[ps%dlzka_hesla])%26)+'A');
            ps++;
        }
    }
    return ret;
}

int kassisk_method::brute_force_kassisk(int dlzka_hesla,int pom)
{
    static int heslo[10];
    //prve volanie funkcie pom=0 inicializuje fstream pre výsledky
    if(pom == 0)    {
        bool fs = init_outputFstream();
        if(!fs)
            return 0;
    }
    if(dlzka_hesla >= 10)
        return 0;

    for(int i = 0;i<26;i++) {
        heslo[pom] = i;
        cezar(&heslo[0],dlzka_hesla);
        if(pom != dlzka_hesla-1)   {
            brute_force_kassisk(dlzka_hesla,pom+1);
        }
    }

    // prve volanie funkcie pom=0 otvorilo fstream aj si ho zatvorí
    if(pom == 0)    {
        output.close();
        return 1;//poced_desifrovanych;
    }
    return 0;
}

void kassisk_method::bruteForce(QList<QList<QChar> *> *mnozin_znakov, int dlzka_hesla, int pom)
{
    static int *heslo;
    QList<QChar>::iterator i;
    //int poz = pom;//dlzka_hesla-pom-1;

    if(pom == 0) {// prvy proces inicializuje
        poced_desifrovanych = 0;
        heslo = new int[dlzka_hesla];
        bool fs = init_outputFstream();
        if(!fs)
            return;
    }

    for(int j = 0;j<mnozin_znakov->operator[](pom)->size();j++) {
        heslo[pom] = mnozin_znakov->operator[](pom)->operator[](j).unicode()-'A';
        if(pom != dlzka_hesla-1)    {
            bruteForce(mnozin_znakov,dlzka_hesla,pom+1);
        } else {
            cezar(&heslo[0],dlzka_hesla);
            poced_desifrovanych++;
            if(poced_desifrovanych%1000)
                qDebug() << "runing bruteforce desifrujem " << QString::number(poced_desifrovanych);
        }
    }
    /*for (i = mnozin_znakov->operator[](poz)->begin(); i != mnozin_znakov->operator[](poz)->end(); i++){
        heslo[pom] = i->unicode()-'A';
        cezar(&heslo[0],dlzka_hesla);
        if(pom != dlzka_hesla-1)
            bruteForce(mnozin_znakov,dlzka_hesla,pom+1);
    }*/

    if(pom == 0) {// prvy proces
        output.close();
        delete heslo;
    }
}

QString kassisk_method::break_passwd(int dlzka_hesla)
{
    QList<double> pravd_part;
    QString part;
    QString desifrovany_part;
    double min_SK,min_EN;
    int index_SK,index_EN;
    QList<double> pravdepodobnosti_znakov_SK;
    QList<double> pravdepodobnosti_znakov_EN;
    QString heslo_SK,heslo_EN;

    for(int i =1;i<dlzka_hesla+1;i++) {
        part = rozdel_na_casti(dlzka_hesla,i);
        for(int j = 0;j<27;j++)    {
            pravd_part.clear();
            desifrovany_part = part_cezar(&j,1,part);
            zistiPravdepodobnostiZnakov(desifrovany_part,pravd_part);
            pravdepodobnosti_znakov_SK.push_back(vzdialenost_vektorov(true,pravd_part));
            pravdepodobnosti_znakov_EN.push_back(vzdialenost_vektorov(false,pravd_part));
        }
        min_SK = pravdepodobnosti_znakov_SK.first();
        min_EN = pravdepodobnosti_znakov_EN.first();
        index_SK = 0;
        index_EN = 0;
        for(int j = 0;j<pravdepodobnosti_znakov_SK.size();j++)  {
            if(min_SK > pravdepodobnosti_znakov_SK[j])   {
                min_SK = pravdepodobnosti_znakov_SK[j];
                index_SK = j;
            }
        }
        for(int j = 0;j<pravdepodobnosti_znakov_EN.size();j++)  {
            if(min_EN > pravdepodobnosti_znakov_EN[j])   {
                min_EN = pravdepodobnosti_znakov_EN[j];
                index_EN = j;
            }
        }
        pravdepodobnosti_znakov_SK.clear();
        pravdepodobnosti_znakov_EN.clear();
        heslo_SK.append(QChar(index_SK+65));
        heslo_EN.append(QChar(index_EN+65));
        //zistiPravdepodobnostiZnakov(dlzka_hesla,i,pravd_part);
        //dlzky vektorov
    }
    return "heslo_SK: " + heslo_SK + "\nheslo_EN: " + heslo_EN;
}

QList<int> *kassisk_method::getDlzky() const
{
    return dlzky;
}

kassisk_method::~kassisk_method()
{
    delete dlzky;
}
