
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <time.h>
#include <tuple>
#include <bitset>
#include <chrono>

using namespace std;

void initLog(string message)
{
    fstream logFile;

    logFile.open("log.txt", ios::app);

    time_t t = time(0);
    string dt = ctime(&t);

    logFile << endl << dt.substr(0, dt.length() - 1) << " : " << message;

    logFile.close();
}

tuple<int, int, float, long> comparison(fstream& file_A, fstream& file_B) 
{

    int diff = 0, comparison = 0;
    long sizeInBytes = 0L;
    float ber = 0.;
    char a, b;
    string bitSetA, bitSetB;

    initLog(" ----------------------- Różnica -------------------");

    auto start = chrono::high_resolution_clock::now();

    while (!file_A.eof()) 
    {

        a = file_A.get();
        b = file_B.get();
        sizeInBytes++;

        if (a != b) 
            {
                bitSetA = bitset<8>(a).to_string();
                bitSetB = bitset<8>(b).to_string();

                for (int i = 7; i >= 0; i--) 
                    {
                        if (bitSetA[i] != bitSetB[i]) diff++;
                        comparison++;
                    }
             }
        }
    sizeInBytes -= 1;
    ber = float(diff) / (sizeInBytes * 8.) * 100.;

    auto stop = chrono::high_resolution_clock::now();

    initLog("---------------------- Zakończone ----------------------");

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    tuple<int, int, float, long> results = make_tuple(comparison, diff, ber, duration.count());

    return results;
}

int main(int argc, char** argv)
{
    initLog(" Start programu ");

    if (argc != 3)
        {
            initLog(" Podaj dwa argumenty" + to_string(argc - 1)  );

            return 0;
        }
    fstream file_A, file_B;

    file_A.open(argv[1]);
    file_B.open(argv[2]);

    if (!file_A.is_open() || !file_B.is_open())
        {
            initLog(" Nie mozna otworzyc jednego z plikow ! ");

            return 0;
        }
    else
        {
            initLog((string)" otwarty plik" + argv[1] + " i " + argv[2]);
        }

    tuple<int, int, float, long> results = comparison(file_A, file_B);

    string resultMsg = " Ilosc porownanych bitow: " + to_string(get<0>(results)) +
        "; Ilosc roznych bitow: " + to_string(get<1>(results)) +
        "; BER: " + to_string(get<2>(results)) + "%" +
        "; Czas obliczen: " + to_string(get<3>(results)) + " ms";

    initLog(resultMsg);
    cout << resultMsg << endl;

    file_A.close();
    initLog((string)" Plik " + argv[1] + " zamkniety ");
    file_B.close();
    initLog((string)" Plik " + argv[2] + " zamkniety ");

    initLog(" Stop programu ");

    return 0;
}