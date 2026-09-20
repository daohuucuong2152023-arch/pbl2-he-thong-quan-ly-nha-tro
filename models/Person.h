#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "Entity.h"

using namespace std;

class Person : public Entity {
  protected:
    string hoTen;
    string cccd;
    string soDienThoai;
  public:
    Person();
    Person(string ten, string cc, string sdt);
    virtual ~Person();

    // set /get

    string getHoTen() const ;
    void setHoTen(const string& ten);

    string getCccd() const;
    void setCccd(const string& cc);

    string getSoDienThoai() const;
    void setSoDienThoai(const string& sdt);

    // or từ 1 phần từ entity (getiD, díplay) vẫn để trống -> tự overrride
    string getId() const override;
};

// định nghĩa hàm ở dưới

inline Person::Person() {}

inline Person::Person(string ten, string cc, string sdt)
            :hoTen(ten), cccd(cc), soDienThoai(sdt) {}

inline Person::~Person() {}

inline string Person::getHoTen() const { return hoTen; }
inline void Person::setHoTen(const string& ten) { hoTen = ten; }

inline string Person::getCccd() const { return cccd; }
inline void Person::setCccd(const string& cc) { cccd = cc;}

inline string Person::getSoDienThoai() const { return soDienThoai;}
inline void Person::setSoDienThoai(const string& sdt) { soDienThoai = sdt;}

inline string Person::getId() const { return cccd;}

#endif
