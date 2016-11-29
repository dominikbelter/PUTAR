#include <iostream>
#include <eigen3/Eigen/Dense>
#include <chrono>
#include <math.h>


using namespace std;
using namespace Eigen;


//Deklaracje macierzy
Matrix2d macierz1;
MatrixXd macierz2(2,2);
MatrixXcf macierz3 = MatrixXcf::Random(2,2);
MatrixXd macierzWielka = MatrixXd::Random(1028,1028);
MatrixXd macierzOdczyt = MatrixXd::Random(10,10);

ArrayXf wector(6);
ArrayXf wector2(6);

void dodawanieMacierzy(auto a, auto b)
{
    cout<<"Wynik dodawania macierzy to:\n"<<a+b<<endl;
}

void odejmowanieMacierzy(auto a, auto b)
{
    cout<<"Wynik odejmowania macierzy to:\n"<<a-b<<endl;
}

void mnozenieMacierzySkalar(auto a, auto b)
{
    cout<<"Wynik mnożenia macierzy razy skalar to:\n"<<b*a<<endl;
}

void mnozenieMacierzy(auto a, auto b)
{
    cout<<"Wynik mnożenia macierzy to:\n"<<a*b<<endl;
}

void transpozycjaMacierzy(auto a)
{
    cout<<"Transpozycja macierzy:\n"<<a.transpose()<<endl;
}

void macierzSprzezona(auto a)
{
    cout<<"Macierz sprzezona:\n"<<a.conjugate()<<endl;
}

void macierzDopelniajaca(auto a)
{
    cout<<"Macierz dopełniająca:\n"<<a.adjoint()<<endl;
}

void porownanieMetod(auto a)
{
    chrono::milliseconds czas = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    for(int i=0;i<50000;i++)
    {
        a.transpose();
    }
    chrono::milliseconds czasK = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    cout<<"Proszę państwa, czas wyko N yw A nia tradycyjnej tran S po Z ycji wynosi tyl K O  dokładnie: "<<czasK.count()-czas.count()<<" ms.\n";

    chrono::milliseconds czas2 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    a.transposeInPlace();
    chrono::milliseconds czasK2 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    cout<<"Proszę państwa, czas wykonywania T ransp O zycji w locie wynosi fi G ur A tywnie: "<<czasK2.count()-czas2.count()<<" ms. Dziękuje M y za  O czywistą pa Ń stwa uwagę\n";

    chrono::milliseconds czas3 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    for(int i=0;i<50000;i++)
    {
        a.adjoint();
    }
    chrono::milliseconds czasK3 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    cout<<"Czas wykonywania tradycyjnego dopełnienia: "<<czasK3.count()-czas3.count()<<" ms.\n";

    chrono::milliseconds czas4 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    a.adjointInPlace();
    chrono::milliseconds czasK4 = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch());
    cout<<"Czas wykonywania dopełnienia w locie wynosi: "<<czasK4.count()-czas4.count()<<" ms.\n";
}

void podstawoweOperacjeNaMacierzy(auto mat)
{
    cout << "Suma elementów:\n" << mat.sum()       << endl;
    cout << "Elementy wymnożone:\n" << mat.prod()      << endl;
    cout << "Średnia:      \n" << mat.mean()      << endl;
    cout << "Minimum:  \n" << mat.minCoeff()  << endl;
    cout << "Max:  \n" << mat.maxCoeff()  << endl;
    cout << "Iloczyn elementów na przekątnej:     \n" << mat.trace()     << endl;
}

void macierzOdwrotna(auto a)
{
    cout<<"Macierz odwrotna to:\n"<<a.inverse()<<endl;
    cout<<"Sprawdzenie:\n"<<a.inverse()*a<<endl;
}

void blokZmacierzy()
{
    cout << "Blok macierzy" << endl;
    cout << macierzOdczyt.block<2,2>(1,1) << endl;
}

void odczytKolumnOrazWierszy()
{
    cout<<"Przykładowa macierz do odczytu wierszy i kolumn"<<endl;
    cout<<macierzOdczyt<<endl;
    cout<<"Trzeci wiersz macierzy to:"<<endl;
    cout<<macierzOdczyt.row(2)<<endl;
    cout<<"Dziesiąta kolumna macierzy to:"<<endl;
    cout<<macierzOdczyt.col(9)<<endl;
    cout<<"Dwie lewe kolumny macierzy to:"<<endl;
    cout<<macierzOdczyt.leftCols(2)<<endl;
    cout<<"Dwa ostatnie wiersze macierzy to:"<<endl;
    cout<<macierzOdczyt.bottomRows<2>()<<endl;
}

void szukanieMinOrazMaxMacierzy()
{
    int i,j;
    float minOfM = macierzOdczyt.minCoeff(&i,&j);
    cout<<"Macierz:\n"<<macierzOdczyt<<endl;
    cout << "Min (" << minOfM
         << ") jest na pozycji (" << i << "," << j << ")\n";
    float maxOfM = macierzOdczyt.maxCoeff(&i,&j);
    cout << "Max (" << maxOfM
         << ") jest na pozycji (" << i << "," << j << ")\n";
}

///Vector
void operacjeBlokoweNaWektorze()
{
    cout<<"Wector:"<<endl<<wector<<endl;
    cout << "Trzy pierwsze elementy wector to:" << endl << wector.head(3) << endl;
    cout << "Trzy ostatnie elementy wector to:" << endl << wector.tail<3>() << endl;
    wector.segment(1,4) *= 3;
    cout << "Wector po pomnożeniu środkowych czterech elementów razy 3 to" << endl << wector << endl;
    wector.segment(1,4) /= 3;
    cout << "Wector po podzieleniu środkowych czterech elementów razy 3 to" << endl << wector << endl;
}

void mnozenieWektorow()
{
    cout<<"Wektor 1:\n"<<wector<<endl;
    cout<<"Wektor 2:\n"<<wector2<<endl;
    cout<<"Wynik mnożenia wektorów to:\n"<<wector*wector2<<endl;
}

void szukanieMinOrazMaxWektora()
{
    int i;
    float minOfV = wector2.minCoeff(&i);
    cout<<"Wektor:\n"<<wector2<<endl;
    cout << "Min (" << minOfV
         << ") jest na pozycji (" << i << ")\n";
    float maxOfV = wector2.maxCoeff(&i);
    cout << "Max (" << maxOfV
         << ") jest na pozycji (" << i << ")\n";
}

void mnozenieMacierzyRazyWektor()
{
    Matrix2d mat;
    mat << 1, 2,
           3, 4;
    VectorXd v(2);
    cout<<"Macierz:\n"<<mat<<endl<<"Wektor: \n"<<v<<endl;
    cout<<"Wynik mnożenia macierz razy wektor:\n"<<mat*v<<endl;
    cout<<"Wynik mnożenia wektor razy macierz:\n"<<v.transpose()*mat<<endl;
}


int main()
{
    macierz1<<1,2,3,4;
    macierz2<<5,6,7,8;
    cout<<"Macierz 1:\n"<<macierz1<<endl;
    cout<<"Macierz 2:\n"<<macierz2<<endl;
    dodawanieMacierzy(macierz1,macierz2);
    odejmowanieMacierzy(macierz1,macierz2);
    mnozenieMacierzySkalar(macierz1,0);
    mnozenieMacierzy(macierz1,macierz2);
    transpozycjaMacierzy(macierz1);\
    cout<<"Macierz 3:\n"<<macierz3<<endl;
    macierzSprzezona(macierz3);
    macierzDopelniajaca(macierz3);
    porownanieMetod(macierzWielka);
    podstawoweOperacjeNaMacierzy(macierz1);
    macierzOdwrotna(macierz1);
    blokZmacierzy();
    odczytKolumnOrazWierszy();
    szukanieMinOrazMaxMacierzy();

    cout<<"\nWektory\n";
    wector << 1, 2, 3, 4, 5, 6;
    operacjeBlokoweNaWektorze();
    wector2 << 7, 8, 9, 10, 11, 12;
    mnozenieWektorow();
    szukanieMinOrazMaxWektora();
    mnozenieMacierzyRazyWektor();

    return 0;
}

