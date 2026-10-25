#ifndef QUEUEIO_H
#define QUEUEIO_H

#include <fstream>
#include <sstream>
#include <string>
#include "../models/Tenant.h"
#include "../data_structures/Queue.h"

using namespace std;

inline void luuQueueFile(Queue<Tenant>& q, const string& tenFile) {
    ofstream fout(tenFile);
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    int n = q.size();
    for (int i = 0; i < n; i++) {
        Tenant t;
        q.dequeue(t);
        fout << t.getMaNguoiThue() << "|"
             << t.getHoTen() << "|"
             << t.getCccd() << "|"
             << t.getSoDienThoai() << "\n";
        q.enqueue(t);
    }
    fout.close();
}

inline void docQueueFile(Queue<Tenant>& q, const string& tenFile) {
    ifstream fin(tenFile);
    if (!fin.is_open()) return;

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string ma, ten, cccd, sdt;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, cccd, '|');
        getline(ss, sdt, '|');

        q.enqueue(Tenant(ma, ten, cccd, sdt, "", 0, ""));
    }
    fin.close();
}

#endif
