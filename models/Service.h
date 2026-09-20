#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <iostream>
#include "Entity.h"
using namespace std;

class Service : public Entity {
  private: 
    string maDichVu;
    string tenDichVu;
    double donGia;
    int soLuong;

  public:
    Service();
    Service(string ma, string ten, double gia, int sl = 1);

    // get/set
    string getMaDichVu() const;
    string getTenDichVu() const;
    void setTenDichVu(const string& ten);

    double getDonGia() const;
    void setDonGia(double gia);

    int getSoLuong() const;
    void setSoLuong(int sl);

    double tinhThanhTien() const; // dongia * sl

    // or from entity
    string getId() const override;
    void display() const override;
};

inline Service::Service() : donGia(0), soLuong(1) {}

inline Service::Service(string ma, string ten, double gia, int sl)
        : maDichVu(ma), tenDichVu(ten), donGia(gia), soLuong(sl) {}

inline string Service::getMaDichVu() const { return maDichVu; }

inline string Service::getTenDichVu() const { return tenDichVu; }
inline void Service::setTenDichVu(const string& ten) { tenDichVu = ten; }

inline double Service::getDonGia() const { return donGia; }
inline void Service::setDonGia(double gia) { donGia = gia; }

inline int Service::getSoLuong() const { return soLuong; }
inline void Service::setSoLuong(int sl) { soLuong = sl; }

inline double Service::tinhThanhTien() const { return donGia * soLuong; }

inline string Service::getId() const { return maDichVu; }

inline void Service::display() const {
  cout << "[Dich vu] " << tenDichVu
       << " | SL: " << soLuong
       << " | Don gia: " << donGia << " VND"
       << " | Thanh Tien: " << tinhThanhTien() << " VND" << endl;
}

#endif