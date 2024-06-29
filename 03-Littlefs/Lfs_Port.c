#include "Lfs_Port.h"

/**
  * @brief LittleFS初始化函数
  * @param cfg 指向lfs_config结构体的指针，用于配置LittleFS所需的函数和参数
  * @retval 返回LittleFS初始化状态，成功返回LFS_ERR_OK，失败返回其他错误码
  */
int lfs_internal_flash_init(struct lfs_config *cfg) 
{

    
    // 配置LittleFS所需的函数
    cfg->read = lfs_internal_flash_read;
    cfg->prog = lfs_internal_flash_prog;
    cfg->erase = lfs_internal_flash_erase;
    cfg->sync = lfs_internal_flash_sync;

    // 配置LittleFS的参数
    cfg->read_size = 16;
    cfg->prog_size = 16;
    cfg->block_size = FLASH_SECTOR_SIZE;
    cfg->block_count = FLASH_SECTOR_NUM;
    cfg->block_cycles = 500;
    cfg->cache_size = 16;
    cfg->lookahead_size = 16;

    return LFS_ERR_OK;
}



/**
  * @brief LittleFS读取函数
  * @param cfg 指向常量lfs_config结构体的指针，配置LittleFS的参数
  * @param block 逻辑块号，从0开始
  * @param off 块内偏移量
  * @param buffer 存储读取数据的缓冲区
  * @param size 要读取的数据大小
  * @retval 返回LittleFS读取状态，成功返回LFS_ERR_OK，失败返回其他错误码
  */
int lfs_internal_flash_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    // 检查读取是否有效
    LFS_ASSERT(off % cfg->read_size == 0);
    LFS_ASSERT(size % cfg->read_size == 0);
    LFS_ASSERT(block < cfg->block_count);

    // 计算地址
    uint32_t address = block * cfg->block_size + off;

    // 执行读取操作
    for (int i = 0; i < size; i += 2) {
        *(uint16_t*)((uint32_t)buffer + i) = *(uint16_t*)(address + i);
    }

    return LFS_ERR_OK;
}



/**
  * @brief LittleFS写入函数
  * @param cfg 指向常量lfs_config结构体的指针，配置LittleFS的参数
  * @param block 逻辑块号，从0开始
  * @param off 块内偏移量
  * @param buffer 存储写入数据的缓冲区
  * @param size 要写入的数据大小
  * @retval 返回LittleFS写入状态，成功返回LFS_ERR_OK，失败返回其他错误码
  */
int lfs_internal_flash_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) 
{
    uint32_t flash_program_start = FLASH_START_ADDR;        // 编程起始地址
    uint32_t flash_program_end = FLASH_START_ADDR + size;   // 编程结束地址
    uint32_t *src = (uint32_t *)buffer;                     // 数据源指针

    HAL_FLASH_Unlock();
    // 在起始地址到结束地址范围内执行编程操作
    while (flash_program_start < flash_program_end) {
        // 执行页面编程操作
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_program_start, src) == HAL_OK) {
            flash_program_start += FLASH_PAGE_SIZE;         // 更新起始地址到下一页
            src++;                                          // 更新数据源指针到下一页的数据
        }
    }
    HAL_FLASH_Lock();

    return LFS_ERR_OK;  // 返回成功状态
}



/**
  * @brief LittleFS擦除函数
  * @param cfg 指向常量lfs_config结构体的指针，配置LittleFS的参数
  * @param block 逻辑块号，从0开始
  * @retval 返回LittleFS擦除状态，成功返回LFS_ERR_OK，失败返回其他错误码
  */
int lfs_internal_flash_erase(const struct lfs_config *cfg, lfs_block_t block) 
{
    // 检查擦除是否有效
    LFS_ASSERT(block < cfg->block_count);

    // 调用内部闪存擦除函数
    uint32_t SECTORError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct = {0};

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORERASE;      // 擦除类型：按扇区擦除
    EraseInitStruct.SectorAddress = FLASH_START_ADDR;             // 擦除起始地址
    EraseInitStruct.NbSectors = 1;                                // 擦除的扇区数量为1

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) { // 执行扇区擦除操作
        APP_ErrorHandler();  // 擦除失败，执行错误处理
    }
    HAL_FLASH_Lock();

    return LFS_ERR_OK;
}



/**
  * @brief LittleFS同步函数
  * @param cfg 指向常量lfs_config结构体的指针，配置LittleFS的参数
  * @retval 返回LittleFS同步状态，成功返回LFS_ERR_OK，失败返回其他错误码
  */
int lfs_internal_flash_sync(const struct lfs_config *cfg) 
{
    // 内部闪存无需同步操作
    return LFS_ERR_OK;
}




