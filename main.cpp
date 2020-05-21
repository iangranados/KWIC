#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <bits/stdc++.h>

using namespace std;

class InputStrategy
{
public:
    virtual ~InputStrategy() {}
    virtual vector<string> execute() const = 0;
};

class TransformStrategy
{
public:
    virtual ~TransformStrategy() {}
    virtual vector<string> execute(vector<string> data) const = 0;
};

class OutputStrategy
{
public:
    virtual ~OutputStrategy() {}
    virtual void execute(vector<string> data) const = 0;
};

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
            // string to lower case
            current = this->toLowerCase(*i);

            // Insert original
            aux.push_back(current);

            // string to vector
            stringstream ss(current);
            istream_iterator<string> begin(ss);
            istream_iterator<string> end;
            vector<string> separatedLine(begin, end);

            // Rotate and insert each one
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

class AlphabeticalOrder : public TransformStrategy
{
public:
    vector<string> execute(vector<string> data) const override {
       sort(data.begin(), data.end());

       return data;
    }
};

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
