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

int main(){
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
}

