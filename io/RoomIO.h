#ifndef ROOMIO_H
#define ROOMIO_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../models/Room.h"
#include "../data_structures/DoublyLinkedList.h"

using namespace std;

// Dinh dang 1 dong trong room.txt:
// maPhong|dienTich|giaCoBan|trangThai

inline void luuRoomFile(const DoublyLinkedList<Room>& ds, const string& tenFile) {
    ofstream fout(tenFile);
    fout << fixed << setprecision(2); // tranh in so dang khoa hoc (VD: 2.5e+06)
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    // Dung lambda co capture [&fout] -> can DoublyLinkedList::traverseForward
    // ban template (da nang cap o phan truoc), khong dung duoc voi con tro ham thuan
    ds.traverseForward([&fout](const Room& r) {
        fout << r.getMaPhong() << "|"
             << r.getDienTich() << "|"
             << r.getGiaCoBan() << "|"
             << r.getTrangThai() << "\n";
    });
    fout.close();
}

inline DoublyLinkedList<Room> docRoomFile(const string& tenFile) {
    DoublyLinkedList<Room> ds;
    ifstream fin(tenFile);
    if (!fin.is_open()) {
        // Chua co file (lan dau chay chuong trinh) -> tra ve danh sach rong, khong bao loi
        return ds;
    }

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string maPhong, dienTichStr, giaCoBanStr, trangThai;

        getline(ss, maPhong, '|');
        getline(ss, dienTichStr, '|');
        getline(ss, giaCoBanStr, '|');
        getline(ss, trangThai, '|');

        Room r(maPhong, stod(dienTichStr), stod(giaCoBanStr), trangThai);
        ds.insertBack(r);
    }
    fin.close();
    return ds;
}

#endif