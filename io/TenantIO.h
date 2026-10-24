#ifndef TENANTIO_H
#define TENANTIO_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../models/Tenant.h"
#include "../data_structures/DoublyLinkedList.h"

using namespace std;

// Dinh dang 1 dong trong tenant.txt:
// maNguoiThue|hoTen|cccd|soDienThoai|maPhong|tienCoc|ngayCheckIn|trangThai

inline void luuTenantFile(const DoublyLinkedList<Tenant>& ds, const string& tenFile) {
    ofstream fout(tenFile);
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    fout << fixed << setprecision(2);
    ds.traverseForward([&fout](const Tenant& t) {
        fout << t.getMaNguoiThue() << "|"
             << t.getHoTen() << "|"
             << t.getCccd() << "|"
             << t.getSoDienThoai() << "|"
             << t.getMaPhong() << "|"
             << t.getTienCoc() << "|"
             << t.getNgayCheckIn() << "|"
             << t.getTrangThai() << "\n";
    });
    fout.close();
}

inline DoublyLinkedList<Tenant> docTenantFile(const string& tenFile) {
    DoublyLinkedList<Tenant> ds;
    ifstream fin(tenFile);
    if (!fin.is_open()) return ds;

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string ma, ten, cc, sdt, phong, cocStr, ngay, trangThai;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, cc, '|');
        getline(ss, sdt, '|');
        getline(ss, phong, '|');
        getline(ss, cocStr, '|');
        getline(ss, ngay, '|');
        getline(ss, trangThai, '|');

        Tenant t(ma, ten, cc, sdt, phong, stod(cocStr), ngay);
        // Constructor luon dat "Dang o" -> khoi phuc lai trang thai da luu
        if (trangThai == "Da tra phong") {
            t.checkOut();
        }
        ds.insertBack(t);
    }
    fin.close();
    return ds;
}

#endif