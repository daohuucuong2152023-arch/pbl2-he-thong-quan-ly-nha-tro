#ifndef SERVICEIO_H
#define SERVICEIO_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../models/Service.h"
#include "../data_structures/DoublyLinkedList.h"

using namespace std;

inline void luuServiceFile(const DoublyLinkedList<Service>& ds, const string& tenFile) {
    ofstream fout(tenFile);
    fout << fixed << setprecision(2);
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    ds.traverseForward([&fout](const Service& s) {
        fout << s.getMaDichVu() << "|"
             << s.getTenDichVu() << "|"
             << s.getDonGia() << "|"
             << s.getSoLuong() << "\n";
    });
    fout.close();
}

inline DoublyLinkedList<Service> docServiceFile(const string& tenFile) {
    DoublyLinkedList<Service> ds;
    ifstream fin(tenFile);
    if (!fin.is_open()) return ds;

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string ma, ten, giaStr, slStr;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, giaStr, '|');
        getline(ss, slStr, '|');

        ds.insertBack(Service(ma, ten, stod(giaStr), stoi(slStr)));
    }
    fin.close();
    return ds;
}

#endif
