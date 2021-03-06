#include "chipeight.h"

uint16_t ChipEight::fetchOpcode()
{
    uint16_t opcode = uint16_t(memory[programCounter] << 8 | memory[programCounter+1]);
    return opcode;
}

void ChipEight::processTimers()
{
    if(delayTimer > 0)
        delayTimer--;
    if(soundTimer > 0)
    {
        if(soundTimer == 1)
            soundFlag = true;
        soundTimer--;
    }
}

void ChipEight::clearScreen()
{
    for(uint8_t& i: displayBuffer)
        i = 0;
    programCounter+=2;
}

void ChipEight::returnFromSubroutine()
{
    if(stackPointer == 0)
        throw std::runtime_error("Cannot return from subroutine: stackpointer is 0.");
    stackPointer--;
    programCounter = jumpStack[stackPointer];
    programCounter+=2;
}

// firstByte opcodes
void ChipEight::gotoAdress(uint16_t opcode)
{
    uint16_t adress = opcode & 0x0FFF;
    programCounter = adress;
}

void ChipEight::gotoSubroutine(uint16_t opcode)
{
    if(stackPointer == 16)
        throw std::runtime_error("Cannot go to subroutine: maximum 16 subroutines.");
    jumpStack[stackPointer] = programCounter;
    stackPointer++;
    programCounter = opcode & 0x0FFF;
}

void ChipEight::skipIfRegisterEqual(uint16_t opcode)
{
    int registerNum = (opcode & 0x0F00) >> 8;
    uint16_t value = opcode & 0x00FF;
    if(registers[registerNum] == value)
        programCounter+=2;
    programCounter+=2;
}

void ChipEight::skipIfRegisterNotEqual(uint16_t opcode)
{
    int registerNum = (opcode & 0x0F00) >> 8;
    uint16_t value = opcode & 0x00FF;
    if(registers[registerNum] != value)
        programCounter+=2;
    programCounter+=2;
}

void ChipEight::skipIfTwoRegistersEqual(uint16_t opcode)
{
    int firstRegister = (opcode & 0x0F00) >> 8;
    int secondRegister = (opcode & 0x00F0) >> 4;
    if(registers[firstRegister] == registers[secondRegister])
        programCounter+=2;
    programCounter+=2;
}

void ChipEight::skipIfTwoRegistersNotEqual(uint16_t opcode)
{
    int firstRegister = (opcode & 0x0F00) >> 8;
    int secondRegister = (opcode & 0x00F0) >> 4;
    if(registers[firstRegister] != registers[secondRegister])
        programCounter+=2;
    programCounter+=2;
}

void ChipEight::setRegister(uint16_t opcode)
{
    int registerNum = (opcode & 0x0F00) >> 8;
    uint8_t registerValue = opcode & 0x00FF;
    registers[registerNum] = registerValue;
    programCounter += 2;
}

void ChipEight::addToRegister(uint16_t opcode)
{
    int registerNum = (opcode & 0x0F00) >> 8;
    uint16_t value = opcode & 0x00FF;
    registers[registerNum] += value;
    programCounter+=2;
}

void ChipEight::setRegisterAnotherRegister(uint16_t opcode)
{
    int firstRegister = (opcode & 0x0F00) >> 8;
    int secondRegister = (opcode & 0x00F0) >> 4;
    registers[firstRegister] = registers[secondRegister];
    programCounter += 2;
}

void ChipEight::registerBitwiseOr(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    registers[first] = registers[first] | registers[second];
    programCounter += 2;
}

void ChipEight::registerBitwiseAnd(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    registers[first] = registers[first] & registers[second];
    programCounter += 2;
}

void ChipEight::registerBitwiseXor(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    registers[first] = registers[first] ^ registers[second];
    programCounter += 2;
}

void ChipEight::sumRegisters(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    // Overflow check
    registers[0xF] = 0;
    if(registers[first] + registers[second] < registers[first])
        registers[0xF] = 1;

    registers[first] += registers[second];
    programCounter += 2;
}

void ChipEight::substractRegisters(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    // Underflow check
    registers[0xF] = 1;
    if(registers[first] - registers[second] > registers[first])
        registers[0xF] = 0;

    registers[first] -= registers[second];
    programCounter += 2;
}

void ChipEight::shiftRightRegister(uint16_t opcode)
{
    int regToShift = opcode & 0x0F00;
    registers[0xF] = registers[regToShift] & 0x1;
    registers[regToShift] >>= 1;
    programCounter += 2;
}

void ChipEight::VySubstractVx(uint16_t opcode)
{
    int first = (opcode & 0x0F00) >> 8;
    int second = (opcode & 0x00F0) >> 4;
    registers[first] = registers[second] - registers[first];
    registers[0xF] = 0;
    if(registers[first] > registers[second])
        registers[0xF] = 1;
    programCounter += 2;
}

void ChipEight::shiftLeftRegister(uint16_t opcode)
{
    int regToShift = opcode & 0x0F00;
    registers[0xF] = registers[regToShift] >> 7;
    registers[regToShift] <<= 1;
    programCounter += 2;
}

void ChipEight::setIAdress(uint16_t opcode)
{
    uint16_t value = opcode & 0x0FFF;
    registerI = value;
    programCounter += 2;
}

void ChipEight::jumpPlusV0(uint16_t opcode)
{
    programCounter = registers[0] + (opcode & 0x0FFF);
}

void ChipEight::bitwiseRandomNumber(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    int value = opcode & 0x00FF;
    registers[reg] = uint8_t(rand()%255)&value;
    programCounter+=2;
}

void ChipEight::drawSprite(uint16_t opcode)
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
                int coordinate = ((x + xline)%64 + ((y + yline)%32 * 64));
                if(displayBuffer[coordinate] == 1)
                    registers[0xF] = 1;
                displayBuffer[coordinate] ^= 1;
            }
        }
    }
    programCounter+=2;
    drawFlag = true;
}

void ChipEight::skipIfKeyPressed(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;

    if(registers[reg] < 0xF && keys[registers[reg]] == 1)
        programCounter += 2;
    programCounter += 2;
}

void ChipEight::skipIfKeyNotPressed(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;

    if(registers[reg] <= 0xF && keys[registers[reg]] != 1)
        programCounter += 2;
    programCounter += 2;
}

void ChipEight::getDelay(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    registers[reg] = delayTimer;
    programCounter += 2;
}

void ChipEight::waitForKey(uint16_t opcode)
{
    bool keyPressed = false;
    for(int i = 0; i < 0x10; i++)
    {
        if(keys[i] == 1)
        {
            keyPressed = true;
            int reg = (opcode & 0x0F00) >> 8;
            registers[reg] = uint8_t(i);
        }
    }
    if(keyPressed)
        programCounter+=2;
}

void ChipEight::setDelay(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    delayTimer = registers[reg];
    programCounter += 2;
}

void ChipEight::setSoundTimer(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    soundTimer = registers[reg];
    programCounter += 2;
}

void ChipEight::addToI(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    // If overflow. Undocumented feature.
    if(registerI + registers[reg] < registerI)
        registers[0xf] = 1;
    else
        registers[0xf] = 0;
    registerI += registers[reg];
    programCounter += 2;
}

void ChipEight::getChar(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    registerI = registers[reg] * 0x5;
    programCounter += 2;
}

void ChipEight::storeRegister(uint16_t opcode)
{
    int reg = (opcode & 0x0F00) >> 8;
    memory[registerI] = registers[reg] / 100;
    memory[registerI + 1] = (registers[reg] / 10)%10;
    memory[registerI + 2] = (registers[reg] % 100)%10;
    programCounter += 2;
}

void ChipEight::registerDump(uint16_t opcode)
{
    int endReg = (opcode & 0x0F00) >> 8;
    for(int i = 0; i <= endReg; i++)
        memory[registerI + i] = registers[i];
    programCounter += 2;
}

void ChipEight::loadRegister(uint16_t opcode)
{
    int endReg = (opcode & 0x0F00) >> 8;
    for(int i = 0; i <= endReg; i++)
        registers[i] = memory[registerI + i];
    programCounter += 2;
}

void ChipEight::processFirstByteOpcode(uint16_t opcode)
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
        switch ((opcode & 0x000F))
        {
        case 0x0:
            setRegisterAnotherRegister(opcode);
            break;
        case 0x1:
            registerBitwiseOr(opcode);
            break;
        case 0x2:
            registerBitwiseAnd(opcode);
            break;
        case 0x3:
            registerBitwiseXor(opcode);
            break;
        case 0x4:
            sumRegisters(opcode);
            break;
        case 0x5:
            substractRegisters(opcode);
            break;
        case 0x6:
            shiftRightRegister(opcode);
            break;
        case 0x7:
            VySubstractVx(opcode); break;
        case 0xE:
            shiftLeftRegister(opcode);
            break;
        default:
            std::cout << "Oh, noes! Unknown 0x8XXX opcode!" << std::hex << opcode << std::dec << std::endl;
            break;
        }
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
        switch (opcode & 0x00FF)
        {
        case 0x9E:
            skipIfKeyPressed(opcode);
            break;
        case 0xA1:
            skipIfKeyNotPressed(opcode);
            break;
        default:
            std::cout << "Oh, noes! Unknown 0xEXXX opcode!" << std::hex << opcode << std::dec << std::endl;
            break;
        }
        break;
    case 0xF000:
        switch(opcode & 0x00FF)
        {
        case 0x07:
            getDelay(opcode);
            break;
        case 0x0A:
            waitForKey(opcode);
            break;
        case 0x15:
            setDelay(opcode);
            break;
        case 0x18:
            setSoundTimer(opcode);
            break;
        case 0x1E:
            addToI(opcode);
            break;
        case 0x29:
            getChar(opcode);
            break;
        case 0x33:
            storeRegister(opcode);
            break;
        case 0x55:
            registerDump(opcode);
            break;
        case 0x65:
            loadRegister(opcode);
            break;
        default:
            std::cout << "Oh, noes! Unknown 0xFXXX opcode!" << std::hex << opcode << std::dec << std::endl;
            break;
        }
        break;
    default:
        programCounter += 2;
        std::cout << "No such opcode as 0x" << std::hex << opcode << std::dec << std::endl;
    }
}

void ChipEight::processLastByteOpcode(uint16_t opcode)
{
    switch(opcode & 0x00FF)
    {
    case 0x00E0:
        clearScreen();
        break;
    case 0x00EE:
        returnFromSubroutine();
        break;
    default:
        std::cout << "WARNING: unknown last bit opcode: 0x" << std::hex
                  << opcode << " at " << programCounter << std::dec << std::endl;
        programCounter += 2;
        break;
    }
}


// PUBLIC
uint8_t ChipEight::getPixel(int x, int y)
{
    return displayBuffer[y*displayWidth+x];
}

void ChipEight::init()
{
    programCounter = 0x200;
    for(auto& i: memory)
        i = 0;
    for(uint8_t& i: displayBuffer)
        i = 0;
    for(uint8_t& i: registers)
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

void ChipEight::load(const std::string &filename)
{
    std::ifstream programFile(filename);
    int memoryPointer = 0;
    while(!programFile.eof())
    {
        uint8_t buffer;
        programFile.read(reinterpret_cast<char*>(&buffer), 1);
        if(programFile.badbit or programFile.failbit)
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

void ChipEight::cycle()
{
    uint16_t opcode = fetchOpcode();
    executeOpcode(opcode);
    processTimers();
}

void ChipEight::executeOpcode(uint16_t opcode)
{
    if((opcode & 0xF000) != 0x0000)
        processFirstByteOpcode(opcode);
    else
        processLastByteOpcode(opcode);
}

void ChipEight::pressKey(int key)
{
    if(key >= keyCount)
        return;
    keys[key] = 1;
}

void ChipEight::releaseKey(int key)
{
    if(key >= keyCount)
        return;
    keys[key] = 0;
}

void ChipEight::printDebug()
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
