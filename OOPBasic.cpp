#include <iostream>
#include <string>
#include <cstdint>


class Point2D {
public:
    Point2D() = default;
    explicit Point2D(double x, double y)
        :x_(x),
        y_(y)
    {}
    //explicit можно применять для любого конструктора но 
    // имеет смысл только для тех где 1 аргумент
    
    Point2D(double both) {
        x_ = both;
        y_ = both;
    }

    void SetX(double new_x) { x_ = new_x; }
    void SetY(double new_y) { y_ = new_y; }

    double GetX() const { return x_; }
    double GetY() const { return y_; }

    std::string ToString()const {
        std::string result;
        result = "(" + std::to_string(x_) + ", " +
            std::to_string(y_) + ")";
        return result;
    }

    double Shift(const Point2D& other) const {
        return ::sqrt(::pow(::abs(x_ - other.x_), 2) + 
                      ::pow(::abs(y_ - other.y_), 2)
        );
    }

    static double Shift(const Point2D& a, const Point2D& b) {
        return ::sqrt(::pow(::abs(a.x_ - b.x_), 2) +
                      ::pow(::abs(a.y_ - b.y_), 2)
        );
    }

private:
    double x_;
    double y_;
};

std::ostream& operator << (std::ostream& out, const Point2D& obj) {
    out << obj.ToString();
    return out;
}

// здeсь должен быть std::istream& operator >>


class Segment {
public:
    Segment() = default;
    ~Segment() = default;

    Segment(Point2D begin,Point2D end)
        :begin_(begin),
        end_(end)
    {}

    double Length()const {
        return Point2D::Shift(begin_, end_);
    }

    void SetBegin(const Point2D& new_point) {
        begin_ = new_point;
    }

    void SetEnd(const Point2D& new_point) {
        end_ = new_point;
    }

    Point2D GetBegin()const {
        return begin_;
    }

    Point2D GetEnd()const {
        return end_;
    }

    /// bool Intersect(Segment other)

private:
    Point2D begin_;
    Point2D end_;
    //тут мы применяем композицию класов
    // в ходе работы с композицией мы составляем клас из 
    // объектовк других классов без которых он физически 
    // не может существовать
    //
};

class Linear {
public:
    Linear()
        :data_(nullptr),
        size_(0)
    {}
    ~Linear() {
        if (data_) {//if (data_ != nullptr)
            delete[]data_;
            size_ = 0;
        }        
    }
    explicit Linear(int n) {
        if (n < 0) throw - 1;        
        size_ = n;
        data_ = nullptr;
        if (n) {
            data_ = new Point2D[n]{};
        }  
        //если так не сделать то придётся следить 
        // за указателем на блок памяти нулевой длинны
    }

    Point2D& operator[](int n)noexcept{
        return  data_[n];
    }
    const Point2D& operator[](int n)const noexcept  {
        return  data_[n];
    }
    Point2D& at(int n) {
        if (n<0 || n>size_ - 1) {
            throw - 1;
        }
        return  data_[n];
    }
    const Point2D& at(int n)const {
        if (n<0 || n>size_ - 1) {
            throw - 1;
        }
        return  data_[n];
    }

    void PushBack(const Point2D& added) {
        if (!size_) {
            size_++;
            data_ = new Point2D[1]{ added };
        }
        auto tmp = new Point2D[size_ + 1]{};
        for (size_t i = 0; i < size_; i++)
        {
            tmp[i] = data_[i];
        }
        tmp[size_] = added;
        size_ += 1;
        std::swap(tmp, data_);
        delete[]tmp;
    }

    void PopBack() {
        if (size_ == 0 || data_ == nullptr) {
            throw - 1;
        }
        if (size_ == 1) {
            size_ -= 1;
            delete[]data_;
            data_ = nullptr;
            return;
        }
        auto tmp = new Point2D[size_ - 1]{};
        for (size_t i = 0; i < size_-1; i++)
        {
            tmp[i] = data_[i];
        }
        size_ -= 1;
        std::swap(tmp, data_);
        delete[]tmp;
    }

    double Length()const{
        double result{};
        for (size_t i = 0; i < size_-1; i++)
        {
            result += Point2D::Shift(this->at(i), 
                                     this->operator[](i + 1));
        }
        return result;
    }

    // Задание
    //реализуйте следующие методы
    void Insert(const Point2D& added, int pos);
    void Remove(int pos);

    // в начтоящее время я класса проблемы с копированием
    // прочитайте про конструктор копирования и реализуйте его 
    // для класса

private:
    Point2D* data_;
    int size_;
    // В этой ситуации у нас получается что
    // указатель Point2D* - это композиционная часть класса
    // без него объект класса не существует
    // НО вот сами точки - это агрегационный компонент
    // есть они в объекте или нет для объекта линии не важно
    //
};

class Triangle;
class Figure;// Базовй класс для фигур - 3 голоса
             // Просто какой то формы фигура - 0 голосов
class Circle;
class Rectangle;

class Figure {
public:
    virtual double Square()const = 0;
    virtual double Perimetr()const = 0;
    //Такая запись делает метод чистым виртуальным методом
    //Класс в таком случае превращается в абстрактный класс
    // Абстарктные классы не могут иметь собственных объектов.
    // В коде на C++ абстрактные классы которые содержат в себе
    // исключительно чистые виртуальные методы принято называть
    // интерфейсам
    
    virtual ~Figure() = default;
    //виртуальный деструктор по умолчанию нужен нам для корректного
    //управления памятью объектов в будущем
};

class Triangle:public Figure {
public:
    Triangle(const Point2D& a, const Point2D& b, const Point2D& c)
    {
        points_[0] = a;
        points_[1] = b;
        points_[2] = c;
    }
    void SetA(const Point2D& a) {
        points_[0] = a;
    }
    void SetB(const Point2D& b) {
        points_[1] = b;
    }
    void SetC(const Point2D& c) {
        points_[2] = c;
    }

    Point2D GetA()const { return points_[0]; }
    Point2D GetB()const { return points_[1]; }
    Point2D GetC()const { return points_[2]; }

    double Perimetr()const override {
        double res{};
        res += points_[0].Shift(points_[1]);
        res += Point2D::Shift(points_[1], points_[2]);
        res += Point2D::Shift(points_[2], points_[0]);
        return res;
    }

    double Square()const override {
        //S = 0.5 * | x1(y2-y3) + x2(y3-y1) + x3(y1-y2) |
        //  | модуль | - ::abs() из библиотеки cmath
        
        return 0.5 * ::abs(
            points_[0].GetX() * 
                (points_[1].GetY() - points_[2].GetY()) +
            points_[1].GetX() * 
                (points_[2].GetY() - points_[1].GetY()) +
            points_[2].GetX() * 
                (points_[0].GetY() - points_[1].GetY()));
    }
private:
    Point2D points_[3];
};

class Rectange :public Figure {
private:
    Point2D points_[4];
};
class Circle : public Figure {
private:
    Point2D centre_;
    Segment radius_;
};
//Задание 1
// По аналогии с треугольником сделать прямоугольник
//  * проверяйте что углы прямые при создании или изменении сторон
//Задание 2
// подумайте надо ли хранить отдельно точку центра у круга
// реализуйте класс круга но учтите что метод получения 
//      центральной точки должен присутствовать независимо от того 
//      оставите ли вы её в полях класса
//

struct Color {
    unsigned char red;
    unsigned __int8 green;
    uint8_t blue;
};
/*
Вопросы для самопроверки
    1 - почему цвет структура а не класс
    2 - почему цвет кодируется 3мя числами
    3 - что такое uint8_t и зачем он нужен 
*/

class GraphicObjects {
public:
    // конструктор по умолчанию. пишем потому что компилятор даст 
    // не корректную для нас версию
    GraphicObjects() :GraphicObjects(Point2D{0.,0.},nullptr,{0,0,0},false,0) {}
    // при наличии параметрического конструктора мы можем скинуть на него всю работу

    // конструктор с параметрами. просто потому что нам надо работать
    GraphicObjects(
        const Point2D& position,
        Triangle* figure,
        const Color& color,
        bool filled,
        int border)
        :position_(position),
        color_(color),
        filled_(filled),
        border_(border)/*,
        figure_(figure)*/// такая запись будет проблемой изза возможных краевых условий
    {
        if (!figure) {//figure == nullptr
            //throw - 1; // один из вариантов. можно обезопасить себя на будущих этапах
            figure_ == nullptr;// такой вариант только из соображений последующих мер безопасности
        }
        else {
            //figure_ = new Figure{ *figure };
            // такая запись не работает изз того что Figure - абстрактный тип
            //figure_ = figure; //пока единственный вариант при работе с абстрактной фигурой
            figure_ = new Triangle{ *figure };
            // при замене на любую конкретную фигуру - всё ок
        }
    }

    // конструктор копирования. если у нас свои правила хранения то нужно
    // объяснить программе как копировать правильно. нет необходимости если 
    // в полях класса только ссылки и полноценные объекты
    // Данный конструктор создаёт новый объект в виде копии уже существующего
    GraphicObjects(const GraphicObjects& other):GraphicObjects() {
        ::memcpy(this, &other, sizeof(other));
        // по умолчаниб данный конструктор занимается таким вот побитовым копированием 
        // областей памяти. но так как у нас есть управление временем жизни фигуры мы словим ошибки

        this->figure_ = new Triangle{ *(other.figure_) };
        // создаём свой треугольник вместо перехвата владения у соседа

        //теперь при уничтожении переданного объекта наш не может пострадать никак

    }
    GraphicObjects(GraphicObjects&& other):GraphicObjects() {
        std::swap(position_, other.position_);
        std::swap(figure_, other.figure_);
        std::swap(color_, other.color_);
        std::swap(filled_, other.filled_);
        std::swap(border_, other.border_);
    }
    // конструктор переноса подменяет переданный временный объект данными-пустышками так как более
    // никому эти данные в программе не пригодятся и мы из можем спокойно переиспользовать 
    // в своих целях

    GraphicObjects& operator=(const GraphicObjects& other);
    GraphicObjects& operator=(GraphicObjects&& other);

    ~GraphicObjects() {
        if (figure_) {
            delete figure_;
            figure_ = nullptr;//технически именно в деструкторе необязательный шаг
        }
    }

private:
    Point2D position_;
    Triangle *figure_;
    Color color_;
    bool filled_;
    int border_;//толщина границы
};
/*
    1 - почему поле фигуры хранится по указателю(по адресу)
    2 - зачем хранить отдельную точку для позиции а не использовать те которые есть 
        в фигуре
    3 - зачем нужна толщина границы
*/


void Comment() {
    Point2D obj;
    // При вызове функции будет выделено место под хранение объекта - точки
    // когда мы дойдём до момента её создания объект будет создан
    // уничтожится он автоматически когда выйдет из зоны видимости
    {
        Point2D* p_obj;
    // При вызове функции будет выделено место под хранение объекта - Указатель на точку
    // когда мы дойдём до момента её создания объект будет создан
    // уничтожится он автоматически когда выйдет из зоны видимости
    }
    Point2D* p_obj = &obj;
    // При вызове функции будет выделено место под хранение объекта - Указатель на точку
    // когда мы дойдём до момента её создания объект будет создан
    // уничтожится он автоматически когда выйдет из зоны видимости
    // При создании этого указателя в него поместится адрес на объект точки
    // Всё время жизни указателя внутри будет храниться адрес какой либо точки
    
    //Point2D& ref_obj; так не работает так как ссылка - это константный тип и 
    // не может изменить хранящееся значение
    Point2D& ref_obj = obj;
    // При вызове функции будет выделено место под хранение объекта - Ссылка на точку
    // при создании туда обязательно нужно записать данные существующего объекта
    // данными при этом будут способы доступа к объекту а не информация внутри него
    // ссылка будет уничтожена по всем правилам работы переменных но это не повлияет 
    // на оригинальный объект

    // с помощью ссылок можно пользоваться объектами(чаще всего внешними для вашего алгоритма)
    // управление же жизненным циклом объекта через ссылки гарантированно приведёт 
    // к ошибкам исполнения

    // с помощью указателей мы можем: пользоваться объектами, управлять их местом хранения, 
    // управлять их жизненным циклом

    p_obj = nullptr;
    // Мы "забыли" адрес хранимого объекта в указателе - данная операция не влияет на объект
    // чей адрес хранился в указателе только на сам указатель

    p_obj = new Point2D{ 1.2,3.4 };
    // шаги оператора new
    //  1 - выяснить размер необходимый для хранения объекта Point2D ( sizeof(Point2D)  )
    //  2 - запрос у операционной системы блока памяти подходящего размера
    //                  malloc(sizeof(Point2D))
    //  3 - запомнить адрес выделенного блока и вызвать Конструктор класса для создания объекта
    //      в выделенном блоке памяти
    //  4 - вернуть пользователю адрес вновь созданного объекта
    //только после этого оператор присваивания занесёт адрес в наш указатель p_obj

    delete p_obj;
    // шаги оператора delete 
    //  1 - вызывает деструктор у объекта чей адрес был получен в указателе
    //  2 - освобождает область памяти которую ханимал переданный в указателе объект
    //      (делается системный вызов аналог функции free )
    //  3 - Вносит в указатель служебное значение не инициализированного указателя
    // 
    // рассчитывать на точное значение по результату работы delete не стоит так как оно зависит 
    // от компилятора, операционной системы и других факторов. лучше ручками внести значение nullptr
    p_obj = nullptr;

}

int main(){
    //Figure r;
    
    const Point2D a{ 1.2,4.5 };
    Point2D b{ 3.,5. };

    double res{};

    res = a.Shift(b);
    res = b.Shift(a);

    res = Point2D::Shift(a, b);

    Linear line(2);
    line[0] = a;
    line[1] = b;
    Point2D c{ 4.,6. };
    line.PushBack(c);

    Triangle tr{ a,b,c };
}

