#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
#include "Entity.h"
using namespace std;

class Room : public Entity {
private:
    string maPhong;
    double dienTich;
    double giaCoBan;
    string trangThai; // "Trong" / "Dang thue" / "Dang sua chua"

public:
    Room();
    Room(string ma, double dt, double gia, string tt = "Trong");

    // ---- Getter/Setter ----
    string getMaPhong() const;
    void setMaPhong(const string& ma);

    double getDienTich() const;
    void setDienTich(double dt);

    double getGiaCoBan() const;
    void setGiaCoBan(double gia);

    string getTrangThai() const;
    void setTrangThai(const string& tt); // co kiem tra hop le

    bool isTrong() const;

    // ---- Override từ Entity ----
    string getId() const override;
    void display() const override;
};

// ====== ĐỊNH NGHĨA Ở DƯỚI ======

inline Room::Room() : dienTich(0), giaCoBan(0), trangThai("Trong") {}

inline Room::Room(string ma, double dt, double gia, string tt)
    : maPhong(ma), dienTich(dt), giaCoBan(gia), trangThai(tt) {}

inline string Room::getMaPhong() const { return maPhong; }
inline void Room::setMaPhong(const string& ma) { maPhong = ma; }

inline double Room::getDienTich() const { return dienTich; }
inline void Room::setDienTich(double dt) { dienTich = dt; }

inline double Room::getGiaCoBan() const { return giaCoBan; }
inline void Room::setGiaCoBan(double gia) { giaCoBan = gia; }

inline string Room::getTrangThai() const { return trangThai; }

inline void Room::setTrangThai(const string& tt) {
    if (tt == "Trong" || tt == "Dang thue" || tt == "Dang sua chua") {
        trangThai = tt;
    }
}

inline bool Room::isTrong() const { return trangThai == "Trong"; }

inline string Room::getId() const { return maPhong; }

inline void Room::display() const {
    cout << "[Phong] " << maPhong
         << " | " << dienTich << "m2"
         << " | " << giaCoBan << " VND"
         << " | " << trangThai << endl;
}

#endif