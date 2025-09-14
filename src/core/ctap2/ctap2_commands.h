#ifndef CTAP2_COMMANDS_H
#define CTAP2_COMMANDS_H

// CTAP2 command opcodes (as per spec)
enum 
{
    CTAP2_CMD_MAKE_CREDENTIAL   = 0x01,
    CTAP2_CMD_GET_ASSERTION     = 0x02,
    CTAP2_CMD_GET_NEXT_ASSERTION= 0x08,
    CTAP2_CMD_GET_INFO          = 0x04,
    CTAP2_CMD_CLIENT_PIN        = 0x06,
    CTAP2_CMD_RESET             = 0x07,
    CTAP2_CMD_CANCEL            = 0x03,
};

#define CTAP2_ERR_INVALID_COMMAND 0x01  // example error code

#endif // CTAP2_COMMANDS_H
