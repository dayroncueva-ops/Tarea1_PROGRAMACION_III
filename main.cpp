#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

// Integrantes
// Dayron Saiyuk Cueva Loayza
// Axel Robert Portal Ruiz

using namespace std;
class Tensor;
//5.1
class TensorTransform {
public :
    virtual Tensor apply ( const Tensor & t) const = 0;
    virtual ~ TensorTransform () = default ;
};
//3.1
class Tensor {
    vector<size_t> shape;
    double* pMatriz;
    size_t total_size;

public:
    Tensor(const vector<size_t>& shape, const vector<double>& values): shape(shape) {
        // Validar número de dimensiones
        if (shape.size() == 0 || shape.size() > 3) {
            throw invalid_argument("El tensor debe tener entre 1 y 3 dimensiones");
        }
        // Calcular tamaño total
        total_size = 1;
        for (size_t dim : shape) {
            if (dim == 0) {
                throw invalid_argument("Las dimensiones no pueden ser 0");
            }
            total_size *= dim;
        }

        // Validar tamaño de values
        if (values.size() != total_size) {
            throw invalid_argument("El tamaño de values no coincide con el producto de shape");
        }

        // Reservar memoria dinámica
        pMatriz = new double[total_size];
        // Copiar valores (orden contiguo)
        for (size_t i = 0; i < total_size; i++) {
            pMatriz[i] = values[i];
        }
    }

    // Destructor
    ~Tensor() {
        delete[] pMatriz;
    }

    void print() const {
        //Creamos un indice externo para recorrer values
        size_t index=0;
        //Para 1D
        if (shape.size() == 1) {
            size_t x=shape[0];
            for (size_t i = 0; i < x; i++) {
                cout << pMatriz[index++] << " ";
            }
            cout<<"\n";
        }
        //Para 2D
        else if (shape.size() == 2) {
            size_t x = shape[0];
            size_t y = shape[1];
            for (size_t i = 0; i < x; ++i) {
                for (size_t j = 0; j < y; ++j) {
                    cout << pMatriz[index++] << " ";
                }
                cout << "\n";
            }
        }

        //Para 3D

        else if (shape.size() == 3) {
            size_t x = shape[0];
            size_t y = shape[1];
            size_t z = shape[2];

            for (size_t i = 0; i < x; ++i) {
                cout << "Capa " << i << ":\n";
                for (size_t j = 0; j < y; ++j) {
                    for (size_t k = 0; k < z; ++k) {
                        cout << pMatriz[index++] << " ";
                    }
                    cout << "\n";
                }
            }
        }
    }

    //3.2

    static Tensor zeros(const vector<size_t>& shape) {
        size_t total = 1;
        for (size_t dim : shape) total *= dim;
        vector<double> values(total, 0.0);
        return Tensor(shape, values);
    }

    static Tensor ones(const vector<size_t>& shape) {
        size_t total = 1;
        for (size_t dim : shape) total *= dim;
        vector<double> values(total, 1.0);
        return Tensor(shape, values);
    }


    static Tensor random(const vector<size_t>& shape, double min, double max) {
        size_t total = 1;
        for (size_t dim : shape)
            total *= dim;
        vector<double> values(total);
        for (size_t i = 0; i < total; ++i) {
            double r = (double)rand() / RAND_MAX;
            values[i] = min + r * (max - min);
        }
        return Tensor(shape, values);
    }

    static Tensor arange(int start, int end) {
        vector<double>values;
        for (int i=start; i<end; i++) {
            values.push_back(i);
        }
        vector<size_t>shape{(values.size())};
        return Tensor(shape,values);
    }

    //4
    //Constructor de copia
    Tensor (const Tensor & other): shape(other.shape), total_size(other.total_size) {
        pMatriz=new double [total_size];
        for (size_t i=0;i<total_size;i++)
            pMatriz[i]=other.pMatriz[i];
    }
    //Constructor de movimiento
    Tensor ( Tensor && other ) noexcept: shape(move(other.shape)), pMatriz(other.pMatriz),
    total_size(other.total_size) {
        other.pMatriz=nullptr;
        other.total_size=0;
    }
    //Asignacion de copia
    Tensor& operator=(const Tensor& other) {
        // Evitar auto-asignación
        if (this == &other) {
            return *this;
        }
        // Liberar memoria actual
        delete[] pMatriz;
        // Copiar shape y tamaño
        shape = other.shape;
        total_size = other.total_size;
        // Reservar nueva memoria
        pMatriz = new double[total_size];
        // Copiar datos
        for (size_t i = 0; i < total_size; ++i)
            pMatriz[i] = other.pMatriz[i];
        return *this;
    }

    //Asignador de movimiento

    Tensor& operator=(Tensor&& other) noexcept {
        // Evitar auto-asignación
        if (this == &other) {
            return *this;
        }

        // Liberar memoria actual
        delete[] pMatriz;

        // Transferir recursos
        shape = move(other.shape);
        pMatriz = other.pMatriz;
        total_size = other.total_size;

        // Dejar el objeto origen en estado válido
        other.pMatriz = nullptr;
        other.total_size = 0;

        return *this;
    }

    //5.2
    Tensor apply ( const TensorTransform & transform ) const {
        return transform.apply(*this);
    }

    //Hacemos que ReLU y Sigmoid sean clases amigas, para no tener que hacer getters;
    friend class ReLU;
    friend class Sigmoid;

    //6
    Tensor operator+(const Tensor &t) const {
        if (shape!=t.shape)
            throw invalid_argument("Las dimensiones no son compatibles");
        vector<double> values(total_size);
        for (size_t i=0;i<total_size;i++)
            values[i]=pMatriz[i]+t.pMatriz[i];
        return Tensor(shape,values);
    }
    Tensor operator-(const Tensor &t) const {
        if (shape!=t.shape)
            throw invalid_argument("Las dimensiones no son compatibles");
        vector<double> values(total_size);
        for (size_t i=0;i<total_size;i++)
            values[i]=pMatriz[i]-t.pMatriz[i];
        return Tensor(shape,values);
    }
    //Multiplicacion por escalar
    Tensor operator*(double escalar) const {
        vector<double> values(total_size);
        for (size_t i=0;i<total_size;i++)
            values[i]=pMatriz[i]*escalar;
        return Tensor(shape,values);
    }
    //Mutiplicacion por otro Tensor
    Tensor operator*(const Tensor &t)  const{
        if (shape!=t.shape)
            throw invalid_argument("Las dimensiones no son compatibles");
        vector<double> values(total_size);
        for (size_t i=0;i<total_size;i++)
            values[i]=pMatriz[i]*t.pMatriz[i];
        return Tensor(shape,values);
    }

    //7
    //7.1
    Tensor view(const vector<size_t>& dim) {
        if (dim.size() == 0 || dim.size() > 3)
            throw invalid_argument("Dimensiones inválidas");

        size_t new_size = 1;
        for (auto d : dim) {
            if (d == 0)
                throw invalid_argument("Dimensión inválida");
            new_size *= d;
        }

        if (new_size != total_size)
            throw invalid_argument("Size incompatible");

        Tensor result = move(*this); //transferimos memoria
        result.shape = dim;
        return result;
    }

    //7.2

    Tensor unsqueeze(size_t posi) {
        if (shape.size() >= 3)
            throw invalid_argument("No se puede agregar otra dimensión");

        if (posi > shape.size())
            throw invalid_argument("Posicion invalida");

        vector<size_t> new_shape = shape;
        new_shape.insert(new_shape.begin() + posi, 1);

        Tensor result = move(*this); //transferimos memoria

        result.shape = new_shape;

        return result;
    }

    //8
    static Tensor concat(const vector<Tensor>& tensors, size_t dimension) {

        const vector<size_t>& base_shape = tensors[0].shape;
        size_t dims = base_shape.size();

        if (dimension >= dims)
            throw invalid_argument("Eje inválido");

        // Validar compatibilidad
        for (const auto& t : tensors) {
            if (t.shape.size() != dims)
                throw invalid_argument("Dimensiones incompatibles");

            for (size_t i = 0; i < dims; i++) {
                if (i != dimension && t.shape[i] != base_shape[i])
                    throw invalid_argument("Shapes incompatibles para concat");
            }
        }
        // Calcular nuevo shape
        vector<size_t> new_shape = base_shape;
        new_shape[dimension] = 0;

        for (const auto& t : tensors) {
            new_shape[dimension] += t.shape[dimension];
        }

        // Calcular tamaño total
        size_t new_total = 1;
        for (auto d : new_shape)
            new_total *= d;

        vector<double> new_values(new_total);
        // Copiar datos
        size_t index= 0;

        for (const auto& t : tensors) {
            for (size_t i = 0; i < t.total_size; i++) {
                new_values[index++] = t.pMatriz[i];
            }
        }

        return Tensor(new_shape, new_values);
    }

    //9
    friend Tensor dot(const Tensor& a, const Tensor& b);
    friend Tensor matmul(const Tensor& a, const Tensor& b);
    //10
    friend Tensor sumar(const Tensor& A, const Tensor& b);
};
Tensor dot(const Tensor& a, const Tensor& b) {
    if (a.shape.size() != 1 || b.shape.size() != 1)
        throw invalid_argument("dot solo funciona con tensores 1D");

    if (a.shape[0] != b.shape[0])
        throw invalid_argument("Tamaños incompatibles");

    double suma = 0.0;

    for (size_t i = 0; i < a.total_size; i++) {
        suma += a.pMatriz[i] * b.pMatriz[i];
    }
    Tensor resultado({1}, {suma});

    return move(resultado);
}

Tensor matmul(const Tensor& a, const Tensor& b) {
    // Verificar que sean 2D
    if (a.shape.size() != 2 || b.shape.size() != 2)
        throw invalid_argument("matmul solo funciona con tensores 2D");

    size_t d1 = a.shape[0];
    size_t d2 = a.shape[1];
    size_t d3 = b.shape[1];

    // Validar compatibilidad
    if (d2 != b.shape[0])
        throw invalid_argument("Dimensiones incompatibles para matmul");

    vector<double> result_values(d1 * d3, 0.0);

    // Multiplicación matricial
    for (size_t i = 0; i < d1; i++) {
        for (size_t j = 0; j < d3; j++) {
            for (size_t k = 0; k < d2; k++) {
                result_values[i * d3 + j] +=a.pMatriz[i * d2 + k] * b.pMatriz[k * d3 + j];
            }
        }
    }
    Tensor resultado({d1, d3}, result_values);

    return move(resultado);
}


//5.2
class ReLU: public TensorTransform {
public:
    Tensor apply(const Tensor &t) const override {
        vector<double> values(t.total_size);
        for (size_t i=0; i<t.total_size;i++) {
            if (t.pMatriz[i]<0)
                values[i]=0;
            else
                values[i]=t.pMatriz[i];
        }
        return Tensor(t.shape,values);
    }
};

class Sigmoid:public TensorTransform {
public:
    Tensor apply(const Tensor &t) const override {
        vector<double> values(t.total_size);
        for (size_t i=0; i<t.total_size;i++) {
            values[i]=1.0/(1.0+exp(-t.pMatriz[i]));
        }
        return Tensor(t.shape,values);
    }
};
//10
//Necesitamos agregar la siguiente funcion para sumar dos matrices de diferente tamaño, esto porque
//el paso 4 de la seccion 10 lo solicita
Tensor sumar(const Tensor& A, const Tensor& b) {
    size_t filas = A.shape[0];
    size_t cols = A.shape[1];
    vector<double> values(filas * cols);
    for (size_t i = 0; i < filas; i++) {
        for (size_t j = 0; j < cols; j++) {
            values[i*cols + j] =
                A.pMatriz[i*cols + j] + b.pMatriz[j];
        }
    }
    return Tensor({filas, cols}, values);
}


int main() {
    srand(time(nullptr));
    //Pruebas para 3.1
    //Tensor T1({5}, {1,2,3,4,5});              // 1D
    //Tensor T2({2,3}, {1,2,3,4,5,6});          // 2D
    Tensor T3({2,2,2}, {1,2,3,4,5,6,7,8});    // 3D
    //T1.print();
    //T2.print();
    T3.print();
    //Pruebas para 3.2
    /*
    Tensor A = Tensor :: zeros ({2 , 3}) ;
    Tensor B = Tensor :: ones ({3 , 3}) ;
    Tensor C = Tensor :: random ({2 , 2}, 0.0 , 1.0) ;
    Tensor D = Tensor :: arange (0, 6);
    A.print();
    B.print();
    C.print();
    D.print();
    */

    //Prueba para 5
    /*
    Tensor E = Tensor :: arange (-5, 5) ;
    ReLU relu ;
    E.print();
    Tensor F = E.apply(relu); // Aplica polimorfismo en tiempo de ejecucion
    F.print();
    */
    //Prueba para 7.1
    Tensor A = Tensor :: arange (0, 12) ;
    Tensor B = A. view ({3 , 4}) ;
    B.print();

    //Prueba para 7.2
    Tensor T1 = Tensor :: arange (0, 3);
    Tensor T2 = T1.unsqueeze (0) ;
    T2.print();

    //Prueba para 10
    //Paso1
    Tensor Tensor1=Tensor::random({1000,20,20},0,10);
    //Paso2
    Tensor Tensor2=Tensor1.view({1000,400});
    //Paso 3
    Tensor Tensor3=Tensor::random({400,100},0,10);
    Tensor Tensor4=matmul(Tensor2,Tensor3);
    //Paso 4
    Tensor b1=Tensor::random({1,100},0,10);
    Tensor Tensor5=sumar(Tensor4,b1);
    //Paso 5
    ReLU relu;
    Tensor Tensor6=Tensor5.apply(relu);
    //Paso 6
    Tensor Tensor7=Tensor::random({100,10},0,10);
    Tensor Tensor8=matmul(Tensor6,Tensor7);
    //Paso 7
    Tensor Tensor9=Tensor::random({1,10},0,10);
    Tensor Tensor10=sumar(Tensor8,Tensor9);
    //Paso 8
    Sigmoid sigmoid;
    Tensor resultado=Tensor10.apply(sigmoid);
    resultado.print();

    return 0;
}
