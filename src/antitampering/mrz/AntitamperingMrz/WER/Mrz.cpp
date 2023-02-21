#include "Mrz.hpp"

#include "MRZ/TD1.hpp"
#include "MRZ/TD2.hpp"
#include "MRZ/TD3.hpp"
#include "MRZ/MRVA.hpp"
#include "MRZ/MRVB.hpp"

Mrz::Mrz(std::vector<Fields> m):
    mrz(m){}

Mrz* Mrz::createMrz(MrzType type, std::vector<Fields> chars)
{ 
    if (type == td1) 
        return new TD1(chars); 
    else if (type == td2) 
        return new TD2(chars); 
    else if (type == td3) 
        return new TD3(chars);
    else if (type == mrva) 
        return new MRVA(chars); 
    else if (type == mrvb) 
        return new MRVB(chars);
    else return NULL; 
} 