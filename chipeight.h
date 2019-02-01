#ifndef CHIPEIGHT_H
#define CHIPEIGHT_H

#include <iostream>
#include <fstream>


class ChipEight
{
    static const int memorySize = 0x1000; // 4096 aka 4K aka 16^3
    static const int displayWidth = 64;
    static const int displayHeight = 32;
    static const int registerCount = 0x10;
    static const int keyCount = 0x10;

    uint8_t memory[memorySize];    
    uint8_t displayBuffer[displayWidth*displayHeight];
    uint8_t keys[keyCount];
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    uint8_t registers[registerCount];
    uint16_t registerI = 0;
    uint16_t programCounter = 0;

    uint16_t jumpStack[0x10];
    uint16_t stackPointer = 0;


    uint16_t fetchOpcode();
    void processTimers();
    void clearScreen();
    void returnFromSubroutine();

    // firstByte opcodes
    void gotoAdress(uint16_t opcode);
    void gotoSubroutine(uint16_t opcode);
    void skipIfRegisterEqual(uint16_t opcode);
    void skipIfRegisterNotEqual(uint16_t opcode);
    void skipIfTwoRegistersEqual(uint16_t opcode);
    void skipIfTwoRegistersNotEqual(uint16_t opcode);
    void setRegister(uint16_t opcode);
    void addToRegister(uint16_t opcode);
    void setRegisterAnotherRegister(uint16_t opcode);
    void registerBitwiseOr(uint16_t opcode);
    void registerBitwiseAnd(uint16_t opcode);
    void registerBitwiseXor(uint16_t opcode);
    void sumRegisters(uint16_t opcode);
    void substractRegisters(uint16_t opcode);
    void shiftRightRegister(uint16_t opcode);
    void VySubstractVx(uint16_t opcode);
    void shiftLeftRegister(uint16_t opcode);
    void setIAdress(uint16_t opcode);
    void jumpPlusV0(uint16_t opcode);
    void bitwiseRandomNumber(uint16_t opcode);
    void drawSprite(uint16_t opcode);
    void skipIfKeyPressed(uint16_t opcode);
    void skipIfKeyNotPressed(uint16_t opcode);
    void getDelay(uint16_t opcode);
    void waitForKey(uint16_t opcode);
    void setDelay(uint16_t opcode);
    void setSoundTimer(uint16_t opcode);
    void addToI(uint16_t opcode);
    void getChar(uint16_t opcode);
    void storeRegister(uint16_t opcode);
    void registerDump(uint16_t opcode);
    void loadRegister(uint16_t opcode);
    void processFirstByteOpcode(uint16_t opcode);
    void processLastByteOpcode(uint16_t opcode);
    
    public:
    bool drawFlag = false;

    ChipEight() = default;

    uint8_t getPixel(int x, int y);
    void init();
    void load(const std::string &filename);
    void cycle();
    void executeOpcode(uint16_t opcode);
    void pressKey(int key);
    void releaseKey(int key);
    void printDebug();
};
#endif // CHIPEIGHT_H
