all:
	g++ ModbusMaster.cpp test/modbus_master.cpp -o master -I ./ -L /usr/lib -lmodbus -g
	g++ test/modbus_slave.cpp -o slave -I ./ -L /usr/lib -lmodbus -g