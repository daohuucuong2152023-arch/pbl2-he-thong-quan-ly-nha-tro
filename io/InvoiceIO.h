#ifndef INVOICEIO_H
#define INVOICEIO_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../models/Invoice.h"
#include "../data_structures/DoublyLinkedList.h"

using namespace std;

// Dinh dang 1 dong trong invoice.txt:
// maHoaDon|maPhong|thangNam|chiSoDienCu|chiSoDienMoi|chiSoNuocCu|chiSoNuocMoi
//   |donGiaDien|donGiaNuoc|giaPhong|daThanhToan(0 hoac 1)

inline void luuInvoiceFile(const DoublyLinkedList<Invoice>& ds, const string& tenFile) {
    ofstream fout(tenFile);
    fout << fixed << setprecision(2); // tranh in so dang khoa hoc (VD: 2.5e+06)
    if (!fout.is_open()) {
        cout << "Loi: khong mo duoc file " << tenFile << " de ghi!" << endl;
        return;
    }
    ds.traverseForward([&fout](const Invoice& hd) {
        fout << hd.getMaHoaDon() << "|"
             << hd.getMaPhong() << "|"
             << hd.getThangNam() << "|"
             << hd.getChiSoDienCu() << "|"
             << hd.getChiSoDienMoi() << "|"
             << hd.getChiSoNuocCu() << "|"
             << hd.getChiSoNuocMoi() << "|"
             << hd.getDonGiaDien() << "|"
             << hd.getDonGiaNuoc() << "|"
             << hd.getGiaPhong() << "|"
             << (hd.isDaThanhToan() ? 1 : 0) << "\n";
    });
    fout.close();
}

inline DoublyLinkedList<Invoice> docInvoiceFile(const string& tenFile) {
    DoublyLinkedList<Invoice> ds;
    ifstream fin(tenFile);
    if (!fin.is_open()) return ds;

    string dong;
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string maHD, maP, thang, dienCu, dienMoi, nuocCu, nuocMoi,
               giaDien, giaNuoc, giaPhong, thanhToanStr;

        getline(ss, maHD, '|');
        getline(ss, maP, '|');
        getline(ss, thang, '|');
        getline(ss, dienCu, '|');
        getline(ss, dienMoi, '|');
        getline(ss, nuocCu, '|');
        getline(ss, nuocMoi, '|');
        getline(ss, giaDien, '|');
        getline(ss, giaNuoc, '|');
        getline(ss, giaPhong, '|');
        getline(ss, thanhToanStr, '|');

        Invoice hd(maHD, maP, thang,
                   stod(dienCu), stod(dienMoi),
                   stod(nuocCu), stod(nuocMoi),
                   stod(giaDien), stod(giaNuoc), stod(giaPhong));

        if (thanhToanStr == "1") {
            hd.danhDauDaThanhToan();
        }
        ds.insertBack(hd);
    }
    fin.close();
    return ds;
}

#endif