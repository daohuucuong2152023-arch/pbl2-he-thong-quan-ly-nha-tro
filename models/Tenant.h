#ifndef TENANT_H
#define TENANT_H

#include <string>
using namespace std;

struct Tenant {
    string maNguoiThue;
    string hoTen;
    string cccd;
    string soDienThoai;
    string maPhong; // khóa ngoại -

    Tenant() {}

    Tenant(string ma, string ten, string cc, string sdt, string phong)
        : maNguoiThue(ma), hoTen(ten), cccd(cc), soDienThoai(sdt), maPhong(phong) {}
};

#endif