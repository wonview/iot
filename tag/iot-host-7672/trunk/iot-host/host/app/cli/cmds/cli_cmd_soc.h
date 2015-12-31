#ifndef _CLI_CMD_SOC_H_
#define _CLI_CMD_SOC_H_

struct soc_cmd_table_st
{
    u8              cmd_id;
    u8              cmd_len;
    u8              *cmd_buf;
};


void cmd_soc(s32 argc, s8 *argv[]);
void cmd_read(s32 argc, s8 *argv[]);
void cmd_write(s32 argc, s8 *argv[]);
void cmd_write_52(s32 argc, s8 *argv[]);

#endif /* _CLI_CMD_SOC_H_ */

