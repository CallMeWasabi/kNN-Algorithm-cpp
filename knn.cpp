#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

vector<string> split_string(string &str, string regex)
{
    vector<string> filterString;
    int l = 0;

    for (int i = 0; i < str.length(); i++)
    {
        if (i + 1 == str.length())
        {
            filterString.push_back(str.substr(l, i - l + 1));
        }
        for (int j = 0; j < regex.length(); j++)
        {
            if (str[i] == regex[j])
            {
                filterString.push_back(str.substr(l, i - l));
                l = i + 1;
            }
        }
    }

    return filterString;
}

vector<vector<string>> load_iris()
{
    ifstream inFileBuffer("iris.data");
    string lineBuffer;
    vector<vector<string>> data;

    while (getline(inFileBuffer, lineBuffer))
    {
        data.push_back(split_string(lineBuffer, ","));
    }
    data.pop_back();
    return data;
}

vector<vector<string>> shuffle_data(vector<vector<string>> data, float deviation = 2)
{
    int state = data.size() / deviation;
    srand(time(NULL));
    int f, s;
    for (int i = 0; i < state; i++)
    {
        do
        {
            f = rand() % data.size();
            s = rand() % data.size();
        } while (f == s);
        data[s].swap(data[f]);
    }
    return data;
}

vector<string> kNN(
    vector<vector<float>> &x_train,
    vector<string> &y_train,
    vector<vector<float>> &x_test,
    int k = 1)
{
    vector<string> predicts;
    vector<float> distance;
    for (int i = 0; i < x_test.size(); i++)
    {
        for (int j = 0; j < x_train.size(); j++)
        {
            float d = 0;
            for (int k = 0; k < x_train[j].size(); k++)
            {
                d += pow((x_train[j][k] - x_test[i][k]), 2);
            }
            distance.push_back(sqrt(d));
        }
        int ind = 0;
        float value = distance[0];
        for (int j = 1; j < distance.size(); j++)
        {
            if (distance[j] < value)
            {
                ind = j;
                value = distance[j];
            }
        }
        predicts.push_back(y_train[ind]);
        distance.clear();
    }
    return predicts;
}

void train_test_split(
    vector<vector<string>> &data,
    vector<vector<float>> &x_train, vector<vector<float>> &x_test,
    vector<string> &y_train, vector<string> &y_test)
{
    vector<float> f;
    for (int i = 0; i < data.size() * 0.7; i++)
    {
        for (int j = 0; j < data[i].size() - 1; j++)
        {
            f.push_back(stof(data[i][j]));
        }
        x_train.push_back(f);
        y_train.push_back(data[i][data[i].size() - 1]);
        f.clear();
    }
    for (int i = data.size() * 0.7 + 1; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size() - 1; j++)
        {
            f.push_back(stof(data[i][j]));
        }
        x_test.push_back(f);
        y_test.push_back(data[i][data[i].size() - 1]);
        f.clear();
    }
}

template <typename type>
void print(vector<vector<type>> &data)
{
    for (vector<type> v : data)
    {
        for (type s : v)
        {
            cout << s << ' ';
        }
        cout << endl;
    }
}

int main()
{
    vector<vector<float>> x_train, x_test;
    vector<string> y_train, y_test;

    vector<vector<string>> data = load_iris();
    vector<vector<string>> fit_data = shuffle_data(data);

    train_test_split(fit_data, x_train, x_test, y_train, y_test);
    vector<string> pred = kNN(x_train, y_train, x_test);

    float acc = 0;
    for (int i=0; i<pred.size(); i++) {
        if (y_test[i]==pred[i]) {
            acc++;
        }
    }
    cout << "Total: " << y_test.size() << " Predicts Correct : " << acc << " Acc: " << (acc/y_test.size())*100 << '%';
    return 0;
}