//ArcanumWorld - ByteReader.hpp: the header of byte parser for binary files.
//Copyright (C) 2023, JordanCpp, alienated from original codebase & used by APAMk2

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class ByteReader
{
public:
    enum
    {
        BigEndian,
        LittleEndian
    };

    bool Reset(const std::string& path, size_t type);
    void Close();
    ~ByteReader();
    size_t Bytes();
    int cursor();
    bool Eof();
    uint8_t u8();
    uint16_t u16();
    uint32_t u32();
    int16_t i16();
    int32_t i32();
    std::string string(int len);
    void Pos(size_t value);
    void Read(void* buffer, size_t bytes);
private:
    size_t _Type;
    size_t _Bytes;
    std::fstream _File;
};