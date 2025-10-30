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

    Point2D operator+(const Point2D& rso)const {
        Point2D result;
        result.x_ = x_ + rso.x_;
        result.y_ = y_ + rso.y_;
        return result;
    }

    friend Point2D operator-(
        const Point2D& lso,
        const Point2D& rso);

    Point2D& operator++() {
        x_ += 1;
        y_ += 1;
        return *this;
    }

    Point2D operator++(int) {
        Point2D result(*this);
        x_ += 1;
        y_ += 1;
        return result;
    }

private:
    double x_;
    double y_;
};

std::ostream& operator << (std::ostream& out, const Point2D& obj) {
    out << obj.ToString();
    return out;
}

Point2D operator-(
    const Point2D& lso,
    const Point2D& rso) {
    Point2D result;
    result.x_ = lso.x_ - rso.x_;
    result.SetX(lso.GetX() - rso.GetX());
    result.y_ = lso.y_ - rso.y_;
    return result;
}

int main(){
    Point2D dummy{ -5.7, 8.6 };
    Point2D point{ 23, -1.2 };
    std::cout << point.ToString() << '\n';

    auto res = dummy + point;
    std::cout << res.ToString() << '\n';
    res = dummy + 5.6;
    std::cout << res.ToString() << '\n';
    //res = 5.6 + dummy; не смотжет сработать
    // если оператор перегружен как метод класса
    res = dummy - point;
    res = dummy - 5.6;
    res = 5.6 - point;
    //так как  -  перегрузка оператора через функцию
    // любая из комбинаций входных данных сработает

    std::cout << res << '\n';
    auto pointer = &res;// & - унарный оператор взятия адреса
    5 * 4; // * бинарнфй оператор умножения
    *pointer;// * унарный оператор разъименования

    int val = -5;
    -val;// унарный минус - возвлащает копию переменной 
         // с противоположным знаком
    +val;// унарный плюс - возвращает копию переменной

    ++val;
    val++;
    ++point;
    point++;

}

