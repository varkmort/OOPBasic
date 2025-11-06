#include <iostream>
#include <string>

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

