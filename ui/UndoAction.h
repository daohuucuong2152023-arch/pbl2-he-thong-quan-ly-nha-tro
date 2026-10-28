#ifndef UNDOACTION_H
#define UNDOACTION_H

#include <string>
#include "../models/Room.h"
#include "../models/Tenant.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"

using namespace std;

struct ThaoTacHoanTac {
    string loai;
    Room phongTruoc;
    Room phongSau;
    Tenant khach;
    Contract hopDong;
    Invoice hoaDon;

    ThaoTacHoanTac();
    ThaoTacHoanTac(const string& l);
};

inline ThaoTacHoanTac::ThaoTacHoanTac() {}

inline ThaoTacHoanTac::ThaoTacHoanTac(const string& l) : loai(l) {}
using UndoAction = ThaoTacHoanTac;

#endif
