#ifndef MATRIX_H
#define MATRIX_H
#include <random>
#include <iostream>
#include <complex>

template <typename T>
class Matrix {
public:
    Matrix();  // конструктор по умолчанию
    Matrix(size_t n); // конструктор квадратной матрицы n×n
    Matrix(size_t n, size_t m); // конструктор прямоугольной матрицы n×m
    Matrix(size_t n, size_t m, T rand_min, T rand_max); // конструктор со случайными значениями
    Matrix(size_t n, size_t m, const std::vector<T>& arr); // конструктор из вектора

    // Геттеры для получения размеров:
    size_t get_rows() const; // получить количество строк
    size_t get_columns() const; // получить количество столбцов

    Matrix(const Matrix& m); // // копирует другую матрицу Конструктор копирования 
    Matrix<T>& operator=(const Matrix<T>& m); //  оператор присваивания:  присваивает другую матрицу
    ~Matrix(); // деструктор (удаляет память)

    // Операторы доступа к элементам
    T& operator()(size_t n, size_t m); // получает ссылку на элемент (для изменения)
    T operator()(size_t n, size_t m) const; // получает значение элемента (только для чтения)

    bool operator==(const Matrix<T>& other) const;   // проверка на равенство
    Matrix<T>& operator+=(const Matrix<T>& other);  // сложение с другой матрицей
    Matrix<T>& operator-=(const Matrix<T>& other); // вычитание другой матрицы
    Matrix<T>& operator*=(const Matrix<T>& other);  // умножение на другую матрицу
    Matrix<T>& operator*=(T scalar);   // умножение на число
    Matrix<T>& operator/=(T scalar);  // деление на число

    // Методы работы с матрицей:
    T trace() const; // след матрицы (сумма диагональных элементов)
    T determinant() const;  // определитель матрицы
    void transpose(); // транспонировать матрицу
    Matrix<T> get_transposed() const; // получить транспонированную копию

private:
    void allocate_();      // выделить память под матрицу
    void destroy_();    // освободить память
    void copy_data_(const Matrix& m);  // скопировать данные

    T random_(T min, T max) const; // генерировать случайное число
    bool is_equal_(T a, T b) const;  // сравнить два числа 
    bool is_greaterq_(T a, T b) const;  // проверить a >= b

    inline static std::random_device rd_; // устройство для генерации случайных чисел
    inline static std::mt19937 random_int_ = std::mt19937(rd_());  // генератор случайных чисел
    inline static const double EPSILON_ = 0.0001; // Константа для сравнения чисел с плавающей точкой

    T** data_ = nullptr; // двумерный массив - сама матрица
    size_t rows_; // количество строк
    size_t columns_; // количество столбцов
};

template <typename T>
bool operator==(const Matrix<T>& a,  const Matrix<T>& b) {
    return a==(b); // создает копию и умножает
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m,  T scalar) {
    return Matrix<T>(m) *= scalar; // позволяет писать 5 * matrix
}

template <typename T>
Matrix<T> operator*(T scalar, const Matrix<T>& m) {
    return Matrix<T>(m) *= scalar; // позволяет писать matrix * 5
}

template <typename T>
Matrix<T> operator/(const Matrix<T>& m,  T scalar) {
    return Matrix<T>(m) /= scalar;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2) {
    return Matrix<T>(m1) += m2;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& m1, const Matrix<T>& m2) {
    return Matrix<T>(m1) += m2;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2) {
    return Matrix<T>(m1) *= m2;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
    for (size_t i = 0; i < m.get_rows(); i++) {
        for (size_t j = 0; j < m.get_columns(); j++) {
            os << m(i+1, j+1) << " ";
        }
        os << "\n";
    }
    return os;
}
template <typename T>
Matrix<T>::Matrix(): rows_(), columns_() {} // конструткор 

template <typename T>
Matrix<T>::Matrix(size_t n): rows_(n), columns_(n) { this->allocate_(); } // выделяет память для матрицы n×n

template <typename T>
Matrix<T>::Matrix(size_t n, size_t m): rows_(n), columns_(m) { this->allocate_(); } // выделяет память для матрицы n×m

template <typename T>
size_t Matrix<T>::get_rows() const { return rows_; } // возвращает колво

template <typename T>
size_t Matrix<T>::get_columns() const { return columns_; } // возвращает колво

template <typename T> // Конструктор со случайными значениями:
Matrix<T>::Matrix(size_t n, size_t m, T rand_min, T rand_max): rows_(n), columns_(m) {
    if (!is_greaterq_(rand_max, rand_min)) { // проверяет, что min <= max
        throw std::logic_error("Минимальный элемент должен быть меньше максимального");
    }
    this->allocate_(); // выделяет память
    for (size_t i = 0; i < rows_; i++) {  // для каждой строки
        for (size_t j = 0; j < rows_; j++) { // для каждого столбца
            data_[i][j] = random_(rand_min, rand_max); // заполняет случайным значением
        }
    }
}

template <typename T> // Конструктор из вектора:
Matrix<T>::Matrix(size_t n, size_t m, const std::vector<T>& arr) {
    rows_ = n; // устанавливает количество 
    columns_ = m; // устанавливает количество 
    this->allocate_();
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            data_[i][j] = arr[(i * m) + j]; // берет элемент из вектора
        }
    }
}

template <typename T> // Конструктор копирования:
Matrix<T>::Matrix(const Matrix& m) {
    this->rows_ = m.rows_; // копирует количество строк
    this->columns_ = m.columns_; // копирует количество столбцов
    this->allocate_(); // выделяет новую память
    this->copy_data_(m); // копирует все данные
}

template <typename T> //  Оператор присваивания:
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m) {
    if (this != &m) { // если не присваиваем себе же
        if (this->rows_ != m.rows_ || this->columns_ != m.columns_) {
            this->destroy_(); // удаляем старую память
            this->rows_ = m.rows_;  // устанавливаем новые размеры
            this->columns_ = m.columns_;
            this->allocate_();  // выделяем новую памят
        }
        copy_data_(m);
    }
    return *this; // возвращаем ссылку на себя
}

template <typename T>
Matrix<T>::~Matrix() {
    this->destroy_(); 
}

// Оператор доступа к элементу:
template <typename T>
T& Matrix<T>::operator()(size_t n, size_t m) {
    return this->data_[n - 1][m - 1]; // // возвращает ссылку на элемент (индексация с 1) для изм
}
template <typename T>
T Matrix<T>::operator()(size_t n, size_t m) const {
    return this->data_[n - 1][m - 1]; // возвращает значение по индексу, для чтения
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    if (rows_ != rows_ || columns_ != columns_) { 
        return false; 
    }
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            if (!is_equal_(data_[i][j], other.data_[i][j])) { // Вложенные циклы проходят по всем элементам текущей матрицы (this->data_)
                return false;                                  // сравнивают их с соответствующими элементами другой матрицы (other.data_).
            }
        }
    }
    return true;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &other) {
    if (rows_ != other.rows_ || columns_ != other.columns_) {
        throw std::logic_error("Матрицы разных размеров");  // выбрасывает ошибку
    }
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            data_[i][j] += other.data_[i][j]; // складывает элементы
        }
    } 
    return *this; // возвращает ссылку на себя
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &other) {
    if (rows_ != other.rows_ || columns_ != other.columns_) {
        throw std::logic_error("Матрицы разных размеров");
    }
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            data_[i][j] -= other.data_[i][j];
        }
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T> &other) {
    if (columns_ != other.rows_) { // проверяет совместимость
        throw std::logic_error("Матрицы не совместны");
    }
    Matrix<T> res(rows_, other.columns_); // создает результат
    
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < other.columns_; j++) {
            for (size_t r = 0; r < columns_; r++) { // суммирование
                res.data_[i][j] += data_[i][r] * other.data_[r][j];  // формула умножения
            }
        }
    }
    *this = res; // заменяет себя на результат
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(T scalar) {
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            data_[i][j] *= scalar;
        }
    }
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/=(T scalar) {
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < columns_; j++) {
            data_[i][j] /= scalar;
        }
    }
    return *this;
}

template <typename T>
T Matrix<T>::trace() const {
    if (rows_ != columns_) { // // проверяет, что матрица квадратная
        throw std::logic_error("Матрица не является квадратной");
    }
    T sum = T(); // создает нулевое значение типа T
    for (size_t i = 0; i < rows_; i++) {
        sum += data_[i][i];  // суммирует диагональные элементы
    }
    return sum;
}

template <typename T> // Вычисление определителя:
T Matrix<T>::determinant() const {
    if (rows_ != columns_) {
        throw std::logic_error("Матрица не является квадратной");
    }
    // Рекурсивное вычисление определителя через миноры матрицы
    if (rows_ == 0) return 1; // определитель пустой матрицы = 1
    if (rows_ == 1) return data_[0][0]; // для 1×1
    if (rows_ == 2) return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0]; // для 2×2

    T result = T(); // результат

    for (size_t v = 0; v < rows_; v++) {  // по первому ряду

        Matrix<T> m = Matrix(rows_- 1, columns_ - 1); // минор
             // заполняет минор (матрица без первой строки и v-го столбца)
        
        for (size_t i = 1; i < rows_; i++) {
            size_t z = 0;
            for (size_t j = 0; j < columns_; j++) {
                if (j != v) {
                    m.data_[i-1][z] = data_[i][j];
                    z++;
                }
            }
        }
        if (v % 2 == 0) // чередование знаков
            result += (data_[0][v] * m.determinant());
        else
            result += (-data_[0][v] * m.determinant());
    }

    return result;
}

template <typename T>
void Matrix<T>::transpose() { //Транспонирование:
    T temp = T(); // временная переменная
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = i; j < columns_; j++) {
            temp = data_[i][j]; // только над диагональю
            data_[i][j] = data_[j][i];  // обменивает элементы
            data_[j][i] = temp;
        }
    }
}

template <typename T>
Matrix<T> Matrix<T>::get_transposed() const {
    Matrix<T> m(*this);
    m.transpose();
    return m;
}

template <typename T> //  Выделение памяти:
void Matrix<T>::allocate_() {
    this->data_ = new T*[rows_];  // создает массив указателей (строки)
    for (size_t i = 0; i < rows_; i++) {
        this->data_[i] = new T[columns_]; // для каждой строки создает массив элементов
        for (size_t j = 0; j < columns_; j++) {
            this->data_[i][j] = T(); // инициализирует нулями
        }
    }
}

template <typename T>
void Matrix<T>::destroy_() {
    for (size_t i = 0; i < rows_; i++) {
        delete[] data_[i]; // удаляет каждую строку
    }
    delete[] data_;
}

template <typename T>
void Matrix<T>::copy_data_(const Matrix& m) {
    for (size_t i = 0; i < m.rows_; i++) {
        for (size_t j = 0; j < m.columns_; j++) {
            this->data_[i][j] = m.data_[i][j];
        }
    }
}

template <typename T>
bool Matrix<T>::is_equal_(T a, T b) const { // cравнить два числа
    return a == b;
}

template <typename T>
bool Matrix<T>::is_greaterq_(T a, T b) const { // проверить a >= b
    return a >= b;
}

template <>
bool Matrix<std::complex<float>>::is_greaterq_(std::complex<float> a, std::complex<float> b) const { // Для комплексных чисел нельзя просто сравнить a >= b, 
    return a.real() >= b.real() && a.imag() >= b.imag(); // оэтому мы сравниваем отдельно действительную часть (real()) и мнимую часть (imag())
} // Bозвращается true, если обе части удовлетворяют условию >=

template <>
bool Matrix<std::complex<double>>::is_greaterq_(std::complex<double> a, std::complex<double> b) const {
    return a.real() >= b.real() && a.imag() >= b.imag();
}

template <>
bool Matrix<float>::is_equal_(float a, float b) const { // ля float чисел нельзя просто писать a == b, потому что из-за неточности вычислений они могут немного отличаться
    return (a > b? (a - b): (b - a)) < EPSILON_; // (a > b? (a - b): (b - a)) - это способ вычислить абсолютное значение разности |a - b|
} // Если эта разность меньше EPSILON_ (маленькое число 0.0001), то числа считаются равными

template <>
bool Matrix<double>::is_equal_(double a, double b) const {
    return (a > b? (a - b): (b - a)) < EPSILON_;
}

template <>
bool Matrix<std::complex<float>>::is_equal_(std::complex<float> a, std::complex<float> b) const {
    std::complex<float> dif = (a - b);
    return std::abs(dif) < EPSILON_;
}

template <>
bool Matrix<std::complex<double>>::is_equal_(std::complex<double> a, std::complex<double> b) const {
    std::complex<double> dif = (a - b); // вычисляет разность двух комплексных чисел
    return std::abs(dif) < EPSILON_; // std::abs(dif) - модуль (абсолютное значение) комплексного числа  Если модуль разности меньше EPSILON_, числа считаются равными
}

template <>
int Matrix<int>::random_(int min, int max) const {
    return (random_int_() % (max - min + 1)) + min; // берет остаток от деления, чтобы получить число от 0 до max
}

template <>
float Matrix<float>::random_(float min, float max) const {
    std::uniform_real_distribution<float> dist(min, max); //  создает распределение, которое генерирует случайные числа в диапазоне [min, max]
    return dist(random_int_); // возвращает случайное число из этого распределения
}

template <>
double Matrix<double>::random_(double min, double max) const {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(random_int_);
}

template <>
std::complex<float> Matrix<std::complex<float>>::random_(std::complex<float> min, std::complex<float> max) const {
    std::uniform_real_distribution<float> real_dist(min.real(), max.real()); // случайное число из этого распределения
    std::uniform_real_distribution<float> imag_dist(min.imag(), max.imag()); // создает распределение для мнимой части
    return std::complex<float>(real_dist(random_int_), imag_dist(random_int_)); // оздает комплексное число из двух вещественных Генерирует случайное комплексное число в прямоугольной области на комплексной плоскости
}

template <>
std::complex<double> Matrix<std::complex<double>>::random_(std::complex<double> min, std::complex<double> max) const {
    std::uniform_real_distribution<double> real_dist(min.real(), max.real());
    std::uniform_real_distribution<double> imag_dist(min.imag(), max.imag());
    return std::complex<double>(real_dist(random_int_), imag_dist(random_int_));

    
#endif // MATRIX_H
