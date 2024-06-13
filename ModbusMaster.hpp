
#ifndef MODBUS_MASTER_API_HPP
#define MODBUS_MASTER_API_HPP


#include <string>
#include <vector>
#include "modbus/modbus.h"

namespace communication {
namespace modbus{

using ModbusTcpAddr = int;
using ModebusTcpData = int;

class ModbusTcpMaster{
public:
    ModbusTcpMaster();
    ~ModbusTcpMaster();

    /// @brief connect to the slave
    /// @param ip ip address of the slave
    /// @param port port number of the slave
    /// @param slave_id slave id, default:1
    /// @return result of the connection
    bool open(std::string ip, unsigned short port, int slave_id = 1);

    /// @brief disconnect from slave
    void close();
    
    /// @brief check if the connection has been established
    /// @return result of the connection
    bool connected();

    /// @brief reconnect to slave
    /// @return result of reconnection
    bool reconnect();

    /// @brief set slave id of the slave
    /// @param slave_id slave id
    /// @return result of setting slave id
    bool set_slave_id(int slave_id);

    /// @brief write data to a coil
    /// @param addr address of the coil
    /// @param data write value
    /// @return result of writing
    bool write_a_coil(ModbusTcpAddr addr, ModebusTcpData data);

    /// @brief write data to a register
    /// @param addr address of the register
    /// @param data write value
    /// @return result of writing
    bool write_register(ModbusTcpAddr addr, ModebusTcpData data);

    /// @brief continuous reading of inputs
    /// @param addr address of the first input
    /// @param size read size
    /// @return result array arranged by address
    std::vector<uint8_t> read_input_bits(ModbusTcpAddr addr, int size);

    /// @brief continuous reading of coils
    /// @param addr address of the first coil
    /// @param size read size
    /// @return result array arranged by address
    std::vector<uint8_t> read_coil_bits(ModbusTcpAddr addr, int size);

    /// @brief continuous reading of registers
    /// @param addr address of the first register
    /// @param size read size
    /// @return result array arranged by address
    std::vector<uint16_t> read_registers(ModbusTcpAddr addr, int size);

private:
    modbus_t *_ctx;
    std::string *_ip;
    int _port;
    int _slave_id;

    /// @brief clear the slave first read response, a bug produced by the modbus device
    void flush_first_read_buffer();
};


}
}


#endif // MODBUS_MASTER_API_HPP