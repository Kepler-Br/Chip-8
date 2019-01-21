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

    uint16_t registers[registerCount];
    uint16_t registerI = 0;
    uint16_t programCounter = 0;

    uint16_t jumpStack[0x10];
    uint16_t stackPointer = 0;



    uint16_t fetchOpcode()
    {
        uint16_t opcode = memory[programCounter] << 8 | memory[programCounter+1];
        return opcode;
    }
    void processTimers()
    {
        if(delayTimer > 0)
            delayTimer--;
        if(soundTimer > 0)
        {
            if(soundTimer == 1)
                std::cout << "beep\n";
            soundTimer--;
        }
    }

    void clearScreen()
    {
        for(uint8_t& i: displayBuffer)
            i = 0;
        programCounter+=2;
        drawFlag = true;
    }

    void returnFromSubroutine()
    {
        stackPointer--;
        programCounter = jumpStack[stackPointer];
    }

    // firstByte opcodes
    void gotoAdress(uint16_t opcode)
    {
        uint16_t adress = opcode & 0x0FFF;
        programCounter = adress;
    }
    void gotoSubroutine(uint16_t opcode)
    {
        jumpStack[stackPointer] = programCounter;
        stackPointer++;
        programCounter = opcode & 0x0FFF;
    }
    void skipIfRegisterEqual(uint16_t opcode)
    {
        int registerNum = (opcode & 0x0F00) >> 8;
        uint16_t value = opcode & 0x00FF;
        if(registers[registerNum] == value)
            programCounter+=2;
        programCounter+=2;
    }
    void skipIfRegisterNotEqual(uint16_t opcode)
    {
        int registerNum = (opcode & 0x0F00) >> 8;
        uint16_t value = opcode & 0x00FF;
        if(registers[registerNum] != value)
            programCounter+=2;
        programCounter+=2;
    }
    void skipIfTwoRegistersEqual(uint16_t opcode)
    {
        int firstRegister = (opcode & 0x0F00) >> 8;
        int secondRegister = (opcode & 0x00F0) >> 4;
        if(registers[firstRegister] == registers[secondRegister])
            programCounter+=2;
        programCounter+=2;
    }

    void skipIfTwoRegistersNotEqual(uint16_t opcode)
    {
        int firstRegister = (opcode & 0x0F00) >> 8;
        int secondRegister = (opcode & 0x00F0) >> 4;
        if(registers[firstRegister] != registers[secondRegister])
            programCounter+=2;
        programCounter+=2;
    }

    void setRegister(uint16_t opcode)
    {
        int registerNum = (opcode & 0x0F00) >> 8;
        uint16_t registerValue = opcode & 0x00FF;
        registers[registerNum] = registerValue;
        programCounter += 2;
    }
    void addToRegister(uint16_t opcode)
    {
        int registerNum = (opcode & 0x0F00) >> 8;
        uint16_t value = opcode & 0x00FF;
        registers[registerNum] += value;
        programCounter+=2;
    }
    void setRegisterAnotherRegister(uint16_t opcode)
    {
        int firstRegister = (opcode & 0x0F00) >> 8;
        int secondRegister = (opcode & 0x00F0) >> 4;
        registers[firstRegister] = registers[secondRegister];
        programCounter += 2;
    }
    void registerBitwiseOr(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] = registers[first] | registers[second];
        programCounter += 2;
    }
    void registerBitwiseAnd(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] = registers[first] & registers[second];
        programCounter += 2;
    }
    void registerBitwiseXor(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] = registers[first] ^ registers[second];
        programCounter += 2;
    }
    void sumRegisters(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] += registers[second];
        registers[0xF] = 0;
        if( 0x100 - registers[first] > registers[second])
            registers[0xF] = 1;
        programCounter += 2;
    }
    void substractRegisters(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] -= registers[second];
        registers[0xF] = 0;
        if(registers[first] < registers[second])
            registers[0xF] = 1;
        programCounter += 2;
    }
    void shiftRightRegister(uint16_t opcode)
    {
        registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x1;
        registers[(opcode & 0x0F00) >> 8] >>= 1;
        /*int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[0xF] = registers[second] & 0x0001;
        registers[first] = registers[second] >> 1;*/
        programCounter += 2;
    }
    void VySubstractVx(uint16_t opcode)
    {
        int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[first] = registers[second] - registers[first];
        registers[0xF] = 0;
        if(registers[first] > registers[second])
            registers[0xF] = 1;
        programCounter += 2;
    }
    void shiftLeftRegister(uint16_t opcode)
    {
        registers[0xF] = registers[(opcode & 0x0F00) >> 8] >> 7;
        registers[(opcode & 0x0F00) >> 8] <<= 1;
        /*int first = (opcode & 0x0F00) >> 8;
        int second = (opcode & 0x00F0) >> 4;
        registers[0xF] = registers[second] & 0x8000;
        registers[first] = registers[second] >> 1;*/
        programCounter += 2;
    }
    void setIAdress(uint16_t opcode)
    {
        uint16_t value = opcode & 0x0FFF;
        registerI = value;
        programCounter += 2;
    }
    void jumpPlusV0(uint16_t opcode)
    {
        programCounter = registers[0] + (opcode & 0x0FFF);
    }

    void bitwiseRandomNumber(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        int value = opcode & 0x00FF;
        registers[reg] = uint16_t(rand()%255)&value;
        programCounter+=2;
    }

    void drawSprite(uint16_t opcode)
    {
        uint16_t x = registers[(opcode & 0x0F00) >> 8];
        uint16_t y = registers[(opcode & 0x00F0) >> 4];
        uint16_t height = opcode & 0x000F;
        uint16_t pixel;

        registers[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
            pixel = memory[registerI + yline];
            for(int xline = 0; xline < 8; xline++)
            {
                if((pixel & (0x80 >> xline)) != 0)
                {
                    if(displayBuffer[(x + xline + ((y + yline) * 64))] == 1)
                        registers[0xF] = 1;                                 
                    displayBuffer[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }
        programCounter+=2;
        drawFlag = true;
    }
    void skipIfKeyPressed(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;

        if(registers[reg] < 0x10 && keys[registers[reg]] == 1)
            programCounter += 2;
        programCounter += 2;
    }
    void skipIfKeyNotPressed(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;

        if(registers[reg] < 0x10 && keys[registers[reg]] != 1)
            programCounter += 2;
        programCounter += 2;
    }
    void getDelay(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        registers[reg] = delayTimer;
        programCounter += 2;
    }
    void waitForKey(uint16_t opcode)
    {
        bool keyPressed = false;
        for(int i = 0; i < 0x10; i++)
        {
            if(keys[i] != 1)
            {
                keyPressed = true;
                int reg = (opcode & 0x0F00) >> 8;
                registers[reg] = i;
            }
        }
        if(keyPressed)
            programCounter+=2;
    }
    void setDelay(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        delayTimer = registers[reg];
        programCounter += 2;
    }
    void setSoundTimer(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        soundTimer = registers[reg];
        programCounter += 2;
    }
    void addToI(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        if(registerI + registers[reg] > 0xFFF)
            registers[0xf] = 1;
        else
            registers[0xf] = 0;
        registerI += registers[reg];
        programCounter += 2;
    }
    void getChar(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        registerI = registers[reg] * 0x5;
        programCounter += 2;
    }
    void storeRegister(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        memory[registerI] = registers[reg] / 100;
        memory[registerI + 1] = (registers[reg] / 10)%10;
        memory[registerI + 2] = (registers[reg] % 100)%10;
        programCounter += 2;
    }
    void registerDump(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        for(int i = 0; i <= reg; i++)
            memory[registerI + i] = registers[i];
        registerI += reg + 1;
        programCounter += 2;
    }
    void loadRegister(uint16_t opcode)
    {
        int reg = (opcode & 0x0F00) >> 8;
        for(int i = 0; i <= reg; i++)
            registers[i] = memory[registerI + 1];
        registerI += reg + 1;
        programCounter += 2;
    }

    void processFirstByteOpcode(uint16_t opcode)
    {
        switch(opcode & 0xF000)
        {
            case 0x1000: //0x1NNN
                gotoAdress(opcode);
                break;
            case 0x2000: //0x2NNN
                gotoSubroutine(opcode);
                break;
            case 0x3000: //0x3XNN
                skipIfRegisterEqual(opcode);
                break;
            case 0x4000: //0x4XNN
                skipIfRegisterNotEqual(opcode);
                break;
            case 0x5000: //0x5XY0
                skipIfTwoRegistersEqual(opcode);
                break;
            case 0x6000: //0x6XNN
                setRegister(opcode);
                break;
            case 0x7000: //0x7XNN
                addToRegister(opcode);
                break;
            case 0x8000:
                if((opcode & 0x000F) == 0x0)
                    setRegisterAnotherRegister(opcode);
                if((opcode & 0x000F) == 0x1)
                    registerBitwiseOr(opcode);
                if((opcode & 0x000F) == 0x2)
                    registerBitwiseAnd(opcode);
                if((opcode & 0x000F) == 0x3)
                    registerBitwiseXor(opcode);
                if((opcode & 0x000F) == 0x4)
                    sumRegisters(opcode);
                if((opcode & 0x000F) == 0x5)
                    substractRegisters(opcode);
                if((opcode & 0x000F) == 0x6)
                    shiftRightRegister(opcode);
                if((opcode & 0x000F) == 0x7)
                    VySubstractVx(opcode);
                if((opcode & 0x000F) == 0xE) //The fuck?
                    shiftLeftRegister(opcode);
                break;
            case 0x9000:
                skipIfTwoRegistersNotEqual(opcode);
                break;
            case 0xA000:
                setIAdress(opcode);
                break;
            case 0xB000:
                jumpPlusV0(opcode);
                break;
            case 0xC000:
                bitwiseRandomNumber(opcode);
                break;
            case 0xD000:
                drawSprite(opcode);
                break;
            case 0xE000:
                if((opcode & 0x00FF) == 0x9E)
                    skipIfKeyPressed(opcode);
                if((opcode & 0x00FF) == 0xA1)
                    skipIfKeyNotPressed(opcode);
                break;
            case 0xF000:
                if((opcode & 0x00FF) == 0x07)
                    getDelay(opcode);
                if((opcode & 0x00FF) == 0x0A)
                    waitForKey(opcode);
                if((opcode & 0x00FF) == 0x15)
                    setDelay(opcode);
                if((opcode & 0x00FF) == 0x18)
                    setSoundTimer(opcode);
                if((opcode & 0x00FF) == 0x1E)
                    addToI(opcode);
                if((opcode & 0x00FF) == 0x29)
                    getChar(opcode);
                if((opcode & 0x00FF) == 0x33)
                    storeRegister(opcode);
                if((opcode & 0x00FF) == 0x55)
                    registerDump(opcode);
                if((opcode & 0x00FF) == 0x65)
                    loadRegister(opcode);
                break;
            default:
                programCounter += 2;
                std::cout << "No such opcode as 0x" << std::hex << opcode << std::dec << std::endl;
        }
    }

    void processLastByteOpcode(uint16_t opcode)
    {
        switch(opcode & 0x000F)
        {
            case 0x00E0:
                clearScreen();
                break;
            case 0x000E:
                returnFromSubroutine();
                break;
            default:
                std::cout << "WARNING: unknown last bit opcode: 0x" << std::hex 
                    << opcode << std::dec << std::endl;
                programCounter += 2;
                break;
        }
    }
    
    public:
    bool drawFlag = false;

    ChipEight() = default;

    uint8_t getPixel(int x, int y)
    {
        return displayBuffer[y*displayWidth+x];
    }
    void init()
    {
        programCounter = 0x200;
        for(auto& i: memory)
            i = 0;
        for(uint8_t& i: displayBuffer)
            i = 0;
        for(uint16_t& i: registers)
            i = 0;
        for(uint8_t& i: keys)
            i = 0;
        uint8_t fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        };
        for(int i = 0; i < 80; i++)
            memory[i] = fontset[i];
    }
    void load(const std::string &filename)
    {
        std::ifstream programFile(filename);
        int memoryPointer = 0;
        while(!programFile.eof())
        {
            uint8_t buffer;
            programFile.read((char*)&buffer, 1);
            if(programFile.badbit || programFile.failbit)
            {
                //std::cout << "ERROR: reading failed at 0x" << std::hex << memoryPointer << std::dec << std::endl;
                //break;
            }
            memory[memoryPointer + 0x200] = buffer;
            memoryPointer++;
            //std::cout << int(buffer) << std::hex << std::endl;

        }
        programFile.close();
    }

    void cycle()
    {
        uint16_t opcode = fetchOpcode();
        executeOpcode(opcode);
        processTimers();
    }

    void executeOpcode(uint16_t opcode)
    {
        if((opcode & 0xF000) != 0x0000)
            processFirstByteOpcode(opcode);
        else
            processLastByteOpcode(opcode);
    }

    void pressKey(int key)
    {
        if(key >= keyCount)
            return;
        keys[key] = 1;
    }

    void releaseKey(int key)
    {
        if(key >= keyCount)
            return;
        keys[key] = 0;
    }

    void printDebug()
    {
        std::cout << "Registers: " << "\n";
        for(int i = 0; i < registerCount; i++)
            std::cout << std::hex << "V" << i << ": " << registers[i] << " ";
        std::cout << "\n" << std::dec;
        std::cout << "Delay timer: " << int(delayTimer) << "\n";
        std::cout << "Beep timer: " << int(soundTimer) << "\n";
        std::cout << std::hex;
        std::cout << "Stack pointer: " << stackPointer << "\n";
        std::cout << "Memory pointer: " << registerI  << "\n";
        std::cout << "Program pointer: " << programCounter << "\n";
        std::cout << "Current opcode: " << fetchOpcode() << "\n";
        std::cout << std::dec;
    }
};
#endif // CHIPEIGHT_H
