#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>

using namespace std;

// Класс булев вектор
class BoolV {
    unsigned int v;
    int nbit; // nbit – количество бит вектора
public:
    BoolV(int nn = 1); // формируется булев вектор из nn бит; все биты со значениями 0
    BoolV(const char *); // формируется булев вектор по строке из нулей и единиц
    BoolV(const BoolV &);//конструктор копирования

    void Set1(int); // устанавливает указанный бит в 1
    void Set0(int); // устанавливает указанный бит в 0
    int operator[](int);//возвращает бит из вектора
    BoolV operator=(const BoolV &);
    BoolV operator|(const BoolV &);//побитовая или
    BoolV operator&(const BoolV &);//побитовая И
    BoolV operator~();//инверсия
    bool operator==(const BoolV &); // равенство векторов
    friend ostream &operator<<(ostream &, const BoolV &);
    friend istream &operator>>(istream &, BoolV &);
    int weight();//вес
};

// Класс булева матрица
class BoolM {
    BoolV *bm;
    int m, n; // m – количество строк, n – количество столбцов
public:
    BoolM(int k = 1, int l = 1); // формируется нулевая булева матрица размера kl
    BoolM(const BoolM &);
    ~BoolM() {
        if (bm) {
            delete[] bm;
        }
    }
    BoolV &operator[](int);
    BoolM operator=(const BoolM &);
    friend ostream &operator<<(ostream &, BoolM &);
    int BuildMatrix();//построение матрицы

};

BoolV::BoolV(int nn) {// формируется булев вектор из nn бит; все биты со значениями 0
    nbit = nn;
    v = 0;
}

BoolV::BoolV(const char *c) {// формируется булев вектор по строке из нулей и единиц
    nbit = strlen(c);
    if (nbit <= 32){
        v = 0;
        for (int i = 0; i < nbit; i++) {
            if (c[i] != '0') v = v | (1 << i);
        }
    } else { cout << "Вышло за пределы!!!"; } 
}


BoolV::BoolV(const BoolV &V) {//конструктор копирования
    nbit = V.nbit;
    v = V.v;
}


void BoolV::Set1(int k) {//
    if ((k < nbit) && (k >= 0)) {
        v = v | (1 << k );//вычисляем в каком элементе стоит эл, в каком бите К , сдвигают единицу на место К
    } else { cout << "Вышло за пределы!!!"; }
}

void BoolV::Set0(int k) {//заменяем к бит на 0
    if ((k < nbit) && (k >= 0)) {
        v = v & (~(1 << k ));
    } else { cout << "Вышло за пределы!!!"; }
}

bool BoolV::operator==(const BoolV &newBoolV) {//сравнение
    if (nbit != newBoolV.nbit || v != newBoolV.v) {
        return false;
    }
    return true;
}

int BoolV::operator[](int k) {//возвращает бит из вектора
    if ((k < nbit) && (k >= 0)) {
        if ((v & (1 << k)) == 0) return 0;
        else return 1;
    } else {
        cout << "Вышло за пределы!!!";//за пределами
        return -1;
    }
}

BoolV BoolV::operator=(const BoolV &V) {//один булев вектор приравниваем другому
    if (this != &V) {
        nbit = V.nbit;
        v = V.v;
    }
    return *this;
}

BoolV BoolV::operator|(const BoolV &vec) {//побитовая или
    BoolV copy = *this;
    copy.v |= vec.v;
    return copy;
}

BoolV BoolV::operator&(const BoolV &newBoolV) {//побитовая и
    BoolV vec = *this;
    vec.v = vec.v & newBoolV.v;
    return vec;
}

BoolV BoolV::operator~() {
    BoolV vec = *this;
    vec.v =~vec.v;
    return vec;
}

istream &operator>>(istream &in, BoolV &vec) {//ввод
    char *buff = new char[32];
    in.getline(buff, 32);
    vec = BoolV(buff);
    delete[] buff;
    return in;
}


ostream &operator<<(ostream &out, const BoolV &vec) {//вывод
    BoolV copy = vec;
    for (int i = 0; i < copy.nbit; i++) {
        if (copy[i]  == 0) {
            out << 0;
        } else {
            out << 1;
        }
    }
    return out;
}

int BoolV::weight() {//считаем вес вектора ( колличество 1 )
    int weight = 0;
    BoolV vec = *this;
    for (int i = 0; i < nbit; i++) {
        if (vec[i] != 0) {
            weight++;
        }
    }
    return weight;
}

BoolM::BoolM(int k, int l) {//создаёт будеву матрицу из бул векторов
    m = k;
    bm = new BoolV[m];
    n = l;
    BoolV v(n);
    for (int i = 0; i < m; i++) {
        bm[i] = v;
    }
}



BoolM::BoolM(const BoolM &M) {//конструктор копирования (размерности и строки)
    n = M.n;
    bm = new BoolV[m = M.m];
    for (int i = 0; i < m; i++) {
        bm[i] = M.bm[i];
    }
}

BoolV &BoolM::operator[](int k) {//возвращает строку
    return this->bm[k];
}

BoolM BoolM::operator=(const BoolM &M) {//приравнивание
    if (this != &M) {
        n = M.n;//колличество столбцов
        delete[]bm;
        bm = new BoolV[m = M.m];
        for (int i = 0; i < m; i++) {//пробегаем по строкам и записываем новые строки
            bm[i] = M.bm[i];
        }
    }
    return *this;
}

ostream &operator<<(ostream &out, BoolM &M) {//выводит строки
    for (int i = 0; i < M.m; i++) out << M.bm[i] << '\n';
    return out;
}

int BoolM::BuildMatrix() {//построение матрицы из файла
    ifstream fin("graf.txt");
    if (fin.is_open()) {
        int a, b;
        int m;
        fin >> m;//колличество вершин
        BoolM BM(m, m);//строим пустую матрицу
        do {
            fin >> a;// вершина начала пути
            fin >> b;//вершина конца пути
            BM[a - 1].Set1(b - 1);// установка единицы в ячейку матрицы
        } while (!fin.eof());// пока можем считать с файла
        fin.close();
        *this = BM;
        return m;//возвращаем колличество вершин
    }
    return 0;
}

void topsort(BoolM &M, int m, int *a) {
    BoolV v0(m); // Какие вершины использовали которые уже рассмотрели
    BoolV v1(m); // Вершины в которые нет пути
    BoolV v2(m); // Ответ на данном шаге цикла while
    BoolV NullV(m);//вектор с 0 везде
    int k = 0;
    for (int i = 0; i < m; i++) {//ответы заполняем нулями
        a[i] = 0;
    }
    while (v0.weight() != m) {
        v1 = NullV;//обнуляем на каждом шаге
        for (int i = 0; i < m; i++) {
            v1 = v1 | M[i];//находим вершины в которые у нас есть пути
        }
        v1 = ~v1;// вершины в которые у нас нет пути
        v2 = v1 & (~v0);//нет пути но мы их использовали
        
        if (v2 == NullV) {//условие для неприминимости топологической сортировки
            throw invalid_argument("Error Matrix");
        };
        for (int i = 0; i < m; i++) { // пробегаем по всему v2
            if (v2[i] == 1) {
                a[k] = i + 1;//добавляем вершину в массив ответов
                M[i] = NullV;//удаляем исходящие пути
                k++;//увеличиваем указатель для массива ответов
            }
        }
        v0 = v0 | v2;//добавляем рассмотренные вершины
    }
}

void matrix() {//матрица
    BoolM mat(2,3);
    BoolM mat1;
    int m;
    m = mat1.BuildMatrix();
    cout<<"Файловая матрица: "<< endl << mat1 << endl;
    cout<<"Первая строка матрицы: "<< mat1[1] << endl;
    cout<<"Нулевая матрица: "<< endl << mat << endl;
    mat = mat1;
    cout << endl << mat << endl;
    int *b = new int[m];//массив ответов
    cout << mat1;
    topsort(mat1, m, b);//производим топологическую сортировку
    printf("Ответ: ");
    for (int i = 0; i < m; i++) {//выводим массив ответов
        cout << b[i];
    }
    cout << endl;
}

int main() {
    BoolV vec1(10);
    BoolV vec2 = vec1;
    if (vec1 == vec2){
        cout << vec1 << endl;
        cout << vec2 << endl; 
        cout <<"Вектора равны\n"<< endl;
    }
    vec2.Set1(4);
    if (!(vec1 == vec2)){
        cout << vec1 << endl;
        cout << vec2 << endl; 
        cout <<"Вектора не равны\n"<< endl;
    }
    BoolV vec3 = "110111";
    cout << vec3 << endl;
    cout << "Инверсия вектора: ";
    vec3 = ~vec3;
    cout << vec3 << endl;
    vec1 = "001101";
    vec2 = "110101";
    BoolV Ivec; 
    Ivec = vec1 & vec2;
    BoolV Ilvec;
    Ilvec = vec1 | vec2;
    cout << "Или: " << Ilvec << endl;
    cout << "И: " << Ivec << endl;
    cout << "Третий бит ИЛИвектора: " << Ilvec[3] << endl;
    cout << "Вес ИЛИвектра: " << Ilvec.weight() << endl;
    cout << "\n";
    matrix();
}