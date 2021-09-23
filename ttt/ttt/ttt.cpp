#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<cstdlib>
#include<sstream>
#include <Windows.h>
#include <filesystem>
#include<thread>
#include<mutex>
#include<conio.h>

using namespace std;


class localobject {
public:
    void get_lock() {
        unique_lock<mutex>local_lock(m_mutex);
        m_lock = move(local_lock);
    }
    void release_lock() {
        unique_lock<mutex>local_lock = move(m_lock);
    }
private:
    mutex m_mutex;
    unique_lock<mutex>m_lock;
};





void turing_handler(string file_name, localobject* lclobj) {


    //SUKURIAMI PAGR KINTAMIEJI

    int steps = 0;
    string eilute;
    string* eilute_masyvu;
    int currant_poz;
    string currant_condition;
    string prev_condition;
    vector<string>kodas;
    //failas
    ifstream input(file_name);
    //eilute
    input >> eilute;
    eilute_masyvu = new string[size(eilute)];
    for (int i = 0; i < size(eilute); ++i) {
        eilute_masyvu[i].assign(1, eilute[i]);
    }
    //pradzios pozicija ir kodas
    input >> currant_poz;
    currant_poz -= 1;
    while (input.peek() != EOF) {
        string temp;
        input >> temp;
        kodas.push_back(temp);
    }



    currant_condition = kodas[0];

    //MAIN CYCLE
    lclobj->get_lock();
    int halt_counter = 0;
    cout << file_name << endl;
    while (true) {



        int flag = 0;

        //ieskom
        for (int i = 0; i < kodas.size(); i += 5) {
            if (currant_condition == kodas[i] && kodas[i + 1] == eilute_masyvu[currant_poz]) {
                //update condition
                currant_condition = kodas[i + 4];
                //change symbol
                eilute_masyvu[currant_poz] = kodas[i + 2];
                //move
                if (kodas[i + 3] == "R") {
                    if (currant_poz == kodas.size() - 1) { cout << "pazeisti reziai" << endl; flag = 1;  break; }
                    else { currant_poz += 1; };
                }
                else {
                    if (currant_poz == 0) { cout << "pazeisti reziai" << endl; flag = 1; break; }
                    else { currant_poz -= 1; };
                };
                halt_counter = 0;
                break;
            }
            else
            {
                halt_counter += 1;
            }
        }
        if (flag == 1)break;


        //HALTING PROBLEM
        //nerasti nurodymai
        if (halt_counter == kodas.size() / 5) {
            cout << " " << currant_condition << " busenai ir " << eilute[currant_poz] << " simbliui nebuvo suteikti jokie nurodymai" << endl;
            break;
        }
        //check if infinite
        if (_kbhit()) {
            cout << " sustabdyta rankiniu budu " << endl;
            break;
        }

        //ISVESTIS
        //isvedam eilute        

        cout << "\r";
        for (int j = 0; j < size(eilute); ++j) {
            cout << eilute_masyvu[j];
        }
        steps += 1;

    }
    while (_kbhit())_getch();
    lclobj->release_lock();

}

int main()
{
    bool repeat = true;
    while (repeat) {


        localobject* testobj = new localobject;
        //suvedam failus
        vector<string>failu_pav;
        string inp;
        cout << "iveskite norimus failus, noredami baigti ivedima iveskite 'stop'" << endl;
        cout << "pamate begalini cikla ir noredami sustabdyti veikianti faila paspauskite bet kuri klavisa" << endl;

        cin >> inp;
        while (inp != "stop") {
            failu_pav.push_back(inp);
            cin >> inp;
        }
        //kuriam threads
        thread* thread_array;
        int size_vec_thr = failu_pav.size();
        thread_array = new thread[size_vec_thr];



        for (int i = 0; i < size_vec_thr; ++i) {
            thread_array[i] = thread(turing_handler, failu_pav[i], testobj);
        }
        for (int i = 0; i < size_vec_thr; ++i) {
            cout << endl;
            if (thread_array[i].joinable()) {
                thread_array[i].join();
            }

        }



        cout << "noredami kartoti darba iveskita 'taip', bet kokia kita ivestis bus traktuojama kaip 'ne'" << endl;
        string ats;
        cin >> ats;
        if (ats == "taip")repeat = true;
        else { repeat = false; };

    }




}
