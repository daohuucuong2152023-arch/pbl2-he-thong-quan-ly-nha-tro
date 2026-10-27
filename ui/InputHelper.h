#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <conio.h>

using namespace std;

namespace ui {
    // --- Các hàm tiện ích console & chuỗi ---
    void xoaManHinh();
    void tamDung();
    string catKhoangTrang(const string& s);
    string vietHoa(string s);
    bool laChuSo(const string& s);
    bool laThangHopLe(const string& s);
    bool laNgayHopLe(const string& s);

    // --- Các hàm nhập liệu có kiểm tra ---
    string docDong(const string& prompt);
    string nhapChuoi(const string& prompt, bool allowEmpty);
    int nhapSoNguyen(const string& prompt);
    int nhapSoNguyenDuong(const string& prompt);
    double nhapSoThuc(const string& prompt, double minValue, bool strictlyGreater);
    double nhapSoThucDuong(const string& prompt);
    double nhapSoThucKhongAm(const string& prompt);
    double nhapTuyChonSoThucDuong(const string& prompt, double current);
    double nhapTuyChonSoThuc(const string& prompt, double current, double minValue);
    string nhapChuSo(const string& prompt, size_t length, bool allowEmpty);
    string nhapNgay(const string& prompt);
    string nhapThang(const string& prompt);
    bool xacNhan(const string& prompt);
    string nhapMatKhau(const string& prompt);

    // --- Bí danh tương thích (Aliases) ---
    inline void clearScreen() { xoaManHinh(); }
    inline void pause() { tamDung(); }
    inline string trim(const string& s) { return catKhoangTrang(s); }
    inline string toUpper(string s) { return vietHoa(s); }
    inline bool isAllDigits(const string& s) { return laChuSo(s); }
    inline bool isValidMonth(const string& s) { return laThangHopLe(s); }
    inline bool isValidDate(const string& s) { return laNgayHopLe(s); }
    inline string readLine(const string& prompt) { return docDong(prompt); }
    inline string readText(const string& prompt, bool allowEmpty) { return nhapChuoi(prompt, allowEmpty); }
    inline int readInt(const string& prompt) { return nhapSoNguyen(prompt); }
    inline int readPositiveInt(const string& prompt) { return nhapSoNguyenDuong(prompt); }
    inline double readDouble(const string& prompt, double minValue, bool strictlyGreater) { return nhapSoThuc(prompt, minValue, strictlyGreater); }
    inline double readPositiveDouble(const string& prompt) { return nhapSoThucDuong(prompt); }
    inline double readNonNegativeDouble(const string& prompt) { return nhapSoThucKhongAm(prompt); }
    inline double readOptionalPositiveDouble(const string& prompt, double current) { return nhapTuyChonSoThucDuong(prompt, current); }
    inline double readOptionalDouble(const string& prompt, double current, double minValue) { return nhapTuyChonSoThuc(prompt, current, minValue); }
    inline string readDigits(const string& prompt, size_t length, bool allowEmpty) { return nhapChuSo(prompt, length, allowEmpty); }
    inline string readDate(const string& prompt) { return nhapNgay(prompt); }
    inline string readMonth(const string& prompt) { return nhapThang(prompt); }
    inline bool confirm(const string& prompt) { return xacNhan(prompt); }
    inline string readPassword(const string& prompt) { return nhapMatKhau(prompt); }
}

inline void ui::xoaManHinh() {
    system("cls");
}

inline void ui::tamDung() {
    cout << "\nNhan Enter de tiep tuc...";
    string s;
    getline(cin, s);
}

inline string ui::catKhoangTrang(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline string ui::vietHoa(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
    return s;
}

inline bool ui::laChuSo(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

inline bool ui::laThangHopLe(const string& s) {
    if (s.size() != 7 || s[4] != '-') return false;
    if (!laChuSo(s.substr(0, 4)) || !laChuSo(s.substr(5, 2))) return false;
    int m = stoi(s.substr(5, 2));
    return m >= 1 && m <= 12;
}

inline bool ui::laNgayHopLe(const string& s) {
    if (s.size() != 10 || s[7] != '-') return false;
    if (!laThangHopLe(s.substr(0, 7))) return false;
    if (!laChuSo(s.substr(8, 2))) return false;
    int d = stoi(s.substr(8, 2));
    return d >= 1 && d <= 31;
}

inline string ui::docDong(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) {
        cout << "\nKhong doc duoc du lieu nhap. Thoat chuong trinh." << endl;
        exit(0);
    }
    return s;
}

inline string ui::nhapChuoi(const string& prompt, bool allowEmpty) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (s.find('|') != string::npos) {
            cout << "Khong duoc nhap ky tu '|'!" << endl;
        } else if (s.empty() && !allowEmpty) {
            cout << "Khong duoc de trong!" << endl;
        } else {
            return s;
        }
    }
}

inline int ui::nhapSoNguyen(const string& prompt) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {
        }
        cout << "Vui long nhap so nguyen!" << endl;
    }
}

inline int ui::nhapSoNguyenDuong(const string& prompt) {
    while (true) {
        int v = nhapSoNguyen(prompt);
        if (v > 0) return v;
        cout << "Gia tri phai lon hon 0!" << endl;
    }
}

inline double ui::nhapSoThuc(const string& prompt, double minValue, bool strictlyGreater) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size()) {
                if (strictlyGreater ? (v > minValue) : (v >= minValue)) return v;
                cout << "Gia tri phai " << (strictlyGreater ? "lon hon " : "tu ") << minValue << " tro len!" << endl;
                continue;
            }
        } catch (...) {
        }
        cout << "Vui long nhap mot so!" << endl;
    }
}

inline double ui::nhapSoThucDuong(const string& prompt) {
    return nhapSoThuc(prompt, 0, true);
}

inline double ui::nhapSoThucKhongAm(const string& prompt) {
    return nhapSoThuc(prompt, 0, false);
}

inline double ui::nhapTuyChonSoThucDuong(const string& prompt, double current) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (s.empty()) return current;
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size() && v > 0) return v;
        } catch (...) {
        }
        cout << "Vui long nhap so lon hon 0 (hoac Enter de giu nguyen)!" << endl;
    }
}

inline double ui::nhapTuyChonSoThuc(const string& prompt, double current, double minValue) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (s.empty()) return current;
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size()) {
                if (v >= minValue) return v;
                cout << "Gia tri phai tu " << minValue << " tro len!" << endl;
                continue;
            }
        } catch (...) {
        }
        cout << "Vui long nhap mot so (hoac Enter de giu nguyen)!" << endl;
    }
}

inline string ui::nhapChuSo(const string& prompt, size_t length, bool allowEmpty) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (s.empty() && allowEmpty) return s;
        if (laChuSo(s) && s.size() == length) return s;
        cout << "Phai gom dung " << length << " chu so!" << endl;
    }
}

inline string ui::nhapNgay(const string& prompt) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (laNgayHopLe(s)) return s;
        cout << "Ngay khong hop le (dinh dang YYYY-MM-DD, VD: 2026-10-24)!" << endl;
    }
}

inline string ui::nhapThang(const string& prompt) {
    while (true) {
        string s = catKhoangTrang(docDong(prompt));
        if (laThangHopLe(s)) return s;
        cout << "Thang khong hop le (dinh dang YYYY-MM, VD: 2026-10)!" << endl;
    }
}

inline bool ui::xacNhan(const string& prompt) {
    string s = catKhoangTrang(docDong(prompt + " (y/n): "));
    return s == "y" || s == "Y";
}

inline string ui::nhapMatKhau(const string& prompt) {
    cout << prompt;
    string pw;
    char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (!pw.empty()) {
                pw.pop_back();
                cout << "\b \b";
            }
        } else {
            pw += c;
            cout << '*';
        }
    }
    cout << endl;
    return pw;
}

#endif
