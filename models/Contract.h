#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include <iostream>
#include "Entity.h"
using namespace std;

class Contract : public Entity {
  private:
    string maHopDong;
    string maPhong;
    string maNguoiThue;
    string ngayBatDau;
    int thoiHan;
    double tienCoc;
    string trangThai; 

    public:
      Contract();
      Contract(string maHD, string maP, string maNT, string ngayBD, int han, double coc);

      // get/set
      string getMaHopDong() const;
      string getMaPhong() const;
      string getMaNguoiThue() const ;
      string getNgayBatDau() const ;
      int getThoiHan() const ;
      double getTienCoc() const;
      string getTrangThai() const;

      void setThoiHan(int han);
      void setTienCoc(double  coc);

      void ketThuc(); // khi checkout thành công toàn bộ
      bool isConHieuLuc() const;

      // or từ entity
      string getId() const override;
      void display() const override;
};

// định nghĩa ở dưới

inline Contract::Contract() : thoiHan(0), tienCoc(0), trangThai("Con hieu luc") {}

inline Contract::Contract(string maHD, string maP, string maNT, string ngayBD, int han, double coc)
      : maHopDong(maHD), maPhong(maP), maNguoiThue(maNT), ngayBatDau(ngayBD), thoiHan(han), tienCoc(coc), trangThai("Con hieu luc") {}

inline string Contract::getMaHopDong() const { return maHopDong; }
inline string Contract::getMaPhong() const { return maPhong; };
inline string Contract::getMaNguoiThue() const { return maNguoiThue; }
inline string Contract::getNgayBatDau() const { return ngayBatDau;}
inline int Contract::getThoiHan() const {return thoiHan;}
inline double Contract::getTienCoc() const {return tienCoc;}
inline string Contract::getTrangThai() const {return trangThai;}

inline void Contract::setThoiHan(int han) { thoiHan = han; }
inline void Contract::setTienCoc(double coc) { tienCoc = coc; }

inline void Contract::ketThuc() { trangThai = "Da ket thuc"; }
inline bool Contract::isConHieuLuc() const { return trangThai == "Con hieu luc"; }

inline string Contract::getId() const { return maHopDong; }

inline void Contract::display() const {
  cout << "[Hop dong] " << maHopDong 
       << " | Phong: " << maPhong
       << " | Khach: " << maNguoiThue
       << " | Bat dau: " << ngayBatDau
       << " | " << thoiHan << ngayBatDau
       << " | Coc: " << tienCoc << " VND"
       << " | " << trangThai << endl;
}

#endif