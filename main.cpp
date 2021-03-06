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
#include <fstream>

/*
    Idea es leer datos
    Procesar Oraciones
    Ordenarlas
    y Imprimir el Output
*/

using namespace std;

string toLowerCase(string data){
    int i = 0;
    char c;
    while( data[i]) {
        c = data[i];
        data[i] = tolower(c);
        i++;
    }
    return data;
}

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
    TransformStrategy* process_2;
    TransformStrategy* process_3;
    TransformStrategy* order_;
    OutputStrategy* output_;
    OutputStrategy* output_2;
public:
    KWIC(InputStrategy* input = nullptr, TransformStrategy* process = nullptr, TransformStrategy* process2 = nullptr, TransformStrategy* process3 = nullptr, TransformStrategy* order = nullptr, OutputStrategy* output = nullptr, OutputStrategy* output2 = nullptr) {
        input_ = input;
        process_ = process;
        process_2 = process2;
        process_3 = process3;
        order_ = order;
        output_ = output;
        output_2 = output2;
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
    void setProcess2(TransformStrategy* process) {
        delete this->process_2;
        this->process_2 = process;
    }
    void setProcess3(TransformStrategy* process) {
        delete this->process_3;
        this->process_3 = process;
    }
    void setOrder(TransformStrategy* order) {
        delete this->order_;
        this->order_ = order;
    }
    void setOutput(OutputStrategy* output) {
        delete this->output_;
        this->output_ = output;
    }
    void setOutput2(OutputStrategy* output) {
        delete this->output_2;
        this->output_2 = output;
    }
    void execute() const {
        vector<string> data;
        data = this->input_->execute();
        this->output_->execute(data);
        data = this->process_->execute(data);
        data = this->process_2->execute(data);
        data = this->process_3->execute(data);
        data = this->order_->execute(data);
        this->output_->execute(data);
        data = this->process_->execute(data);
        this->output_2->execute(data);
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
    Input Strategy from file
*/
class FileRead : public InputStrategy
{
public:
    vector<string> execute() const override {
        vector<string> data;
        string fileName;

        cout << "file name pls: ";
        cin >> fileName;
        cout << endl;
        ifstream in(fileName);

        if(!in) {
            cout << "Cannot open input file" << endl;
            return data;
        }

        string line;
        while (getline(in, line)) {
            data.push_back(line);
        }

        in.close();

        return data;
    }
};

/*
    Creamos RemoveLines

    La cual es capaz de obtener una serie de digitos
    que representan las líneas que el usuario quiere borrar
*/
class RemoveLines : public TransformStrategy
{
public:
    vector<string> execute(vector<string> data) const override {
        vector<int> linesToRemove;
        int num;
        cout << "Ingresa las los números de líneas que te gustaría borrar (-1 para terminar)" << endl;
        cin >> num;
        while((int)num != -1) {
            linesToRemove.push_back((int)num);
            cin >> num;
        }

        vector<string> newData;
        int lineCount = 0;
        for(vector<string>::iterator i=data.begin(); i!=data.end(); ++i) {
            if (find(linesToRemove.begin(), linesToRemove.end(), lineCount+1) == linesToRemove.end()){
                newData.push_back(*i);
            }
            lineCount++;
        }
        return newData;
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
    vector<string> execute(vector<string> data) const override {
        vector<string> aux;
        string current;
        for(vector<string>::iterator i=data.begin(); i!=data.end(); ++i) {
            current = *i;
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
        for(vector<string>::iterator i=data.begin(); i!=data.end(); ++i) {
            *i = toLowerCase(*i);
        }

       int num;
       cout << "Quieres que tu sort sea Ascendente o Descendente?" << endl;
       cout << "1 para Ascendente / 2 para Descendente" << endl;
       cin >> num;

       if (num == 2){
           sort(data.begin(), data.end());
           reverse(data.begin(), data.end());
       }
       else{
            //No escogio lo hago ascendente
            sort(data.begin(), data.end());
       }

       return data;
    }
};

class StopWords : public TransformStrategy
{
public:
    vector<string> execute(vector<string> data) const override {
        string ans;
        cout << "quieres usar stop words? (y/n): ";
        cin >> ans;
        if (ans == "y") {
            vector<string> aux;

            // Get stop words from file
            InputStrategy* stopStrat = new FileRead;
            vector<string> stops = stopStrat->execute();

            // Remove from every data string, any ocurrence of the stop
            for(int i=0; i < data.size(); ++i) {
                // String a Vector
                stringstream ss(data[i]);
                istream_iterator<string> begin(ss);
                istream_iterator<string> end;
                vector<string> separatedLine(begin, end);

                for(int j=0; j < separatedLine.size(); j++) {
                    if(find(stops.begin(), stops.end(), toLowerCase(separatedLine[j])) != stops.end()) {
                        separatedLine.erase(separatedLine.begin() + j);
                        j--;
                    }
                }
                string current = "";
                for (vector<string>::iterator it=separatedLine.begin(); it!=separatedLine.end(); ++it)
                    current += *it + ' ';
                aux.push_back(current);
            }

            return aux;
        } else {
            return data;
        }
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

/*
    Imprime en un archivo
*/
class FilePrint : public OutputStrategy
{
public:
    void execute(vector<string> data) const override {
        string outputName;
        cout << "nombre de output pls: ";
        cin >> outputName;
        ofstream outputFile(outputName);

        for(vector<string>::iterator it=data.begin(); it!=data.end(); ++it)
            outputFile << *it << endl;

        outputFile.close();
    }
};

int main()
{
    KWIC* system = new KWIC();
    system->setInput(new FileRead);
    system->setProcess(new RemoveLines);
    system->setProcess2(new StopWords);
    system->setProcess3(new CircularShift);
    system->setOrder(new AlphabeticalOrder);
    system->setOutput(new ConsolePrint);
    system->setOutput2(new FilePrint);

    system->execute();

    return 0;
}
