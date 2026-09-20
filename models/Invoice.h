#ifndef TENANT_H
#define TENANT_H

#include <string>
#include <iostream>
#include "Entity.h"

using namespace std;

class Invoice : public Entity {
  private: 
    string maHoaDon;
    string maPhong;
    string thangNam;

    double chiSoDienCu, chiSoDienMoi;
    double chiSoNuocCu, chiSoNuocMoi;
    double donGiaDien, donGiaNuoc;
    double giaPhong;

    bool daThanhToan;

  public:
    Invoice();
    Invoice(string maHD, string maP, string thang, 
            double dienCu, double dienMoi, 
            double nuocCu, double nuocMoi,  
            double giaDien, double giaNuoc, double giaP);
    
    // tinhs tieen
    double tinhTienDien() const;
    double tinhTienNuoc() const;
    double tinhTongTien() const;

    // get/set

    string getMaHoaDon() const;
    string getMaPhong() const;
    string getThangNam() const;
    bool isDaThanhToan() const;

    double getChiSoDienCu() const;
    double getChiSoDienMoi() const;
    double getChiSoNuocCu() const;
    double getChiSoNuocMoi() const;
    double getDonGiaDien() const;
    double getDonGiaNuoc() const;
    double getGiaPhong() const;

    void danhDauDaThanhToan();
    void capNhatChiSoMoi(double dienMoi, double nuocMoi);


    // or từ entity
    string getId() const override;
    void display() const override;
};


// định nghĩa ở dưới 

inline Invoice::Invoice()
      : chiSoDienCu(0), chiSoDienMoi(0),
        chiSoNuocCu(0), chiSoNuocMoi(0),
        donGiaDien(0), donGiaNuoc(0),
        giaPhong(0), daThanhToan(false) {}

inline Invoice::Invoice(string maHD, string maP, string thang,
                          double dienCu, double dienMoi,
                          double nuocCu, double nuocMoi,
                          double giaDien, double giaNuoc, double giaP)
              : maHoaDon(maHD), maPhong(maP), thangNam(thang),
                chiSoDienCu(dienCu), chiSoDienMoi(dienMoi),
                chiSoNuocCu(nuocCu), chiSoNuocMoi(nuocMoi),
                donGiaDien(giaDien), donGiaNuoc(giaNuoc),
                giaPhong(giaP), daThanhToan(false) {}
                
            
inline double Invoice::tinhTienDien() const { return (chiSoDienMoi - chiSoDienCu) * donGiaDien; }
inline double Invoice::tinhTienNuoc() const { return (chiSoNuocMoi - chiSoNuocCu) * donGiaNuoc; }
inline double Invoice::tinhTongTien() const { return giaPhong + tinhTienDien() + tinhTienNuoc(); }
 
inline string Invoice::getMaHoaDon() const { return maHoaDon; }
inline string Invoice::getMaPhong() const { return maPhong; }
inline string Invoice::getThangNam() const { return thangNam; }
inline bool Invoice::isDaThanhToan() const { return daThanhToan; }
 
inline double Invoice::getChiSoDienCu() const { return chiSoDienCu; }
inline double Invoice::getChiSoDienMoi() const { return chiSoDienMoi; }
inline double Invoice::getChiSoNuocCu() const { return chiSoNuocCu; }
inline double Invoice::getChiSoNuocMoi() const { return chiSoNuocMoi; }
inline double Invoice::getDonGiaDien() const { return donGiaDien; }
inline double Invoice::getDonGiaNuoc() const { return donGiaNuoc; }
inline double Invoice::getGiaPhong() const { return giaPhong; }
 
inline void Invoice::danhDauDaThanhToan() { daThanhToan = true; }
 
inline void Invoice::capNhatChiSoMoi(double dienMoi, double nuocMoi) {
    chiSoDienMoi = dienMoi;
    chiSoNuocMoi = nuocMoi;
}
 
inline string Invoice::getId() const { return maHoaDon; }
 
inline void Invoice::display() const {
    cout << "[Hoa don] " << maHoaDon
         << " | Phong: " << maPhong
         << " | Thang: " << thangNam
         << " | Dien: " << tinhTienDien() << " VND"
         << " | Nuoc: " << tinhTienNuoc() << " VND"
         << " | Tong: " << tinhTongTien() << " VND"
         << " | " << (daThanhToan ? "Da thanh toan" : "Chua thanh toan")
         << endl;
}
 
#endif                          