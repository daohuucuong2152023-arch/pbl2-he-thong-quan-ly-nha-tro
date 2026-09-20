#ifndef CONTRACTIO_H
#define CONTRACTIO_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../models/Contract.h"
#include "../data_structures/DoublyLinkedList.h"

using namespace std;

// Dinh dang 1 dong trong contract.txt:
// maHopDong|maPhong|maNguoiThue|ngayBatDau|thoiHan|tienCoc|trangThai

inline void luuContractFile(const DoublyLinkedList<Contract>& ds, const string& tenFile) {
    ofstream fout(tenFile);
    fout << fixed << setprecision(2); // tranh in so dang khoa hoc (VD: 2.5e+06)
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    ds.traverseForward([&fout](const Contract& hd) {
        fout << hd.getMaHopDong() << "|"
             << hd.getMaPhong() << "|"
             << hd.getMaNguoiThue() << "|"
             << hd.getNgayBatDau() << "|"
             << hd.getThoiHan() << "|"
             << hd.getTienCoc() << "|"
             << hd.getTrangThai() << "\n";
    });
    fout.close();
}

inline DoublyLinkedList<Contract> docContractFile(const string& tenFile) {
    DoublyLinkedList<Contract> ds;
    ifstream fin(tenFile);
    if (!fin.is_open()) return ds;

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string maHD, maP, maNT, ngayBD, hanStr, cocStr, trangThai;

        getline(ss, maHD, '|');
        getline(ss, maP, '|');
        getline(ss, maNT, '|');
        getline(ss, ngayBD, '|');
        getline(ss, hanStr, '|');
        getline(ss, cocStr, '|');
        getline(ss, trangThai, '|');

        Contract hd(maHD, maP, maNT, ngayBD, stoi(hanStr), stod(cocStr));
        // Constructor luon dat trangThai = "Con hieu luc" -> khoi phuc lai
        // dung trang thai da luu neu hop dong nay da ket thuc
        if (trangThai == "Da ket thuc") {
            hd.ketThuc();
        }
        ds.insertBack(hd);
    }
    fin.close();
    return ds;
}

#endif