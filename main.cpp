//Ian Granados
//Andrea Tamez
//Emilio 
//Martin

/*
    La razon por la cual
    se ven varias interfaces es porque 
    cada una de ellas se dedica 
    a un proposito especifico.
    
    Esto es para poder mantener la consistencia ya que 
    unas de ellas reciben parametros y otras no, al mismo
    tiempo algunas de ellas tienen valor de retorno y otras no.
    
    Debido a que hay diferencias en como hacer el proceso 
    decidimos crear diferentes interfaces para cuidar la 
    coherencia de nuestro resultado y del proceso.
*/


//Agregamos librerias necesarias
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <bits/stdc++.h>

/*
    Idea es leer datos
    Procesar Oraciones
    Ordenarlas
    y Imprimir el Output
*/

using namespace std;

/* 
    Creamos clase Input Strategy
    En esta clase recibimos el vector de string que se va utilizar
    para la transformacion. 
    
    +execute():Vector
*/
class InputStrategy
{
public:
    virtual ~InputStrategy() {}
    virtual vector<string> execute() const = 0;
};

/* 
    Creamos clase TransformStrategy
    En esta clase se hace la transformacion. 
    
    Recibimos los datos para poder creaer la transformacion
    adecuada.
    
    +execute(data):Vector
*/
class TransformStrategy
{
public:
    virtual ~TransformStrategy() {}
    virtual vector<string> execute(vector<string> data) const = 0;
};

/* 
    Creamos clase Output Strategy
    En esta clase que se tiene lo que se 
    imprimir.
    
    Recibimos los datos para poder creaer el output
    adecuado.
    
    +execute(data)
*/
class OutputStrategy
{
public:
    virtual ~OutputStrategy() {}
    virtual void execute(vector<string> data) const = 0;
};

/* 
    Creamos clase KWIC 
    la cual utiliza todas las clases creadas.

    Recibe el input por medio de la clase Input Strategy.
    El proceso y el orden de la transformacion proviene de la clase
    TransformStrategy.
    Y el Output proviene del Output strategy.
*/
class KWIC
{
private:
    InputStrategy* input_;
    TransformStrategy* process_;
    TransformStrategy* order_;
    OutputStrategy* output_;
public:
    KWIC(InputStrategy* input = nullptr, TransformStrategy* process = nullptr, TransformStrategy* order = nullptr, OutputStrategy* output = nullptr) {
        input_ = input;
        process_ = process;
        order_ = order;
        output_ = output;
    }
    ~KWIC() {
        delete this->input_;
        delete this->process_;
        delete this->order_;
        delete this->output_;
    }
    void setInput(InputStrategy* input) {
        delete this->input_;
        this->input_ = input;
    }
    void setProcess(TransformStrategy* process) {
        delete this->process_;
        this->process_ = process;
    }
    void setOrder(TransformStrategy* order) {
        delete this->order_;
        this->order_ = order;
    }
    void setOutput(OutputStrategy* output) {
        delete this->output_;
        this->output_ = output;
    }
    void execute() const {
        vector<string> data;
        data = this->input_->execute();
        data = this->process_->execute(data);
        data = this->order_->execute(data);
        this->output_->execute(data);
    }
};


/* 
    Creamos ConsoleRead 
    la cual recibe lo que queremos leer 
    para utilizarlo en nuestro programa.
*/
class ConsoleRead : public InputStrategy
{
public:
    vector<string> execute() const override {
        vector<string> data;
        string line;
        do {
            getline(cin, line);
            if (line != "") data.push_back(line);
        } while (line != "");
        return data;
    }
};

/* 
    Creamos CircularShift 
    
    La cual empieza a hacer la rotacion circular
    utilizando los datos recibidos.
*/
class CircularShift : public TransformStrategy
{
public:
    string toLowerCase(string data) const{
        int i = 0;
        char c;
        while( data[i]) {
            c = data[i];
            data[i] = tolower(c);
            i++;
        }
        return data;
    }

    vector<string> execute(vector<string> data) const override {
        vector<string> aux;

        string current;
        for(vector<string>::iterator i=data.begin(); i!=data.end(); ++i) {
            // String a Minuscula
            current = this->toLowerCase(*i);

            // Inserto el Original
            aux.push_back(current);

            // String a Vector
            stringstream ss(current);
            istream_iterator<string> begin(ss);
            istream_iterator<string> end;
            vector<string> separatedLine(begin, end);

            // Roto y inserto cada una
            for (int j = 1; j < separatedLine.size(); ++j) {
                rotate(separatedLine.begin(), separatedLine.begin()+1, separatedLine.end());
                current = "";
                for (vector<string>::iterator it=separatedLine.begin(); it!=separatedLine.end(); ++it)
                    current += *it + ' ';
                aux.push_back(current);
            }
        }
        return aux;
    }
};
/* 
    Creamos clase Aplphbetical Order
    
    que sortea los strings que tenemos.
*/
class AlphabeticalOrder : public TransformStrategy
{
public:
    vector<string> execute(vector<string> data) const override {
       sort(data.begin(), data.end());

       return data;
    }
};
/* 
    Creamos ConsolePrint
    
    la cual imprime lo que tenemos del Output Strategy
*/
class ConsolePrint : public OutputStrategy
{
public:
    void execute(vector<string> data) const override {
        for(vector<string>::iterator it=data.begin(); it!=data.end(); ++it)
            cout << *it << endl;
    }
};

int main()
{
    KWIC* system = new KWIC();
    system->setInput(new ConsoleRead);
    system->setProcess(new CircularShift);
    system->setOrder(new AlphabeticalOrder);
    system->setOutput(new ConsolePrint);

    system->execute();

    return 0;
}
