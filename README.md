Instrucciones de compilacion y ejecucion de codigo:
# Tarea 1: 
"Desarrollo de una librería de Tensores en C++"
# Descripción:
En este proyecto se ha creado una clase principal llamada Tensor que maneja tensores de hasta 3 dimensiones (3D). Además, se ha usado, principalmente, memoria dinámica de arrays dinámicos contiguos para almacenar los datos del tensor. Asimismo, se ha implementado métodos (como arange, zeros, view, apply, etc) para facilitar la realización de operaciones matemáticas a los tensores.
# Requisitos:
- Compilador: g++ (MinGW)
- Versión: 6.3.0
- Estándar: C++14 o superior
- Sistema operativo: Windows
# Estructura del proyecto:
- main.cpp
# Compilación y ejecución: 
- Los Tensores de 1D, 2D y 3D son creados a partir del manejo de un array dinámico contiguo.
- Las funciones estáticas sirven para crear diferentes valores a los Tensores con las dimensiones insertadas correctamente.
- Para facilitar la manipulación y vistas de Tensores se crearon métodos como unsqueeze, view, etc, manteniendo siempre un cuidado adecuado del manejo de memoria.
# Ejemplo de uso:
Creamos un Tensor de dimensiones 1000x20x20 con valores de la funcion random
    Tensor Tensor1=Tensor::random({1000,20,20},0,10);
Lo transformamos a 1000x40 con view
    Tensor Tensor2=Tensor1.view({1000,400});
Creamos un tensor3 de tamaño 400x100 y luego creamos otro tensor4 para aplicar matmul para multiplicar tensor2 y tensor3 yasginarselo a tensor4
    Tensor Tensor3=Tensor::random({400,100},0,10);
    Tensor Tensor4=matmul(Tensor2,Tensor3);
Creamos un Tensor 1x100 y lo sumamos con Tensor4
    Tensor b1=Tensor::random({1,100},0,10);
    Tensor Tensor5=sumar(Tensor4,b1);
Aplicamos relu
    ReLU relu;
    Tensor Tensor6=Tensor5.apply(relu);
  Creamos tensor4 de 100x10 y usamos matmul entre tensor6 y tensor7
    Tensor Tensor7=Tensor::random({100,10},0,10);
    Tensor Tensor8=matmul(Tensor6,Tensor7);
Creamos un tensor9 de 1x10 y lo sumamos con tensor8
    Tensor Tensor9=Tensor::random({1,10},0,10);
    Tensor Tensor10=sumar(Tensor8,Tensor9);
Finalmente, aplicamos la función Sigmoid
    Sigmoid sigmoid;
    Tensor resultado=Tensor10.apply(sigmoid);
# Resultado esperado de salida:

