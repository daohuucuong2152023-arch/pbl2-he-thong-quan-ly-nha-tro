#ifndef TENANT_H
#define TENANT_H

#include <string>
#include <iostream>
#include "Person.h"
using namespace std;

// Tenant "la mot" Person + them thong tin thue phong
class Tenant : public Person {
private:
    string maNguoiThue;
    string maPhong;      // khóa ngoại -> Room
    double tienCoc;
    string ngayCheckIn;
    string trangThai;    // "Dang o" / "Da tra phong"

public:
    Tenant();
    Tenant(string ma, string ten, string cc, string sdt,
           string phong, double coc, string ngayVao);

    //
    string getMaNguoiThue() const;
    void setMaNguoiThue(const string& ma);

    string getMaPhong() const;
    void setMaPhong(const string& phong);

    double getTienCoc() const;
    void setTienCoc(double coc);

    string getNgayCheckIn() const;
    string getTrangThai() const;

    void checkOut();

    // 
    string getId() const override; // ghi de lai: dung ma nguoi thue thay vi CC
    void display() const override;
};

// định nghĩa ở dưới

inline Tenant::Tenant() : tienCoc(0), trangThai("Dang o") {}

inline Tenant::Tenant(string ma, string ten, string cc, string sdt,
                       string phong, double coc, string ngayVao)
    : Person(ten, cc, sdt),
      maNguoiThue(ma), maPhong(phong), tienCoc(coc),
      ngayCheckIn(ngayVao), trangThai("Dang o") {}

inline string Tenant::getMaNguoiThue() const { return maNguoiThue; }
inline void Tenant::setMaNguoiThue(const string& ma) { maNguoiThue = ma; }

inline string Tenant::getMaPhong() const { return maPhong; }
inline void Tenant::setMaPhong(const string& phong) { maPhong = phong; }

inline double Tenant::getTienCoc() const { return tienCoc; }
inline void Tenant::setTienCoc(double coc) { tienCoc = coc; }

inline string Tenant::getNgayCheckIn() const { return ngayCheckIn; }
inline string Tenant::getTrangThai() const { return trangThai; }

inline void Tenant::checkOut() { trangThai = "Da tra phong"; }

inline string Tenant::getId() const { return maNguoiThue; }

inline void Tenant::display() const {
    cout << "[Khach thue] " << maNguoiThue
         << " | " << hoTen
         << " | CCCD: " << cccd
         << " | SDT: " << soDienThoai
         << " | Phong: " << maPhong
         << " | Coc: " << tienCoc << " VND"
         << " | " << trangThai << endl;
}

#endif