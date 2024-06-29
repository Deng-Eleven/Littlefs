/**
  * @file lfs_port.h
  * @brief LittleFS端口适配器头文件
  * 
  * 该文件定义了LittleFS的端口适配器接口和配置参数，用于与具体的存储设备（如内部闪存）进行集成。
  * 包括初始化、读取、写入、擦除和同步等操作函数的声明。
  *
  * 使用说明：
  *   01 - 使用前需确保已定义好存储设备的硬件参数和LittleFS配置参数，然后调用lfs_internal_flash_init()函数进行初始化。
  *   02 - 可通过调用lfs_internal_flash_read()函数从指定块中读取数据。
  *   03 - 使用lfs_internal_flash_prog()函数向指定块中的指定偏移位置写入数据。
  *   04 - 使用lfs_internal_flash_erase()函数擦除指定块。
  *   05 - 调用lfs_internal_flash_sync()函数进行LittleFS的同步操作。
  *
  * @note
  *   - 使用本头文件前，请确保已经包含了主程序所需的头文件（如main.h）和LittleFS头文件（如lfs.h）。
  *   - 在使用过程中，需要根据具体的存储设备和LittleFS的要求进行适配和调整。
  *   - 若对LittleFS具体操作有疑问或需要进一步的技术支持，请参考LittleFS官方文档。
  *
  * @copyright (c) 年份 风翎科技
  * @author 邓思忆
  * @date   2024.6.25
  * @version 1.0
  */
  
  
  /*
//可以复制如下代码进行测试，User_Init()放在main()函数中while循环外部
#include "Lfs_Port.h"

struct lfs_config cfg;
lfs_t lfs;
lfs_file_t file;

void User_Init(void)
{
	// 初始化 LittleFS
	lfs_internal_flash_init(&cfg);
	
	// 假设要写入的数据缓冲区
	uint8_t write_buffer[] = "Hello LittleFS!";
	
	// 假设要写入的块号和偏移量
	lfs_block_t block = 0;
	lfs_off_t off = 0;
	
	 // 调用写入函数进行数据写入
	int result = lfs_internal_flash_prog(&cfg, block, off, write_buffer, sizeof(write_buffer));
	
	// 读取数据的缓冲区
	uint8_t read_buffer[sizeof(write_buffer)] = {0};

	// 调用读取函数进行数据读取
	result = lfs_internal_flash_read(&cfg, block, off, read_buffer, sizeof(read_buffer));
	
	__NOP();
}
  */
#ifndef __LFS_PORT__
#define __LFS_PORT__

#include "main.h"   // 包含主程序所需的头文件
#include "lfs.h"    // 包含LittleFS头文件

#define FLASH_START_ADDR    0x08000000    // 内部闪存起始地址
//#define FLASH_SECTOR_SIZE   4096        // 内部闪存扇区大小
#define FLASH_SECTOR_NUM    15            // FLASH_END - FLASH_BASE = 65535 ,65535 / 4096 = 15个扇区

// 函数声明
int lfs_internal_flash_init(struct lfs_config *cfg);    // LittleFS初始化函数
int lfs_internal_flash_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);  // LittleFS读取函数
int lfs_internal_flash_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);  // LittleFS写入函数
int lfs_internal_flash_erase(const struct lfs_config *cfg, lfs_block_t block);   // LittleFS擦除函数
int lfs_internal_flash_sync(const struct lfs_config *cfg);   // LittleFS同步函数



#endif  // __LFS_PORT__
