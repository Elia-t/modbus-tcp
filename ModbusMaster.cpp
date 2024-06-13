// #include <iostream>

#include "ModbusMaster.hpp"
#include <cstring>

namespace communication {
namespace modbus{

const int MAX_READ_REGISTERS = 128;
const int TO_RETRY = 10;

ModbusTcpMaster::ModbusTcpMaster()
{
    _ctx = nullptr;
    _ip = nullptr;
    _port = 0;
    _slave_id = 1;
}

ModbusTcpMaster::~ModbusTcpMaster()
{
    close();
}

bool ModbusTcpMaster::open(std::string ip, unsigned short port, int slave_id)
{
    if(_ctx) return true; // already open a connection

    _ctx = modbus_new_tcp(ip.c_str(), port);
    if(modbus_connect(_ctx) == -1 || !set_slave_id(slave_id)) {
        modbus_free(_ctx);
        _ctx = nullptr;
        return false;
    }

    // backup the information
    if(!_ip){
        _ip = new std::string(ip);
        _port = port;
    }

    flush_first_read_buffer();

    return true;
}

void ModbusTcpMaster::close()
{
    if(_ctx){
        modbus_close(_ctx);
        modbus_free(_ctx);
        _ctx = nullptr;
    }
    if(_ip){
        delete _ip;
        _ip = nullptr;
    }
}

bool ModbusTcpMaster::connected()
{
    return _ip;
}

bool ModbusTcpMaster::reconnect()
{
    if(_ctx){
        modbus_close(_ctx);
        modbus_free(_ctx);
        _ctx = nullptr;
    }
    return open(*_ip, _port, _slave_id);
}

bool ModbusTcpMaster::set_slave_id(int slave_id)
{
    if(!_ctx) return false;

    _slave_id = slave_id;
    return modbus_set_slave(_ctx, slave_id) == 0;
}

bool ModbusTcpMaster::write_a_coil(ModbusTcpAddr addr, ModebusTcpData data)
{   
    if(_ctx){
        for(int i=0; i < TO_RETRY; ++i){
            if(modbus_write_bit(_ctx, addr, data) == 1) return true;
        }
    }
    // retry connect to slave
    if(_ip && reconnect()){
        return modbus_write_bit(_ctx, addr, data) == 1;
    }
    return false;
}

bool ModbusTcpMaster::write_register(ModbusTcpAddr addr, ModebusTcpData data)
{
    if(_ctx){
        for(int i = 0; i <TO_RETRY; ++i){
            if(modbus_write_register(_ctx, addr, data) == 1) return true;
        }
    }
    // retry connect to slave
    if(_ip && reconnect()){
        return modbus_write_register(_ctx, addr, data) == 1;
    }
    return false;
}

std::vector<uint8_t> ModbusTcpMaster::read_input_bits(ModbusTcpAddr addr, int size)
{
    std::vector<uint8_t> res;
    if(size > MAX_READ_REGISTERS) return res;

    uint8_t* read_input_bits = new uint8_t[MAX_READ_REGISTERS+1];
    if(_ctx){
        memset(read_input_bits, 0, (MAX_READ_REGISTERS + 1) * sizeof(uint8_t));
        for(int i = 0; i < TO_RETRY; ++i){
            if(modbus_read_input_bits(_ctx, addr, size, read_input_bits) == size){
                for(int i = 0; i < size; i++){
                    res.emplace_back(read_input_bits[i]);
                }
                delete[] read_input_bits;
                return res;
            }
        }
    }
    // retry connect to slave
    if(_ip  && reconnect()){
        memset(read_input_bits, 0, (MAX_READ_REGISTERS + 1) * sizeof(uint8_t));
        if(modbus_read_input_bits(_ctx, addr, size, read_input_bits) == size){
            for(int i = 0; i < size; i++){
                res.emplace_back(read_input_bits[i]);
            }
        }
    }

    delete[] read_input_bits;
    return res;
}

std::vector<uint8_t> ModbusTcpMaster::read_coil_bits(ModbusTcpAddr addr, int size)
{
    std::vector<uint8_t> res;
    if(size > MAX_READ_REGISTERS) return res;

    uint8_t* read_coil_bits = new uint8_t[MAX_READ_REGISTERS+1];
    if(_ctx){
        memset(read_coil_bits, 0, (MAX_READ_REGISTERS + 1) * sizeof(uint8_t));
        for(int i=0; i < TO_RETRY; ++i){
            if(modbus_read_bits(_ctx, addr, size, read_coil_bits) == size){
                for(int i = 0; i < size; i++){
                    res.emplace_back(read_coil_bits[i]);
                }
                delete[] read_coil_bits;
                return res;
            }
        }
    }
    // retry connect to slave
    if(_ip  && reconnect()){
        memset(read_coil_bits, 0, (MAX_READ_REGISTERS + 1) * sizeof(uint8_t));
        if(modbus_read_bits(_ctx, addr, size, read_coil_bits) == size){
            for(int i = 0; i < size; i++){
                res.emplace_back(read_coil_bits[i]);
            }
        }
    }

    delete[] read_coil_bits;
    return res;
}

std::vector<uint16_t> ModbusTcpMaster::read_registers(ModbusTcpAddr addr, int size)
{
    std::vector<uint16_t> res;
    if(size > MAX_READ_REGISTERS) return res;

    uint16_t* read_registers = new uint16_t[MAX_READ_REGISTERS+1];
    if(_ctx){
        memset(read_registers , 0, (MAX_READ_REGISTERS + 1) * sizeof(uint16_t));
        for(int i=0; i<TO_RETRY; ++i){
            if(modbus_read_registers(_ctx, addr, size, read_registers) == size){
                for(int i = 0; i < size; i++){
                    res.emplace_back(read_registers[i]);
                }
                delete[] read_registers;
                return res;
            }
        }
    }
    // retry connect to slave
    if(_ip  && reconnect()){
        memset(read_registers , 0, (MAX_READ_REGISTERS + 1) * sizeof(uint16_t));
        if(modbus_read_registers(_ctx, addr, size, read_registers) == size){
            for(int i = 0; i < size; i++){
                res.emplace_back(read_registers[i]);
            }
        } 
    }
    delete[] read_registers;
    return res;
}

void ModbusTcpMaster::flush_first_read_buffer(){
    uint8_t read_input_bits;
    auto len = modbus_read_input_bits(_ctx, 1, 1, &read_input_bits);
    modbus_flush(_ctx);
}

}
}