#include <iostream>
#include "string.h"

#include "ModbusMaster.hpp"

using namespace communication::modbus;
int main(){
    ModbusTcpMaster modbus_;

    std::string ip = "127.0.0.1";
    int port = 8086;
    // std::string ip = "192.168.1.117";
    // int port = 502;

    int slave_id = 1;

    if(modbus_.open(ip, port))
        std::cout << "ip: " << ip << ", port: " << port << std::endl;
    else
        std::cout << "Error opening modbus" << std::endl;

    while (1)
    {
        int addr;
        bool value;
        std::cout << "请输入寄存器地址" << std::endl;
        std::cin >> addr;
        std::cout << "请输入寄存器值" << std::endl;
        std::cin >> value;
        std::cout << "addr: " << addr << ", value: " << value << std::endl;
        std::cout << modbus_.write_a_coil(addr, value) << std::endl;
    }

    // while (1)
    // {
    //     int addr;
    //     uint8_t value;
    //     std::cout << "请输入寄存器地址" << std::endl;
    //     std::cin >> addr;
    //     std::cout << "addr: " << addr << std::endl;
    //     auto res = modbus_.read_input_bits(addr, 1, value);
    //     if(res){
    //         std::cout << "reading " << value << std::endl;
    //     }
    //     else{
    //         std::cout << "Error reading " << addr << std::endl;
    //     }
    // }

    // while (1)
    // {
    //     int addr;
    //     uint8_t value;
    //     std::cout << "请输入寄存器地址" << std::endl;
    //     std::cin >> addr;
    //     if(addr < 0) break;
    //     std::cout << "addr: " << addr << std::endl;
    //     auto res = modbus_.read_coil_bits(addr, 1);
    //     if(res.size() > 0){
    //         std::cout << "reading " << value << std::endl;
    //     }
    //     else{
    //         std::cout << "Error reading " << addr << std::endl;
    //     }
    // }

    modbus_.close();
    
    return 0;
}