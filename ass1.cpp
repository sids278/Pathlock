#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

void parseFile(const string &filename, map<string, int> &data)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string name;
        int age;
        while (file >> name >> age)
        {
            data[name] = age;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }
}

void writeToFile(const string &filename, const map<string, int> &data)
{
    ofstream file(filename);
    if (file.is_open())
    {
        for (const pair<const string, int> &entry : data)
        {
            file << entry.first << " " << entry.second << endl;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }
}

int searchAge(const string &name, const map<string, int> &data)
{
    map<string, int>::const_iterator it = data.find(name);
    if (it != data.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}

int main()
{
    map<string, int> data;

    string filename = "/Users/siddharthsharma/Downloads/input.txt";
    parseFile(filename, data);

    map<string, int> sortedData(data.begin(), data.end());

    string outputFilename = "/Users/siddharthsharma/Downloads/output.txt";
    writeToFile(outputFilename, sortedData);

    cout << "Sorted names and ages have been written to " << outputFilename << endl;

    cout << "\nEnter a name to search for: ";
    string searchName;
    getline(cin, searchName);

    int age = searchAge(searchName, sortedData);
    if (age != -1)
    {
        cout << searchName << "'s age is: " << age << endl;
    }
    else
    {
        cout << "Name not found!" << endl;
    }

    return 0;
}
